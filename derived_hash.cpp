#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "derived_hash.h"

//which bucket to place the vertex in is determined through the numerical characteristics of the vertex coordinates
unsigned long int vertex_hash::Hash(vertex* feature)
{
	double X = feature->x;
	double Y = feature->y;
	double Z = feature->z;

	return ((unsigned long int)(fabs(X + Y + Z)*1E6)) % tableSize;
}

//which bucket to place the edge in is determined through the numerical characteristics of the 2 vertex coordinates
unsigned long int edge_hash::Hash(edge* feature)
{
	double X = (feature->vertex_of_edge[0]->x + feature->vertex_of_edge[1]->x);
	double Y = (feature->vertex_of_edge[0]->y + feature->vertex_of_edge[1]->y);
	double Z = (feature->vertex_of_edge[0]->z + feature->vertex_of_edge[1]->z);	
	
	return ((unsigned long int)(fabs(X + Y + Z)*1E6)) % tableSize;
}

void edge_hash::draw_free_edge(char* fo)
{
	//write "osnap none" to the script which is with the name same as fo
	FILE *fpout;
	fpout = fopen(fo, "w");
	fprintf(fpout, "osnap none\n");
	unsigned long int _count = 0;
	//traverse all edges in the executing edge_hash
	for (unsigned long int i = 0; i < tableSize; i++)
	{
		item* ptr = HashTable[i];
		while (ptr != NULL)
		{
			//calculate completeness of the process in order to show to users
			_count++;
			if (_count % (unsigned long int)( count/ 10.0) == 0)
			{
					if(_count / (unsigned long int)(count / 10.0) <= 10)
					{
						cout << "	";
						cout << _count / (unsigned long int)(count / 10.0) *10.0 << " %" << endl;
					}				
			}
			//if facet_of_edge has only 1 element, that is, edge only has 1 related facet, this edge is a free edge.
			//write this edge to script.
			if (ptr->feature != NULL && ptr->feature->facet_of_edge.size() == 1)
			{
				fprintf(fpout, "LINE ");
				for (int j = 0; j < 2; j++)
				{
					fprintf(fpout, "%f,%f,%f ", \
						ptr->feature->vertex_of_edge[j]->x, \
						ptr->feature->vertex_of_edge[j]->y, \
						ptr->feature->vertex_of_edge[j]->z);
				}
				fprintf(fpout, "\n");

			}
			ptr = ptr->next;
		}
	}
	cout << "	complete!!" << endl;
	fprintf(fpout, "zoom E\n");
}

//which bucket to place the facet in is determined through the numerical characteristics of the 3 vertex coordinates
unsigned long int facet_hash::Hash(facet* feature)
{
	double X = (feature->vertex_of_facet[0]->x + feature->vertex_of_facet[1]->x + feature->vertex_of_facet[2]->x);
			 
	double Y = (feature->vertex_of_facet[0]->y + feature->vertex_of_facet[1]->y + feature->vertex_of_facet[2]->y);
			 
	double Z = (feature->vertex_of_facet[0]->z + feature->vertex_of_facet[1]->z + feature->vertex_of_facet[2]->z);
	
	return ((unsigned long int)(fabs(X + Y + Z)*1E6)) % tableSize;
}

void facet_hash:: draw_intersection_line(facet_hash& facet_list, char* fo)
{
	//write "osnap none" to the script which is with the name same as fo
	FILE *fpout;
	fpout = fopen(fo, "w");
	fprintf(fpout, "osnap none\n");
	unsigned long int _count = 0;
	//traverse all facets in the executing facet_hash
	for (unsigned long int i = 0; i < tableSize; i++)
	{
		item* ptr1 = HashTable[i];
		while (ptr1 != NULL)
		{
			if (ptr1->feature != NULL)
			{
				//for each facets in the excuting facet_hash, 
				//in order to find whether this facet intersects with any facets in facet_hash facet_list, traverse all facets in facet_list.
				for (unsigned long int j = 0; j < facet_list.tableSize; j++)
				{
					item* ptr2 = facet_list.HashTable[j];
					while (ptr2 != NULL)
					{
						if (ptr2->feature != NULL)
						{
							//calculate completeness of the process in order to show to users
							_count++;
							if (_count % (unsigned long int)(count*facet_list.count / 10.0) == 0)
							{
								if(_count / (unsigned long int)(count*facet_list.count / 10.0) <= 10)
								{
									cout << "	";
									cout << _count / (unsigned long int)(count*facet_list.count / 10.0) *10.0 << " %" << endl;
								}				
							}

							//if 2 facets are possible to intersect
							if(ptr1->feature->if_facets_are_possible_to_intersect(ptr2->feature))
							{
								//find the intersection line and return the coordinates of 2 end points of the line to double* edge
								double* edge = new double[6];
								edge = ptr1->feature->find_intersect_line(edge, ptr2->feature);
								//if intersection line do exist, write the coordinates of 2 end points of the line to script
								if (edge != NULL)
								{
									fprintf(fpout, "LINE ");
									fprintf(fpout, "%f,%f,%f ", edge[0], edge[1], edge[2]);
									fprintf(fpout, "%f,%f,%f \n", edge[3], edge[4], edge[5]);
								}
							}														
						}
						ptr2 = ptr2->next;
					}
				}
			}
			ptr1 = ptr1->next;			
		}
	}
	cout << "	complete!!" << endl;
	fprintf(fpout, "zoom E\n");
}

unsigned long int sort_edge_for_edge_hash::Hash(edge* feature)
{
	//if edge has 2 related facets, which bucket to put the edge in is determined by dihedral angle
	//(all the mentioned dihedral angle are in fact, included angle between the normal vectors of the 2 related facets)
	if (feature->facet_of_edge.size() == 2)
	{
		double product = feature->facet_of_edge[0]->dot(*feature->facet_of_edge[1]);
		double modulus = feature->facet_of_edge[0]->length()*feature->facet_of_edge[1]->length();
		double theta; 
		if (product / modulus > 1.0)
		{
			theta = 0.0;
		}
		else if (product / modulus < -1.0)
		{
			theta = 3.1415926;
		}
		else
		{
			theta = acos(product / modulus);
		}
		return (unsigned long int)fabs(theta / 3.1415926*180.0) % tableSize;
	}
	//if edge does not have 2 related facets, 
	//which may be caused by the error of the stl file,
	//which bucket to put the edge in is determined by the dihedral angle of the adjacent edge
	else
	{
		unsigned long int _Hash = 180;
		bool flag = 0;
		//find adjacent edges from 2 vertexes
		for (int i = 0; i < 2; i++)
		{
			if (flag == 1) break;
			//for any of the adjacent edges that has 2 related facets,
			//if its included angle with the executing edge is small enough,
			//which implies that the adjacent edge is an extension of the executing edge,
			//dihedral angle of the adjacent edge can be see as the dihedral angle of the executing edge
			for (int j = 0; j < feature->vertex_of_edge[i]->edge_of_vertex.size(); j++)
			{
				if (feature->vertex_of_edge[i]->edge_of_vertex[j]->facet_of_edge.size() == 2)
				{
					double product = feature->vertex_of_edge[i]->edge_of_vertex[j]->dot(*feature);
					double modulus = feature->vertex_of_edge[i]->edge_of_vertex[j]->length()*feature->length();
					double theta;
					if (product / modulus > 1.0)
					{
						theta = 0.0;
					}
					else if (product / modulus < -1.0)
					{
						theta = 3.1415926;
					}
					else
					{
						theta = acos(product / modulus);
					}
					//check if the included angle between edge and the adjacent edge is small enough
					if (theta < 0.0087 || theta > 3.1328)
					{
						flag = 1;
						_Hash = Hash(feature->vertex_of_edge[i]->edge_of_vertex[j]);
						break;
					}
				}
			}
		}
		//if all of the adjacent edges that has 2 related facets can not be see as the extension of the executing edge,
        //the executing edge will be put into the last bucket, which is 180
		return _Hash;
	}
}

void sort_edge_for_edge_hash::sort_edge(edge_hash referent)
{
	unsigned long int count = 0;
	//traverse all edges in edge_hash referent
	cout << endl;
	for (unsigned long int i = 0; i < referent.tableSize; i++)
	{
		item* ptr = referent.HashTable[i];
		while (ptr != NULL)
		{
			if (ptr->feature != NULL)
			{
				//add edge to the executing edge_hash
				AddItem(ptr->feature);
			}
			ptr = ptr->next;

			//calculate completeness of the process in order to show to users
			if ((count + 1) % (unsigned long int)(referent.count / 10.0) == 0)
			{
				if((count + 1) / (unsigned long int)(referent.count / 10) <= 10)
				{
					cout << "	";
					cout << (count + 1) / (unsigned long int)(referent.count / 10) * 10.0 << " %" << endl;
				}				

			}
			count++;
		}
	}
	cout << "	complete!!" << endl;
}

void sort_edge_for_edge_hash::draw_edge(char* fo, double degree_ll, double degree_ul)
{
	//write "osnap none" to the script which is with the name same as fo
	FILE *fpout;
	fpout = fopen(fo, "w");
	fprintf(fpout, "osnap none\n");
	//traverse edges from bucket int(degree_ll) to int(degree_ul)
	for (int i = int(degree_ll); i <= int(degree_ul); i++)
	{
		item* ptr = HashTable[i];
		while (ptr != NULL)
		{
			//write edge to script
			if (ptr->feature != NULL)
			{
				fprintf(fpout, "LINE ");
				for (int j = 0; j < 2; j++)
				{
					fprintf(fpout, "%f,%f,%f ", \
						ptr->feature->vertex_of_edge[j]->x, \
						ptr->feature->vertex_of_edge[j]->y, \
						ptr->feature->vertex_of_edge[j]->z);
				}
				fprintf(fpout, "\n");
			}
			ptr = ptr->next;
		}
	}
	fprintf(fpout, "zoom E\n");
}