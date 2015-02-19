#include"Node.h"

list <Node*> Node::nodes;


Node::Node()
{
	nType = Parent;
	parent = NULL;
	nodes.push_back(this);
}
Node::~Node()
{
	nodes.remove(this);
}

void Node::setParent(Node * _parent)
{
	if (_parent == NULL && parent == NULL)
		return;
	if (_parent == NULL && parent != NULL)
	{
		nType = Parent;
		parent->children.remove(this);
		parent = NULL;
	}
	
	else if (_parent->getParent() != this)
	{
		nType = Child;
		parent = _parent;
		parent->children.push_back(this);        
	}
}
Node* Node::getParent()
{
	if (parent == NULL)
		return NULL;
	return parent;
}

list<Node *>&Node::getNodes()
{
	return nodes;
}
list<Node *>&Node::getChildren()
{
	return children;
}