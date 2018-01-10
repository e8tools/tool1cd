/*
 *
 *
 *
 *
 */

#include "Field.h"
#include "Base64.h"
#include "TempStream.h"
#include "FieldType.h"

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
	len = 0;
	offset = 0;
	name = "";
}

//---------------------------------------------------------------------------
String Field::getname() const
{
	return name;
}

//---------------------------------------------------------------------------
int32_t Field::getlen() const // возвращает длину поля в байтах
{
	return (null_exists ? 1 : 0) + type_manager->getlen();
}

//---------------------------------------------------------------------------
// При ignore_showGUID binary16 всегда преобразуется в GUID
String Field::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	const char* fr = rec + offset;
	if (getnull_exists()) {
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
bool Field::get_binary_value(char *binary_value, bool null, const String &value) const
{
	memset(binary_value, 0, len);

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
String Field::get_XML_presentation(const char *rec, bool ignore_showGUID) const
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
type_fields Field::gettype() const
{
	return type_manager->gettype();
}

//---------------------------------------------------------------------------
Table* Field::getparent() const
{
	return parent;
}

//---------------------------------------------------------------------------
bool Field::getnull_exists() const
{
	return null_exists;
}

//---------------------------------------------------------------------------
int32_t Field::getlength() const
{
	return type_manager->getlength();
}

//---------------------------------------------------------------------------
int32_t Field::getprecision() const
{
	return type_manager->getprecision();
}

//---------------------------------------------------------------------------
bool Field::getcase_sensitive() const
{
	return type_manager->getcase_sensitive();
}

//---------------------------------------------------------------------------
int32_t Field::getoffset() const
{
	return offset;
}

//---------------------------------------------------------------------------
String Field::get_presentation_type() const
{
	return type_manager->get_presentation_type();
}

//---------------------------------------------------------------------------
String TrimSpacesRight(String s)
{
	while(s.SubString(s.GetLength(), 1) == " ") s.SetLength(s.GetLength() - 1);
	return s;
}

//---------------------------------------------------------------------------
uint32_t Field::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{
	const char *fr = rec + offset;
	if (null_exists) {
		if (*fr == 0) {
			*(SortKey++) = 0;
			memcpy(SortKey, (void *)null_index, len);
			return 0;
		}
		*(SortKey++) = 1;

		fr++;
	}

	try {

		return type_manager->getSortKey(fr, SortKey, maxlen);

	} catch (SerializationException &exception) {
		exception.add_detail("Таблица", parent->name)
				.add_detail("Поле", name)
				.show();
	}
	return 0;
}

//---------------------------------------------------------------------------
bool Field::save_blob_to_file(char* rec, String _filename, bool unpack) const
{
	TStream* blob_stream;
	TStream* _s;
	TStream* _s2;
	TStream* _sx;
	TStream* _sx2;
	uint32_t k, l;
	bool usetemporaryfiles = false;

	char *orec = rec;
	rec += offset;
	if (getnull_exists()) {
		if (*rec == 0) {
			return false;
		}
		rec++;
	}

	if (*(uint32_t*)rec == 0 || *(uint32_t*)(rec + 4) == 0) {
		return false;
	}

	if(!unpack) {
		TFileStream temp_stream(_filename, fmCreate);
		parent->readBlob(&temp_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
		return true;
	}

	usetemporaryfiles = *(uint32_t*)(rec + 4) > 10 * 1024 * 1024;
	if(usetemporaryfiles) blob_stream = new TTempStream;
	else blob_stream = new TMemoryStream;
	parent->readBlob(blob_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
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
		String tabname = tab->getname();
		bool is_users_usr = false;
		if(tabname.CompareIC("PARAMS") == 0)
		{
			Field *_f = tab->getfield(0);
			if(_f->get_presentation(orec).CompareIC("users.usr") == 0) is_users_usr = true;
		}
		else if(tabname.CompareIC("V8USERS") == 0) is_users_usr = true;

		bool maybezipped_twice = true;
		if(tabname.CompareIC("CONFIG") == 0 || tabname.CompareIC("CONFIGSAVE") == 0)
		{
			Field *_f = tab->getfield(0);
			maybezipped_twice = _f->get_presentation(orec).GetLength() > 72;
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
			TFileStream temp_stream(_filename, fmCreate);
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

			v8catalog *cat = new v8catalog(_sx2, zippedContainer, true);
			if(!cat->GetFirst())
			{
				TFileStream temp_stream(_filename, fmCreate);
				temp_stream.CopyFrom(_sx2, 0);
			}
			else cat->SaveToDir(_filename);
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

		TFileStream temp_stream(_filename, fmCreate);
		temp_stream.CopyFrom(_s, 0);
	}

	delete _s;
	delete blob_stream;

	return true;
}

Field *Field::field_from_tree(tree *field_tree, bool &has_version, Table *parent)
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
