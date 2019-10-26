#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include <algorithm>
#include <unordered_map>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	struct modNode
	{
		modNode()
		{
			pos = genomeIndex = 0;
		}
		int genomeIndex;
		int pos;
	};
	modNode m;
	int min_length;
	vector<Genome> m_genomes;
	Trie<modNode> tree;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	min_length = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)//gotta be something wrong with this code or the tree
{
	m.pos = 0;//added
	m.genomeIndex = m_genomes.size();
	m_genomes.push_back(genome);//push genome into library maintained by class
	int c = 0;
	string s = "";
	while (genome.extract(c, min_length, s))//extracting each part of the string
	{
		m.pos = c;
		tree.insert(s, m);//insert using trie's insert
		c++;
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return min_length;
}

bool compareDNA(DNAMatch d1, DNAMatch d2)//helper function used to sort DNA
{
	if (d1.genomeName > d2.genomeName)
		return true;
	else if (d2.genomeName > d1.genomeName)
		return false;
	else
	{
		if (d1.length > d2.length)
			return false;
		else if (d2.length > d1.length)
			return true;
		else
		{
			if (d1.position <= d2.position)
				return false;
			else
				return true;
		}
	}
}


bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	string firstMin = fragment.substr(0, min_length);
	bool result = false;
	if (fragment.length() < minimumLength)
		return result;
	if (minimumLength < min_length)
		return result;//base cases
	string searcher = fragment.substr(0, min_length);
	vector<modNode> v1;
	if (exactMatchOnly)
		v1 = tree.find(searcher, true);
	else
		v1 = tree.find(searcher, false);//fill a vector up from the tree containing relevant data ie. matching genomes
	for (int c = 0; c < v1.size(); c++)
	{
		Genome g = m_genomes[v1[c].genomeIndex];
		int pos = v1[c].pos;
		string s;
		string snipCheck;
		bool check1=g.extract(pos, min_length, snipCheck);
		bool check2=g.extract(pos, fragment.length(), s);//check first part of genome with first part of each retrieved sequence
		if (!check1 || !check2)
			continue;
		int j;
		bool doesInitiallyMatch = true;
		bool nosniPotential = exactMatchOnly;
		for (j = min_length; j < minimumLength; j++)
		{
			if (snipCheck != firstMin)
				nosniPotential = true;//This variable will tell us whether a certain genome still has the ability to get one character in the sequence wrong (become an snip) or not
			if (s[j] != fragment[j])//check each of the characters after the min_length
			{
				if (snipCheck==firstMin&&nosniPotential==false)
				{
					nosniPotential = true;
					continue;//if the sequence hasn't been wrong yet, it's wrong now, so change nosnipotential and continue with the loop
				}
				doesInitiallyMatch = false;
				break;//If more than two characters dont match, break. no more checking required. 
			}
		}
		if (!doesInitiallyMatch)
			continue;
		result = true;
		int posAftermL = 0;
		for (j = minimumLength; j < fragment.length(); j++)
		{
			if (s[j] == fragment[j])
			{
				posAftermL++;
			}
			else
			{
				if (nosniPotential == false)
				{
					posAftermL++;
					nosniPotential == true;
					continue;//This checks the number of characters that are correct AFTER a set minimumlength, so that the largest genome may be returned in case one genome has two or more hits. 
				}
				break;
			}
		}
		DNAMatch d;
		d.genomeName = g.name();
		d.length = posAftermL + minimumLength;
		d.position = pos;
		matches.push_back(d);//this never executes so code before it is wrong.
	}
	sort(matches.begin(), matches.end(), compareDNA);//sort array based on name, the last element of each name being the only one you want
	vector<DNAMatch>::iterator it = matches.begin();
	while (it != matches.end())
	{
		if (it + 1 != matches.end())
		{
			if (it->genomeName == (it + 1)->genomeName)
			{
				it = matches.erase(it);//this erases the wrong one
				continue;
			}
		}
		it++;
	}
	return result;
}

bool compareMatch(GenomeMatch g1, GenomeMatch g2) //comparison helper function
{
	if (g1.percentMatch > g2.percentMatch)
		return true;
	else if (g1.percentMatch < g2.percentMatch)
		return false;
	else
	{
		if (g1.genomeName < g2.genomeName)
			return true;
		else
			return false;
	}
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	bool res = false;
	unordered_map<string, int> m1;
	int s = query.length() / fragmentMatchLength;//how many comparisons required
	string fragger;
	vector<DNAMatch> v1;
	for (int c = 0; c < s; c++)
	{
		if (query.extract(c*fragmentMatchLength, fragmentMatchLength, fragger))
		{
			bool t = findGenomesWithThisDNA(fragger, fragmentMatchLength, exactMatchOnly, v1);
			if (t)
			{
				for (int j = 0; j < v1.size(); j++)
				{
					DNAMatch d = v1[j];
						m1[d.genomeName]++;
				}
			}
		}
		v1.clear();
	}
	for (auto x : m1)//for every element in the hash table (unordered map)
	{
		double perc = 100 * x.second / s;
		if (perc >= matchPercentThreshold) {
			res = true;
			GenomeMatch gm;
			gm.genomeName = x.first;
			gm.percentMatch = perc;
			results.push_back(gm);
		}
	}
	sort(results.begin(), results.end(), compareMatch);
	return res;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
