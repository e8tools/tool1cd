
#include "Parse_tree.h"
#include "Common.h"
#include "DetailedException.h"

#include <boost/regex.hpp>

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
tree::tree(const String& _value, const node_type _type, tree* _parent)
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
tree::~tree()
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
tree* tree::add_child(const String& _value, const node_type _type)
{
	return new tree(_value, _type, this);
}

//---------------------------------------------------------------------------
tree* tree::add_child()
{
	return new tree("", node_type::nd_empty, this);
}

//---------------------------------------------------------------------------
tree* tree::add_node()
{
	return new tree("", node_type::nd_empty, this->parent);
}

//---------------------------------------------------------------------------
std::string tree::get_value() const
{
	return value;
}

//---------------------------------------------------------------------------
node_type tree::get_type()
{
	return type;
}

//---------------------------------------------------------------------------
void tree::set_value(const String& v, const node_type t)
{
	value = v;
	type = t;
}

//---------------------------------------------------------------------------
int tree::get_num_subnode()
{
	return num_subnode;
}

//---------------------------------------------------------------------------
tree* tree::get_subnode(int _index)
{
	if(_index >= num_subnode) return nullptr;
	tree* t = first;
	while(_index)
	{
		t = t->next;
		--_index;
	}
	return t;
}

//---------------------------------------------------------------------------
tree* tree::get_subnode(const std::string &node_name)
{
	tree* t = first;
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
tree* tree::get_next()
{
	return next;
}

//---------------------------------------------------------------------------
tree* tree::get_parent()
{
	return parent;
}

//---------------------------------------------------------------------------
tree* tree::get_first()
{
	return first;
}

//---------------------------------------------------------------------------
tree* tree::get_last()
{
	return last;
}

//---------------------------------------------------------------------------
tree& tree::operator [](int _index)
{
	if(!this) return *this; //-V704

	tree* ret = first;
	while(_index)
	{
		if(ret) ret = ret->next;
		--_index;
	}
	return *ret;
}

//---------------------------------------------------------------------------
void tree::outtext(std::string &text)
{
	node_type lt = node_type::nd_unknown;
	TReplaceFlags _ReplaceAll = TReplaceFlags::rfReplaceAll;

	if(num_subnode)
	{
		if (!text.empty()) {
			text += "\r\n";
		}
		text += "{";
		tree* t = first;
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
				//_ReplaceAll << rfReplaceAll;
				(_ReplaceAll << rfReplaceAll);
				text += StringReplace(value, "\"", "\"\"", _ReplaceAll);
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
string tree::path() const
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
bool read_next_flow(const String &source, int &index, char &sym)
{
	if (index > source.size()) {
		return false;
	}
	sym = source.at(index - 1);
	index++;
	return true;
}

template<typename flow_type>
tree* parse_flow(flow_type source, const std::string &path)
{
	TStringBuilder cur_value; // TODO: избавиться от класса TStringBuilder

	String curvalue;
	tree* ret;
	tree* t;
	int pos = 1;
	char sym;
	node_type nt;

	state_type state = state_type::s_value;

	ret = new tree("", node_type::nd_list, nullptr);
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
						cur_value.Clear();
						state = state_type::s_string;
						break;
					case '{':
						t = new tree("", node_type::nd_list, t);
						break;
					case '}':
						if(t->get_first()) {
							t->add_child("", node_type::nd_empty);
						}
						t = t->get_parent();
						if(!t)
						{
							throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
								.add_detail("Позиция", pos)
								.add_detail("Путь", path);
						}
						state = state_type::s_delimiter;
						break;
					case ',':
						t->add_child("", node_type::nd_empty);
						break;
					default:
						cur_value.Clear();
						cur_value.Append(sym);
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
											.add_detail("Позиция", pos)
											.add_detail("Путь", path);
						}
						break;
					default:
						throw DetailedException("Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.")
										.add_detail("Символ", sym)
										.add_detail("Код символа", to_hex_string(sym))
										.add_detail("Путь", path);
				}
				break;
			case state_type::s_string:
				if(sym == '"'){
					state = state_type::s_quote_or_endstring;
				}
				else cur_value.Append(sym);
				break;
			case state_type::s_quote_or_endstring:
				if(sym == '"')
				{
					cur_value.Append(sym);
					state = state_type::s_string;
				}
				else
				{
					t->add_child(cur_value.ToString(), node_type::nd_string);
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
												.add_detail("Позиция", pos)
												.add_detail("Путь", path);
							}
							state = state_type::s_delimiter;
							break;
						default:
							throw DetailedException("Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.")
											.add_detail("Символ", sym)
											.add_detail("Код символа", to_hex_string(sym))
											.add_detail("Путь", path);
					}
				}
				break;
			case state_type::s_nonstring:
				switch(sym)
				{
					case ',':
						curvalue = cur_value.ToString();
						nt = classification_value(curvalue);
						if(nt == node_type::nd_unknown) {
							throw DetailedException("Ошибка формата потока. Неизвестный тип значения.")
											.add_detail("Значение", curvalue)
											.add_detail("Путь", path);
						}
						t->add_child(curvalue, nt);
						state = state_type::s_value;
						break;
					case '}':
						curvalue = cur_value.ToString();
						nt = classification_value(curvalue);
						if(nt == node_type::nd_unknown) {
							throw DetailedException("Ошибка формата потока. Неизвестный тип значения.")
											.add_detail("Значение", curvalue)
											.add_detail("Путь", path);
						}
						t->add_child(curvalue, nt);
						t = t->get_parent();
						if(!t)
						{
							throw DetailedException("Ошибка формата потока. Лишняя закрывающая скобка }.")
											.add_detail("Позиция", pos)
											.add_detail("Путь", path);
						}
						state = state_type::s_delimiter;
						break;
					default:
						cur_value.Append(sym);
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
		curvalue = cur_value.ToString();
		nt = classification_value(curvalue);
		if(nt == node_type::nd_unknown) {
			msreg_g.AddError("Ошибка формата потока. Неизвестный тип значения.")
							.with("Значение", curvalue)
							.with("Путь", path);
		}
		t->add_child(curvalue, nt);
	}
	else if(state == state_type::s_quote_or_endstring) {
		t->add_child(cur_value.ToString(), node_type::nd_string);
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

tree* parse_1Cstream(TStream *str, const string &path)
{
	return parse_flow(new TStreamReader(str, true), path);
}


tree* parse_1Ctext(const String &text, const string &path)
{
	return parse_flow(text, path);
}

string outtext(tree *t)
{
	string text;
	if(t) {
		if(t->get_first()) {
			t->get_first()->outtext(text);
		}
	}
	return text;
}

