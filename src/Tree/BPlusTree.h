#pragma once
#include "../Nodes/BPlusTreeNode.h"

template<typename T>
class BPlusTree
{
public:
	BPlusTree<T>();
	~BPlusTree();

	bool Insert(T value);
	bool DeleteAt(T value);
	bool Find(T value);

#ifdef _DEBUG
	void Printf();
#endif

private:
	BPlusTreeNode<T>* m_pRoot;

	void SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex);
};

template<typename T>
inline BPlusTree<T>::BPlusTree()
{

}

template<typename T>
inline BPlusTree<T>::~BPlusTree()
{

}

template<typename T>
bool BPlusTree<T>::Insert(T value)
{
	if (m_pRoot->m_key.size() == m_pRoot->m_key.capacity())
	{
		BPlusTreeNode<T>* node = new BPlusTreeNode<T>(false);
		node->m_pChild[0] = m_pRoot;
		node->m_key.push_back(m_pRoot->m_key.back());
		m_pRoot = node;
	}

	BPlusTreeNode<T>* node = m_pRoot;

	while (node)
	{

	}

	return false;
}

template<typename T>
void BPlusTree<T>::SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex)
{

}

template<typename T>
bool BPlusTree<T>::Find(T value)
{
	BTreeNode<T>* node = m_pRoot;

	while (node != nullptr)
	{
		int nSize = node->m_key.size();
		int nIndex = nSize;
		for (int i = 0; i < nSize; ++i)
		{
			if (node->m_key[i] == value)
			{
				if (node->m_bLeaf)
					return true;
				break;
			}
			else if (node->m_key[i] > value)
				break;
		}

		node = node->m_pChild[nIndex];
	}
	return false;
}