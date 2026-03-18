#pragma once
#include "../Nodes/BPlusTreeNode.hpp"

#include<list>
#ifdef _DEBUG
#include <iostream>
#include <queue>
#endif // _DEBUG

template<typename T, int degree = 4>
class BPlusTree
{
public:
	BPlusTree<T, degree>();
	~BPlusTree();

	bool Insert(const T& value);
	bool DeleteAt(const T& value);
	bool Find(const T& value) const;

	static int KeyMinLimitation();

#ifdef _DEBUG
	void Printf();
	void PrintfForLeaf();		//使用叶子节点的上下游指针查看数据
#endif
	

private:
	BPlusTreeNode<T, degree>* m_pRoot;

	/*
	* node:当前节点的父节点
	* childrenIndex:当前节点在父节点中的索引
	*/
	void SplittingChildren(BPlusTreeNode<T, degree>* node, int childrenIndex);		//分裂子结点
	int RemoveNodeKey(BPlusTreeNode<T, degree>* node, const T& value);				//删除关键字
	std::list<std::pair<BPlusTreeNode<T, degree>*, int>> TraceFromValue(const T& value);	//返回value的踪迹
	void MergeChildren(BPlusTreeNode<T, degree>* parentNode, int keyIndex);			//合并子结点

	int StandardAlong(BPlusTreeNode<T, degree>* node, int index);					//标准化
};

template<typename T, int degree>
inline BPlusTree<T, degree>::BPlusTree()
{
	m_pRoot = new BPlusTreeNode<T, degree>(true);
}

template<typename T, int degree>
inline BPlusTree<T, degree>::~BPlusTree()
{
	delete m_pRoot;
}

template<typename T, int degree>
bool BPlusTree<T, degree>::Insert(const T& value)
{
	if (m_pRoot->m_key.size() == m_pRoot->m_key.capacity())
	{//增高
		BPlusTreeNode<T, degree>* node = new BPlusTreeNode<T, degree>(false);
		node->SetChild(0, m_pRoot);
		m_pRoot = node;

		SplittingChildren(m_pRoot, 0);
	}

	BPlusTreeNode<T, degree>* node = m_pRoot;

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

template<typename T, int degree>
bool BPlusTree<T, degree>::DeleteAt(const T& value)
{
	auto undergone = TraceFromValue(value);

	if (undergone.empty())
	{
		return false;
	}

	const auto endPair = undergone.back();
	const auto tagNdoe = endPair.first->Child(endPair.second);
	int pos = RemoveNodeKey(tagNdoe, value);		//删除后节点中的关键字可能已经不满足限制

	//删除节点
	/*
	* 1:向上遍历替换所有删除的索引值
	* 2:向上遍历查找所有不满足条件的节点进行合并
	*/

	//old,new
	std::list<std::pair<T, T>> reValue;
	int keyMinLimit = KeyMinLimitation();
	for (auto it = undergone.rbegin(); it != undergone.rend(); ++it)
	{
		auto parentNode = it->first;
		auto curNode = parentNode->Child(it->second);
		
		if (curNode->m_key.size() >= keyMinLimit)
		{//从底向上皆满足条件停止回溯
			break;
		}

		bool curNodeLeaf = curNode->m_bLeaf;		//处理后该节点可能已经被销毁

		int res = StandardAlong(parentNode, it->second);

		if (curNodeLeaf)
		{
			switch (res)
			{
			case 1:
			{//更新索引节点中第二个key或者value的key值
				reValue.push_back({ (pos == 0 ? value : curNode->m_key.at(1)),
					curNode->m_key.front() });
			}break;
			case 2:
			{//更新索引节点中值为value的key值以及该叶子节点中收纳的key值(即最后一个key)
				if (pos == 0)
				{
					reValue.push_back({ value, curNode->m_key.front() });
				}
			}break;
			case 3:
			{//如果删除的是叶子节点中的第一个则进行更新
				if (pos == 0)
				{
					reValue.push_back({ value, curNode->m_key.front() });
				}
			}break;
			case 4://无需更新索引
			default: break;
			}
		}
		else
		{//需要更新回溯路径中的指针,指针位置已经不重要了
			if (res == 4)
			{
				--(it->second);
				auto tempIt = it;
				--tempIt;
				tempIt->first = it->first->Child(it->second);
			}

			if (parentNode == m_pRoot
				&& (res == 4 || res == 3)
				&& m_pRoot->m_key.empty())	
			{//只剩下一个孩子减高
				auto tempNode = m_pRoot->Child(0);
				m_pRoot->Clear();
				delete m_pRoot;
				m_pRoot = tempNode;
				m_pRoot->m_bLeaf = false;
				undergone.pop_front();
				break;
			}
		}
	}

	if (!reValue.empty())
	{
		for (auto it = undergone.rbegin(); it != undergone.rend(); ++it)
		{//本次循环将所有索引值更新
			const auto curNode = it->first;
			for (auto tagValue = reValue.begin(); tagValue != reValue.end();)
			{
				auto tagIndex = std::find(curNode->m_key.begin(),
					curNode->m_key.end(),
					tagValue->first);

				if (tagIndex != curNode->m_key.end())
				{
					(*tagIndex) = tagValue->second;
					//索引节点仅仅只会出现一次因为索引节点的值一般为其右子树中的最小值
					tagValue = reValue.erase(tagValue);
					continue;
				}

				++tagValue;
			}

			if (reValue.empty())
			{
				break;
			}
		}
	}

	return true;
}

template<typename T, int degree>
bool BPlusTree<T, degree>::Find(const T& value) const
{
	BTreeNode<T, degree>* node = m_pRoot;

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
template<typename T, int degree>
inline void BPlusTree<T, degree>::Printf()
{
	std::queue<BPlusTreeNode<T, degree>*> queue;
	queue.push(m_pRoot);

	while (!queue.empty())
	{
		std::queue<BPlusTreeNode<T, degree>*> copyQueue;
		queue.swap(copyQueue);
		
		while (!copyQueue.empty())
		{
			BPlusTreeNode<T, degree>* node = copyQueue.front();
			copyQueue.pop();
			for (int i = 0; i < node->TreeDegree(); ++i)
			{
				auto it = node->Child(i);
				if (it == nullptr)
					break;

				queue.push(it);
			}
			
			for (auto it : node->m_key)
			{
				std::cout << it << "     ";
			}

			if (!copyQueue.empty())
			{
				std::cout << "|     ";
			}
		}
		std::cout << std::endl;
		std::cout << "-------------------------------------" << std::endl;
	}
}

template<typename T, int degree>
inline void BPlusTree<T, degree>::PrintfForLeaf()
{
	//先找到首个叶子节点
	BPlusTreeNode<T, degree>* firstLeaf = m_pRoot;

	while (firstLeaf != nullptr
		&& !firstLeaf->m_bLeaf)
	{
		//firstLeaf = firstLeaf->Child(0);

		auto tempPtr = firstLeaf->Child(0);
		for (int i = 0; i < degree; ++i)
		{
			if (firstLeaf->Child(i) == nullptr)
			{
				break;
			}
			tempPtr = firstLeaf->Child(i);
		}

		firstLeaf = tempPtr;
	}


	while (firstLeaf != nullptr)
	{
		//显示数据
		for (auto& it  : firstLeaf->m_key)
		{
			std::cout << it << "     ";
		}
		std::cout << "|     ";
		//firstLeaf = firstLeaf->m_next;
		firstLeaf = firstLeaf->m_prev;
	}
}
#endif

template<typename T, int degree>
inline int BPlusTree<T, degree>::KeyMinLimitation()
{
	BPlusTreeNode<T, degree> node;
	return (int)node.TreeDegree() / 2 - 1;
}


template<typename T, int degree>
void BPlusTree<T, degree>::SplittingChildren(BPlusTreeNode<T, degree>* node, int childrenIndex)
{
	BPlusTreeNode<T, degree>* left = node->Child(childrenIndex);
	BPlusTreeNode<T, degree>* right = new BPlusTreeNode<T, degree>(left->m_bLeaf);
	int nIndex = (int)left->m_key.size() / 2;

	if (left->m_bLeaf)
	{//中间结点不需要单独拎出来放到父结点而是将值复制到父结点中
		right->m_key.insert(right->m_key.begin(), left->m_key.end() - nIndex - 1, left->m_key.end());

		//更新叶子的上下游指针
		right->m_prev = left;
		right->m_next = left->m_next;
		if (left->m_next != nullptr)
		{
			left->m_next->m_prev = right;
		}
		
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

template<typename T, int degree>
void BPlusTree<T, degree>::MergeChildren(BPlusTreeNode<T, degree>* parentNode, int keyIndex)
{
	BPlusTreeNode<T, degree>* leftNode = parentNode->Child(keyIndex);
	BPlusTreeNode<T, degree>* rightNode = parentNode->Child(keyIndex + 1);

	if (!leftNode->m_bLeaf)
	{
		int leftChildEnd = static_cast<int>(leftNode->m_key.size() + 1);

		leftNode->m_key.push_back(std::move(parentNode->m_key[keyIndex]));
		leftNode->m_key.insert(leftNode->m_key.end(), rightNode->m_key.begin(), rightNode->m_key.end());

		int nChildSize = static_cast<int>(rightNode->m_key.size() + 1);
		for (int i = 0; i < nChildSize; ++i)
		{
			leftNode->SetChild(i + leftChildEnd, rightNode->Child(i));
		}
	}
	else
	{
		leftNode->m_key.insert(leftNode->m_key.end(), rightNode->m_key.begin(), rightNode->m_key.end());
		//更新索引值
		if (rightNode->m_next != nullptr)
		{
			rightNode->m_next->m_prev = leftNode;
			leftNode->m_next = rightNode->m_next;
		}
		else
		{
			leftNode->m_next = nullptr;
		}
	}

	rightNode->Clear();
	delete rightNode;

	for (int i = keyIndex + 1; i < parentNode->m_key.size(); ++i)
		parentNode->SetChild(i, parentNode->Child(i + 1));

	parentNode->SetChild((int)parentNode->m_key.size(), nullptr);
	parentNode->m_key.erase(parentNode->m_key.begin() + keyIndex);
}

template<typename T, int degree>
inline int BPlusTree<T, degree>::RemoveNodeKey(BPlusTreeNode<T, degree>* node, const T& value)
{
	auto index = std::find(node->m_key.begin(), node->m_key.end(), value);

	if (index == node->m_key.end())
	{
		return -1;
	}

	int pos = static_cast<int>(index - node->m_key.begin());
	node->m_key.erase(index);
	return pos;
}

template<typename T, int degree>
inline std::list<std::pair<BPlusTreeNode<T, degree>*, int>> BPlusTree<T, degree>::TraceFromValue(const T& value)
{
	//回溯需要存储经过的节点指针以及在节点中的index方便后续查找兄弟节点
	std::list<std::pair<BPlusTreeNode<T, degree>*, int>> undergone;

	BPlusTreeNode<T, degree>* node = m_pRoot;
	int index = -1;
	bool find = false;
	//先查找对应的值的节点
	while (node != nullptr)
	{
		index = (int)node->m_key.size();
		for (int i = 0; i < node->m_key.size(); ++i)
		{
			if (node->m_key[i] > value)
			{//在该节点子树下
				index = i;
				break;
			}
			else if (node->m_key[i] == value)
			{//找到
				find = true;
				index = i + 1;	//在索引节点中找到是该节点的右子子树
				break;
			}
		}

		if (find)
		{//找到了节点
			if (node->m_bLeaf)
			{//叶子节点
				return undergone;
				break;
			}
		}

		undergone.push_back({ node, index });
		node = node->Child(index);
	}

	return std::list<std::pair<BPlusTreeNode<T, degree>*, int>>();
}

template<typename T, int degree>
int BPlusTree<T, degree>::StandardAlong(BPlusTreeNode<T, degree>* node, int index)
{
	int limitation = KeyMinLimitation();
	BPlusTreeNode<T, degree>* leftNode = nullptr;
	BPlusTreeNode<T, degree>* rightNode = nullptr;
	BPlusTreeNode<T, degree>* posNode = node->Child(index);

	if (index > 0)
		leftNode = node->Child(index - 1);

	if (index < node->m_key.size())
		rightNode = node->Child(index + 1);

	if (leftNode != nullptr 
		&& leftNode->m_key.size() > limitation)
	{//左兄弟可以借到
		if (!posNode->m_bLeaf)
		{
			int posIndex = index - 1;

			posNode->m_key.insert(posNode->m_key.begin(), std::move(node->m_key[posIndex]));
			node->m_key[posIndex] = std::move(leftNode->m_key.back());
			leftNode->m_key.pop_back();

			for (int i = (int)posNode->m_key.size(); i > 0; --i)
				posNode->SetChild(i, posNode->Child(i - 1));

			posNode->SetChild(0, leftNode->Child((int)leftNode->m_key.size() + 1));
			leftNode->SetChild((int)leftNode->m_key.size() + 1, nullptr);
		}
		else
		{//叶子节点
			posNode->m_key.insert(posNode->m_key.begin(), std::move(leftNode->m_key.back()));
			leftNode->m_key.pop_back();
			//node->m_key.erase(node->m_key.begin() + index - 1);
		}
		return 1;		//叶子节点借左兄弟的key需要更新该节点父节点缝隙左边的索引key值
	}

	if (rightNode != nullptr 
		&& rightNode->m_key.size() > limitation)
	{//右兄弟可以借到
		if (!posNode->m_bLeaf)
		{
			posNode->m_key.push_back(std::move(node->m_key[index]));
			node->m_key[index] = std::move(rightNode->m_key.front());
			rightNode->m_key.erase(rightNode->m_key.begin());

			posNode->SetChild((int)posNode->m_key.size(), rightNode->Child(0));

			for (int i = 0; i < rightNode->m_key.size() + 2; ++i)
				rightNode->SetChild(i, rightNode->Child(i + 1));
		}
		else
		{//叶子节点
			posNode->m_key.push_back(std::move(rightNode->m_key.front()));
			rightNode->m_key.erase(rightNode->m_key.begin());
			//node->m_key.erase(node->m_key.begin() + index);
		}
		return 2;		//叶子节点借右兄弟的key需要更新该节点父节点缝隙右边边的索引key值
	}

	//合并节点
	if (rightNode != nullptr)
	{
		MergeChildren(node, index);
		return 3;	//合并叶子节点(与右兄弟节点)	
	}
	else
	{
		MergeChildren(node, index - 1);
		return 4;	//合并叶子节点(与左兄弟节点)
	}
}