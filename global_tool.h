#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>

#ifndef GLOBAL_TOOL_H_H
#define GLOBAL_TOOL_H_H

using namespace std;

double* CramersFormula(double*& X, double**& Ab, int nr);
double Det(double**& A, int nr);
class VEC
{
public:
	double x = 0.0, y = 0.0, z = 0.0;
	VEC() {}
	VEC(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	double dot(VEC V)
	{
		return (x*V.x + y*V.y + z*V.z);
	}
	VEC cross(VEC V)
	{
		return VEC(y*V.z - z*V.y, z*V.x - x*V.z, x*V.y - y*V.x);
	}
	double length()
	{
		return (sqrt(x*x + y*y + z*z));
	}
	bool operator==(VEC referent)
	{
		if (x == referent.x && y == referent.y && z == referent.z)
			return 1;
		else
			return 0;
	}
};

#endif