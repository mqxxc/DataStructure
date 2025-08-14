#pragma once
#include <vector>
#ifndef B_TREE_HALF_DEGREE
#define B_TREE_HALF_DEGREE 2
#endif // B_TREE_DEGREE

constexpr int degree_ = B_TREE_HALF_DEGREE * 2;

template <typename T>
class BTreeNode
{
public:
	std::vector<T> m_key;
	BTreeNode* m_pChild[degree_];
	bool m_bLeaf;

	BTreeNode(bool bLeaf = false);
	virtual ~BTreeNode();

	int TreeDegree();
	void Clear();
};

template<typename T>
inline BTreeNode<T>::BTreeNode(bool bLeaf)
{
	m_key.reserve(degree_ - 1);
	m_bLeaf = bLeaf;

	for (int i = 0; i < degree_; ++i)
	{
		m_pChild[i] = nullptr;
	}
}

template<typename T>
inline BTreeNode<T>::~BTreeNode()
{
	for (int i = 0; i < degree_; ++i)
	{
		if(m_pChild[i] != nullptr)
			delete m_pChild[i];
	}
}

template<typename T>
inline int BTreeNode<T>::TreeDegree()
{
	return degree_;
}

template<typename T>
inline void BTreeNode<T>::Clear()
{
	m_key.clear();
	for (int i = 0; i < degree_; ++i)
	{
		m_pChild[i] = nullptr;
	}
}

