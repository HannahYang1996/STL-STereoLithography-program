#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "features.h"
#include "hash.h"

using namespace std;
#ifndef DERIVED_HASH_H_H
#define DERIVED_HASH_H_H

class vertex_hash : public _hash<vertex>
{
public:
	vertex_hash() :_hash() {}
	vertex_hash(unsigned long int _tableSize) :_hash(_tableSize) {}
	unsigned long int Hash(vertex* feature);
};

class edge_hash : public _hash<edge>
{
public:
	edge_hash() :_hash() { }
	edge_hash(unsigned long int _tableSize) :_hash(_tableSize) { }
	unsigned long int Hash(edge* feature);
	void draw_free_edge(char* fo);
	friend class sort_edge_for_edge_hash;
};

class facet_hash : public _hash<facet>
{
public:
	facet_hash() :_hash() {}
	facet_hash(unsigned long int _tableSize) :_hash(_tableSize) {}
	unsigned long int Hash(facet* feature);
	void draw_intersection_line(facet_hash& facet_list, char* fo);
};

class sort_edge_for_edge_hash : public _hash<edge>
{
public:
	unsigned long int Hash(edge* feature);
	void sort_edge(edge_hash referent);
	void draw_edge(char* fo, double degree_ll, double degree_ul);
};

#endif