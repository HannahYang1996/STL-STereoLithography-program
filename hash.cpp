#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "hash.h"

template <class T> void _hash<T>::AddItem(T* feature)
{
	//get the ID of the feature
	unsigned long int index = Hash(feature);
	//if bucket corresponding to the ID is empty, 
	//setup new item and make item pointer of the bucket point to this new item
	if (HashTable[index] == NULL)
	{
		HashTable[index] = new item;
		HashTable[index]->feature = feature;
	}
	//if bucket corresponding to the ID is not empty,
	//item pointer ptr traverse from the first item in bucket to the last item
	//append new item at the end and setup its item pointer and feature
	else
	{
		item* ptr = HashTable[index];
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = new item;
		ptr->next->previous = ptr;
		ptr->next->feature = feature;
	}
	//as item is added, increase the number of items
	count++;
}
template <class T> T* _hash<T>::FindItem(T* feature)
{
	//get the ID of the feature
	unsigned long int index = Hash(feature);
	//item pointer ptr traverse from the first item in bucket to the last item
	//if same feature is found in bucket, break loop before traverse
	//if traverse do happen, return NULL, implies not found
	//else return the found feature
	item* ptr = HashTable[index];
	while (ptr != NULL)
	{
		if (*(ptr->feature) == *feature)
		{
			break;
		}
		ptr = ptr->next;
	}
	if (ptr == NULL)
		return NULL;
	else
		return ptr->feature;
}

template <class T> void _hash<T>::RemoveItem(T* feature)
{
	//get the ID of the feature
	unsigned long int index = Hash(feature);
	//item pointer ptr traverse from the first item in bucket to the last item
	//if same feature is found in bucket, break loop before traverse
	item* ptr = HashTable[index];
	while (ptr != NULL)
	{
		if (*(ptr->feature) == *feature)
		{
			break;
		}
		ptr = ptr->next;
	}
	//item is found
	if (ptr != NULL)
	{
		if (ptr->previous != NULL)
		{
			if (ptr->next != NULL)
			{
				//for the case:item is in the middle of list
				//modify the pointers of the previous item and the next item
				//then delete the found item
				ptr->previous->next = ptr->next;
				ptr->next->previous = ptr->previous;
				delete ptr;
			}
			else
			{
				//for the case:item is in the end of list, and item is not the only one
				//modify the pointer of the previous item
				//then delete the found item
				ptr->previous->next = NULL;
				delete ptr;
			}
		}
		else
		{
			if (ptr->next != NULL)
			{
				//for the case:item is in the beginning of list, and item is not the only one
				//modify the pointer of the next item, 
				//and make item pointer of the bucket point to the next item
				//then delete the found item
				ptr->next->previous = NULL;
				HashTable[index] = ptr->next;
				delete ptr;
			}
			else
			{
				//for the case:item is the only one
				//make item pointer of the bucket point to NULL
				//then delete the found item
				HashTable[index] = NULL;
				delete ptr;
			}
		}
		//as item is removed, decrease the number of items
		count--;
	}
	return;
}

