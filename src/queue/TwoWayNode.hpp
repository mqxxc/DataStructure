#pragma once

template<class T>
class TwoWayNode
{
public:
    T m_Value;
    TwoWayNode* m_pNextNode = nullptr;
    TwoWayNode<T>* m_pPrevious = nullptr;

    TwoWayNode(const T& value);
    TwoWayNode();
};

template<class T>
inline TwoWayNode<T>::TwoWayNode(const T& value)
{
    this->m_Value = value;
}

template<class T>
inline TwoWayNode<T>::TwoWayNode()
{
    m_Value = T();
}
