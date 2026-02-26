#ifndef LIST_HPP
#define LIST_HPP

#include <string>
using Type = std::string;

class List
{
private:
    struct Node // 嵌套类型/内部类
    {
        Node(Type x);
        Type _data;
        struct Node *_next;
        struct Node *_prev;
    };
    Node *_head;
    Node *_tail;
    int _size;

public:
    // friend std::ostream& operator<<(std::ostream& out, const List& list);
    List();
    List(const List &rhs);
    ~List();
    void push_front(Type x);
    void push_back(Type x);
    void pop_front();
    void pop_back();
    Type front() const;
    Type back() const;
    bool empty() const;
    int size() const;
    void print() const;
    Type getElem(int k) const;

    class iterator
    {
    public:
        iterator(Node *p);
        Type &operator*();
        iterator &operator++();
        bool operator!=(const iterator &rhs) const;

    private:
        Node *p;
    };
    iterator begin();
    iterator end();
};

std::ostream &operator<<(std::ostream &out, const List &list);

#endif