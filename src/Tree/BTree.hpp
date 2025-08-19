#pragma once
#include "../Nodes/BTreeNode.h"
#ifdef _DEBUG
#include <iostream>
#include <queue>
#endif // _DEBUG


//b树(不允许重复key，插入重复key时什么也不做)
template <typename T>
class BTree
{
public:
	BTree();
	~BTree();

	bool Insert(T value);
	bool DeleteAt(T value);
	bool Find(T Value);

#ifdef _DEBUG
	void Printf();
#endif

private:
	BTreeNode<T>* m_pRoot;
	
	void SplittingChildren(BTreeNode<T>* node, int childrenIndex);
	void MergeChildren(BTreeNode<T>* parentNode, int keyIndex);
	T RemoveNodeKey(BTreeNode<T>* node, int& index);
	BTreeNode<T>* StandardAlong(BTreeNode<T>* node, int index);
};

template<typename T>
inline BTree<T>::BTree()
{
	m_pRoot = new BTreeNode<T>(true);
}

template<typename T>
inline BTree<T>::~BTree()
{
	delete m_pRoot;
}

template<typename T>
inline bool BTree<T>::Insert(T value)
{
	if (m_pRoot->m_key.size() == m_pRoot->m_key.capacity())
	{//增高
		BTreeNode<T>* node = new BTreeNode<T>(false);
		node->m_pChild[0] = m_pRoot;

		m_pRoot = node;

		SplittingChildren(m_pRoot, 0);
	}

	BTreeNode<T>* node = m_pRoot;

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
		else if (node->m_pChild[index]->m_key.size() == node->m_pChild[index]->m_key.capacity())
		{
			SplittingChildren(node, index);

			if (value > node->m_key[index])
			{
				node = node->m_pChild[index + 1];
				continue;
			}
			else if (value == node->m_key[index])
				return false;
		}

		node = node->m_pChild[index];
	}
	return false;
}

/*
在删除B树节点时，为了避免回溯，当遇到需要合并的节点时就立即执行合并，B树的删除算法如下：从root向叶子节点按照search规律遍历：
（1）  如果target在叶节点x中，则直接从x中删除target，情况（2）和（3）会保证当在叶子节点找到target时，肯定能借节点或合并成功而不会引起父节点的关键字个数少于t - 1。
（2）  如果target在分支节点x中：
	（a）  如果x的左分支节点y至少包含t个关键字，则找出y的最右的关键字prev，并替换target，并在y中递归删除prev。
	（b）  如果x的右分支节点z至少包含t个关键字，则找出z的最左的关键字next，并替换target，并在z中递归删除next。
	（c）  否则，如果y和z都只有t - 1个关键字，则将targe与z合并到y中，使得y有2t - 1个关键字，再从y中递归删除target。
（3）  如果关键字不在分支节点x中，则必然在x的某个分支节点p[i]中，如果p[i]节点只有t - 1个关键字。
	（a）  如果p[i - 1]拥有至少t个关键字，则将x的某个关键字降至p[i]中，将p[i - 1]的最大节点上升至x中。
	（b）  如果p[i + 1]拥有至少t个关键字，则将x个某个关键字降至p[i]中，将p[i + 1]的最小关键字上升至x个。
	（c）  如果p[i - 1]与p[i + 1]都拥有t - 1个关键字，则将p[i]与其中一个兄弟合并，将x的一个关键字降至合并的节点中，成为中间关键字。
*
*/

template<typename T>
inline bool BTree<T>::DeleteAt(T value)
{
	int limitation = m_pRoot->TreeDegree() / 2 - 1;
	if (m_pRoot->m_key.size() == 1 &&
		m_pRoot->m_pChild[0] != nullptr &&
		m_pRoot->m_pChild[1] != nullptr &&
		(int)m_pRoot->m_pChild[0]->m_key.size() == limitation &&
		(int)m_pRoot->m_pChild[1]->m_key.size() == limitation)
	{//合并并减少层数
		MergeChildren(m_pRoot, 0);

		BTreeNode<T>* temp = m_pRoot;
		m_pRoot = m_pRoot->m_pChild[0];
		delete temp;
	}

	BTreeNode<T>* node = m_pRoot;

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
				return true;
			}
			value = RemoveNodeKey(node, index);
		}
		else if (node->m_pChild[index]->m_key.size() == limitation)
		{
			
			node = StandardAlong(node, index);
			continue;
		}
		
		node = node->m_pChild[index];
	}

	return false;
}

template<typename T>
inline bool BTree<T>::Find(T Value)
{
	BTreeNode<T>* node = m_pRoot;
	
	while (node != nullptr)
	{
		int keyNum = (int)node->m_key.size();
		int index = keyNum;
		for (int i = 0; i < keyNum; ++i)
		{
			if (node->m_key[i] == Value)
				return true;
			else if (node->m_key[i] > Value)
			{
				index = i;
				break;
			}
		}
		node = node->m_pChild[index];
	}

	return false;
}

#ifdef _DEBUG
template<typename T>
inline void BTree<T>::Printf()
{
	std::queue<BTreeNode<T>*> queue;
	queue.push(m_pRoot);

	while (!queue.empty())
	{
		BTreeNode<T>* node = queue.front();
		queue.pop();
		for (auto it : node->m_pChild)
		{
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
}
#endif

template<typename T>
inline void BTree<T>::SplittingChildren(BTreeNode<T>* node, int childrenIndex)
{
	BTreeNode<T>* children = node->m_pChild[childrenIndex];

	//分裂子节点为左右节点
	BTreeNode<T>* right = new BTreeNode<T>(true);
	right->m_bLeaf = children->m_bLeaf;
	int index = (int)children->m_key.size() / 2;

	for (int i = index + 1; i < children->m_key.size(); ++i)
		right->m_key.push_back(children->m_key[i]);
	
	if (!children->m_bLeaf)
	{
		++index;
		for (int i = 0; i <= index; ++i)
		{
			right->m_pChild[i] = children->m_pChild[index + i];
			children->m_pChild[index + i] = nullptr;
		}
		--index;
	}

	//将中间节点插入到父节点中
	int InsterIndex = (int)node->m_key.size();
	for (int i = 0; i < node->m_key.size(); ++i)
	{
		if (node->m_key[i] > children->m_key[index])
		{
			InsterIndex = i;
			break;
		}
	}
	node->m_key.insert(node->m_key.begin() + InsterIndex, children->m_key[index]);
	children->m_key.erase(children->m_key.begin() + index, children->m_key.end());

	//插入父节点孩子指针
	for (int i = (int)node->m_key.size(); i > childrenIndex; --i)
		node->m_pChild[i] = node->m_pChild[i - 1];

	node->m_pChild[childrenIndex + 1] = right;
	node->m_bLeaf = false;
}

template<typename T>
inline void BTree<T>::MergeChildren(BTreeNode<T>* parentNode, int keyIndex)
{
	BTreeNode<T>* leftNode = parentNode->m_pChild[keyIndex];
	BTreeNode<T>* rightNode = parentNode->m_pChild[keyIndex + 1];

	int pos = (int)leftNode->m_key.size() + 1;
	leftNode->m_key.push_back(parentNode->m_key[keyIndex]);
	leftNode->m_key.insert(leftNode->m_key.end(), rightNode->m_key.begin(), rightNode->m_key.end());

	for (int i = 0; i < rightNode->m_key.size() + 1; ++i)
		leftNode->m_pChild[pos + i] = rightNode->m_pChild[i];

	rightNode->Clear();
	delete rightNode;

	for (int i = keyIndex + 1; i < parentNode->m_key.size(); ++i)
		parentNode->m_pChild[i] = parentNode->m_pChild[i + 1];

	parentNode->m_pChild[parentNode->m_key.size()] = nullptr;
	parentNode->m_key.erase(parentNode->m_key.begin() + keyIndex);
}

template<typename T>
inline T BTree<T>::RemoveNodeKey(BTreeNode<T>* node, int& index)
{
	int limitation = (int)node->TreeDegree() / 2 - 1;
	if (node->m_pChild[index]->m_key.size() > limitation)
	{//左节点满足
		BTreeNode<T>* tagNode = node->m_pChild[index];
		while (!tagNode->m_bLeaf)
			tagNode = tagNode->m_pChild[tagNode->m_key.size()];

		node->m_key[index] = tagNode->m_key.back();
		return node->m_key[index];
	}

	if (node->m_pChild[index + 1]->m_key.size() > limitation)
	{//右节点满足
		BTreeNode<T>* tagNode = node->m_pChild[index + 1];
		while (!tagNode->m_bLeaf)
			tagNode = tagNode->m_pChild[0];

		node->m_key[index] = tagNode->m_key.front();
		return node->m_key[index++];
	}
	
	//左右都不满足合并节点
	T temp = node->m_key[index];
	MergeChildren(node, index);
	return temp;
}

template<typename T>
inline BTreeNode<T>* BTree<T>::StandardAlong(BTreeNode<T>* node, int index)
{
	int limitation = m_pRoot->TreeDegree() / 2 - 1;
	BTreeNode<T>* leftNode = nullptr;
	BTreeNode<T>* rightNode = nullptr;
	BTreeNode<T>* posNode = node->m_pChild[index];

	if (index > 0)
		leftNode = node->m_pChild[index - 1];

	if (index < node->m_key.size())
		rightNode = node->m_pChild[index + 1];

	if (leftNode != nullptr && leftNode->m_key.size() > limitation)
	{
		int posIndex = index - 1;

		posNode->m_key.insert(posNode->m_key.begin(), std::move(node->m_key[posIndex]));
		node->m_key[posIndex] = std::move(leftNode->m_key.back());
		leftNode->m_key.pop_back();

		if (!posNode->m_bLeaf)
		{
			for (int i = (int)posNode->m_key.size(); i > 0; --i)
				posNode->m_pChild[i] = posNode->m_pChild[i - 1];

			posNode->m_pChild[0] = leftNode->m_pChild[leftNode->m_key.size() + 1];
			leftNode->m_pChild[leftNode->m_key.size() + 1] = nullptr;
		}
	}
	else if (rightNode != nullptr && rightNode->m_key.size() > limitation)
	{
		posNode->m_key.push_back(std::move(node->m_key[index]));
		node->m_key[index] = std::move(rightNode->m_key.front());
		rightNode->m_key.erase(rightNode->m_key.begin());

		if (!posNode->m_bLeaf)
		{
			posNode->m_pChild[posNode->m_key.size()] = rightNode->m_pChild[0];

			for (int i = 0; i < rightNode->m_key.size() + 2; ++i)
				rightNode->m_pChild[i] = rightNode->m_pChild[i + 1];
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
			return node->m_pChild[index - 1];
		}
	}
	return node;
}
