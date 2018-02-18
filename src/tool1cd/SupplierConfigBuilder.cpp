#include "SupplierConfigBuilder.h"
#include "ConfigStorage.h"
#include "System.Classes.hpp"
#include "TempStream.h"
#include "DetailedException.h"
#include "UZLib.h"

extern Registrator msreg_g;

using namespace std;

string NODE_GENERAL() {
	return string("9cd510cd-abfc-11d4-9434-004095e12fc7");
}

SupplierConfigBuilder::SupplierConfigBuilder(TableFile *table_file)
	: _table_file(table_file)
{
	std::unique_ptr<ContainerFile> container_file( new ContainerFile(_table_file, _table_file->name) );
	if(!container_file->open()) {
		throw SupplierConfigReadException("Ошибка открытия конейнера файлов")
				.add_detail("Имя", container_file->name);
	}

	TStream *out_stream =  new TTempStream;

	try {
		ZInflateStream(container_file->stream, out_stream);
	}
	catch(ZError) {
		throw SupplierConfigReadException("Ошибка распаковки конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->getname())
				.add_detail("Имя", _table_file->name);
	}

	container_file->close();

	main_catalog = std::unique_ptr<V8Catalog>( new V8Catalog(out_stream, true) );
}

std::shared_ptr<SupplierConfig> SupplierConfigBuilder::build() {
	String _name; // имя конфигурация поставщика
	String _supplier; // синоним конфигурация поставщика
	String _version; // версия конфигурация поставщика

	V8File* root_file = get_file("root");
	if(!root_file) {
		throw SupplierConfigReadException("Не найден файл root в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->getname())
				.add_detail("Имя файла", _table_file->name);
	}

	String file_name_meta;
	{
		std::unique_ptr<tree> root_tree ( root_file->get_tree() );
		file_name_meta = (*root_tree)[0][1].get_value();
	}

	V8File* meta_file = get_file(file_name_meta);
	if(!meta_file) {
		throw SupplierConfigReadException("Не найден файл метаданных в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->getname())
				.add_detail("Имя файла", _table_file->name)
				.add_detail("Имя мета", file_name_meta);
	}

	#ifdef _DEBUG
	msreg_g.AddDebugMessage("Найден файл метаданных в конфигурации поставщика", MessageState::Info,
		"Таблица", _table_file->t->getname(),
		"Имя файла", _table_file->name,
		"Имя мета", file_name_meta);
	#endif

	std::unique_ptr<tree> meta_tree ( meta_file->get_tree() );
	int32_t numnode = stoi((*meta_tree)[0][2].get_value());
	int32_t current_node_number = 0;
	for(current_node_number = 0; current_node_number < numnode; current_node_number++) {
		tree& node = (*meta_tree)[0][3 + current_node_number];
		std::string nodetype = node[0].get_value();
		if (EqualIC(nodetype, NODE_GENERAL())) { // узел "Общие"
			tree& confinfo = node[1][1];
			int32_t verconfinfo = stoi(confinfo[0].get_value());
			switch(verconfinfo)	{
				case 15:
					_name = confinfo[1][1][2].get_value();
					_supplier = confinfo[11].get_value();
					_version = confinfo[12].get_value();
					break;
				case 22:
				case 32:
				case 34:
				case 36:
				case 37:
					_name = confinfo[1][1][2].get_value();
					_supplier = confinfo[14].get_value();
					_version = confinfo[15].get_value();
					break;
				default:
					_name = confinfo[1][1][2].get_value();
					_supplier = confinfo[14].get_value();
					_version = confinfo[15].get_value();
					#ifdef _DEBUG
					msreg_g.AddDebugMessage("Неизвестная версия свойств конфигурации поставщика", MessageState::Info,
						"Таблица", _table_file->t->getname(),
						"Имя файла", _table_file->name,
						"Имя мета", file_name_meta,
						"Версия свойств", verconfinfo);
					#endif
					break;
			}
			break;
		}
	}

	if(current_node_number >= numnode) {
		msreg_g.AddError("Не найден узел Общие в метаданных конфигурации поставщика", // TODO: критичная ошибка? Обработка на месте?
			"Таблица", _table_file->t->getname(),
			"Имя файла", _table_file->name,
			"Имя мета", file_name_meta);
		_supplier = "";
		_version = "";
	}
	#ifdef _DEBUG
	else {
		msreg_g.AddDebugMessage("Найдена конфигурация поставщика", MessageState::Info,
			"Таблица", _table_file->t->getname(),
			"Имя файла", _table_file->name,
			"Имя", _name,
			"Версия", _version,
			"Поставщик", _supplier);
	}
	#endif

	std::shared_ptr<SupplierConfig> sup_conf( new SupplierConfig(_table_file, _name, _supplier, _version) );

	return sup_conf;
}

int32_t SupplierConfigBuilder::get_version() const {
	V8File* version_file = main_catalog->GetFile("version");
	if(!version_file) {
		throw SupplierConfigReadException("Не найден файл version в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->getname())
				.add_detail("Имя файла", _table_file->name);
	}

	std::unique_ptr<tree> version_tree ( version_file->get_tree() );
	int32_t result = stoi((*version_tree)[0][0][0].get_value());

#ifdef _DEBUG
msreg_g.AddDebugMessage("Найдена версия контейнера конфигурации поставщика", MessageState::Info,
	"Таблица", _table_file->t->getname(),
	"Имя файла", _table_file->name,
	"Версия", result);
#endif

	return result;
}

V8File* SupplierConfigBuilder::get_file(const string &file_name) const {
	V8File* result = nullptr;

	if(get_version() < 100) { // 8.0
		V8File* metadata_file = main_catalog->GetFile("metadata");
		if(metadata_file == nullptr) {
			throw SupplierConfigReadException("Не найден каталог metadata в конфигурации поставщика")
					.add_detail("Таблица", _table_file->t->getname())
					.add_detail("Имя файла", _table_file->name);
		}

		V8Catalog* cat2 = metadata_file->GetCatalog();
		if(cat2 == nullptr) {
			throw SupplierConfigReadException("Файл metadata не является каталогом в конфигурации поставщика")
					.add_detail("Таблица", _table_file->t->getname())
					.add_detail("Имя файла", _table_file->name);
		}

		result = cat2->GetFile(file_name);
	} else {
		result = main_catalog->GetFile(file_name); // else 8.1 или 8.2
	}

	return result;
}
