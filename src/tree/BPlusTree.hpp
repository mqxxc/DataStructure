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
	BPlusTree<T>();
	~BPlusTree();

	bool Insert(const T& value);
	bool DeleteAt(const T& value);
	bool Find(const T& value) const;

	static int KeyMinLimitation();

#ifdef _DEBUG
	void Printf();
#endif
	

private:
	BPlusTreeNode<T, degree>* m_pRoot;

	/*
	* node:当前节点
	* index:分裂位置
	* return:上提的key以及该key右子节点指针
	*/
	std::pair<T, BPlusTreeNode<T, degree>*> SplittingChildren(BPlusTreeNode<T, degree>* node, int index);		//分裂子结点
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
	//查找插入位置的路径
	auto undergone = TraceFromValue(value);

	if (undergone.empty())
	{
		return false;
	}

	const auto endPair = undergone.back();
	const auto tagNdoe = endPair.first;		//该节点为叶子节点

	//叶子节点操作
	if (tagNdoe->m_key.size() < tagNdoe->TreeDegree() - 1)
	{
		tagNdoe->m_key.insert(tagNdoe->m_key.begin() + endPair.second, value);
	}
	else
	{//叶子节点满，需要进行分裂
		//处理叶子节点
		for (auto curNode = undergone.rbegin() + 1; curNode != undergone.rend(); ++curNode)
		{
			if()
		}
	}

	return true;
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
	int pos = RemoveNodeKey(tagNdoe, value);

	//删除节点
	/*
	* 1:向上遍历替换所有删除的索引值
	* 2:向上遍历查找所有不满足条件的节点进行合并
	*/

	if (pos == 0
		&& endPair.second != 0)
	{//仅当删除的关键字不在最左侧的叶子节点并且位于叶子节点第1个的时候需要更新父节点的索引值

		const auto newValue = tagNdoe->m_key.front();
		for (auto it = undergone.rbegin(); it != undergone.rend(); ++it)
		{//本次循环将所有索引值更新
			const auto curPair = (*it);
			auto tagIndex = std::find(curPair.first->m_key.begin(),
				curPair.first->m_key.end(),
				value);

			if (tagIndex != curPair.first->m_key.end())
			{
				(*tagIndex) = newValue;
				break;		//索引节点仅仅只会出现一次因为索引节点的值一般为其右子树中的最小值
			}
		}
	}
	
	//确保整满足约束
	int limitation = KeyMinLimitation();
	if (tagNdoe->m_key.size() < limitation)
	{//不满足限制触发规整化
		//叶子节点合并需要更新叶子节点之间的上下游指针
		StandardAlong(endPair.first, endPair.second);
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
inline int BPlusTree<T, degree>::KeyMinLimitation()
{
	BPlusTreeNode<T, degree> node;
	return (int)node.TreeDegree() / 2 - 1;
}
#endif

template<typename T, int degree>
std::pair<T, BPlusTreeNode<T, degree>*> BPlusTree<T, degree>::SplittingChildren(BPlusTreeNode<T, degree>* node, int index)
{
	std::pair<T, BPlusTreeNode<T, degree>*> res(T(), nullptr);
	res.first = node->m_key[index];
	res.second = new BPlusTreeNode<T, degree>(node->m_bLeaf);

	if (!node->m_bLeaf)
	{//当需要分裂的结点不是叶子时，走和B树一样的流程即可

		++index;
		res.second->m_key.insert(res.second->m_key.begin(), node->m_key.begin() + index, node->m_key.end());
		for (int i = 0; i < index; ++i)
		{
			res.second->SetChild(i, node->Child(index + i));
			node->SetChild(index + i, nullptr);
		}
		--index;
	}
	else
	{//叶子结点不需要单独将中间key拎出来放到父结点而是将值复制到父结点中
		res.second->m_key.insert(res.second->m_key.begin(), node->m_key.begin() + index, node->m_key.end());

		//更新叶子的上下游指针
		res.second->m_prev = node;
		res.second->m_next = node->m_next;
		node->m_next = res.second;
	}
	node->m_key.erase(node->m_key.begin() + index, node->m_key.end());

	return res;
}

template<typename T, int degree>
void BPlusTree<T, degree>::MergeChildren(BPlusTreeNode<T, degree>* parentNode, int keyIndex)
{
	BPlusTreeNode<T, degree>* leftNode = parentNode->Child(keyIndex);
	BPlusTreeNode<T, degree>* rightNode = parentNode->Child(keyIndex + 1);

	if (leftNode->m_bLeaf)
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
				index = i;
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

		return 1;		//叶子节点借左兄弟的key需要更新该节点父节点缝隙左边的索引key值
	}

	if (rightNode != nullptr 
		&& rightNode->m_key.size() > limitation)
	{//右兄弟可以借到
		posNode->m_key.push_back(std::move(node->m_key[index]));
		node->m_key[index] = std::move(rightNode->m_key.front());
		rightNode->m_key.erase(rightNode->m_key.begin());

		if (!posNode->m_bLeaf)
		{
			posNode->SetChild((int)posNode->m_key.size(), rightNode->Child(0));

			for (int i = 0; i < rightNode->m_key.size() + 2; ++i)
				rightNode->SetChild(i, rightNode->Child(i + 1));
		}

		return 2;		//叶子节点借右兄弟的key需要更新该节点父节点缝隙右边边的索引key值
	}

	//合并节点
	if (rightNode != nullptr)
	{
		MergeChildren(node, index);
	}
	else
	{
		MergeChildren(node, index - 1);
	}
	return 3;		//叶子节点合并后需要更新上下游索引指针
}