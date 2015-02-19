#ifndef NODE_H
#define NODE_H

#include <list>
using std::list;

enum NodeType{
	Parent,
	Child
};
class Node
{
public :
	Node();
	virtual ~Node();
	char name[256];
	NodeType nType;
	void setParent(Node * parent);
	Node* getParent();

	static list <Node*> &getNodes();
	list <Node*> &getChildren();
	static list <Node*> nodes;
protected:
	Node* parent;
	list <Node*> children;

};


#endif