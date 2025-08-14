#pragma once
#include "BTreeNode.h"

template<typename T>
class BPlusTreeNode : public BTreeNode<T>
{
public:
	BPlusTreeNode* m_prev;
	BPlusTreeNode* m_next;

	BPlusTreeNode(bool bLeaf = false);
	~BPlusTreeNode();
};

template<typename T>
inline BPlusTreeNode<T>::BPlusTreeNode(bool bLeaf)
	: BTreeNode<T>(bLeaf)
{
}

template<typename T>
inline BPlusTreeNode<T>::~BPlusTreeNode()
{
}
