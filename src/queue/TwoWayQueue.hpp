#pragma once
#include "../Nodes/TwoWayNode.hpp"
#include <stdexcept>

template<class T>
class TwoWayQueue
{
public:
	TwoWayQueue();
	~TwoWayQueue();
	int Size();
	bool Empty();
	void Push(const T& Value, bool bIsFront);
	void Push_front(const T& Value);
	void Push_rear(const T& Value);
	T Pop(bool bIsFront);
	T Pop_front();
	T Pop_rear();
	T peekFront();
	T peekRear();

protected:
	TwoWayNode<T>* m_pFront = nullptr;
	TwoWayNode<T>* m_pRear = nullptr;
	int m_nSize = 0;
};

template<class T>
inline TwoWayQueue<T>::TwoWayQueue()
{
}

template<class T>
inline TwoWayQueue<T>::~TwoWayQueue()
{
	while (m_pFront != nullptr)
	{
		TwoWayNode<T>* temp = m_pFront;
		m_pFront = m_pFront->m_pNextNode;
		delete temp;
	}
}

template<class T>
inline int TwoWayQueue<T>::Size()
{
	return m_nSize;
}

template<class T>
inline bool TwoWayQueue<T>::Empty()
{
	return m_pFront == nullptr;
}

template<class T>
inline void TwoWayQueue<T>::Push(const T& Value, bool bIsFront)
{
	if (bIsFront)
	{
		Push_front(Value);
	}
	else
	{
		Push_rear(Value);
	}
}

template<class T>
inline void TwoWayQueue<T>::Push_front(const T& Value)
{
	if (Empty())
	{
		m_pFront = new TwoWayNode<T>(Value);
		m_pRear = m_pFront;
	}
	else
	{
		TwoWayNode<T>* temp = new TwoWayNode<T>(Value);
		temp->m_pNextNode = m_pFront;
		m_pFront->m_pPrevious = temp;
		m_pFront = temp;
	}
	++m_nSize;
}

template<class T>
inline void TwoWayQueue<T>::Push_rear(const T& Value)
{
	if (Empty())
	{
		m_pFront = new TwoWayNode<T>(Value);
		m_pRear = m_pFront;
	}
	else
	{
		m_pRear->m_pNextNode = new TwoWayNode<T>(Value);
		m_pRear->m_pNextNode->m_pPrevious = m_pRear;
		m_pRear = m_pRear->m_pNextNode;
	}
	++m_nSize;
}

template<class T>
inline T TwoWayQueue<T>::Pop(bool bIsFront)
{
	if (bIsFront)
	{
		return Pop_front();
	}
	return Pop_rear();
}

template<class T>
inline T TwoWayQueue<T>::Pop_front()
{
	if (m_pFront == nullptr)
	{
		throw std::out_of_range("quenue is empty");
	}

	T value = m_pFront->m_Value;
	TwoWayNode<T>* temp = m_pFront;
	m_pFront = m_pFront->m_pNextNode;
	delete temp;

	m_pFront->m_pPrevious = nullptr;

	--m_nSize;

	return value;
}

template<class T>
inline T TwoWayQueue<T>::Pop_rear()
{
	if (m_pRear == nullptr)
	{
		throw std::out_of_range("quenue is empty");
	}

	T value = m_pRear->m_Value;
	TwoWayNode<T>* temp = m_pRear;
	m_pRear = m_pRear->m_pPrevious;
	delete temp;

	m_pRear->m_pNextNode = nullptr;

	--m_nSize;

	return value;
}

template<class T>
inline T TwoWayQueue<T>::peekFront()
{
	if (m_pFront == nullptr)
	{
		throw std::out_of_range("quenue is empty");
	}

	return m_pFront->m_Value;
}

template<class T>
inline T TwoWayQueue<T>::peekRear()
{
	if (m_pRear == nullptr)
	{
		throw std::out_of_range("quenue is empty");
	}

	return m_pRear->m_Value;
}