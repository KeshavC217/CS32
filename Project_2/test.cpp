//
//  TEST_DON'T_TURN_IN.cpp
//  32_Project_2
//
//

#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
	Set s;//Tests default constructor
	assert(s.size() == 0);//Tests if size is 0
	assert(s.empty());//Tests if the set is initialized to be empty
	for (int c = 1; c <= 10; c++)
	{
		assert(s.insert(c));//Checks if insert is successful
		assert(s.contains(c));//Checks if contain function works on the recently inserted variable
		assert(!s.empty());//Checks if empty returns false if the set is not empty
		assert(s.size() == c);//Checks if size accurately keeps up with the current size of the linked list
	}
	for (int c = 1; c <= 10; c++)
	{
		assert(!s.insert(c));//Checks that trying to insert a duplicate value makes insert return false
		assert(s.size() == 10);//Make sure that size doesn't change during this
	}
	for (int c = 1; c <= 10; c++)
	{
		assert(s.erase(c));//Checks if erase is successful
		assert(!s.erase(c));//Checks if erase function doesn't work immediately after that same element is already erased
		assert(!s.contains(c));//Makes sure that the element is actually removed
		assert(s.size() == 10-c);//Ensures that size is continuously maintained
	}
	assert(s.empty());//After the whole loop runs, the size of the set should be 0
	for (int c = 1; c <= 10; c++)
	{
		assert(!s.erase(c));//Checks that there is no unexpected behaviour operating on an empty set 
		assert(s.empty());//Makes sure that the size stays the same
	}
	assert(!s.contains(11));
	assert(s.insert(11));
	assert(s.contains(11));
	assert(s.erase(11));
	assert(s.empty());//These 5 tests summarize the testing of insert, erase, contains, and empty
	for (int c = 0; c <= 20; c += 2)
	{
		s.insert(c);
	}
	assert(s.size() == 11);//Make sure that the size is stored correctly
	ItemType x;
	for (int c = 0; c < 11; c++)
	{
		s.get(c, x);
		assert(x == c * 2);//Makes sure that the get function retrieves everything in order via a for loop
		cerr << x << endl;//Display to double check
	}
	assert(s.get(11, x) == false);//Check out of bounds cases
	for (int c = 0; c <= 20; c += 2)
	{
		s.erase(c);//Empty the set
	}
	assert(!s.get(0, x));//Check how get works if the set is empty
	assert(s.insert(69));
	s.get(0, x);
	assert(x == 69);//Check that get works with only one element
	assert(!s.get(1, x));//Check that out of bounds arguments are returned false
	assert(s.erase(69));
	for (int c = 1; c <= 9; c++)
	{
		s.insert(c * 3);
	}
	Set a;
	for (int c = 1; c <= 11; c++)
	{
		a.insert(c * 6);
	}
	cerr << "S";
	s.dump();
	cerr << "A";
	a.dump();
	assert(s.size() == 9 && a.size() == 11);//Check that size values are what we intended
	swap(s, a);//Test swap function
	assert(s.size() == 11 && a.size() == 9);//Check that the sizes are swapped
	for (int c = 0; c < 9; c++)
	{
		ItemType s1, a1;
		assert(s.get(c, s1));
		assert(a.get(c, a1));
		assert(s1 / a1 == 2);//This checks that the elements are switched. Initially, all the elements in a were twice of s. Now, its the reverse
	}
	cerr << "S";
	s.dump();
	cerr << "A";
	a.dump();
	for (int c = 1; c <= 9; c++)
	{
		assert(a.erase(c * 3));
	}
	for (int c = 1; c <= 11; c++)
	{
		assert(s.erase(c * 6));
	}
	assert(s.empty());
	assert(a.empty());
	s.insert(1);
	s.insert(2);
	s.insert(3);
	a.insert(4);
	a.insert(5);
	a.insert(6);
	a.insert(7);
	assert(s.size() == 3 && a.size() == 4);
	s.swap(a);
	assert(s.size() == 4 && a.size() == 3);
	for (int c = 1; c <= s.size(); c++)
	{
		if (c <= a.size())
			assert(a.contains(c));
		assert(s.contains(c + 3));//Similar test to the last but with different elements
	}
	Set b;
	b= a = s; //Test the equal operator
	assert(a.size() == 4&&b.size()==4);
	for (int c = 4; c <= 7; c++)
	{
		assert(a.contains(c) && s.contains(c) && b.contains(c));//Check that the initial elements in s are now in both a and b
	}

	Set c;
	a = b = c;//Set a and b to both be empty
	assert(a.empty() && b.empty());//Check the above
	Set d(b);//Check if copy constructor works with an empty set
	assert(d.empty());
	d.insert(1);
	d.insert(2);
	d.insert(3);//Give d some elements
	Set e(d);//Check if copy constructor works when trying to copy another set with elements
	assert(e.size() == 3 && e.contains(1) && e.contains(2) && e.contains(3));//Check if all elements copied

	a.insert(4);
	a.insert(5);
	a.insert(6);
	unite(d, a, b);//Check if the unite function works properly
	for (int c = 1; c <= 6; c++)
	{
		assert(b.contains(c));//Make sure that all elements from d and a get put into b
	}
	unite(b, a, b);//Check if unite works if the result and argument sets are the same
	assert(b.size() == 6);//Check that size is still maintained
	for (int c = 1; c <= 6; c++)
	{
		assert(b.contains(c));//Check that all elements from b are maintained
	}
	Set m;
	m.insert(1);
	m.insert(2);
	m.insert(3);
	Set n = m;
	n.erase(3);
	Set p1;
	subtract(m, n, p1);//Check if subtract normally works for two sets, with one bigger than the other
	assert(p1.size() == 1);
	ItemType l;
	p1.get(0, l);
	assert(l == 3);
	subtract(n, m, p1);
	assert(p1.get(0, l) == false);//Check if the result is empty if the second set is bigger than the first set and has all the elements that the first one has
	assert(p1.empty());
	subtract(d, a, a);//Check if subtract function works if result and arguments are the same
	for (int c = 1; c <= 3; c++)
	{
		ItemType a1, b1;
		d.get(c, a1);
		a.get(c, b1);
		assert(a1 == b1);//Since I subtracted a from d, and no elements were in common, d should be the only thing that's left. 
	}
	cerr << "All tests passed";
}
