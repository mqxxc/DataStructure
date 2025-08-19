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

	inline BPlusTreeNode<T>* Child(int index);
	inline void SetChild(int index, BPlusTreeNode<T>* node);

private:
	using BTreeNode<T>::m_pChild;
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

template<typename T>
inline BPlusTreeNode<T>* BPlusTreeNode<T>::Child(int index)
{
	return dynamic_cast<BPlusTreeNode<T>*>(m_pChild[index]);
}

template<typename T>
inline void BPlusTreeNode<T>::SetChild(int index, BPlusTreeNode<T>* node)
{
	m_pChild[index] = node;
}
