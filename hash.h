#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>

using namespace std;
#ifndef HASH_H_H
#define HASH_H_H

template <class T> class _hash
{
protected:
	//HashTable provides #tableSize of buckets
	//each buckets contains several items
	//items in the same bucket are linked by item pointers to their previous and the next items
	//class type of the feature of the item can be vertex, edge or facet

	unsigned long int tableSize;
	struct item
	{
		T* feature = NULL;
		item* previous = NULL;
		item* next = NULL;
	};
	vector<item*>HashTable;

public:
	//default of the tableSize is 181 if you do not set a specific value
	_hash()
	{
		tableSize = 181;
		//construct _hash by making the first item in each buckets points to NULL
		for (unsigned long int i = 0; i < tableSize; i++)
		{
			HashTable.push_back(NULL);
		}
	}
	_hash(unsigned long int _tableSize)
	{
		tableSize = _tableSize;
		//construct _hash by making the first item in each buckets points to NULL
		for (unsigned long int i = 0; i < tableSize; i++)
		{
			HashTable.push_back(NULL);
		}
	}
	//count the number of items in the table in order to show to user
	unsigned long int count = 0;
	//different Hash functions for different derived classes
	virtual unsigned long int Hash(T* feature) = 0;
	void AddItem(T* feature);
	T* FindItem(T* feature);
	void RemoveItem(T* feature);
};

#include "hash.cpp"
#endif

