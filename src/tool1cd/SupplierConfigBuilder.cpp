/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "SupplierConfigBuilder.h"
#include "ConfigStorage.h"
#include "System.Classes.hpp"
#include "TempStream.h"
#include "DetailedException.h"
#include "UZLib.h"
#include "SystemClasses/String.hpp"

extern Registrator msreg_g;

using namespace std;
using namespace System;

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
				.add_detail("Таблица", _table_file->t->get_name())
				.add_detail("Имя", _table_file->name);
	}

	container_file->close();

	main_catalog = std::unique_ptr<V8Catalog>( new V8Catalog(out_stream, true) );
}

std::shared_ptr<SupplierConfig> SupplierConfigBuilder::build() {
	std::string _name; // имя конфигурация поставщика
	std::string _supplier; // синоним конфигурация поставщика
	std::string _version; // версия конфигурация поставщика

	V8File* root_file = get_file("root");
	if(!root_file) {
		throw SupplierConfigReadException("Не найден файл root в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->get_name())
				.add_detail("Имя файла", _table_file->name);
	}

	std::string file_name_meta;
	{
		auto root_tree = root_file->get_tree();
		if(root_tree != nullptr)
		{
			file_name_meta = (*root_tree)[0][1].get_value();
		}
	}

	V8File* meta_file = get_file(file_name_meta);
	if(!meta_file) {
		throw SupplierConfigReadException("Не найден файл метаданных в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->get_name())
				.add_detail("Имя файла", _table_file->name)
				.add_detail("Имя мета", file_name_meta);
	}

	#ifdef _DEBUG
	msreg_g.AddDebugMessage("Найден файл метаданных в конфигурации поставщика", MessageState::Info)
		.with("Таблица", _table_file->t->get_name())
		.with("Имя файла", _table_file->name)
		.with("Имя мета", file_name_meta);
	#endif

	auto meta_tree = meta_file->get_tree();
	if(meta_tree == nullptr)
	{
		throw SupplierConfigReadException("Ошибка чтения дерева метаданных в конфигурации поставщика")
				.add_detail("Имя мета", meta_file->get_file_name());
	}
	int32_t numnode = stoi((*meta_tree)[0][2].get_value());
	int32_t current_node_number = 0;
	for(current_node_number = 0; current_node_number < numnode; current_node_number++) {
		Tree& node = (*meta_tree)[0][3 + current_node_number];
		std::string nodetype = node[0].get_value();
		if (EqualIC(nodetype, NODE_GENERAL())) { // узел "Общие"
			Tree& confinfo = node[1][1];
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
					msreg_g.AddDebugMessage("Неизвестная версия свойств конфигурации поставщика", MessageState::Info)
						.with("Таблица", _table_file->t->get_name())
						.with("Имя файла", _table_file->name)
						.with("Имя мета", file_name_meta)
						.with("Версия свойств", verconfinfo);
					#endif
					break;
			}
			break;
		}
	}

	if(current_node_number >= numnode) {
		// TODO: критичная ошибка? Обработка на месте?
		msreg_g.AddMessage("Не найден узел Общие в метаданных конфигурации поставщика", MessageState::Warning)
			.with("Таблица", _table_file->t->get_name())
			.with("Имя файла", _table_file->name)
			.with("Имя мета", file_name_meta);
		_supplier = "";
		_version = "";
	}
	#ifdef _DEBUG
	else {
		msreg_g.AddDebugMessage("Найдена конфигурация поставщика", MessageState::Info)
			.with("Таблица", _table_file->t->get_name())
			.with("Имя файла", _table_file->name)
			.with("Имя", _name)
			.with("Версия", _version)
			.with("Поставщик", _supplier);
	}
	#endif

	std::shared_ptr<SupplierConfig> sup_conf( new SupplierConfig(_table_file, _name, _supplier, _version) );

	return sup_conf;
}

int32_t SupplierConfigBuilder::get_version() const {
	int32_t result = 0;

	V8File* version_file = main_catalog->get_file("version");
	if(!version_file) {
		throw SupplierConfigReadException("Не найден файл version в конфигурации поставщика")
				.add_detail("Таблица", _table_file->t->get_name())
				.add_detail("Имя файла", _table_file->name);
	}

	auto version_tree = version_file->get_tree();
	if(version_tree != nullptr)
	{
		result = stoi((*version_tree)[0][0][0].get_value());
	}

#ifdef _DEBUG
msreg_g.AddDebugMessage("Найдена версия контейнера конфигурации поставщика", MessageState::Info)
	.with("Таблица", _table_file->t->get_name())
	.with("Имя файла", _table_file->name)
	.with("Версия", result);
#endif

	return result;
}

V8File* SupplierConfigBuilder::get_file(const string &file_name) const {
	V8File* result = nullptr;

	if(get_version() < 100) { // 8.0
		V8File* metadata_file = main_catalog->get_file("metadata");
		if(metadata_file == nullptr) {
			throw SupplierConfigReadException("Не найден каталог metadata в конфигурации поставщика")
					.add_detail("Таблица", _table_file->t->get_name())
					.add_detail("Имя файла", _table_file->name);
		}

		V8Catalog* cat2 = metadata_file->get_catalog();
		if(cat2 == nullptr) {
			throw SupplierConfigReadException("Файл metadata не является каталогом в конфигурации поставщика")
					.add_detail("Таблица", _table_file->t->get_name())
					.add_detail("Имя файла", _table_file->name);
		}

		result = cat2->get_file(file_name);
	} else {
		result = main_catalog->get_file(file_name); // else 8.1 или 8.2
	}

	return result;
}
