#pragma once
#include <vector>
//BÊ÷½Úµã

template <typename T, int degree>
class BTreeNode
{
public:
	std::vector<T> m_key;
	BTreeNode* m_pChild[degree];
	bool m_bLeaf;

	BTreeNode(bool bLeaf = false);
	virtual ~BTreeNode();

	int TreeDegree();
	void Clear();
};

template<typename T, int degree>
inline BTreeNode<T, degree>::BTreeNode(bool bLeaf)
{
	m_key.reserve(degree_ - 1);
	m_bLeaf = bLeaf;

	for (int i = 0; i < degree_; ++i)
	{
		m_pChild[i] = nullptr;
	}
}

template<typename T, int degree>
inline BTreeNode<T, degree>::~BTreeNode()
{
	for (int i = 0; i < degree_; ++i)
	{
		if(m_pChild[i] != nullptr)
			delete m_pChild[i];
	}
}

template<typename T, int degree>
inline int BTreeNode<T, degree>::TreeDegree()
{
	return degree_;
}

template<typename T, int degree>
inline void BTreeNode<T, degree>::Clear()
{
	m_key.clear();
	for (int i = 0; i < degree_; ++i)
	{
		m_pChild[i] = nullptr;
	}
}

