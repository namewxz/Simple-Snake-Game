#include "list.hpp"
#include <iostream>
using namespace std;

List::List() : _head(nullptr), _tail(nullptr), _size(0) {}

List::List(const List &rhs) : _head(nullptr), _tail(nullptr), _size(0)
{
    for (int i = 1; i < rhs.size(); i++)
    {
        this->push_back(rhs.getElem(i));
    }
}

List::~List()
{
    while (!empty())
    {
        pop_back();
    }
    cout << "~List()" << endl;
}

List::Node::Node(Type x) : _data(x), _next(nullptr), _prev(nullptr) {}

void List::push_front(Type x)
{
}
void List::push_back(Type x)
{
    // 创建一个新的结点
    Node *p = new Node(x);

    // 把新结点插入到尾部
    if (_head == nullptr)
    {
        _head = p;
        _tail = p;
    }
    else
    {
        _tail->_next = p;
        p->_prev = _tail;
        _tail = p;
    }
    // 元素个数+1
    _size++;
}
void List::pop_front()
{
}
void List::pop_back()
{
    if (_size == 0)
        return;

    if (_size == 1)
    {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
    }
    else
    {
        Node *p = _tail;
        _tail = p->_prev;
        _tail->_next = nullptr;
        p->_prev = nullptr;
        delete p;
    }
    _size--;
}
// 如果链表为空，则结果未定义
Type List::front() const
{
    return _head->_data;
}
Type List::back() const
{
    return _tail->_data;
}
bool List::empty() const
{
    return _size == 0;
}
int List::size() const
{
    return _size;
}
void List::print() const
{
    Node *p = _head;
    while (p != nullptr)
    {
        cout << p->_data << " ";
        p = p->_next;
    }
    cout << endl;
}

Type List::getElem(int k) const
{
    if (k < 1 or k > _size)
        throw std::out_of_range("k < 1 or k > _size");

    Node *p = _head;
    for (int i = 1; i < k; i++)
        p = p->_next;
    return p->_data;
}

std::ostream &operator<<(std::ostream &out, const List &list)
{
    // List::Node *p = list._head;
    // while (p != nullptr)
    // {
    //     cout << p->_data << " ";
    //     p = p->_next;
    // }
    list.print();
    // cout << endl;
    return out;
}

List::iterator::iterator(Node *p) : p(p) {}
Type &List::iterator::operator*()
{
    return p->_data;
}
List::iterator &List::iterator::operator++()
{
    p = p->_next;
    return *this;
}
bool List::iterator::operator!=(const iterator &rhs) const
{
    return this->p != rhs.p;
}

List::iterator List::begin()
{
    return iterator(_head);
}
List::iterator List::end()
{
    return iterator(_tail->_next);
}