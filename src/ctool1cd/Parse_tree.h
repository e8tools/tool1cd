
#ifndef Parse_treeH
#define Parse_treeH

#include <vcl.h>
#pragma hdrstop

#include "NodeTypes.h"
#include "MessageRegistration.h"

class tree
{
private:
	String value;
	node_type type;
	int num_subnode; // количество подчиненных
	tree* parent; // +1
	tree* next; // 0
	tree* prev; // 0
	tree* first; // -1
	tree* last; // -1
	unsigned int index;

public:
	tree(const String& _value, const node_type _type, tree* _parent);
	~tree();
	tree* add_child(const String& _value, const node_type _type);
	tree* add_child();
	tree* add_node();
	String& get_value();
	node_type get_type();
	int get_num_subnode();
	tree* get_subnode(int _index);
	tree* get_subnode(const String& node_name);
	tree* get_next();
	tree* get_parent();
	tree* get_first();
	tree* get_last();
	tree& operator [](int _index);
	void set_value(const String& v, const node_type t);
	void outtext(String& text);
	String path();
};

typedef tree* treeptr;


tree* parse_1Ctext(const String& text, const String& path);
tree* parse_1Cstream(TStream* str, const String& path);
bool test_parse_1Ctext(TStream* str, const String& path);
String outtext(tree* t);

#endif

