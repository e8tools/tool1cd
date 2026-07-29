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

#include "Class_1CD.h"
#include "Field.h"
#include "Table.h"

#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>
#include <vector>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

namespace {

std::unique_ptr<T_1CD> g_db;
std::string g_last_error;

char *dup_cstr(const std::string &s) {
    char *out = static_cast<char *>(std::malloc(s.size() + 1));
    if (!out) {
        out = static_cast<char *>(std::malloc(1));
        if (!out) return nullptr; // is it possible?
        out[0] = '\0';
        return out;
    }

    std::memcpy(out, s.c_str(), s.size() + 1);
    return out;
}

std::string json_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 16);

    for (char ch : s) {
        switch (ch) {
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b"; break;
        case '\f': out += "\\f"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            if (static_cast<unsigned char>(ch) < 0x20) {
                out += "\\u00";
                const char* hex = "0123456789abcdef";
                out += hex[(static_cast<unsigned char>(ch) >> 4) & 0x0F];
                out += hex[static_cast<unsigned char>(ch) & 0x0F];
            } else {
                out += ch;
            }
            break;
        }
    }

    return out;
}

void set_error(const std::string& message) {
    g_last_error = message;
}

uint64_t object_size(const V8Object* file) {
    return file ? file->get_len() : 0;
}

uint64_t table_total_size(const Table* table) {
    if (!table) {
        return 0;
    }

    return object_size(table->get_file_data())
         + object_size(table->get_file_blob())
         + object_size(table->get_file_index());
}

Table* find_table_by_name(const std::string& table_name) {
    if (!g_db || !g_db->is_open()) {
        return nullptr;
    }

    const int table_count = g_db->get_numtables();
    for (int i = 0; i < table_count; ++i) {
        Table* table = g_db->get_table(i);
        if (table && table->get_name() == table_name) {
            return table;
        }
    }

    return nullptr;
}

} // namespace

extern "C" {
EMSCRIPTEN_KEEPALIVE int onecd_open(const char *path) {
    if (!path || !*path) {
        set_error("onecd_open: empty path");
        return -1;
    }

    try {
        std::unique_ptr<T_1CD> db(new T_1CD());
        db->open(path, false);
        g_db = std::move(db);
        g_last_error.clear();
        return 0;
    } catch (const std::exception &ex) {
        set_error(ex.what());
        return -2;
    } catch (...) {
        set_error("onecd_open: unknown error");
        return -3;
    }
}

EMSCRIPTEN_KEEPALIVE const char* onecd_list_tables_json() {
    if (!g_db || !g_db->is_open()) {
        set_error("Database is not open");
        return dup_cstr("[]");
    }

    try {
        const int table_count = g_db->get_numtables();
        std::string json;
        json.reserve(static_cast<size_t>(table_count) * 64 + 2);
        json.push_back('[');

        for (int i = 0; i < table_count; ++i) {
            if (i > 0) {
                json.push_back(',');
            }

            Table* table = g_db->get_table(i);
            const std::string table_name = table ? table->get_name() : std::string();
            const uint64_t table_size = table_total_size(table);

            json += "{\"name\":\"";
            json += json_escape(table_name);
            json += "\",\"size\":";
            json += std::to_string(table_size);
            json.push_back('}');
        }

        json.push_back(']');
        g_last_error.clear();
        return dup_cstr(json);
    } catch (const std::exception& ex) {
        set_error(ex.what());
        return dup_cstr("[]");
    } catch (...) {
        set_error("onecd_list_tables_json: unknown error");
        return dup_cstr("[]");
    }
}

EMSCRIPTEN_KEEPALIVE const char* onecd_get_table_rows_json(const char* table_name, int offset, int limit) {
    if (!g_db || !g_db->is_open()) {
        set_error("Database is not open");
        return dup_cstr("{\"error\":\"Database is not open\"}");
    }

    if (!table_name || !*table_name) {
        set_error("onecd_get_table_rows_json: empty table name");
        return dup_cstr("{\"error\":\"empty table name\"}");
    }

    if (offset < 0) {
        offset = 0;
    }
    if (limit <= 0) {
        limit = 100;
    }
    if (limit > 200) {
        limit = 200;
    }

    try {
        Table* table = find_table_by_name(table_name);
        if (!table) {
            set_error(std::string("Table not found: ") + table_name);
            return dup_cstr("{\"error\":\"table not found\"}");
        }

        const uint32_t total_rows = table->get_phys_numrecords();
        const int field_count = table->get_num_fields();
        const int start = offset > static_cast<int>(total_rows) ? static_cast<int>(total_rows) : offset;
        const int end = (start + limit) > static_cast<int>(total_rows)
                      ? static_cast<int>(total_rows)
                      : (start + limit);

        std::string json;
        json.reserve(static_cast<size_t>(field_count) * 64 + static_cast<size_t>(end - start) * 256 + 256);
        json += "{\"table\":\"";
        json += json_escape(table->get_name());
        json += "\",\"offset\":";
        json += std::to_string(start);
        json += ",\"limit\":";
        json += std::to_string(limit);
        json += ",\"totalRows\":";
        json += std::to_string(total_rows);
        json += ",\"fields\":[";

        for (int i = 0; i < field_count; ++i) {
            if (i > 0) {
                json.push_back(',');
            }
            Field* field = table->get_field(i);
            json += "{\"name\":\"";
            json += json_escape(field ? field->get_name() : std::string());
            json += "\"}";
        }

        json += "],\"rows\":[";
        std::vector<char> record_buf(static_cast<size_t>(table->get_recordlen()));

        for (int row = start; row < end; ++row) {
            if (row > start) {
                json.push_back(',');
            }

            table->get_record(static_cast<uint32_t>(row), record_buf.data());

            json += "{\"row\":";
            json += std::to_string(row);
            json += ",\"deleted\":";
            json += (record_buf[0] != '\0') ? "true" : "false";
            json += ",\"values\":[";

            for (int col = 0; col < field_count; ++col) {
                if (col > 0) {
                    json.push_back(',');
                }

                Field* field = table->get_field(col);
                std::string value;
                try {
                    value = field ? field->get_presentation(record_buf.data()) : std::string();
                } catch (const std::exception& ex) {
                    value = std::string("{ERROR: ") + ex.what() + "}";
                } catch (...) {
                    value = "{ERROR}";
                }

                json.push_back('"');
                json += json_escape(value);
                json.push_back('"');
            }

            json += "]}";
        }

        json += "]}";
        g_last_error.clear();
        return dup_cstr(json);
    } catch (const std::exception& ex) {
        set_error(ex.what());
        return dup_cstr("{\"error\":\"internal error\"}");
    } catch (...) {
        set_error("onecd_get_table_rows_json: unknown error");
        return dup_cstr("{\"error\":\"unknown error\"}");
    }
}

EMSCRIPTEN_KEEPALIVE const char* onecd_last_error() {
    return dup_cstr(g_last_error);
}

EMSCRIPTEN_KEEPALIVE void onecd_close() {
    g_db.reset();
    g_last_error.clear();
}

EMSCRIPTEN_KEEPALIVE void onecd_free_string(const char* ptr) {
    std::free(const_cast<char*>(ptr));
}

} // extern "C"
