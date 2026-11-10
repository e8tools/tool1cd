/*
    test_project provides tests for Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of test_project.

    test_project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    test_project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with test_project.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../catch.hpp"
#include <Class_1CD.h>
#include <Table.h>
#include <TableRecord.h>
#include <V8Object.h>
#include <boost/filesystem.hpp>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

using boost::filesystem::path;

// Копирует эталонную базу из дерева исходников во временный файл, чтобы тесты
// записи не меняли фикстуру в репозитории.
static path copy_fixture(const std::string &relpath)
{
	path src = path(CMAKE_SOURCE_DIR) / relpath;
	path dst = boost::filesystem::temp_directory_path()
	         / boost::filesystem::unique_path("tool1cd_%%%%%%%%.1CD");
	boost::filesystem::copy_file(src, dst);
	return dst;
}

// Первая таблица, у которой есть объект данных.
static Table *first_table_with_data(T_1CD &base)
{
	for (int i = 0; i < base.get_numtables(); i++) {
		if (base.get_table(i)->get_file_data() != nullptr) {
			return base.get_table(i);
		}
	}
	return nullptr;
}

static Table *table_by_name(T_1CD &base, const std::string &name)
{
	for (int i = 0; i < base.get_numtables(); i++) {
		if (base.get_table(i)->get_name() == name) {
			return base.get_table(i);
		}
	}
	return nullptr;
}

// Запись объекта в формате 8.3.8 с ростом числа блоков (fatlevel 0).
// Раньше при росте не обновлялось число блоков объекта и запись уходила
// в нулевой блок (заголовок базы) — здесь это проверяется явно.
TEST_CASE("set_data растит объект формата 8.3.8 и читает данные обратно",
          "[tool1cd][V8Object][write][8.3.8]")
{
	path dbfile = copy_fixture("tests/db838/db01/1Cv8.1CD");

	{
		T_1CD base(dbfile, nullptr, true);
		REQUIRE(base.is_open());

		Table *t = first_table_with_data(base);
		REQUIRE(t != nullptr);

		V8Object *fd = t->get_file_data();
		uint64_t old_len = fd->get_len();

		// Гарантированно несколько блоков (fatlevel 0): +30 страниц по 8192 байта.
		uint64_t new_len = old_len + 30ull * 8192ull;
		std::vector<char> pattern(new_len);
		for (uint64_t i = 0; i < new_len; i++) {
			pattern[i] = static_cast<char>((i * 131 + 17) & 0xFF);
		}

		WHEN("Записываем объект целиком большего размера") {
			// Если число блоков не обновлено, запись попадёт в блок 0 и сработает
			// защита get_block_for_write — set_data бросит исключение.
			REQUIRE(fd->set_data(pattern.data(), new_len));

			THEN("Длина объекта и его данные соответствуют записанным") {
				REQUIRE(fd->get_len() == new_len);

				std::vector<char> readback(new_len);
				fd->get_data(readback.data(), 0, new_len);
				REQUIRE(std::memcmp(readback.data(), pattern.data(), new_len) == 0);
			}
		}
	}

	boost::filesystem::remove(dbfile);
}

// set_version должна писать версию по смещению, верному для формата 8.3.8,
// и не затрагивать длину объекта (раньше версия писалась поверх длины).
TEST_CASE("set_version пишет версию и не затирает длину объекта (8.3.8)",
          "[tool1cd][V8Object][write][8.3.8]")
{
	path dbfile = copy_fixture("tests/db838/db01/1Cv8.1CD");

	std::string table_name;
	uint64_t original_len = 0;

	{
		T_1CD base(dbfile, nullptr, true);
		REQUIRE(base.is_open());

		Table *t = first_table_with_data(base);
		REQUIRE(t != nullptr);
		table_name = t->get_name();

		V8Object *fd = t->get_file_data();
		original_len = fd->get_len();

		fd->set_version(0x01020304, 0x05060708);
		base.flush();
	}

	GIVEN("Переоткрытая после записи версии база") {
		T_1CD reopened(dbfile, nullptr, true);
		REQUIRE(reopened.is_open());

		Table *t = table_by_name(reopened, table_name);
		REQUIRE(t != nullptr);
		V8Object *fd = t->get_file_data();

		THEN("Длина объекта сохранилась, а версия записана корректно") {
			REQUIRE(fd->get_len() == original_len);

			auto ver = fd->get_current_version();
			REQUIRE(ver.version_1 == 0x01020304);
			REQUIRE(ver.version_2 == 0x05060708);
		}
	}

	boost::filesystem::remove(dbfile);
}

// Пометка записи удалённой должна сохраняться при переоткрытии базы.
TEST_CASE("mark_record_removed помечает запись удалённой (8.3.8)",
          "[tool1cd][Table][write][8.3.8]")
{
	path dbfile = copy_fixture("tests/db838/db01/1Cv8.1CD");

	std::string table_name;
	uint32_t record_no = 0;

	{
		T_1CD base(dbfile, nullptr, true);
		REQUIRE(base.is_open());

		// Ищем первую неудалённую запись в любой таблице.
		for (int i = 0; i < base.get_numtables() && record_no == 0; i++) {
			Table *t = base.get_table(i);
			uint32_t total = t->get_phys_numrecords();
			for (uint32_t r = 1; r < total; r++) {
				TableRecord *rec = t->get_record(r);
				bool removed = rec->is_removed();
				delete rec;
				if (!removed) {
					table_name = t->get_name();
					record_no = r;
					t->begin_edit();
					t->mark_record_removed(r);
					break;
				}
			}
		}
		REQUIRE(record_no != 0);
		base.flush();
	}

	GIVEN("Переоткрытая после пометки база") {
		T_1CD reopened(dbfile, nullptr, true);
		REQUIRE(reopened.is_open());

		Table *t = table_by_name(reopened, table_name);
		REQUIRE(t != nullptr);

		THEN("Запись отмечена как удалённая") {
			TableRecord *rec = t->get_record(record_no);
			bool removed = rec->is_removed();
			delete rec;
			REQUIRE(removed);
		}
	}

	boost::filesystem::remove(dbfile);
}
