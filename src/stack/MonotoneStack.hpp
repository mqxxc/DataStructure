#include "../Nodes/ListNode.hpp"

#include <stdexcept>

template <class T>
class MonotoneStack
{
public:
	MonotoneStack();
	~MonotoneStack;

private:
	ListNode<T>* m_root;
};