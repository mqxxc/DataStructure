#pragma once
#include "../Nodes/ListNode.hpp"
#include <stdexcept>

//单向列表
template <class T>
class List
{
public:
    List();
    ~List();
    void Append(const T& value);
    bool Insert(int nIndex, const T& value);
    bool Remove(int nIndex);
    const T At(int nIndex); 
    //返回-1表示未找到
    int IndexOf(const T& vlaue);
    T& operator[](int nIndex);
    int Size();
private:
    ListNode<T>* m_pRootNode;
    int m_nCount = 0;
};

template <class T>
inline List<T>::List()
{
    m_pRootNode = new ListNode<T>();
}

template <class T>
inline List<T>::~List()
{
    ListNode<T>* pIndex = m_pRootNode;
    while (pIndex != nullptr)
    {
        ListNode<T>* temp = pIndex;
        pIndex = pIndex->m_pNextNode;
        delete temp;
    }
    m_pRootNode = nullptr;
}

template <class T>
inline void List<T>::Append(const T &value)
{
    ListNode<T>* tempNode = m_pRootNode;
    while (tempNode->m_pNextNode != nullptr)
    {
        tempNode = tempNode->m_pNextNode;
    }

    tempNode->m_pNextNode = new ListNode<T>(value);
    ++m_nCount;
}

template <class T>
inline bool List<T>::Insert(int nIndex, const T &value)
{
    ListNode<T>* tempNode = m_pRootNode;
    while (nIndex > 0 && tempNode->m_pNextNode != nullptr)
    {
        --nIndex;
        tempNode = tempNode->m_pNextNode;
    }
    if(nIndex == 0)
    {
        ListNode<T>* pNextNode = tempNode->m_pNextNode;
        tempNode->m_pNextNode = new ListNode<T>(value);
        tempNode->m_pNextNode->m_pNextNode = pNextNode;
        ++m_nCount;
        return true;
    }
    return false;
}

template <class T>
inline bool List<T>::Remove(int nIndex)
{
    if(nIndex < 0 || nIndex >= m_nCount)
    {
        return false;
    }

    ListNode<T>* tempNode = m_pRootNode;
    while (nIndex > 0)
    {
        tempNode = tempNode->m_pNextNode;
        --nIndex;
    }

    ListNode<T>* nextNode = tempNode->m_pNextNode->m_pNextNode;
    delete tempNode->m_pNextNode;
    tempNode->m_pNextNode = nextNode;
    --m_nCount;
    return true;
}

template <class T>
inline const T List<T>::At(int nIndex)
{
    if(nIndex < 0 || nIndex > m_nCount)
    {
        return T();
    }

    ListNode<T>* pIndex = m_pRootNode->m_pNextNode;

    while (nIndex > 0)
    {
        pIndex = pIndex->m_pNextNode;
        --nIndex;
    }

    return pIndex->m_Value;
}

template <class T>
inline int List<T>::IndexOf(const T &vlaue)
{
    ListNode<T>* pIndex = m_pRootNode->m_pNextNode;
    int nIndex = 0;

    while (pIndex != nullptr)
    {
        if(pIndex->m_Value == vlaue)
        {
            return nIndex;
        }
        pIndex = pIndex->m_pNextNode;
        ++nIndex;
    }

    return -1;
}

template <class T>
inline T &List<T>::operator[](int nIndex)
{
    if (nIndex > m_nCount || nIndex < 0)
    {
        throw std::out_of_range("index is invalid");
    }

    ListNode<T>* pIndex = m_pRootNode->m_pNextNode;

    while (nIndex > 0)
    {
        pIndex = pIndex->m_pNextNode;
        --nIndex;
    }

    return pIndex->m_Value;
}

template<class T>
inline int List<T>::Size()
{
    return m_nCount;
}
