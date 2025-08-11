#include "Read_black_tree.h"

Node::Node(Node* parent,int value) {
	this->parent = parent;
	left_son = nullptr;
	right_son = nullptr;
	color = false;
	this->value = value;
}

Node::~Node(){
	if (left_son != nullptr) {
		delete left_son;
		left_son = nullptr;
	}
	if (right_son != nullptr) {
		delete right_son;
		right_son = nullptr;
	}
}

void Node::blacken(){
	color = true;
}

void Node::redden(){
	color=false;
}

void Node::setParent(Node* parent){
	this->parent = parent;
}

void Read_black_tree::append(int value){

}
