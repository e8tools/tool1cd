
#include "Field.h"
#include "Base64.h"
#include "TempStream.h"
#include "FieldType.h"
#include "UZLib.h"
#include "Common.h"

using namespace std;

extern Registrator msreg_g;

char Field::null_index[4096];
bool Field::null_index_initialized = false;


//---------------------------------------------------------------------------
Field::Field(Table* _parent)
{
	if(!null_index_initialized)
	{
		*null_index = 1;
		memset(null_index + 1, 0, 4095);
		null_index_initialized = true;
	}

	parent = _parent;
	name = "";
}

//---------------------------------------------------------------------------
std::string Field::get_name() const
{
	return name;
}

void Field::set_name(const string &value)
{
	name = value;
}

//---------------------------------------------------------------------------
int32_t Field::get_size() const // возвращает длину поля в байтах
{
	return (null_exists ? 1 : 0) + type_manager->get_size();
}

//---------------------------------------------------------------------------
// При ignore_showGUID binary16 всегда преобразуется в GUID
std::string Field::get_presentation(const char *rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID,
									bool detailed) const
{
	const char* fr = rec + offset;
	if (get_null_exists()) {
		if (fr[0] == 0) {
			return EmptyNull ? "" : "{NULL}";
		}
		fr++;
	}
	return type_manager->get_presentation(
			fr,
			EmptyNull, Delimiter, ignore_showGUID, detailed
	);
}

//---------------------------------------------------------------------------
bool Field::get_binary_value(char *binary_value, bool null, const std::string &value) const
{
	memset(binary_value, 0, get_size());

	if (null_exists) {
		if (null) {
			return true;
		}
		*binary_value = 1;
		binary_value++;
	}
	return type_manager->get_binary_value(binary_value, value);
}

//---------------------------------------------------------------------------
std::string Field::get_XML_presentation(const char *rec, bool ignore_showGUID) const
{
	const char *fr = rec + offset;
	if (null_exists) {
		if (fr[0] == 0) {
			return "";
		}
		fr++;
	}
	return type_manager->get_XML_presentation(fr, parent, ignore_showGUID);
}

//---------------------------------------------------------------------------
type_fields Field::get_type() const
{
	return type_manager->get_type();
}

//---------------------------------------------------------------------------
Table* Field::get_parent() const
{
	return parent;
}

//---------------------------------------------------------------------------
bool Field::get_null_exists() const
{
	return null_exists;
}

//---------------------------------------------------------------------------
int32_t Field::get_length() const
{
	return type_manager->get_length();
}

//---------------------------------------------------------------------------
int32_t Field::get_precision() const
{
	return type_manager->get_precision();
}

//---------------------------------------------------------------------------
bool Field::get_case_sensitive() const
{
	return type_manager->get_case_sensitive();
}

//---------------------------------------------------------------------------
std::string Field::get_presentation_type() const
{
	return type_manager->get_presentation_type();
}

//---------------------------------------------------------------------------
std::string TrimSpacesRight(const std::string &s)
{
	std::string result = s;
	while (result.back() == ' ') {
		result.erase(result.size() - 1);
	}
	return s;
}

//---------------------------------------------------------------------------
uint32_t Field::get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{
	const char *fr = rec + offset;
	if (null_exists) {
		if (*fr == 0) {
			*(SortKey++) = 0;
			memcpy(SortKey, (void *)null_index, get_size());
			return 0;
		}
		*(SortKey++) = 1;

		fr++;
	}

	try {

		return type_manager->get_sort_key(fr, SortKey, maxlen);

	} catch (SerializationException &exception) {
		exception.add_detail("Таблица", parent->get_name())
				.add_detail("Поле", name)
				.show();
	}
	return 0;
}

FieldType *Field::get_type_manager() const
{
	return type_manager;
}

void Field::set_type_manager(FieldType *value)
{
	type_manager = value;
}

int32_t Field::get_offset() const
{
	return offset;
}

void Field::set_offset(const int32_t value)
{
	offset = value;
}

//---------------------------------------------------------------------------
bool Field::save_blob_to_file(const TableRecord *rec, const std::string &_filename, bool unpack) const
{
	TStream* blob_stream;
	TStream* _s;
	TStream* _s2;
	TStream* _sx;
	TStream* _sx2;
	
	if (rec->is_null_value(this)) {
		return false;
	}

	auto bp = rec->get<table_blob_file>(this);
	if (bp.blob_start == 0 || bp.blob_length == 0) {
		return false;
	}

	if(!unpack) {
		TFileStream temp_stream(boost::filesystem::path(_filename), fmCreate);
		parent->readBlob(&temp_stream, bp.blob_start, bp.blob_length);
		return true;
	}

	bool usetemporaryfiles = bp.blob_length > 10 * 1024 * 1024;
	if(usetemporaryfiles) blob_stream = new TTempStream;
	else blob_stream = new TMemoryStream;
	parent->readBlob(blob_stream, bp.blob_start, bp.blob_length);
	if(blob_stream->GetSize() == 0)
	{
		delete blob_stream;
		return false;
	}

	Table *tab = parent;
	if(usetemporaryfiles) _s = new TTempStream;
	else _s = new TMemoryStream;

	if(tab->get_issystem())
	{

		// спецобработка для users.usr
		string tabname = tab->get_name();
		bool is_users_usr = false;
		if (EqualIC(tabname, "PARAMS")) {
			Field *_f = tab->get_field(0);
			if (EqualIC(rec->get_string(_f), "users.usr")) {
				is_users_usr = true;
			}
		}
		else if(EqualIC(tabname, "V8USERS")) {
			is_users_usr = true;
		}

		bool maybezipped_twice = true;
		if (EqualIC(tabname, "CONFIG") || EqualIC(tabname, "CONFIGSAVE")) {
			Field *_f = tab->get_field(0);
			maybezipped_twice = rec->get_string(_f).size() > GUID_LEN*2;
		}

		if(is_users_usr)
		{

			size_t stream_size = blob_stream->GetSize();
			char *_bb = new char[stream_size];
			blob_stream->Seek(0, soFromBeginning);
			blob_stream->Read(_bb, stream_size);

			size_t xor_mask_size = _bb[0];
			char *_xor_mask = &_bb[1];
			char *_xor_buf = &_xor_mask[xor_mask_size];
			size_t data_size = stream_size - xor_mask_size - 1;
			for(size_t i = 0, k = 0; i < data_size; i++, k++)
			{
				if (k >= xor_mask_size) {
					k = 0;
				}
				_xor_buf[i] ^= _xor_mask[k];
			}
			TFileStream temp_stream(boost::filesystem::path(_filename), fmCreate);
			temp_stream.SetSize(0);
			temp_stream.WriteBuffer(_xor_buf, data_size);
			delete[] _bb;
		}
		else
		{
			bool zippedContainer = false;
			bool zipped = false;
			try
			{
				blob_stream->Seek(0, soFromBeginning);
				ZInflateStream(blob_stream, _s);
				zipped = true;
				if(maybezipped_twice) _sx = _s;
				else _sx2 = _s;
				_s = nullptr;
				delete blob_stream;
				blob_stream = nullptr;
			}
			catch (...)
			{
				_sx2 = blob_stream;
				delete _s;
				_s = nullptr;
				blob_stream = nullptr;
				zipped = false;
			}

			if(zipped && maybezipped_twice)
			{
				if(usetemporaryfiles) _s2 = new TTempStream;
				else _s2 = new TMemoryStream;
				try
				{
					_sx->Seek(0, soFromBeginning);
					ZInflateStream(_sx, _s2);
					zippedContainer = true;
					_sx2 = _s2;
					_s2 = nullptr;
					delete _sx;
					_sx = nullptr;
				}
				catch (...)
				{
					_sx2 = _sx;
					_sx = nullptr;
					delete _s2;
					_s2 = nullptr;
				}
			}

			V8Catalog *cat = new V8Catalog(_sx2, zippedContainer, true);
			if(!cat->get_first())
			{
				TFileStream temp_stream(boost::filesystem::path(_filename), fmCreate);
				temp_stream.CopyFrom(_sx2, 0);
			}
			else {
				cat->SaveToDir(boost::filesystem::path(_filename));
			}
			delete cat;
			delete _sx2;

		}
	}
	else /*if(tab->get_issystem())*/
	{
		char _buf[16];
		_s->CopyFrom(blob_stream, 0);
		blob_stream->Seek(0, soFromBeginning);
		if(blob_stream->Read(_buf, 2) >= 2) if((_buf[0] == 1 || _buf[0] == 2) && _buf[1] == 1)
		{
			if(usetemporaryfiles) _s2 = new TTempStream;
			else _s2 = new TMemoryStream;
			bool isOK = true;
			if(_buf[0] == 1) // неупакованное хранилище
			{
				_s2->CopyFrom(blob_stream, blob_stream->GetSize() - 2);
			}
			else
			{
				if(blob_stream->Read(_buf, 16) < 16) isOK = false;
				else
				{
					if(memcmp(_buf, SIG_ZIP, 16) != 0) isOK = false;
					else
					{
						try
						{
							ZInflateStream(blob_stream, _s2);
						}
						catch(...)
						{
							isOK = false;
						}
					}
				}
			}
			if(isOK)
			{
				_s2->Seek(0, soFromBeginning);
				if(_s2->Read(_buf, 8) < 8) isOK = false;
				else
				{
					_s->SetSize(0);
					_s->CopyFrom(_s2, _s2->GetSize() - 8);
				}

			}

			if(isOK)
			{
				int64_t len1C = *(int64_t*)_buf;
				if(_s->GetSize() > len1C)
				{
					_s->Seek(len1C, (TSeekOrigin)soFromBeginning);
					_s2->SetSize(0);
					_s2->CopyFrom(_s, _s->GetSize() - len1C);
					_s2->Seek(0, soFromBeginning);
					if(_s2->Read(_buf, 12) >= 12)
					{
						len1C = *(int64_t*)&_buf[4];
						if(len1C <= _s2->GetSize() - 12)
						{
							_s->SetSize(0);
							_s->CopyFrom(_s2, len1C);
						}
					}
				}
			}
			delete _s2;
		}

		TFileStream temp_stream(boost::filesystem::path(_filename), fmCreate);
		temp_stream.CopyFrom(_s, 0);
	}

	delete _s;
	delete blob_stream;

	return true;
}

Field *Field::field_from_tree(Tree *field_tree, bool &has_version, Table *parent)
{
	Field *fld = new Field(parent);

	if (field_tree->get_type() != node_type::nd_string) {
		throw FieldStreamParseException("Ошибка получения имени поля таблицы. Узел не является строкой.");
	}
	fld->name = field_tree->get_value();

	field_tree = field_tree->get_next();

	field_type_declaration type_declaration;
	try {

		type_declaration = field_type_declaration::parse_tree(field_tree);

	} catch (FieldStreamParseException &formatError) {
		throw formatError.add_detail("Поле", fld->name);
	}

	fld->type         = type_declaration.type;
	fld->null_exists  = type_declaration.null_exists;
	fld->type_manager = FieldType::create_type_manager(type_declaration);

	if (fld->type == type_fields::tf_version) {
		has_version = true;
	}
	return fld;
}
