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

#include <boost/regex.hpp>

#include "Parse_tree.h"
#include "Common.h"
#include "DetailedException.h"
#include "MessageRegistration.h"

using namespace System;
using namespace std;

const boost::regex exp_number("^-?\\d+$");
const boost::regex exp_number_exp("^-?\\d+(\\.?\\d*)?((e|E)-?\\d+)?$");
const boost::regex exp_guid("^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$");
const boost::regex exp_binary("^#base64:[0-9a-zA-Z\\+=\\r\\n\\/]*$");
const boost::regex exp_binary2("^[0-9a-zA-Z\\+=\\r\\n\\/]+$");
const boost::regex exp_link("^[0-9]+:[0-9a-fA-F]{32}$");
const boost::regex exp_binary_d("^#data:[0-9a-zA-Z\\+=\\r\\n\\/]*$");

extern Registrator msreg_g;

//---------------------------------------------------------------------------
Tree::Tree(const string &_value, const node_type _type, Tree *_parent)
{
	value = _value;
	type = _type;
	parent = _parent;

	num_subnode = 0;
	index = 0;
	if(parent)
	{
		parent->num_subnode++;
		prev = parent->last;
		if(prev)
		{
			prev->next = this;
			index = prev->index + 1;
		}
		else parent->first = this;
		parent->last = this;
	}
	else prev = nullptr;
	next = nullptr;
	first = nullptr;
	last = nullptr;
}

//---------------------------------------------------------------------------
Tree::~Tree()
{
	while(last) delete last;
	if(prev) prev->next = next;
	if(next) next->prev = prev;
	if(parent)
	{
		if(parent->first == this) parent->first = next;
		if(parent->last == this) parent->last = prev;
		parent->num_subnode--;
	}
}

//---------------------------------------------------------------------------
Tree* Tree::add_child(const string &_value, const node_type _type)
{
	return new Tree(_value, _type, this);
}

//---------------------------------------------------------------------------
Tree* Tree::add_child()
{
	return new Tree("", node_type::nd_empty, this);
}

//---------------------------------------------------------------------------
Tree* Tree::add_node()
{
	return new Tree("", node_type::nd_empty, this->parent);
}

//---------------------------------------------------------------------------
std::string Tree::get_value() const
{
	return value;
}

//---------------------------------------------------------------------------
node_type Tree::get_type() const
{
	return type;
}

//---------------------------------------------------------------------------
void Tree::set_value(const string &v, const node_type t)
{
	value = v;
	type = t;
}

//---------------------------------------------------------------------------
int Tree::get_num_subnode() const
{
	return num_subnode;
}

//---------------------------------------------------------------------------
Tree* Tree::get_subnode(int _index)
{
	if(_index >= num_subnode) return nullptr;
	Tree* t = first;
	while(_index)
	{
		t = t->next;
		--_index;
	}
	return t;
}

//---------------------------------------------------------------------------
Tree* Tree::get_subnode(const std::string &node_name)
{
	Tree* t = first;
	while(t)
	{
		if (Equal(t->value, node_name)) {
			return t;
		}
		t = t->next;
	}
	return nullptr;
}

//---------------------------------------------------------------------------
Tree* Tree::get_next()
{
	return next;
}

//---------------------------------------------------------------------------
Tree* Tree::get_parent()
{
	return parent;
}

//---------------------------------------------------------------------------
Tree* Tree::get_first()
{
	return first;
}

//---------------------------------------------------------------------------
Tree* Tree::get_last()
{
	return last;
}

//---------------------------------------------------------------------------
Tree& Tree::operator [](int _index)
{
	if(!this) return *this; //-V704

	Tree* ret = first;
	while(_index)
	{
		if(ret) ret = ret->next;
		--_index;
	}
	return *ret;
}

//---------------------------------------------------------------------------
void Tree::outtext(std::string &text)
{
	node_type lt = node_type::nd_unknown;
	TReplaceFlags _ReplaceAll = TReplaceFlags::rfReplaceAll;

	if(num_subnode)
	{
		if (!text.empty()) {
			text += "\r\n";
		}
		text += "{";
		Tree* t = first;
		while(t)
		{
			t->outtext(text);
			lt = t->type;
			t = t->next;
			if(t) text += ",";
		}
		if(lt == node_type::nd_list) text += "\r\n";
		text += "}";
	}
	else
	{
		switch(type)
		{
			case node_type::nd_string:
				text += "\"";
				text += StringReplace(value, "\"", "\"\"", rfReplaceAll);
				text += "\"";
				break;
			case node_type::nd_number:
			case node_type::nd_number_exp:
			case node_type::nd_guid:
			case node_type::nd_list:
			case node_type::nd_binary:
			case node_type::nd_binary2:
			case node_type::nd_link:
			case node_type::nd_binary_d:
				text += value;
				break;
			default:
				break;
		}
	}
}

//---------------------------------------------------------------------------
string Tree::path() const
{
	if (!this) {
		return ":??"; //-V704
	}

	string p = "";

	for (auto t = this; t->parent; t = t->parent)
	{
		p = string(":").append(to_string(t->index)).append(p);
	}
	return p;
}


//---------------------------------------------------------------------------
node_type classification_value(const std::string &value)
{
	if(value.size() == 0) {
		return node_type::nd_empty;
	}

	if(regex_match(value.c_str(), exp_number)) {
		return node_type::nd_number;
	}

	if(regex_match(value.c_str(), exp_number_exp)) {
		return node_type::nd_number_exp;
	}

	if(regex_match(value.c_str(), exp_guid)) {
		return node_type::nd_guid;
	}

	if(regex_match(value.c_str(), exp_binary)) {
		return node_type::nd_binary;
	}

	if(regex_match(value.c_str(), exp_link)) {
		return node_type::nd_link;
	}

	if(regex_match(value.c_str(), exp_binary2)) {
		return node_type::nd_binary2;
	}

	if(regex_match(value.c_str(), exp_binary_d)) {
		return node_type::nd_binary_d;
	}

	return node_type::nd_unknown;
}

bool read_next_flow(TStreamReader *reader, int &index, char &sym)
{
	int data = reader->Read();
	if (data == -1) {
		return false;
	}

	sym = (char)data;
	if (sym == '\0') {
		return false;
	}

	index++;
	return true;
}

// HINT: index - с единички
bool read_next_flow(const string &source, int &index, char &sym)
{
	if (index > source.size()) {
		return false;
	}
	sym = source.at(index - 1);
	index++;
	return true;
}

template<typename flow_type>
Tree* parse_flow(flow_type source, const std::string &path)
{
	string cur_value;

	Tree* ret;
	Tree* t;
	int pos = 1;
	char sym;

	state_type state = state_type::s_value;

	ret = new Tree("", node_type::nd_list, nullptr);
	t = ret;

	while (read_next_flow(source, pos, sym))  {
		switch(state)
		{
			case state_type::s_value:
				switch(sym)
				{
					case ' ': // space
					case '\t':
					case '\r':
					case '\n':
						break;
					case '"':
						cur_value.clear();
						state = state_type::s_string;
						break;
					case '{':
						t = new Tree("", node_type::nd_list, t);
						break;
					case '}':
						if(t->get_first()) {
							t->add_child("", node_type::nd_empty);
						}
						t = t->get_parent();
						if(!t)
						{
							throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
								.add_detail("Позиция", to_string(pos))
								.add_detail("Путь", path);
						}
						state = state_type::s_delimiter;
						break;
					case ',':
						t->add_child("", node_type::nd_empty);
						break;
					default:
						cur_value.clear();
						cur_value.push_back(sym);
						state = state_type::s_nonstring;
						break;
				}
				break;
			case state_type::s_delimiter:
				switch(sym)
				{
					case ' ': // space
					case '\t':
					case '\r':
					case '\n':
						break;
					case ',':
						state = state_type::s_value;
						break;
					case '}':
						t = t->get_parent();
						if(!t)
						{
							throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
											.add_detail("Позиция", to_string(pos))
											.add_detail("Путь", path);
						}
						break;
					default:
						throw DetailedException("Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.")
										.add_detail("Символ", string(1, sym))
										.add_detail("Код символа", to_hex_string(sym))
										.add_detail("Путь", path);
				}
				break;
			case state_type::s_string:
				if(sym == '"'){
					state = state_type::s_quote_or_endstring;
				}
				else cur_value.push_back(sym);
				break;
			case state_type::s_quote_or_endstring:
				if(sym == '"')
				{
					cur_value.push_back(sym);
					state = state_type::s_string;
				}
				else
				{
					t->add_child(cur_value, node_type::nd_string);
					switch(sym)
					{
						case ' ': // space
						case '\t':
						case '\r':
						case '\n':
							state = state_type::s_delimiter;
							break;
						case ',':
							state = state_type::s_value;
							break;
						case '}':
							t = t->get_parent();
							if(!t)
							{
								throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
												.add_detail("Позиция", to_string(pos))
												.add_detail("Путь", path);
							}
							state = state_type::s_delimiter;
							break;
						default:
							throw DetailedException("Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.")
											.add_detail("Символ", string(1, sym))
											.add_detail("Код символа", to_hex_string(sym))
											.add_detail("Путь", path);
					}
				}
				break;
			case state_type::s_nonstring:
				switch(sym)
				{
					case ',': {
						node_type nt = classification_value(cur_value);
						if (nt == node_type::nd_unknown) {
							throw DetailedException("Ошибка формата потока. Неизвестный тип значения.")
									.add_detail("Значение", cur_value)
									.add_detail("Путь", path);
						}
						t->add_child(cur_value, nt);
						state = state_type::s_value;
						break;
					}
					case '}': {
						node_type nt = classification_value(cur_value);
						if (nt == node_type::nd_unknown) {
							throw DetailedException("Ошибка формата потока. Неизвестный тип значения.")
									.add_detail("Значение", cur_value)
									.add_detail("Путь", path);
						}
						t->add_child(cur_value, nt);
						t = t->get_parent();
						if (!t) {
							throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
									.add_detail("Позиция", to_string(pos))
									.add_detail("Путь", path);
						}
						state = state_type::s_delimiter;
						break;
					}
					default:
						cur_value.push_back(sym);
						break;
				}
				break;
			default:
				throw DetailedException("Ошибка формата потока. Неизвестный режим разбора.")
								.add_detail("Режим разбора", to_hex_string((int)state))
								.add_detail("Путь", path);

		}
	}

	if(state == state_type::s_nonstring)
	{
		node_type nt = classification_value(cur_value);
		if(nt == node_type::nd_unknown) {
			msreg_g.AddError("Ошибка формата потока. Неизвестный тип значения.")
							.with("Значение", cur_value)
							.with("Путь", path);
		}
		t->add_child(cur_value, nt);
	}
	else if(state == state_type::s_quote_or_endstring) {
		t->add_child(cur_value, node_type::nd_string);
	}
	else if(state != state_type::s_delimiter)
	{
		throw DetailedException("Ошибка формата потока. Незавершенное значение")
						.add_detail("Режим разбора", to_hex_string((int)state))
						.add_detail("Путь", path);
	}

	if(t != ret)
	{
		throw DetailedException("Ошибка формата потока. Не хватает закрывающих скобок } в конце текста разбора.")
						.add_detail("Путь", path);
	}

	return ret;
}

unique_ptr<Tree> parse_1Cstream(TStream *str, const string &path)
{
	unique_ptr<Tree> result( parse_flow(new TStreamReader(str, true), path) );
	return result;
}


unique_ptr<Tree> parse_1Ctext(const string &text, const string &path)
{
	unique_ptr<Tree> result( parse_flow(text, path) );
	return result;
}

string outtext(Tree *t)
{
	string text;
	if(t) {
		if(t->get_first()) {
			t->get_first()->outtext(text);
		}
	}
	return text;
}

