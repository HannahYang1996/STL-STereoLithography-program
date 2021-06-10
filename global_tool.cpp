#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "global_tool.h"

double* CramersFormula(double*& X, double**& Ab, int nr)
{
	//set up the delta matrix
	double** delta = new double*[nr];
	for(int i = 0; i < nr; i++)
	{
		delta[i] = new double[nr];
		for(int j = 0; j < nr; j++)
		{
			delta[i][j] = Ab[i][j];
		}
	}
	
	//calculate the deteminant of delta matrix
	//if ~= 0, implies that linear equations have no solution or have infinite solutions
	//else, linear equations have only one solution
	double Det_delta = Det(delta,nr);
	if(fabs(Det_delta) < 1E-5)
	{
		return NULL;
	}
	else
	{
		//set up the delta_n matrix for calculating different X[i]
		double** delta_n = new double*[nr];
		for(int i = 0; i < nr; i++)
		{
			delta_n[i] = new double[nr];
		}

		for(int i = 0; i < nr; i++)
		{
			for(int j = 0; j < nr; j++)
			{
				for(int k = 0; k < nr; k++)
				{
					if(k==i)
					{
						delta_n[j][k] = Ab[j][nr];
					}
					else
					{
						delta_n[j][k] = Ab[j][k];
					}			
				}
			}
			X[i] = Det(delta_n, nr)/Det_delta;
		}
		return X;
	}
}

double Det(double**& A, int nr)
{
	double det = 0;
	//for every elements in the first row
	for(int c = 0; c < nr; c++)
	{
	    double ac = A[0][c];	    
	    if(nr > 2)
	    {
	        //set up the submatrix
	        double** Aij = new double*[nr-1];
	        for(int i = 0; i < nr-1; i++)
	        {
	            Aij[i] = new double[nr-1];
	            int j = 0, k = 0;
	            while(j < nr-1)
	            {
	                if(k != c)
	                {
	                    Aij[i][j] = A[0+i+1][k];
	                    j++;
	                }
	                k++;
	            }
	        }
			//update the determinant by multiplying the element with its cofactor
			det+= ac*pow(-1,c)*Det(Aij,nr-1);
	    }
	    else if(nr == 2)
	    {
	        //update the determinant by multiplying the element with its cofactor
			det+= ac*pow(-1,c)*A[1][(c+1)%2];
		}
	}
	return det;
}