#pragma once
#include "BTreeNode.hpp"

//B+树节点

template<typename T, int degree>
class BPlusTreeNode : public BTreeNode<T, degree>
{
public:
	BPlusTreeNode* m_prev;		//仅供叶子节点索引使用
	BPlusTreeNode* m_next;

	BPlusTreeNode(bool bLeaf = false);
	~BPlusTreeNode();

	inline BPlusTreeNode<T, degree>* Child(int index);
	inline void SetChild(int index, BPlusTreeNode<T, degree>* node);

private:
	using BTreeNode<T, degree>::m_pChild;
};

template<typename T, int degree>
inline BPlusTreeNode<T, degree>::BPlusTreeNode(bool bLeaf)
	: BTreeNode<T, degree>(bLeaf)
{
}

template<typename T, int degree>
inline BPlusTreeNode<T, degree>::~BPlusTreeNode()
{
}

template<typename T, int degree>
inline BPlusTreeNode<T, degree>* BPlusTreeNode<T, degree>::Child(int index)
{
	return dynamic_cast<BPlusTreeNode<T>*>(m_pChild[index]);
}

template<typename T, int degree>
inline void BPlusTreeNode<T, degree>::SetChild(int index, BPlusTreeNode<T, degree>* node)
{
	m_pChild[index] = node;
}
