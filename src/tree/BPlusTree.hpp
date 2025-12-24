#pragma once
#include "../Nodes/BPlusTreeNode.hpp"
#ifdef _DEBUG
#include <iostream>
#include <queue>
#endif // _DEBUG

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

	void SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex);		//分裂子结点
	void MergeChildren(BPlusTreeNode<T>* parentNode, int keyIndex);			//合并子结点
	T RemoveNodeKey(BPlusTreeNode<T>* node, int& index);					//删除关键字
	BPlusTreeNode<T>* StandardAlong(BPlusTreeNode<T>* node, int index);		//标准化
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
	{//增高
		BPlusTreeNode<T>* node = new BPlusTreeNode<T>(false);
		node->SetChild(0, m_pRoot);
		m_pRoot = node;

		SplittingChildren(m_pRoot, 0);
	}

	BPlusTreeNode<T>* node = m_pRoot;

	while (node != nullptr)
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

#ifdef _DEBUG
template<typename T>
inline void BPlusTree<T>::Printf()
{
	std::queue<BPlusTreeNode<T>*> queue;
	queue.push(m_pRoot);

	while (!queue.empty())
	{
		BPlusTreeNode<T>* node = queue.front();
		queue.pop();
		for(int i = 0; i < node->TreeDegree(); ++i)
		{
			auto it = node->Child(i);
			if (it == nullptr)
				break;

			queue.push(it);
		}

		std::cout << "-------------------------------------" << std::endl;
		for (auto it : node->m_key)
		{
			std::cout << it << "     ";
		}
		std::cout << std::endl;
		std::cout << "*************************************" << std::endl;
	}

	std::cout << "real data:" << std::endl;
}
#endif

template<typename T>
void BPlusTree<T>::SplittingChildren(BPlusTreeNode<T>* node, int childrenIndex)
{
	BPlusTreeNode<T>* left = node->Child(childrenIndex);
	BPlusTreeNode<T>* right = new BPlusTreeNode<T>(left->m_bLeaf);
	int nIndex = (int)left->m_key.size() / 2;

	if (left->m_bLeaf)
	{//中间结点不需要单独拎出来放到父结点而是将值复制到父结点中
		right->m_key.insert(right->m_key.begin(), left->m_key.end() - nIndex - 1, left->m_key.end());

		//更新叶子的上下游指针
		right->m_prev = left;
		right->m_next = left->m_next;
		left->m_next = right;
	}
	else
	{//当需要分裂的结点不是叶子时，走和B树一样的流程即可
		right->m_key.insert(right->m_key.begin(), left->m_key.end() - nIndex, left->m_key.end());

		++nIndex;
		for (int i = 0; i < nIndex; ++i)
		{
			right->SetChild(i, left->Child(nIndex + i));
			left->SetChild(nIndex + i, nullptr);
		}
		--nIndex;
	}

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
	left->m_key.erase(left->m_key.begin() + nIndex, left->m_key.end());

	//插入父节点孩子指针
	for (int i = (int)node->m_key.size(); i > childrenIndex; --i)
		node->SetChild(i, node->Child(i - 1));

	node->SetChild(childrenIndex + 1, right);
	node->m_bLeaf = false;
}

template<typename T>
void BPlusTree<T>::MergeChildren(BPlusTreeNode<T>* parentNode, int keyIndex)
{
	BPlusTreeNode<T>* leftNode = parentNode->Child(keyIndex);
	BPlusTreeNode<T>* rightNode = parentNode->Child(keyIndex + 1);

	if (leftNode->m_bLeaf)
	{
		leftNode->m_key.insert(leftNode->m_key.end(), rightNode->m_key.begin(), rightNode->m_key.end());
		if (rightNode->m_next != nullptr)
		{
			rightNode->m_next->m_prev = leftNode;
			leftNode->m_next = rightNode->m_next;
		}
	}
	else
	{//非叶子结点合并时与
		int pos = (int)leftNode->m_key.size() + 1;
		leftNode->m_key.push_back(parentNode->m_key[keyIndex]);
		leftNode->m_key.insert(leftNode->m_key.end(), rightNode->m_key.begin(), rightNode->m_key.end());

		for (int i = 0; i < rightNode->m_key.size() + 1; ++i)
			leftNode->SetChild(pos + i, rightNode->Child(i));
	}

	rightNode->Clear();
	delete rightNode;

	for (int i = keyIndex + 1; i < parentNode->m_key.size(); ++i)
		parentNode->SetChild(i, parentNode->Child(i + 1));

	parentNode->SetChild((int)parentNode->m_key.size(), nullptr);
	parentNode->m_key.erase(parentNode->m_key.begin() + keyIndex);
}

template<typename T>
T BPlusTree<T>::RemoveNodeKey(BPlusTreeNode<T>* node, int& index)
{
	int limitation = (int)node->TreeDegree() / 2 - 1;
	if (node->Child(index)->m_key.size() > limitation)
	{//左节点满足
		BPlusTreeNode<T>* tagNode = node->Child(index);
		while (!tagNode->m_bLeaf)
			tagNode = tagNode->Child((int)tagNode->m_key.size());

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

			posNode->SetChild(0, leftNode->Child((int)leftNode->m_key.size() + 1));
			leftNode->SetChild((int)leftNode->m_key.size() + 1, nullptr);
		}
	}
	else if (rightNode != nullptr && rightNode->m_key.size() > limitation)
	{
		posNode->m_key.push_back(std::move(node->m_key[index]));
		node->m_key[index] = std::move(rightNode->m_key.front());
		rightNode->m_key.erase(rightNode->m_key.begin());

		if (!posNode->m_bLeaf)
		{
			posNode->SetChild((int)posNode->m_key.size(), rightNode->Child(0));

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