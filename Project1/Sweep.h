#pragma once


#include "HalfEdge.h"
#include "Euler.h"

struct Sweep {
	//ƽ��ɨ��
	static Solid* translation(Solid*& solid, double d, double* direction);
};
