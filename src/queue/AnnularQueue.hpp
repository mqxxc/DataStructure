#pragma once
#include "../Nodes/TwoWayNode.hpp"

#include <stdexcept>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif // _DEBUG


#define DEFAULT_SIZE 15
//环形队列
template<class T>
class AnnularQueue
{
public:
	AnnularQueue();
	AnnularQueue(size_t nSize);
	~AnnularQueue();
	T Deque();
	void Enque(const T& value);
	size_t Size() const;
	T Front() const;
	T Rear() const;
	bool Empty() const;
	bool Full() const;
	const size_t Count();
	const T* Buffer();
#ifdef _DEBUG
	void Print_test();
#endif

protected:
	long m_front = -1;
	long m_rear = -1;
	size_t m_size = 0;
	size_t m_maxsize;
	T* m_pArrData;
};

template<class T>
AnnularQueue<T>::AnnularQueue()
{
	m_maxsize = DEFAULT_SIZE;
	m_pArrData = new T[DEFAULT_SIZE];
}

template<class T>
inline AnnularQueue<T>::AnnularQueue(size_t nSize)
{
	m_maxsize = nSize;
	m_pArrData = new T[nSize];
}

template<class T>
AnnularQueue<T>::~AnnularQueue()
{
	delete[] m_pArrData;
}

template<class T>
inline T AnnularQueue<T>::Deque()
{
	if (Empty())
	{
		throw std::out_of_range("quenue is empty");
	}

	T temp = m_pArrData[m_front];
	if (m_front == m_rear)
	{
		m_front = m_rear = -1;
	}
	else if (m_front == m_maxsize - 1)
	{
		m_front = 0;
	}
	else
	{
		++m_front;
	}
	--m_size;
	return std::move(temp);
}

template<class T>
void AnnularQueue<T>::Enque(const T& value)
{
	if (Full())
	{
		throw std::out_of_range("quenue is full");
	}

	if (m_rear == -1)
	{
		m_front = m_rear = 0;
	}
	else if (m_rear == m_maxsize - 1)
	{
		m_rear = 0;
	}
	else
	{
		++m_rear;
	}
	++m_size;
	m_pArrData[m_rear] = value;
}

template<class T>
size_t AnnularQueue<T>::Size() const
{
	return m_maxsize;
}

template<class T>
T AnnularQueue<T>::Front() const
{
	if (Empty())
	{
		throw std::out_of_range("quenue is empty");
	}
	return m_pArrData[m_front];
}

template<class T>
T AnnularQueue<T>::Rear() const
{
	if (Empty())
	{
		throw std::out_of_range("quenue is empty");
	}
	return m_pArrData[m_rear];
}

template<class T>
bool AnnularQueue<T>::Empty() const
{
	return m_size == 0;
}

template<class T>
bool AnnularQueue<T>::Full() const
{
	return (m_rear + 1) % m_maxsize == m_front;
}

template<class T>
const size_t AnnularQueue<T>::Count()
{
	return m_size;
}

template<class T>
const T* AnnularQueue<T>::Buffer()
{
	return m_pArrData;
}

#ifdef _DEBUG
template<class T>
void AnnularQueue<T>::Print_test()
{
	for (int i = 0; i < m_size; ++i)
	{
		cout << m_pArrData[((int)m_front + i) % m_maxsize] << endl;
	}
}
#endif // _DEBUG