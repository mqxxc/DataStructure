template <class T>
//单行列表节点
class ListNode
{
public:
    T m_Value;
    ListNode* m_pNextNode = nullptr;
    ListNode(const T& value);
    ListNode(){};
};

template <class T>
ListNode<T>::ListNode(const T &value)
{
    this->m_Value = value;

}