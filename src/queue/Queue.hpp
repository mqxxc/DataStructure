#pragma once
#include "../Nodes/ListNode.hpp"
#include <stdexcept>

//队列
template<class T>
class Queue 
{
public:
	Queue();
	~Queue();
	int Size();
	bool Empty();
	void Push(const T& value);
	T Pop();
	T Peek();

protected:
	ListNode<T>* m_pFront = nullptr;
	ListNode<T>* m_pRear = nullptr;
	int m_nSize = 0;
};

template<class T>
inline Queue<T>::Queue()
{
}

template<class T>
inline Queue<T>::~Queue()
{
	while (m_pFront != nullptr)
	{
		ListNode<T>* temp = m_pFront;
		m_pFront = m_pFront->m_pNextNode;
		delete temp;
	}
}

template<class T>
inline int Queue<T>::Size()
{
	return m_nSize;
}

template<class T>
inline bool Queue<T>::Empty()
{
	return m_pFront == nullptr;
}

template<class T>
inline void Queue<T>::Push(const T& value)
{
	if (m_pFront == nullptr)
	{
		m_pFront = new ListNode<T>(value);
		m_pRear = m_pFront;
	}
	else
	{
		m_pRear->m_pNextNode = new ListNode<T>(value);
		m_pRear = m_pRear->m_pNextNode;
	}
	++m_nSize;
}

template<class T>
inline T Queue<T>::Pop()
{
	T value = Peek();

	ListNode<T>* temp = m_pFront;
	m_pFront = m_pFront->m_pNextNode;
	delete temp;

	--m_nSize;
	return value;
}

template<class T>
inline T Queue<T>::Peek()
{
	if (Size() == 0)
	{
		throw std::out_of_range("queue is empty");
	}
	return m_pFront->m_Value;
}