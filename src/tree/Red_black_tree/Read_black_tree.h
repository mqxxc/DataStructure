#pragma once
class Node {

private:
	/*
	*
	* trueΪ�ڣ�falseΪ��
	* 
	*/
	bool color;
	int value;
	Node* parent;
	Node* left_son;
	Node* right_son;

public:
	Node(Node* parent,int value);
	~Node();
	void blacken();
	void redden();
	void setParent(Node* parent);
};

class Read_black_tree {

private:
	Node* root;
	
public:
	void append(int value);
	void find(int value);
	void deValue(int value);
private:

};