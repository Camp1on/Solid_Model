#include "Sweep.h"
#include <vector>

//平移扫成
Solid* Sweep::translation(Solid*& solid, double d, double* direc) {
	Face* bottonFace = solid->sface->nextf;
	Face* upFace = solid->sface;
	Loop* loop = upFace->out_loop;
	//第一个in loop
	Loop* out_loop = bottonFace->out_loop;
	HalfEdge* he = out_loop->he;

	Vertex* firstv = he->ev;
	Vertex* firstup = new Vertex(firstv->cdt.x + d * direc[0], firstv->cdt.y + d * direc[1], firstv->cdt.z + d * direc[2]);
	Vertex* nextv = NULL, * nextup = NULL, * startup = NULL;

	Euler::mev(loop, firstv, firstup);
	nextup = firstup;
	startup = firstup;
	he = he->nexth;
	nextv = he->ev;

	while (nextv != firstv) {
		nextup = new Vertex(nextv->cdt.x + d * direc[0], nextv->cdt.y + d * direc[1], nextv->cdt.z + d * direc[2]);
		Euler::mev(loop, nextv, nextup);
		Euler::mef(loop, firstup, nextup);
		//做下一条边
		firstup = nextup;
		he = he->nexth;
		nextv = he->ev;
	}
	nextup = startup;
	Euler::mef(loop, firstup, nextup);

	loop = upFace->in_loop;
	out_loop = bottonFace->in_loop;
	while (loop && out_loop) {
		he = out_loop->he;

		firstv = he->ev;
		firstup = new Vertex(firstv->cdt.x + d * direc[0], firstv->cdt.y + d * direc[1], firstv->cdt.z + d * direc[2]);
		nextv = NULL, nextup = NULL, startup = NULL;

		Euler::mev(loop, firstv, firstup);
		nextup = firstup;
		startup = firstup;
		he = he->nexth;
		nextv = he->ev;

		while (nextv != firstv) {
			nextup = new Vertex(nextv->cdt.x + d * direc[0], nextv->cdt.y + d * direc[1], nextv->cdt.z + d * direc[2]);
			Euler::mev(loop, nextv, nextup);
			Euler::mef(loop, firstup, nextup);
			//做下一条边
			firstup = nextup;
			he = he->nexth;
			nextv = he->ev;
		}
		nextup = startup;
		Euler::mef(loop, firstup, nextup);
		loop = loop->nextl;
		out_loop = out_loop->nextl;
	}

	return solid;
}
