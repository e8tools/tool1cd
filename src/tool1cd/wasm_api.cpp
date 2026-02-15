#include "Class_1CD.h"
#include "Table.h"

#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

namespace {

T_1CD* g_db = nullptr;
std::string g_last_error;

char* dup_cstr(const std::string& s) {
    char* out = static_cast<char*>(std::malloc(s.size() + 1));
    if (!out) {
        return nullptr;
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

} // namespace

extern "C" {

EMSCRIPTEN_KEEPALIVE int onecd_open(const char* path) {
    if (!path || !*path) {
        set_error("onecd_open: empty path");
        return -1;
    }

    try {
        delete g_db;
        g_db = new T_1CD();
        g_db->open(path, false);
        g_last_error.clear();
        return 0;
    } catch (const std::exception& ex) {
        set_error(ex.what());
        delete g_db;
        g_db = nullptr;
        return -2;
    } catch (...) {
        set_error("onecd_open: unknown error");
        delete g_db;
        g_db = nullptr;
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

EMSCRIPTEN_KEEPALIVE const char* onecd_last_error() {
    return dup_cstr(g_last_error);
}

EMSCRIPTEN_KEEPALIVE void onecd_close() {
    delete g_db;
    g_db = nullptr;
    g_last_error.clear();
}

EMSCRIPTEN_KEEPALIVE void onecd_free_string(const char* ptr) {
    std::free(const_cast<char*>(ptr));
}

} // extern "C"
