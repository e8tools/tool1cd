//---------------------------------------------------------------------------

#ifndef Parse_treeH
#define Parse_treeH

#include <vcl.h>
#pragma hdrstop

#include "NodeTypes.h"
#include "MessageRegistration.h"

//---------------------------------------------------------------------------
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
	__fastcall tree(const String& _value, const node_type _type, tree* _parent);
	__fastcall ~tree();
	tree* __fastcall add_child(const String& _value, const node_type _type);
	tree* __fastcall add_child();
	tree* __fastcall add_node();
	String& __fastcall get_value();
	node_type __fastcall get_type();
	int __fastcall get_num_subnode();
	tree* __fastcall get_subnode(int _index);
	tree* __fastcall get_subnode(const String& node_name);
	tree* __fastcall get_next();
	tree* __fastcall get_parent();
	tree* __fastcall get_first();
	tree* __fastcall get_last();
	tree& __fastcall operator [](int _index);
	void __fastcall set_value(const String& v, const node_type t);
	void __fastcall outtext(String& text);
	String __fastcall path();
};

typedef tree* treeptr;



//tree* __fastcall parse_1Ctext_old(String& text, String path);
tree* __fastcall parse_1Ctext(const String& text, const String& path);
tree* __fastcall parse_1Cstream(TStream* str, const String& path);
bool __fastcall test_parse_1Ctext(TStream* str, const String& path);
String outtext(tree* t);

#endif

