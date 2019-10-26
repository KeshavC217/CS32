#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;
Set::Set()
{
	dummy = new Node;
	dummy->next = nullptr;
	dummy->prev = nullptr;
	m_size = 0;
}
Set::~Set()
{
	Node *p = dummy;//Temp variable to store next node while deleting current node
	while (p != nullptr)
	{
		Node *q = p->next;
		delete p;
		p = q;
	}
}
Set::Set(const Set &other) 
{
	
	dummy = new Node;
	dummy->next = nullptr;
	dummy->prev = nullptr;//Create a new dummy node
	m_size = other.m_size;
	Node *temp = other.dummy->next;//Start temp at first node of other list
	Node *t1 = dummy;
	while (temp != nullptr)
	{
		Node *n = new Node;
		t1->next = n;//Link t1 to new node
		n->prev = t1;//Link new node to previous node
		n->s = temp->s;
		temp = temp->next;//Store values, and increment temp
		t1 = n;//Update t1 to be the newest node
	}
	t1->next = nullptr;//The loop works in a way such that the last node doesn't have a nullptr, so this line fixes that

}
Set& Set::operator=(const Set &rhs)
{
	if (this != &rhs)//If both don't refer to the same set
	{
		Set temp(rhs);//Create a temp that is rhs
		swap(temp);//make this set temp
	}
	return *this;
}
bool Set::empty() const
{
	return (m_size == 0);
}
int Set::size() const
{
	return m_size;
}
bool Set::insert(const ItemType &value)
{
	if (dummy->next == nullptr)//If the list is empty
	{
		Node *t = new Node;
		t->s = value;
		t->prev = dummy;
		dummy->next = t;
		t->next = nullptr;//Create new node t, store value in it, link it to dummy
		m_size++;
		return true;
	}
	Node *temp = dummy->next;//Make temp the first element in the list
	while (temp != nullptr)
	{
		if (temp->s < value && temp->next!=nullptr)
		{
			temp = temp->next;//Since it is inserted in order, keep traversing the list until you find a node with a value greater than the value to be inserted
			continue;
		}
		if (temp->s < value&&temp->next == nullptr)//If you hit the last element in the list
		{
			Node *t = new Node;
			t->s = value;
			t->prev = temp;
			temp->next = t;
			t->next = nullptr;
			m_size++;//Insert a node at the end of the list, give it value, and make next nullptr
			return true;
		}
		if (temp->s == value)
			return false;//If value is found, then insert should not work
		if (temp->s > value)
			break;
	} 
	Node *n = temp->prev;//Node that value should be inserted in front of
	Node *t = new Node;
	t->s = value;
	n->next = t;
	t->prev = n;
	t->next = temp;
	temp->prev = t;//Insert node in between temp and n;
	m_size++;
	return true;
}
bool Set::contains(const ItemType &value)const
{
	Node *temp = dummy;
	while (temp->next != nullptr)
	{
		temp = temp->next;
		if (temp->s == value)
			return true;//If any node in the list has the value, return true
	}
	return false;
}
bool Set::erase(const ItemType &value)
{
	if (!contains(value))
		return false;//If the set does not have the value, return false
	Node *temp = dummy->next;
	while (temp != nullptr)
	{
		if (temp->s == value&&temp->next!=nullptr)
		{
			Node *n = temp->prev;
			n->next = temp->next;
			temp->next->prev = n;//Link node before temp to the node after temp, then delete temp
			delete temp;
			m_size--;
			return true;
		}
		if (temp->s == value && temp->next == nullptr)
		{
			Node *n = temp->prev;
			n->next = nullptr;//Set the next of the previous to null and then delete the node
			delete temp;
			m_size--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}
bool Set::get(int pos, ItemType &value) const
{
	if (pos < 0 || pos >= m_size)//If out of bounds, make it false
		return false;
	Node *temp = dummy->next;
	for (int c = 0; c < pos; c++)
	{
		temp = temp->next;//Find the node at the pos position because the list is already sorted
	}
	value = temp->s;
	return true;
}
void Set::swap(Set &other)
{
	Node *t = dummy;
	dummy = other.dummy;
	other.dummy = t;

	int temp = m_size;
	m_size = other.m_size;
	other.m_size = temp;
}
void Set::dump()const
{
	Node *temp = dummy->next;
	while (temp != nullptr)
	{
		cerr << temp->s << endl;
		temp = temp->next;
	}

}
void unite(const Set& s1, const Set& s2, Set& result)
{
	Set temp = s1;
	for (int c = 0; c < s2.size(); c++)
	{
		ItemType s;
		s2.get(c, s);
		temp.insert(s);//Insert values of s2 into the temp which is s1. Duplicate values will cancel thanks to insert
	}
	result = temp;
}
void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set temp = s1;
	for (int c = 0; c < s2.size(); c++)
	{
		ItemType s;
		s2.get(c, s);
		temp.erase(s);//Erase values that are in s2 from s1.
	}
	result = temp;

}