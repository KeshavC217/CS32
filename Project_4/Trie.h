#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED
#include <string>
#include <vector>
#include <iostream>
using namespace std;
template<typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;
private:
	struct Node
	{
		Node()
		{
			label = '\0';
		}
		char label;
		vector<Node*> m_children;
		vector<ValueType> m_values;//Node has a label for checking what type of child it is (ACTG or N), a vector of node pointers to the next nodes, and a vector to store values that need to be pushed on. 
	};
	Node* m_root;//root pointer
	void freeAllNodes(Node* root);
	void init();
	void findHelper(Node* m, string key, bool exactMatchOnly, vector<ValueType>& result) const;
};
template<typename ValueType>
void Trie<ValueType>::init()
{
	Node *head = new Node;
	m_root = head;
}//Helper function to create a new head node, acts as a dummy, labeled null

template<typename ValueType>
Trie<ValueType>::Trie()
{
	init();
}

template<typename ValueType>
void Trie<ValueType>::freeAllNodes(Node* root)
{
	if (root != nullptr)
	{
		for (int c = 0; c < root->m_children.size(); c++)
		{
			freeAllNodes(root->m_children[c]);//Recursively trace back to ever leaf in the trie and then delete it. 
		}
		delete root;
	}
}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
	freeAllNodes(m_root);
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
	freeAllNodes(m_root);
	init();
}

template<typename ValueType>
void Trie<ValueType>::insert(const string& key, const ValueType& value)
{
	string temp = key;
	Node* cur = m_root;
	bool match = false;
	while (temp != "")
	{
		char b = temp[0];
		for (int c = 0; c < cur->m_children.size(); c++)//traverse through every child in the current node
		{
			if (b == cur->m_children[c]->label)//if the next character in the key is the label of one of the children, we move to that child and continue down the path
			{
				cur = cur->m_children[c];
				temp=temp.substr(1);
				match = true;
				break;
			}
		}
		if (match)
		{
			match = false;
			continue;
		}
		Node* discard = new Node;
		discard->label = b;
		cur->m_children.push_back(discard);
		temp=temp.substr(1);
		cur = discard;//This is the end of the tree after all the keys characters have been exhausted, so we push a value back
	}
	cur->m_values.push_back(value);
}

template<typename ValueType>
void Trie<ValueType>::findHelper(Node* m, string key, bool exactMatchOnly, vector<ValueType>& result) const
{
	char b = key[0];
	if (key == "")//base case
	{
		result.insert(result.end(), m->m_values.begin(), m->m_values.end());
		return;
	}
	if (m->m_children.size() == 0)//another base case
		return;
	if (exactMatchOnly)
	{
		for (int c = 0; c < m->m_children.size(); c++)
		{
			if (b == m->m_children[c]->label) 
			{
				findHelper(m->m_children[c], key.substr(1), exactMatchOnly, result);//call helper for only the relevant child
				return;
			}
		}
	}
	else
	{
		for (int c = 0; c < m->m_children.size(); c++)
		{
			if (m->m_children[c]->label != b)
					findHelper(m->m_children[c], key.substr(1), true, result);//If a mistake has been made, all matches must be true from now on
			else
				findHelper(m->m_children[c], key.substr(1), exactMatchOnly, result);//call helper for all children and set exactMatchOnly accordingly
		}
	}
}

template<typename ValueType>
vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	char b = key[0];
	Node* cur = m_root;
	for (int c = 0; c < cur->m_children.size(); c++)
	{
		if (b == cur->m_children[c]->label)
		{
			cur = cur->m_children[c];
			break;
		}
	}
	vector<ValueType> result;
	findHelper(cur, key.substr(1), exactMatchOnly, result);
	return result;//implementation function, sets the node cur to the relevant child ie. the first character in the key string
}

#endif // TRIE_INCLUDED
