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

#ifndef Parse_treeH
#define Parse_treeH

#include <memory>

#include "NodeTypes.h"
#include "SystemClasses/TStream.hpp"

enum class state_type {
	s_value,        // ожидание начала значения
	s_delimiter,   // ожидание разделителя
	s_string,       // режим ввода строки
	s_quote_or_endstring, // режим ожидания конца строки или двойной кавычки
	s_nonstring // режим ввода значения не строки
};

class Tree
{
public:
	Tree(const std::string &_value, const node_type _type, Tree *_parent);
	~Tree();
	Tree* add_child(const std::string &_value, const node_type _type);
	Tree* add_child();
	Tree* add_node();
	std::string get_value() const;
	node_type get_type() const;
	int get_num_subnode() const;
	Tree* get_subnode(int _index);
	Tree* get_subnode(const std::string &node_name);
	Tree* get_next();
	Tree* get_parent();
	Tree* get_first();
	Tree* get_last();
	Tree& operator [](int _index);
	void set_value(const std::string &v, const node_type t);
	void outtext(std::string &text);
	std::string path() const;

private:
	std::string value;
	node_type type;
	int num_subnode; // количество подчиненных
	Tree* parent;    // +1
	Tree* next;      // 0
	Tree* prev;      // 0
	Tree* first;     // -1
	Tree* last;      // -1
	unsigned int index;
};

std::unique_ptr<Tree> parse_1Ctext(const std::string &text, const std::string &path);
std::unique_ptr<Tree> parse_1Cstream(TStream *str, const std::string &path);
std::string outtext(Tree *t);

#endif

