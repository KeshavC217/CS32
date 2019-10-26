#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <cctype>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	char b;
	string name;
	string seq;
	bool dataIsThere = false;
	bool onAName = false;
	while (genomeSource.get(b))
	{
		if (b != '>' && !dataIsThere)
			return false;
		if (b == '\n'||b=='\r') 
			continue;//handling edge cases
		if (b == '>')
		{
			if (dataIsThere)//if this is the second title or later
			{
				Genome a(name, seq);
				genomes.push_back(a);//push back whatever data the first genome had, and start again
			}
			dataIsThere = true;
			onAName = true;
			name = seq = "";
			getline(genomeSource, name);
			if (name[name.length() - 1] == '\n')
				name = name.substr(0, name.length() - 1);
			continue;//get the entire genome name in one go
		}
		if (b == 'C' || b == 'T' || b == 'G' || b == 'A' || b == 'N' || b == 'c' || b == 't' || b == 'g' || b == 'a' || b == 'n')
		{
			seq += toupper(b);
			onAName = false;
			continue;//take the data character by character and add it to a sequence, which will be pushed when a new name is encountered
		}
		else
			return false;
	}
	if (onAName)
		return false;
	Genome a(name, seq);
	genomes.push_back(a);//the last one doesn't go through the loop, so you need this extra pushback call
	return true;
}

int GenomeImpl::length() const
{
    return m_sequence.length();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
    return m_name;  
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position + length > m_sequence.length())
		return false;
	fragment = "";
	for (int c = position; c < position+length; c++)
	{
		fragment += m_sequence[c];
	}
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
