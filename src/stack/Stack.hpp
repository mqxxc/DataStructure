#pragma once
#include "../Nodes/ListNode.hpp"

#include <stdexcept>

template<class T>
class Stack
{
public:
	Stack();
	~Stack();
	int Size();
	bool Empty();
	void Push(const T& value);
	T Pop();
	T Top();

protected:
	ListNode<T>* m_pTop = nullptr;
	int m_nSize = 0;
};

template<class T>
inline Stack<T>::Stack()
{
}

template<class T>
inline Stack<T>::~Stack()
{
	while (m_pTop != nullptr)
	{
		ListNode<T>* temp;
		delete m_pTop;
		m_pTop = temp->m_pNextNode;
	}
}

template<class T>
inline int Stack<T>::Size()
{
	return m_nSize;
}

template<class T>
inline bool Stack<T>::Empty()
{
	return m_pTop == nullptr;
}

template<class T>
inline void Stack<T>::Push(const T& value)
{
	ListNode<T>* pNode = ListNode<T>(value);
	pNode->m_pNextNode = m_pTop;
	m_pTop = pNode;
	++m_nSize;
}

template<class T>
inline T Stack<T>::Pop()
{
	T value = Top();
	ListNode<T>* temp = m_pTop;
	m_pTop = m_pTop->m_pNextNode;
	delete temp;
	--m_nSize
	return value;
}

template<class T>
inline T Stack<T>::Top()
{
	if (Empty())
	{
		throw std::out_of_range("stack is empty");
	}
	return m_pTop->m_Value;
}