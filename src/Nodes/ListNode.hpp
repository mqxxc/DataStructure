#pragma once
//单行列表节点
template <class T>
class ListNode
{
public:
    T m_Value;
    ListNode* m_pNextNode = nullptr;
    ListNode(const T& value);
    ListNode() { this->m_Value = T(); };
};

template <class T>
ListNode<T>::ListNode(const T &value)
{
    this->m_Value = value;
}