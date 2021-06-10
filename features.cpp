#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "features.h"

//construct vertex by (x,y,z), (x,y,z) of vertex is then initialized
vertex::vertex(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
//construct edge by 2 vertex pointer, vertex_of_edge array is then initialized
edge::edge(vertex* v0, vertex* v1)
{
	vertex_of_edge[0] = v0;
	vertex_of_edge[1] = v1;
}
//construct facet by 3 vertex pointer, vertex_of_facet array is then initialized
facet::facet(vertex* v0, vertex* v1, vertex* v2)
{
	vertex_of_facet[0] = v0;
	vertex_of_facet[1] = v1;
	vertex_of_facet[2] = v2;
}

//compare a vertex to another by their (x,y,z)
bool vertex::operator==(vertex referent)
{
	if (fabs(x - referent.x) <= 1E-6 && fabs(y - referent.y) <= 1E-6 && fabs(z - referent.z) <= 1E-6)
		return 1;
	else
		return 0;
}
//compare an edge to another by their vertex_of_edge array
bool edge::operator==(edge referent)
{
	bool flag = 0;
	for (int i = 0; i < 2; i++)
	{
		if (*vertex_of_edge[0] == *(referent.vertex_of_edge[i])\
			&& *vertex_of_edge[1] == *(referent.vertex_of_edge[(i + 1) % 2]))
		{
			flag = 1;
		}
	}
	return flag;
}

//compare a facet to another by their vertex_of_facet array
bool facet::operator==(facet referent)
{
	bool flag = 0;
	for (int i = 0; i < 3; i++)
	{
		if (*vertex_of_facet[0] == *(referent.vertex_of_facet[i])\
				&& *vertex_of_facet[1] == *(referent.vertex_of_facet[(i + 1) % 3])\
				&& *vertex_of_facet[2] == *(referent.vertex_of_facet[(i + 2) % 3]))
		{
			flag = 1;
		}		
	}
	return flag;
}

double* facet:: line_intersection(double* &Ans, double* &L1, double* &L2, double* &P1, double* &P2)
{
	//if 2 lines parallel
	if (fabs(L1[0] * L2[1] - L1[1] * L2[0]) < 1E-6 && \
		fabs(L1[1] * L2[2] - L1[2] * L2[1]) < 1E-6 && \
		fabs(L1[2] * L2[0] - L1[0] * L2[2]) < 1E-6)
	{
		return NULL;
	}
	//if 2 lines do not parallel	
	else
	{
		//Ab is set up to store informations of the 2 lines in order to find their intersection points later on
		double** Ab = new double*[2];
		for(int i = 0; i < 2; i++)
		{
			Ab[i] = new double[3];
		}
		
		//                                            [L1x L1y]
		//if the deteminant of the coefficient matrix [L2x L2y] is not zero,
		//x, y terms are used to establish relationships as follow:
		//x: L1x*s+P1x = L2x*t+P2x ---> L1x*s-L2x*t = P2x-P1x,
		//y: L1y*s+P1y = L2y*t+P2y ---> L1y*s-L2y*t = P2y-P1y,
		//where s and t stands for parameter of L1 and L2, respectively, which are the unknowns going to be solved
		if(fabs(L1[0]*L2[1]-L1[1]*L2[0]) > 1E-5)
		{
			Ab[0][0] = L1[0];//L1x
			Ab[0][1] = -L2[0];//-L2x
			Ab[0][2] = -P1[0] + P2[0];//P2x-P1x
			Ab[1][0] = L1[1];//L1y
			Ab[1][1] = -L2[1];//-L2y
			Ab[1][2] = -P1[1] + P2[1];//P2y-P1y
		}
		//                                            [L1y L1z]
		//if the deteminant of the coefficient matrix [L2y L2z] is not zero,
		//y, z terms are used to establish relationships as follow:
		//y: L1y*s+P1y = L2y*t+P2y ---> L1y*s-L2y*t = P2y-P1y,
		//z: L1z*s+P1z = L2z*t+P2z ---> L1z*s-L2z*t = P2z-P1z,
		//where s and t stands for parameter of L1 and L2, respectively, which are the unknowns going to be solved
		else if(fabs(L1[1]*L2[2]-L1[2]*L2[1]) > 1E-5)
		{
			Ab[0][0] = L1[1];//L1y
			Ab[0][1] = -L2[1];//-L2y
			Ab[0][2] = -P1[1] + P2[1];//P2y-P1y
			Ab[1][0] = L1[2];//L1z
			Ab[1][1] = -L2[2];//-L2z
			Ab[1][2] = -P1[2] + P2[2];//P2z-P1z
		}
		//                                            [L1x L1y]
		//if the deteminant of the coefficient matrix [L2x L2y] is zero,
		//                                             [L1y L1z]
		//and the deteminant of the coefficient matrix [L2y L2z] is zero as well,
		//x, z terms are used to establish relationships as follow:
		//x: L1x*s+P1x = L2x*t+P2x ---> L1x*s-L2x*t = P2x-P1x,
		//z: L1z*s+P1z = L2z*t+P2z ---> L1z*s-L2z*t = P2z-P1z,
		//where s and t stands for parameter of L1 and L2, respectively, which are the unknowns going to be solved
		else
		{
			Ab[0][0] = L1[0];//L1x
			Ab[0][1] = -L2[0];//-L2x
			Ab[0][2] = -P1[0] + P2[0];//P2x-P1x
			Ab[1][0] = L1[2];//L1z
			Ab[1][1] = -L2[2];//-L2z
			Ab[1][2] = -P1[2] + P2[2];//P2z-P1z
		}
		
		//X is set up to store the solved s and t
		double* X = new double[2];
		X[0] = 0; X[1] = 0;
		//P_intersect is set up to store the coordinates of the intersection point
		double P_intersect[3] = {0,0,0};

		//s,t are solved by Cramer's Formula and return to X
		X = CramersFormula(X, Ab, 2);
		//if 2 lines are skewed
		if (X == NULL)
		{
			return NULL;
		}
		//if 2 lines are not skewed
		else
		{
			//calculate coordinates of the intersection point according to the solved s, line vector of L1 and point of L1
			P_intersect[0] = X[0] * L1[0] + P1[0];
			P_intersect[1] = X[0] * L1[1] + P1[1];
			P_intersect[2] = X[0] * L1[2] + P1[2];

			//store the point coordinate and parameters s, t to Ans then return
			Ans = new double[5];
			Ans[0] = P_intersect[0]; Ans[1] = P_intersect[1]; Ans[2] = P_intersect[2];
			Ans[3] = X[0]; Ans[4] = X[1];
			return Ans;
		}
	}
}

double* facet::plane_intersection(double* &Ans, facet* F)
{
	//C1 and C2 are set up to store the plane constant of the executing facet and facet F, respectively
	double C1 = nx*vertex_of_facet[0]->x + ny*vertex_of_facet[0]->y + nz*vertex_of_facet[0]->z;
	double C2 = F->nx*F->vertex_of_facet[0]->x + F->ny*F->vertex_of_facet[0]->y + F->nz*F->vertex_of_facet[0]->z;

	//if 2 planes parallel
	if (fabs(cross(*F).length()) < 1E-6)
	{
		return NULL;
	}
	//if 2 planes do not parallel		
	else
	{
		//Ab is set up to store informations of the 2 planes in order to find a point on their intersection line later on
		double** Ab = new double*[2];
		for(int i = 0; i < 2; i++)
		{
			Ab[i] = new double[3];
		}
		//                                            [n1x n1y]
		//if the deteminant of the coefficient matrix [n2x n2y] is not zero,
		//z coordinate of the point on intersection line can be assumed arbitrarily,
		//x, y coordinates of the point can be solved by the relationships in the following:
		//n1x*x+n1y*y = C1,
		//n2x*y+n2y*y = C2.
		if(fabs(nx*F->ny-ny*F->nx)>1E-5)
		{
			Ab[0][0] = nx;//n1x
			Ab[0][1] = ny;//n1y
			Ab[0][2] = C1 - nz * 0;//C1-n1z*z, where z is assumed to be 0
			Ab[1][0] = F->nx;//n2x
			Ab[1][1] = F->ny;//n2y
			Ab[1][2] = C2 - F->nz * 0;//C2-n2z*z, where z is assumed to be 0
		}
		//                                            [n1y n1z]
		//if the deteminant of the coefficient matrix [n2y n2z] is not zero,
		//x coordinate of the point on intersection line can be assumed arbitrarily,
		//y, z coordinates of the point can be solved by the relationships in the following:
		//n1y*y+n1z*z = C1,
		//n2y*y+n2z*z = C2.
		else if(fabs(ny*F->nz-nz*F->ny)>1E-5)
		{
			Ab[0][0] = ny;//n1y
			Ab[0][1] = nz;//n1z
			Ab[0][2] = C1 - nx * 0;//C1-n1x*x, where x is assumed to be 0
			Ab[1][0] = F->ny;//n2y
			Ab[1][1] = F->nz;//n2z
			Ab[1][2] = C2 - F->nx * 0;//C2-n2x*x, where x is assumed to be 0
		}
		//                                            [n1x n1y]
		//if the deteminant of the coefficient matrix [n2x n2y] is zero,
		//                                             [n1y n1z]
		//and the deteminant of the coefficient matrix [n2y n2z] is zero as well,
		//y coordinate of the point on intersection line can be assumed arbitrarily,
		//x, z coordinates of the point can be solved by the relationships in the following:
		//n1x*x+n1z*z = C1,
		//n2x*x+n2z*z = C2.
		else
		{
			Ab[0][0] = nx;//n1x
			Ab[0][1] = nz;//n1z
			Ab[0][2] = C1 - ny * 0;//C1-n1y*y, where y is assumed to be 0
			Ab[1][0] = F->nx;//n2x
			Ab[1][1] = F->nz;//n2z
			Ab[1][2] = C2 - F->ny * 0;//C2-n2y*y, where y is assumed to be 0
		}
		
		//X is set up to store the solved coordinates
		double* X = new double[2];
		X[0] = 0; X[1] = 0;

		//coordinates are solved by Cramer's Formula and return to X
		X = CramersFormula(X, Ab, 2);
		
		//store the point coordinates to Ans for different cases
		if(fabs(nx*F->ny-ny*F->nx) > 1E-5)
		{
			Ans[0] = X[0]; Ans[1] = X[1]; Ans[2] = 0;
		}
		else if(fabs(ny*F->nz-nz*F->ny) > 1E-5)
		{
			Ans[1] = X[0]; Ans[2] = X[1]; Ans[0] = 0;
		}
		else
		{
			Ans[0] = X[0]; Ans[2] = X[1]; Ans[1] = 0;
		}
		//calculate the vector of the intersection line and store to Ans then return
		Ans[3] = cross(*F).x; Ans[4] = cross(*F).y; Ans[5] = cross(*F).z;
		return Ans;		
	}
}

bool facet::if_point_in_facet(double* &P)
{
	//calculate the 3 vectors that start from the 3 vertexes of facet and end up at point P
	VEC V1 = VEC(P[0] - vertex_of_facet[0]->x, P[1] - vertex_of_facet[0]->y, P[2] - vertex_of_facet[0]->z);
	VEC V2 = VEC(P[0] - vertex_of_facet[1]->x, P[1] - vertex_of_facet[1]->y, P[2] - vertex_of_facet[1]->z);
	VEC V3 = VEC(P[0] - vertex_of_facet[2]->x, P[1] - vertex_of_facet[2]->y, P[2] - vertex_of_facet[2]->z);
	//calculate the facet area by the 2 vectors formed by 2 edges of facet
	double area_by_edge = fabs(edge_of_facet[0]->cross(*edge_of_facet[1]).length() / 2.0);
	//calculate the facet area by the 3 vectors formed by 3 vertexes of facets and point P
	double area_by_point = fabs((V1.cross(V2).length() + V2.cross(V3).length() + V3.cross(V1).length()) / 2.0);
	//if area_by_edge ~= area_by_point, implies that point P is in the facet
	return fabs((area_by_edge - area_by_point)/area_by_edge) < 0.005;
}

double* facet::find_intersect_line(double* &P_intersect, facet* F)
{
	//find the intersection line between facet F and the executing facet
	//return the coordinate of the point on line and the vector of the line to Ans1
	double* Ans1 = new double[6];
	Ans1 = plane_intersection(Ans1, F);
	//if 2 facets do intersect
	if (Ans1 != NULL)
	{
		//intersect_parameter is set up to store the parameters when intersection line intersects with the 6 edges of the 2 facets
		//L1 is set up to store the vector of the intersection line of the 2 facets, and P1 is set up to store the coordinate of the point on such line
		//L2 is set up to store the vector of each edges from the 2 facets, and P2 is set up to store the coordinate of the point on such line 
		double intersect_parameter[6] = { 10000,10000,10000,10000,10000,10000 };
		double* L1 = new double[3];
		double* L2 = new double[3];
		double* P1 = new double[3];
		double* P2 = new double[3];
		L1[0] = Ans1[3]; L1[1] = Ans1[4]; L1[2] = Ans1[5];
		P1[0] = Ans1[0]; P1[1] = Ans1[1]; P1[2] = Ans1[2];

		//for each edges from the executing facet,
		//if it intersects with the intersection line, return the intersection point and the corresponding parameter to Ans2
		double* Ans2 = new double[5];
		for (int i = 0; i < 3; i++)
		{
			L2[0] = vertex_of_facet[i % 3]->x - vertex_of_facet[(i + 1) % 3]->x;
			L2[1] = vertex_of_facet[i % 3]->y - vertex_of_facet[(i + 1) % 3]->y;
			L2[2] = vertex_of_facet[i % 3]->z - vertex_of_facet[(i + 1) % 3]->z;
			P2[0] = vertex_of_facet[i % 3]->x;
			P2[1] = vertex_of_facet[i % 3]->y;
			P2[2] = vertex_of_facet[i % 3]->z;

			Ans2 = line_intersection(Ans2, L1, L2, P1, P2);
			//if edge does intersect with the intersection line, and the intersection point locates in the executing facet as well as facet F
			if (Ans2 != NULL && if_point_in_facet(Ans2) && F->if_point_in_facet(Ans2))
			{
				//flag is set up to ensure same parameter is not stored repeatedly
				bool flag = 0;
				for(int j = 0; j < i; j++)
				{
					if(intersect_parameter[j] == Ans2[3])
					{
						flag = 1;
						break;
					}
				}
				if(!flag)
				{
					intersect_parameter[i] = Ans2[3];			
				}
			}
		}

		//for each edges from facet F,
		//if it intersects with the intersection line, return the intersection point and the corresponding parameter to Ans2
		for (int i = 0; i < 3; i++)
		{
			L2[0] = F->vertex_of_facet[i % 3]->x - F->vertex_of_facet[(i + 1) % 3]->x;
			L2[1] = F->vertex_of_facet[i % 3]->y - F->vertex_of_facet[(i + 1) % 3]->y;
			L2[2] = F->vertex_of_facet[i % 3]->z - F->vertex_of_facet[(i + 1) % 3]->z;
			P2[0] = F->vertex_of_facet[i % 3]->x;
			P2[1] = F->vertex_of_facet[i % 3]->y;
			P2[2] = F->vertex_of_facet[i % 3]->z;

			Ans2 = line_intersection(Ans2, L1, L2, P1, P2);
			//if edge does intersect with the intersection line, and the intersection point locates in the executing facet as well as facet F
			if (Ans2 != NULL && if_point_in_facet(Ans2) && F->if_point_in_facet(Ans2))
			{
				//flag is set up to ensure same parameter is not stored repeatedly
				bool flag = 0;
				//check the intersect_parameter[j] from 0 to (3+i), which 3 implies the 3 parameters that have stored from the executing facet
				for(int j = 0; j < 3+i; j++)
				{
					if(intersect_parameter[j] == Ans2[3])
					{
						flag = 1;
						break;
					}
				}
				if(!flag)
				{
					intersect_parameter[3+i] = Ans2[3];			
				}
			}
		}
		
		//do the bubble sort for elements in intersect_paramter to make the left always smaller than the right
		bool flag = 1;
		for (int i = 0; (i < 6) && flag; i++)
		{
			flag = 0;
			for (int j = 0; j < (6 - 1); j++)
			{
				if (intersect_parameter[j + 1] < intersect_parameter[j])
				{
					double tmp = intersect_parameter[j];
					intersect_parameter[j] = intersect_parameter[j + 1];
					intersect_parameter[j + 1] = tmp;
					flag = 1;
				}
			}
		}

		//parameter_range is set up to store the largest range of parameter while 10000 is not involved
		double parameter_range[2] = {10000.0,10000.0};
		if(intersect_parameter[0] != 10000)
		{
			parameter_range[0] = intersect_parameter[0];
			for(int i = 1; i < 6; i++)
			{
				if(intersect_parameter[i] != 10000)
				{
					parameter_range[1] = intersect_parameter[i];
				}
				else
				{
					break;
				}
			}
		}
		//if 10000 is involved in the parameter range, 
		//it implies that there's no 2 proper intersection points between the intersection line and the 6 edges
		if (parameter_range[0] == 10000 || parameter_range[1] == 10000)
			return NULL;
		else
		{
			//the resulted intersection line is returned in form of the 2 intersection points coordinates
			//coordinates of the intersection points are caculated according to the vector, point of the intersection line, and the parameter
			P_intersect = new double[6];
			P_intersect[0] = L1[0]*parameter_range[0] + P1[0];
			P_intersect[1] = L1[1]*parameter_range[0] + P1[1];
			P_intersect[2] = L1[2]*parameter_range[0] + P1[2];
			P_intersect[3] = L1[0]*parameter_range[1] + P1[0];
			P_intersect[4] = L1[1]*parameter_range[1] + P1[1];
			P_intersect[5] = L1[2]*parameter_range[1] + P1[2];
			return P_intersect;			
		}			
	}
	//if 2 facets do not intersect
	else
	{
		return NULL;
	}
	
}

bool facet::if_facets_are_possible_to_intersect(facet* F)
{
	bool flag1 = 0;
	bool flag2 = 0;
	//C1 and C2 are set up to store the plane constant of the executing facet and facet F, respectively
	double C1 = nx*vertex_of_facet[0]->x + ny*vertex_of_facet[0]->y + nz*vertex_of_facet[0]->z;
	double C2 = F->nx*F->vertex_of_facet[0]->x + F->ny*F->vertex_of_facet[0]->y + F->nz*F->vertex_of_facet[0]->z;
	
	//if at least 1 vertex of the executing facet is above the plane formed by facet F, and 1 below,
	//that is, 1 vertex (x1,y1,z1) makes n2x*x1+n2y*y1+n2z*z1-C2 > 0 while 1 vertex (x2,y2,z2) makes n2x*x2+n2y*y2+n2z*z2-C2 < 0,
	//flag1 is set to 1 then break loop
	for(int i = 0; i < 3; i++)
	{
		double tmp1 = F->nx*vertex_of_facet[i]->x + F->ny*vertex_of_facet[i]->y + F->nz*vertex_of_facet[i]->z - C2;
		double tmp2 = F->nx*vertex_of_facet[(i+1)%3]->x + F->ny*vertex_of_facet[(i+1)%3]->y + F->nz*vertex_of_facet[(i+1)%3]->z - C2;
		if( tmp1*tmp2 <= 1E-5 )
		{
			flag1 = 1;
			break;
		}
	}
	//if at least 1 vertex of facet F is above the plane formed by the executing facet, and 1 below,
	//that is, 1 vertex (x1,y1,z1) makes n1x*x1+n1y*y1+n1z*z1-C1 > 0 while 1 vertex (x2,y2,z2) makes n1x*x2+n1y*y2+n1z*z2-C1 < 0,
	//flag2 is set to 1 then break loop
	for(int i = 0; i < 3; i++)
	{
		double tmp1 = nx*F->vertex_of_facet[i]->x + ny*F->vertex_of_facet[i]->y + nz*F->vertex_of_facet[i]->z - C1;
		double tmp2 = nx*F->vertex_of_facet[(i+1)%3]->x + ny*F->vertex_of_facet[(i+1)%3]->y + nz*F->vertex_of_facet[(i+1)%3]->z - C1;
		if( tmp1*tmp2 <= 1E-5 )
		{
			flag2 = 1;
			break;
		}
	}
	//only if flag1 and flag2 are 1 can 2 facet intersect
	return flag1*flag2;
}