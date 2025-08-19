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
	bool Find(T value) const;

#ifdef _DEBUG
	void Printf();
#endif

private:
	BPlusTreeNode<T>* m_pRoot;

	void SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex);
	void MergeChildren(BPlusTreeNode<T>* parentNode, int keyIndex);
	T RemoveNodeKey(BPlusTreeNode<T>* node, int& index);
	BPlusTreeNode<T>* StandardAlong(BPlusTreeNode<T>* node, int index);
};

template<typename T>
inline BPlusTree<T>::BPlusTree()
{
	m_pRoot = new BPlusTreeNode<T>(true);
}

template<typename T>
inline BPlusTree<T>::~BPlusTree()
{
	delete m_pRoot;
}

template<typename T>
bool BPlusTree<T>::Insert(T value)
{
	if (m_pRoot->m_key.size() == m_pRoot->m_key.capacity())
	{
		BPlusTreeNode<T>* node = new BPlusTreeNode<T>(false);
		node->SetChild(0, m_pRoot);
		m_pRoot = node;

		SplittingChildren(m_pRoot, 0);
	}

	BPlusTreeNode<T>* node = m_pRoot;

	while (node)
	{
		int keyNum = (int)node->m_key.size();
		int index = keyNum;
		for (int i = 0; i < keyNum; ++i)
		{
			if (node->m_key[i] == value)
				return false;
			else if (node->m_key[i] > value)
			{
				index = i;
				break;
			}
		}

		if (node->m_bLeaf)
		{//抵达叶子
			node->m_key.insert(node->m_key.begin() + index, value);
			return true;
		}
		else if (node->Child(index)->m_key.size() == node->Child(index)->m_key.capacity())
		{
			SplittingChildren(node, index);

			if (value > node->m_key[index])
			{
				node = node->Child(index + 1);
				continue;
			}
			else if (value == node->m_key[index])
				return false;
		}

		node = node->Child(index);
	}

	return false;
}

template<typename T>
bool BPlusTree<T>::DeleteAt(T value)
{
	int limitation = m_pRoot->TreeDegree() / 2 - 1;
	if (m_pRoot->m_key.size() == 1 &&
		m_pRoot->Child(0) != nullptr &&
		m_pRoot->Child(1) != nullptr &&
		(int)m_pRoot->Child(0)->m_key.size() == limitation &&
		(int)m_pRoot->Child(1)->m_key.size() == limitation)
	{//合并并减少层数
		MergeChildren(m_pRoot, 0);
		m_pRoot = m_pRoot->Child(0);
	}

	BPlusTreeNode<T>* node = m_pRoot;

	int index = -1;

	while (node != nullptr)
	{
		index = (int)node->m_key.size();
		bool find = false;
		for (int i = 0; i < node->m_key.size(); ++i)
		{
			if (node->m_key[i] > value)
			{
				index = i;
				break;
			}
			else if (node->m_key[i] == value)
			{
				find = true;
				index = i;
				break;
			}
		}

		if (find)
		{//找到了节点
			if (node->m_bLeaf)
			{
				node->m_key.erase(node->m_key.begin() + index);
				//如果删除的是叶子节点最大/最小值还需要更新其父节点的值
				return true;
			}
			value = RemoveNodeKey(node, index);
		}
		else if (node->Child(index)->m_key.size() == limitation)
		{

			node = StandardAlong(node, index);
			continue;
		}

		node = node->Child(index);
	}

	return false;
}

template<typename T>
bool BPlusTree<T>::Find(T value) const
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
				nIndex = i;
				break;
			}
			else if (node->m_key[i] >= value)
			{
				nIndex = i;
				break;
			}
		}

		node = node->m_pChild[nIndex];
	}
	return false;
}

template<typename T>
void BPlusTree<T>::SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex)
{
	BPlusTreeNode<T>* left = node->Child(childrenIndex);
	BPlusTreeNode<T>* right = new BPlusTreeNode<T>(left->m_bLeaf);
	int nIndex = (int)left->m_key.size() / 2;

	right->m_key.insert(right->m_key.begin(), left->m_key.end() - left->m_key.size() / 2, left->m_key.end());

	if (!left->m_bLeaf)
	{
		++nIndex;
		for (int i = 0; i < nIndex; ++i)
		{
			right->SetChild(i, left->Child(nIndex + i));
			left->SetChild(nIndex + i, nullptr);
		}
		--nIndex;

		left->m_key.resize(nIndex);
	}
	else
		left->m_key.resize(nIndex + 1);

	//将中间节点插入到父节点中
	int InsterIndex = (int)node->m_key.size();
	for (int i = 0; i < node->m_key.size(); ++i)
	{
		if (node->m_key[i] > left->m_key[nIndex])
		{
			InsterIndex = i;
			break;
		}
	}
	node->m_key.insert(node->m_key.begin() + InsterIndex, left->m_key[nIndex]);

	//插入父节点孩子指针
	for (int i = (int)node->m_key.size(); i > childrenIndex; --i)
		node->SetChild(i, node->Child(i - 1));

	node->SetChild(childrenIndex + 1, right);
	node->m_bLeaf = false;

	//更新叶子的上下游指针
	if (left->m_bLeaf)
	{
		right->m_prev = left;
		right->m_next = left->m_next;
		left->m_next = right;
	}
}

template<typename T>
void BPlusTree<T>::MergeChildren(BPlusTreeNode<T>* parentNode, int keyIndex)
{
	BPlusTreeNode<T>* leftNode = parentNode->Child(keyIndex);
	BPlusTreeNode<T>* rightNode = parentNode->Child(keyIndex + 1);
}

template<typename T>
T BPlusTree<T>::RemoveNodeKey(BPlusTreeNode<T>* node, int& index)
{
	int limitation = (int)node->TreeDegree() / 2 - 1;
	if (node->Child(index)->m_key.size() > limitation)
	{//左节点满足
		BPlusTreeNode<T>* tagNode = node->Child(index);
		while (!tagNode->m_bLeaf)
			tagNode = tagNode->Child(tagNode->m_key.size());

		node->m_key[index] = tagNode->m_key.back();
		return node->m_key[index];
	}

	if (node->Child(index + 1)->m_key.size() > limitation)
	{//右节点满足
		BPlusTreeNode<T>* tagNode = node->Child(index + 1);
		while (!tagNode->m_bLeaf)
			tagNode = tagNode->Child(0);

		node->m_key[index] = tagNode->m_key.front();
		return node->m_key[index++];
	}

	//左右都不满足合并节点
	T temp = node->m_key[index];
	MergeChildren(node, index);
	return temp;
}

template<typename T>
BPlusTreeNode<T>* BPlusTree<T>::StandardAlong(BPlusTreeNode<T>* node, int index)
{
	int limitation = m_pRoot->TreeDegree() / 2 - 1;
	BPlusTreeNode<T>* leftNode = nullptr;
	BPlusTreeNode<T>* rightNode = nullptr;
	BPlusTreeNode<T>* posNode = node->Child(index);

	if (index > 0)
		leftNode = node->Child(index - 1);

	if (index < node->m_key.size())
		rightNode = node->Child(index + 1);

	if (leftNode != nullptr && leftNode->m_key.size() > limitation)
	{
		int posIndex = index - 1;

		posNode->m_key.insert(posNode->m_key.begin(), std::move(node->m_key[posIndex]));
		node->m_key[posIndex] = std::move(leftNode->m_key.back());
		leftNode->m_key.pop_back();

		if (!posNode->m_bLeaf)
		{
			for (int i = (int)posNode->m_key.size(); i > 0; --i)
				posNode->SetChild(i, posNode->Child(i - 1));

			posNode->SetChild(0, leftNode->Child(leftNode->m_key.size() + 1));
			leftNode->SetChild(leftNode->m_key.size() + 1, nullptr);
		}
	}
	else if (rightNode != nullptr && rightNode->m_key.size() > limitation)
	{
		posNode->m_key.push_back(std::move(node->m_key[index]));
		node->m_key[index] = std::move(rightNode->m_key.front());
		rightNode->m_key.erase(rightNode->m_key.begin());

		if (!posNode->m_bLeaf)
		{
			posNode->SetChild(posNode->m_key.size(), rightNode->Child(0));

			for (int i = 0; i < rightNode->m_key.size() + 2; ++i)
				rightNode->SetChild(i, rightNode->Child(i + 1));
		}
	}
	else
	{
		if (rightNode != nullptr)
		{
			MergeChildren(node, index);
		}
		else
		{
			MergeChildren(node, index - 1);
			return node->Child(index - 1);
		}
	}
	return node;
}