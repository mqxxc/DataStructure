#pragma once
#include <stdexcept>
#include <functional>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


//优先队列
template <class T>
class PriorityQueue
{
public:
	enum SortMode
	{
		eIncremental,
		eDecrease
	};
	PriorityQueue(int nSize, SortMode model = eIncremental);
	~PriorityQueue();

	T Deque();
	void Enque(const T& value);
	size_t Size() const;
	T Top() const;
	bool IsEmpty() const;
	bool Full() const;
	const size_t Count();
	const T* Buffer();
#ifdef _DEBUG
	void Print_test();
#endif

protected:
	T* m_root;
	int m_nSize;
	int m_nCount = 0;

	std::function<bool(const T&, const T&)> m_Compare;
	void swim(int nIndex);
	void sink(int nIndex);
};

template<class T>
PriorityQueue<T>::PriorityQueue(int nSize, SortMode model)
{
	switch (model)
	{
	case eIncremental:
	{
		m_Compare = [](const T& src, const T& tag) {
			return src > tag;
		};
	}break;
	case eDecrease:
	{
		m_Compare = [](const T& src, const T& tag) {
			return src < tag;
		};
	}break;
	default:
		break;
	}
	m_nSize = nSize;
	m_root = new T[nSize];
}

template<class T>
PriorityQueue<T>::~PriorityQueue()
{
	delete[] m_root;
}

template<class T>
T PriorityQueue<T>::Deque()
{
	if (IsEmpty())
	{
		throw std::out_of_range("stack is empty");
	}

	--m_nCount;
	T temp = m_root[0];
	m_root[0] = m_root[m_nCount];
	m_root[m_nCount] = temp;

	sink(0);
	
	return m_root[m_nCount];
}

template<class T>
void PriorityQueue<T>::Enque(const T& value)
{
	if (m_nCount >= m_nSize)
	{
		T* temp = m_root;
		m_nSize <<= 1;
		m_root = new T[m_nSize];
		for (int i = 0; i < m_nCount; ++i)
		{
			m_root[i] = temp[i];
		}
		delete[] temp;
	}

	m_root[m_nCount] = value;
	swim(m_nCount);
	++m_nCount;
}

template<class T>
size_t PriorityQueue<T>::Size() const
{
	return m_nSize;
}

template<class T>
T PriorityQueue<T>::Top() const
{
	if (IsEmpty())
	{
		throw std::out_of_range("stack is empty");
	}
	return m_root[0];
}

template<class T>
bool PriorityQueue<T>::IsEmpty() const
{
	return m_nCount == 0;
}

template<class T>
bool PriorityQueue<T>::Full() const
{
	return m_nCount == m_nSize;
}

template<class T>
const size_t PriorityQueue<T>::Count()
{
	return m_nCount;
}

template<class T>
const T* PriorityQueue<T>::Buffer()
{
	return m_root;
}

template<class T>
void PriorityQueue<T>::swim(int nIndex)
{
	while (nIndex > 0)
	{
		int k = (nIndex - 1) / 2;
		if (m_Compare(m_root[k], m_root[nIndex]))
		{
			break;
		}
		T temp = m_root[nIndex];
		m_root[nIndex] = m_root[k];
		m_root[k] = temp;
		nIndex = k;
	}
}

template<class T>
void PriorityQueue<T>::sink(int nIndex)
{
	while (nIndex * 2 + 1 < m_nCount)
	{
		int k = nIndex * 2 + 1;
		if (k + 1 < m_nCount && m_Compare(m_root[k + 1], m_root[k]))
		{
			++k;
		}
		if (m_Compare(m_root[nIndex], m_root[k]))
		{
			break;
		}
		T temp = m_root[nIndex];
		m_root[nIndex] = m_root[k];
		m_root[k] = temp;
		nIndex = k;
	}
}

#ifdef _DEBUG
template<class T>
void PriorityQueue<T>::Print_test()
{
	for (int i = 0; i < m_nCount; ++i)
	{
		std::cout << i << ": " << m_root[i]<< "   ";
	}
}
#endif // DEBUG
