#pragma once


#include "HalfEdge.h"
#include "Euler.h"

struct Sweep {
	//Æ½ÒÆÉ¨³É
	static Solid* translation(Solid*& solid, double d, double* direction);
};
