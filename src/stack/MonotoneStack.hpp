#pragma once
#include "../Nodes/ListNode.hpp"

#include <stdexcept>
#include <functional>

//单调栈
template <class T>
class MonotoneStack
{
public:
	enum SortMode
	{
		eIncremental,
		eDecrease
	};

	MonotoneStack(SortMode mode);
	~MonotoneStack();
	int Size();
	bool IsEmpty();
	void Push(const T& value);
	T Pop();
	T Top();
#ifdef _DEBUG
	void TestOutPut();
#endif

private:
	ListNode<T>* m_root = nullptr;
	int m_nSize = 0;

	std::function<bool(const T&, const T&)> m_Compare;
};

template<class T>
MonotoneStack<T>::MonotoneStack(SortMode mode)
{
	if (mode == eIncremental)
	{
		m_Compare = [](const T& src, const T& tag) {
			return src > tag;
		};
	}
	else
	{
		m_Compare = [](const T& src, const T& tag) {
			return src < tag;
		};
	}
}

template<class T>
MonotoneStack<T>::~MonotoneStack()
{
	while (m_root != nullptr)
	{
		ListNode<T>* temp = m_root;
		m_root = temp->m_pNextNode;
		delete temp;
	}
}

template<class T>
int MonotoneStack<T>::Size()
{
	return  m_nSize;
}

template<class T>
bool MonotoneStack<T>::IsEmpty()
{
	return m_nSize == 0;
}

template<class T>
void MonotoneStack<T>::Push(const T& value)
{
	ListNode<T>* sentry = new ListNode<T>(0);
	sentry->m_pNextNode = m_root;
	ListNode<T>** temp = &sentry;
	ListNode<T>* explore = m_root;

	while ((explore) != nullptr)
	{
		if (m_Compare(value, explore->m_Value))
		{
			break;
		}
		explore = explore->m_pNextNode;
		temp = &(*temp)->m_pNextNode;
	}

	if ((*temp) == sentry)
	{
		auto node = new ListNode<T>(value);
		node->m_pNextNode = m_root;
		m_root = node;
	}
	else
	{
		auto node = new ListNode<T>(value);
		node->m_pNextNode = (*temp)->m_pNextNode;
		(*temp)->m_pNextNode = node;
	}
	
	++m_nSize;
	delete sentry;
}

template<class T>
T MonotoneStack<T>::Pop()
{
	if (IsEmpty())
	{
		throw std::out_of_range("stack is empty");
	}

	T value = Top();
	ListNode<T>* temp = m_root;
	m_root = m_root->m_pNextNode;
	delete temp;
	--m_nSize;
	return value;
}

template<class T>
T MonotoneStack<T>::Top()
{
	if (IsEmpty())
	{
		throw std::out_of_range("stack is empty");
	}
	return m_root->m_Value;
}

#ifdef _DEBUG
#include <iostream>
template<class T>
void MonotoneStack<T>::TestOutPut()
{
	ListNode<T>* temp = m_root;
	while (temp != nullptr)
	{
		std::cout << temp->m_Value << "  ";
		temp = temp->m_pNextNode;
	}
}
#endif // _DEBUG
