#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "global_tool.h"

#ifndef FEATURES_H_H
#define FEATURES_H_H

using namespace std;

class vertex;
class edge;
class facet;

class vertex
{
public:
	vertex() {}
	vertex(float _x, float _y, float _z);
	float x, y, z;
	vector<facet*> facet_of_vertex;
	vector<edge*> edge_of_vertex;
	bool operator==(vertex referent);

};
class edge
{
public:
	edge() {}
	edge(vertex* v0, vertex* v1);
	vertex* vertex_of_edge[2];
	vector<facet*> facet_of_edge;
	bool operator==(edge referent);
	double dot(edge E)
	{
		VEC VEC_E1(vertex_of_edge[1]->x - vertex_of_edge[0]->x, \
			vertex_of_edge[1]->y - vertex_of_edge[0]->y, \
			vertex_of_edge[1]->z - vertex_of_edge[0]->z);
		VEC VEC_E2(E.vertex_of_edge[1]->x - E.vertex_of_edge[0]->x, \
			E.vertex_of_edge[1]->y - E.vertex_of_edge[0]->y, \
			E.vertex_of_edge[1]->z - E.vertex_of_edge[0]->z);
		return(VEC_E1.dot(VEC_E2));
	}
	VEC cross(edge E)
	{
		VEC VEC_E1(vertex_of_edge[1]->x - vertex_of_edge[0]->x, \
			vertex_of_edge[1]->y - vertex_of_edge[0]->y, \
			vertex_of_edge[1]->z - vertex_of_edge[0]->z);
		VEC VEC_E2(E.vertex_of_edge[1]->x - E.vertex_of_edge[0]->x, \
			E.vertex_of_edge[1]->y - E.vertex_of_edge[0]->y, \
			E.vertex_of_edge[1]->z - E.vertex_of_edge[0]->z);
		return(VEC_E1.cross(VEC_E2));
	}
	double length()
	{
		VEC VEC_E1(vertex_of_edge[1]->x - vertex_of_edge[0]->x, \
			vertex_of_edge[1]->y - vertex_of_edge[0]->y, \
			vertex_of_edge[1]->z - vertex_of_edge[0]->z);
		return(VEC_E1.length());
	}
};
class facet
{
	double* plane_intersection(double*& Ans, facet* F);
	double* line_intersection(double*& Ans, double* &L1, double* &L2, double* &P1, double* &P2);
public:
	facet() {}
	facet(vertex* v0, vertex* v1, vertex* v2);
	float nx, ny, nz;
	vertex* vertex_of_facet[3];
	edge* edge_of_facet[3];
	bool operator==(facet referent);
	double dot(facet F)
	{
		VEC VEC_F1(nx, ny, nz);
		VEC VEC_F2(F.nx, F.ny, F.nz);
		return (VEC_F1.dot(VEC_F2));
	}
	VEC cross(facet F)
	{
		VEC VEC_F1(nx, ny, nz);
		VEC VEC_F2(F.nx, F.ny, F.nz);
		return (VEC_F1.cross(VEC_F2));
	}
	double length()
	{
		VEC VEC_F1(nx, ny, nz);
		return(VEC_F1.length());
	}
	double* find_intersect_line(double*& P_intersect, facet* F);
	bool if_point_in_facet(double*& P);
	bool if_facets_are_possible_to_intersect(facet* F);

};

#endif