#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "features.h"
#include "derived_hash.h"

using namespace std;
char** input_file_list = new char*[10];
vertex_hash vertex_list[10];
edge_hash edge_list[10];
facet_hash facet_list[10];
sort_edge_for_edge_hash sort_edge_list[10];

void read_stl_and_establish_vertex_edge_facet_relationship(char* fn);
char* output_file_format(char* fo, char** input_file_list, int input_file_num);
void find_edges(char* fo, int input_file_num);
void find_free_edges(char* fo, int input_file_num);
void find_intersection_line_of_two_model(facet_hash& facet_list_1, facet_hash& facet_list_2, char* fo);

int num_of_input_file = 0;

int main()
{
	char ch, ans;//store for the read-in character, and answer to whether to input more file, respectively
	char _fn[] = "cylinder.stl";//store for default input file name
	int i = 0, j = 0;//i, j are set up to count for any purpose
	FILE *fp = NULL;
	
	//if fp = NULL, implies that this is the first loop, or such input file name do not exist, or file is input repeatedly
	//for fp = NULL or to input more file, if number of the input files do not exceed 10, repeat the loop
	while ((fp == NULL || ans == 'y' || ans == 'Y') && (ans != 'n' && ans != 'N') && num_of_input_file < 10)
	{
		cout << "======================================================================" << endl;
		cout << "----------------------------------------------------------------------" << endl;
		cout << "Enter the input file name." << endl;
		cout << "Input file name (.stl) (default: cylinder.stl): ";
		bool EnterLoop_flag = 0, FileFormat_flag = 0;

		//if neither enter is detected nor number of characters input exceed 100, 
		//keep storing characters to the current input_file_list element
		input_file_list[num_of_input_file] = new char[100];
		for (i = 0; i < 100 && (ch = getchar()) != '\n'; ++i)
		{
			//ever if the read characters loop is entered should EnterLoop_flag set to 1
			EnterLoop_flag = 1;
			//as point is detected, that is, file format is mentioned by the users, FileFormat_flag is set to 1
			if (ch == '.') FileFormat_flag = 1;
			input_file_list[num_of_input_file][i] = ch;
		}
		
		//if FileFormat_flag = 0, that is, lack of file format, file format ".stl" will be filled up automatically
		if (FileFormat_flag == 0)
		{
			input_file_list[num_of_input_file][i + 0] = '.';
			input_file_list[num_of_input_file][i + 1] = 's';
			input_file_list[num_of_input_file][i + 2] = 't';
			input_file_list[num_of_input_file][i + 3] = 'l';
			input_file_list[num_of_input_file][i + 4] = '\0';
			i+=4;
		}
		else
		{
			input_file_list[num_of_input_file][i + 0] = '\0';
		}			

		bool FileRepeat_flag = 0;
		//if EnterLoop_flag = 0, establish vertex-edge-facet relationship for "cylinder.stl" if it is not yet in input_file_list
		if (EnterLoop_flag == 0)
		{
			cout << endl;
			//traverse the input_file_list to check whether input file is already in list.
			//if input file is already in list, set FileRepeat_flag to 1, set fp to NULL, show warning to users
			for(i = 0; i < num_of_input_file; i++)
			{
				string str1 = input_file_list[i];
				string str2 = _fn;
				if(str1 == str2)
				{
					FileRepeat_flag = 1;
					fp = NULL;
					cout << endl;
					cout << "*** Input file repeatedly!" << endl;
					break;
				}
			}
			//if FileRepeat_flag = 0, that is, input file is not yet in list, establish vertex-edge-facet relationship for "cylinder.stl"
			if(FileRepeat_flag == 0)
			{
				input_file_list[num_of_input_file] = _fn;
				fp = fopen(input_file_list[num_of_input_file], "rb");
				cout << ">>>>>>>>>> " << "Input file: " << input_file_list[num_of_input_file] << endl;
				read_stl_and_establish_vertex_edge_facet_relationship(input_file_list[num_of_input_file]);
				//increase the size of the input_file_list
				num_of_input_file++;
			}
		}
		//if EnterLoop_flag = 1, establish vertex-edge-facet relationship for file input by users
		else
		{
			//traverse the input_file_list to check whether input file is already in list.
			//if input file is already in list, set FileRepeat_flag to 1, set fp to NULL, show warning to users
			for(i = 0; i < num_of_input_file; i++)
			{
				string str1 = input_file_list[i];
				string str2 = input_file_list[num_of_input_file];
				if(str1 == str2)
				{
					FileRepeat_flag = 1;
					fp = NULL;
					cout << endl;
					cout << "*** Input file repeatedly!" << endl;
					break;
				}
			}
			//if FileRepeat_flag = 0, that is, input file is not yet in list, establish vertex-edge-facet relationship for file input by users
			if(FileRepeat_flag == 0)
			{
				fp = fopen(input_file_list[num_of_input_file], "rb");
				//if fn = NULL, warnings is show to users
				if (fp == NULL)
				{
					cout << endl;
					cout << "*** Error in opening " << input_file_list[num_of_input_file] << "!" << endl;
				}
				else
				{
					cout << endl;
					cout << ">>>>>>>>>> " << "Input file: " << input_file_list[num_of_input_file] << endl;
					read_stl_and_establish_vertex_edge_facet_relationship(input_file_list[num_of_input_file]);
					//increase the size of the input_file_list
					num_of_input_file++;
				}
			}			
		}
		cout << endl;
		if(num_of_input_file < 10)
		{
			cout << "Input more file? (y/n) ";
			ans = getchar();
			cout << endl;
		}		
		//clean up the residual '\n' character in buffer
		getchar();
	}
	ans = 'y';

	//to input more file, repeat the loop
	while (ans == 'y' || ans == 'Y' && ans != 'n' && ans != 'N')
	{
		//for ans is not '1', '2', or '3', repeat the loop
		while (ans != '1' && ans != '2' && ans != '3')
		{
			cout << "======================================================================" << endl;
			cout << "----------------------------------------------------------------------" << endl;
			cout << "1) Output free edges of the selected model as script" << endl;
			cout << endl <<  "2) Output edges of the selected model as script" << endl;
			cout << endl <<  "3) Output the intersection lines of the selected models as script" << endl;
			cout << endl << "Enter the number of option to select the function to use" << " (default: 2): ";
			
			bool EnterLoop_flag = 0;
			//if enter is not detected, store character to ans and set EnterLoop_flag to 1
			if((ch = getchar()) != '\n')
			{
				EnterLoop_flag = 1;
				ans = ch;
				//clean up the residual '\n' character in buffer
				getchar();
				cout << endl;			
			}					
			//if EnterLoop_flag = 0, ans is set to '2' by default
			if (EnterLoop_flag == 0)
			{
				ans = '2';			
				cout << endl;
			}
			//if ans = '1'
			if (ans == '1')
			{
				cout << ">>> Output free edges of the selected model as script." << endl;			
				cout << "    From input files shown below, " << endl << endl;
				//print the input_file_list to users
				for(i = 0; i < num_of_input_file; i++)
				{
					cout << "    ";
					cout << i+1 << ") ";
					cout << input_file_list[i];
					cout << "," << endl << endl;
				}
				//if the selected option is invalid, repeat the loop
				i = -1;
				while(i > num_of_input_file-1 || i < 0)
				{
					cout << "    select the model you want to output free edges";
					cout << " (" << 1 << "~" << num_of_input_file << ") ";
					cout << "(default: 1): ";
					ch = getchar();
					//if enter is not detected, read character and store to i 
					if(ch != '\n')
					{
						i = ch - '0'; i-=1;
						//clean up the residual '\n' character in buffer
						getchar();
					}
					else
					{
						i = 0;
					}
				}
				cout << endl << "    >>>>>>>>>> Output free edges of  " << input_file_list[i] << endl;	
				//name the ouput file name by users' input or according to the chosen input file name
				char* fo = new char[100];	
				cout << endl << "    ";
				fo = output_file_format(fo,input_file_list, i);
				cout << "    >>>>>>>>>> " << "Output file: " << fo << endl;
				getchar();
				//output free edges to the ouput file
				find_free_edges(fo,i);
			}
			//if ans = '2'
			else if (ans == '2')
			{
				cout << ">>> Output edges of the selected model as script." << endl;
				cout << "    From input files shown below, " << endl << endl;
				//print the input_file_list to users
				for(i = 0; i < num_of_input_file; i++)
				{
					cout << "    ";
					cout << i+1 << ") ";
					cout << input_file_list[i];
					cout << "," << endl << endl;
				}
				//if the selected option is invalid, repeat the loop
				i = -1;
				while(i > num_of_input_file-1 || i < 0)
				{
					cout << "    select the model you want to output edges";
					cout << " (" << 1 << "~" << num_of_input_file << ") ";
					cout << "(default: 1): ";
					//clean up the residual '\n' character in buffer
					ch = getchar();
					//if enter is not detected, read character and store to i
					if(ch != '\n')
					{
						i = ch - '0'; i-=1;
						//clean up the residual '\n' character in buffer
						getchar();
					}
					else
					{
						i = 0;
					}
				}
				cout << endl << "    >>>>>>>>>> Output edges of  " << input_file_list[i] << endl;	
				//name the ouput file name by users' input or according to the chosen input file name
				char* fo = new char[100];
				cout << endl << "    ";
				fo = output_file_format(fo,input_file_list, i);
				cout << "    >>>>>>>>>> " << "Output file: " << fo << endl;
				getchar();
				//output edges to the ouput file
				find_edges(fo,i);
			}
			//if ans = '3'
			else if (ans == '3')
			{
				cout << ">>> Output the intersection lines of the selected models as script." << endl;
				cout << "    From input files shown below, " << endl << endl;
				//print the input_file_list to users
				for(i = 0; i < num_of_input_file; i++)
				{
					cout << "    ";
					cout << i+1 << ") ";
					cout << input_file_list[i];
					cout << "," << endl << endl;
				}
				//if the selected option is invalid, repeat the loop
				i = -1; j = -1;
				while(i > num_of_input_file-1 || i < 0 || j > num_of_input_file-1 || j < 0)
				{
					cout << "    select the model you want to output intersection lines";
					cout << " (" << 1 << "~" << num_of_input_file << ") ";
					cout << "(default: 1 " << num_of_input_file << "): ";
					//if enter is not detected, read characters and store to i and j
					if((ch = getchar()) != '\n')
					{
						i = ch - '0';
						getchar();
						j = getchar() - '0';
						i-=1; j-=1;
						getchar();
					}
					else
					{
						i = 0; j = num_of_input_file-1;
					}
				}
				cout << endl << "    >>>>>>>>>> Output intersection lines between  " << input_file_list[i] << "  and  " << input_file_list[j] << endl;			
			
				//name the ouput file name by users' input or according to the chosen input file name
				char* fo = new char[100];
				cout << endl << "    ";
				fo = output_file_format(fo,input_file_list, i);
				cout << "    >>>>>>>>>> " << "Output file: " << fo << endl;
				getchar();
				//output intersection lines to the ouput file
				find_intersection_line_of_two_model(facet_list[i],facet_list[j],fo);				
			}
		}	
		cout << endl << "Use more function? (y/n) ";
		ans = getchar();
		//clean up the residual '\n' character in buffer
		getchar();
	}
	getchar();

	return 0;
}

void read_stl_and_establish_vertex_edge_facet_relationship(char* fn)
{
	//read stl binary file which is with the name same as fn
	FILE *fpin;
	fpin = fopen(fn, "rb");

	unsigned long int maxf;//total number of facets
	unsigned short int count;//attribute byte count
	float normal_vector[3];//normal vector of the facet
	float vertex_coordinate[9];//coordinates of the 3 vertexes of facet
	char str[81];//header

	fseek(fpin, 0L, SEEK_SET);
	fread(str, 1, 80, fpin);
	fread(&maxf, 4, 1, fpin);

	//set tableSize equal to total number of facets to maximize the searching efficiency
	vertex_list[num_of_input_file] = vertex_hash(maxf/2);
	edge_list[num_of_input_file] = edge_hash(maxf);
	facet_list[num_of_input_file] = facet_hash(maxf);

	cout << "======================================================================" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "Establish vertex-edge-facet relationship:" << endl << endl;
	for (unsigned long int nf = 0; nf < maxf; nf++)
	{
		//calculate completeness of the process in order to show to users
		if ((nf + 1) % (unsigned long int)(maxf / 10.0) == 0)
		{
			if((nf + 1) / (unsigned long int)(maxf / 10.0) <= 10)
			{
				cout << "	";
				cout << (nf + 1) / (unsigned long int)(maxf / 10.0) *10.0 << " %" << endl;
			}				
		}
		
		fread(normal_vector, 4, 3, fpin);
		fread(vertex_coordinate, 4, 9, fpin);
		fread(&count, 2, 1, fpin);

		//generate 3 vertexes, 3 edges, 1 facet and their pointers
		vertex* vertex_temp[3];
		edge* edge_temp[3];
		for (int i = 0; i < 3; i++)
		{
			vertex_temp[i] = new vertex;
			edge_temp[i] = new edge;
		}
		facet* facet_temp = new facet;

		//redirect the vertex pointer if vertex is already in vertex_list, make it point to vertex in vertex_list, 
		//else remain pointing to new vertex  
		for (int i = 0; i < 3; i++)
		{
			//(x,y,z) of vertex is initialized by assigning right object to the left
			*vertex_temp[i] = vertex(vertex_coordinate[3 * i + 0], vertex_coordinate[3 * i + 1], vertex_coordinate[3 * i + 2]);
			
			//if vertex is already in vertex_list, modify vertex pointer from new vertex to vertex in vertex_list 
			if (vertex_list[num_of_input_file].FindItem(vertex_temp[i]) != NULL)
			{
				vertex_temp[i] = vertex_list[num_of_input_file].FindItem(vertex_temp[i]);
			}
			//if vertex is not in vertex_list, push vertex pointer that point to new vertex to vertex_list 
			else
			{
				vertex_list[num_of_input_file].AddItem(vertex_temp[i]);
			}
		}

		//redirect the edge pointer if edge is already in edge_list, make it point to edge in edge_list, 
		//else remain pointing to new edge
		for (int i = 0; i < 3; i++)
		{
			//vertex_of_edge array is initialized by assigning right object to the left
			*edge_temp[i] = edge(vertex_temp[i], vertex_temp[(i + 1) % 3]);
			
			//if edge is already in edge_list, modify edge pointer from new edge to edge in edge_list 
			if (edge_list[num_of_input_file].FindItem(edge_temp[i]) != NULL)
			{
				edge_temp[i] = edge_list[num_of_input_file].FindItem(edge_temp[i]);
			}
			//if edge is not in edge_list, push edge pointer that point to new edge to edge_list
			else
			{
				edge_list[num_of_input_file].AddItem(edge_temp[i]);
			}
		}

		//redirect the facet pointer if facet is already in facet_list, make it point to facet in facet_list,
		//else remain pointing to new facet

		//vertex_of_facet array is initialized by assigning right object to the left
		*facet_temp = facet(vertex_temp[0], vertex_temp[1], vertex_temp[2]);
		//store normal vector (nx,ny,nz) for facet
		facet_temp->nx = normal_vector[0];
		facet_temp->ny = normal_vector[1];
		facet_temp->nz = normal_vector[2];

		//if facet is already in facet_list, modify facet pointer from new facet to facet in facet_list 
		if (facet_list[num_of_input_file].FindItem(facet_temp) != NULL)
		{
			facet_temp = facet_list[num_of_input_file].FindItem(facet_temp);
		}
		//if facet is not in facet_list, push facet pointer that point to new facet to facet_list
		else
		{
			facet_list[num_of_input_file].AddItem(facet_temp);
		}

		//establish the vertex-edge-facet relationship
		for (int i = 0; i < 3; i++)
		{
			facet_temp->edge_of_facet[i] = edge_temp[i]; //facet point to 3 edges
			facet_temp->vertex_of_facet[i] = vertex_temp[i];//facet point to 3 vertexes
		}
		for (int i = 0; i < 3; i++)
		{
			vertex_temp[i]->facet_of_vertex.push_back(facet_temp);//3 vertexes point to facet
			edge_temp[i]->facet_of_edge.push_back(facet_temp); //3 edges point to facet

			vertex_temp[i]->edge_of_vertex.push_back(edge_temp[i]);
			vertex_temp[i]->edge_of_vertex.push_back(edge_temp[(i + 2) % 3]);//3 vertexes point to 2 edge

			edge_temp[i]->vertex_of_edge[0] = vertex_temp[i];
			edge_temp[i]->vertex_of_edge[1] = vertex_temp[(i + 1) % 3];//3 edges point to 2 vertexes 
		}
	}
	cout << "	complete!!" << endl;
	cout << endl;
	cout << facet_list[num_of_input_file].count << " facets, ";
	cout << edge_list[num_of_input_file].count << " edgs, ";
	cout << vertex_list[num_of_input_file].count << " vertexes ";
	cout << "is established!!" << endl;
}

char* output_file_format(char* fo, char** input_file_list, int input_file_num)
{
	char ch = ' '; //store for the read-in character
	int i = 0;//i is set up to count the number of character of file name
	cout << "Output file name (.scr) ";
	cout << "(default: ";

	//copy the characters from the chosen input file name to the output file name for default
	//instead of copying the file format, ".stl" is added at the end of the file name
	while(ch != '.')
	{
		ch = input_file_list[input_file_num][i];
		fo[i] = ch;
		i++;
	}
	fo[i] = 's'; fo[i+1] = 'c'; fo[i+2] = 'r'; fo[i+3] = '\0';	
	cout << fo << "): ";

	//as neither enter is detected nor number of characters input exceed 100, keep storing characters to fo
	bool EnterLoop_flag = 0, FileFormat_flag = 0;
	for (i = 0; i < 100 && (ch = getchar()) != '\n'; ++i)
	{
		//ever if the read characters loop is entered should EnterLoop_flag set to 1
		EnterLoop_flag = 1;
		//as point is detected, that is, file format is mentioned by the users, FileFormat_flag is set to 1
		if (ch == '.') FileFormat_flag = 1;
		fo[i] = ch;
	}
		
	//if EnterLoop_flag = 1, output file is copied from the file name that users had input
	if (EnterLoop_flag == 1)
	{
		//if FileFormat_flag = 0, that is, lack of file format, file format ".scr" will be filled up automatically
		if (FileFormat_flag == 0)
		{
			fo[i + 0] = '.';
			fo[i + 1] = 's';
			fo[i + 2] = 'c';
			fo[i + 3] = 'r';
			fo[i + 4] = '\0';
		}
		else
			fo[i + 0] = '\0';
		cout << endl;
	}
	//if EnterLoop_flag = 0, file name copied from the input file name is used as default
	else
	{
		cout << endl;
	}
	return fo;
}

void find_edges(char* fo, int input_file_num)
{
	double ll = -1.0, ul = -1.0;//ll and ul is set up to store the lower and upper limit of the dihedral angle, respectively
	bool WrongFormat_flag = 1;
	//if WrongFormat_flag = 1, implies that either this is the first loop or input is with wrong format
	while (WrongFormat_flag)
	{
		WrongFormat_flag = 0;
		cout << "======================================================================" << endl;
		cout << "----------------------------------------------------------------------" << endl;
		cout << "Find the target edge." << endl;
		cout << "Enter the angle interval between two related facets (0~180) " << " (default: 60 120): ";
		char num;
		bool EnterLoop_flag = 0, DecimalPoint_flag = 0;
		double i = 0.0;
		int count = 0;
		//as enter is not detected, keep reading characters
		while ((num = getchar()) != '\n')
		{
			//ever if the read characters loop is entered should EnterLoop_flag set to 1
			EnterLoop_flag = 1;
			//if the read-in character is not '0'~'9'
			if (num > '9' || num < '0')
			{
				if (num == '.')
				{
					//as decimal point is detected, DecimalPoint_flag is set to 1
					DecimalPoint_flag = 1;
				}
				else if (num == ' ')
				{
					//as space is detected, DecimalPoint_flag is set to 0, that is, 
					//old data has finished reading, and new data reading starts
					DecimalPoint_flag = 0;
					//store old data to ll
					ll = i;
					//zero i to store new data
					i = 0.0;
					//zero count to caclulate number of the digit after decimal point for new data
					count = 0;
				}
				else
					//if the read-in character is not '0'~'9', '.', or ' ',
					//set the WrongFormat_flag to 1
					WrongFormat_flag = 1;
			}
			//if the read-in character is '0'~'9'
			else
			{
				//as decimal point is not detected yet, store number as interger
				if (DecimalPoint_flag == 0)
				{
					i *= 10;
					i += num - '0';
				}
				//as decimal point is detected, store number as decimal
				else
				{
					count++;
					i += (num - '0') * pow(0.1, count);
				}
			}
		}
		//after enter is detected, store new data to ul
		ul = i;
		cout << endl;

		//if ul < ll, swap
		double tmp;
		if (ul < ll)
		{
			tmp = ll; ll = ul; ul = tmp;
		}
		//if EnterLoop_flag = 0, that is, enter is typed at the very beginning,
		//ll = 60.0 and ul = 120.0 for default
		if (EnterLoop_flag == 0)
		{
			ll = 60.0;
			ul = 120.0;
			cout << endl;
		}
		//if {ll,ul} is out of range {0,180}, WrongFormat_flag will be set to 1
		if(ul > 180)
		{
			WrongFormat_flag = 1;
		}
		//if WrongFormat_flag = 1, output warning to users
		if (WrongFormat_flag == 1)
		{
			cout << endl;
			cout << "*** Wrong format of input!" << endl;
			cout << "Format must be: /lower limit of the angle (0~180)/ /space/ /upper limit of the angle (0~180)/ " << endl;
			cout << endl;
		}
	}
	cout << ">>> " << "Output edges that angle between 2 related facets range from " << ll << " to " << ul << " degrees:" << endl;
	
	sort_edge_list[input_file_num] = sort_edge_for_edge_hash();
	sort_edge_list[input_file_num].sort_edge(edge_list[input_file_num]);
	//there's a difference of 180 between dihedral angle and included angle of the normal vector of 2 facets
	sort_edge_list[input_file_num].draw_edge(fo, 180.0 - ul, 180.0 - ll);
}

void find_free_edges(char* fo,int input_file_num)
{
	cout << "======================================================================" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "Find free edges:" << endl << endl;
	edge_list[input_file_num].draw_free_edge(fo);
}

void find_intersection_line_of_two_model(facet_hash& facet_list_1, facet_hash& facet_list_2,char* fo)
{
	cout << "======================================================================" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "Find intersection lines:" << endl << endl;
	facet_list_1.draw_intersection_line(facet_list_2, fo);
}
