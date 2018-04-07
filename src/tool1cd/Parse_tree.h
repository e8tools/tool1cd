
#ifndef Parse_treeH
#define Parse_treeH

#include <memory>

#include "NodeTypes.h"

using namespace std;

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
	Tree(const string &_value, const node_type _type, Tree *_parent);
	~Tree();
	Tree* add_child(const string &_value, const node_type _type);
	Tree* add_child();
	Tree* add_node();
	string get_value() const;
	node_type get_type() const;
	int get_num_subnode() const;
	Tree* get_subnode(int _index);
	Tree* get_subnode(const string &node_name);
	Tree* get_next();
	Tree* get_parent();
	Tree* get_first();
	Tree* get_last();
	Tree& operator [](int _index);
	void set_value(const string &v, const node_type t);
	void outtext(string &text);
	string path() const;

private:
	string value;
	node_type type;
	int num_subnode; // количество подчиненных
	Tree* parent;    // +1
	Tree* next;      // 0
	Tree* prev;      // 0
	Tree* first;     // -1
	Tree* last;      // -1
	unsigned int index;
};

unique_ptr<Tree> parse_1Ctext(const string &text, const string &path);
unique_ptr<Tree> parse_1Cstream(TStream *str, const string &path);
string outtext(Tree *t);

#endif

