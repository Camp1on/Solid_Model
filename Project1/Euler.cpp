#include "Euler.h"

vector<Vertex*> vlist;

//mvsf
Solid* Euler::mvsf(Vertex* v) {
	Solid* solid = new Solid();
	Face* face = new Face();
	Loop* out_loop = new Loop();
	//实体与面建立映射关系
	solid->sface = face;
	face->solid = solid;
	//环与面建立映射关系
	face->out_loop = out_loop;
	out_loop->face = face;
	//存储顶点
	vlist.push_back(v);

	return solid;
}

//mev
Loop* Euler::mev(Loop*& loop, Vertex* v1, Vertex* v2) {
	vlist.push_back(v2);

	Solid* solid = loop->face->solid;
	HalfEdge* he1 = new HalfEdge(v1, v2);
	HalfEdge* he2 = new HalfEdge(v2, v1);
	Edge* edge = new Edge(v1, v2, he1, he2);
	//半边和边的指针域的初试化 
	edge->he1 = he1;
	edge->he2 = he2;

	he1->edge = edge;
	he2->edge = edge;

	he1->loop = loop;
	he2->loop = loop;

	//将两条半边加入loop中
	if (loop->he == NULL) {//环中无边 
		he1->nexth = he2;
		he1->preh = he2;

		he2->nexth = he1;
		he2->preh = he1;

		loop->he = he1;
	}
	else {//环中有边 
		HalfEdge* he=NULL; //临时变量
		for (he = loop->he; he->ev != v1; he = he->nexth); //找到终止点为v1的半边
		// while (he->ev != v1)  he = he->nexth;
		// 改变半边的前趋后继的关系
		he1->nexth = he2;
		he2->preh = he1;

		he2->nexth = he->nexth;
		he->nexth->preh = he2;

		he1->preh = he;
		he->nexth = he1;
	}

	//维护边表，将边加入边表中
	Euler::addEdge(solid, edge);

	return loop;
}

//mef
Face* Euler::mef(Loop*& loop, Vertex* v1, Vertex* v2) {
	//新建边和面
	Solid* solid = loop->face->solid;
	Face* face = new Face();
	Loop* newloop = new Loop();
	HalfEdge* he1 = new HalfEdge(v1, v2);
	HalfEdge* he2 = new HalfEdge(v2, v1);
	Edge* edge = new Edge(v1, v2, he1, he2);

	//连接边和半边
	edge->he1 = he1;
	edge->he2 = he2;

	he1->edge = edge;
	he2->edge = edge;

	HalfEdge* hev1, * hev2, * hev3, * he;
	//找v1的前一条边
	for (he = loop->he; he->ev != v1; he = he->nexth);
	// while (he->ev != v1) he = he->nexth;
	hev1 = he;
	//找v2的前一条边
	for (; he->ev != v2; he = he->nexth);
	// while (he->ev != v2) he = he->nexth;
	hev2 = he;

	he = hev2->nexth;
	while (he->ev != v2) he = he->nexth; //用于判断是否有无内环
	hev2 = he;

	he1->nexth = hev2->nexth;
	he2->nexth = hev1->nexth;

	he1->preh = hev1;
	he2->preh = hev2;

	hev2->nexth = he2;
	hev1->nexth = he1;

	he1->nexth->preh = he1;
	he2->nexth->preh = he2;

	//loop
	he1->loop = newloop;
	he2->loop = loop;

	newloop->he = he1;
	loop->he = he2;

	//add edge to solid
	addEdge(solid, edge);

	//add face to solid
	addFace(solid, face);

	//add loop to face
	addLoop(face, newloop);

	return face;
}

//kemr
Loop* Euler::kemr(Loop*& loop, Vertex* v1, Vertex* v2) {
	Solid* solid = loop->face->solid;
	Face* face = loop->face;
	Loop* in_loop = new Loop();

	HalfEdge* he1 = NULL, * he2 = NULL, * he = NULL;
	//找v1->v2的半边
	for (he = loop->he; (he->sv) != v1 || (he->ev != v2); he = he->nexth);
	// while (he->sv != v1 || he->ev != v2)
		// he = he->nexth;
	he1 = he;
	//通过edge找到另一半边
	//he2 = he->edge->he1 == he ? he->edge->he2 : he->edge->he1;
	for (; (he->sv != v2) || (he->ev != v1); he = he->nexth);
	// while (he->sv != v2 || he->ev != v1)
		// he = he->nexth;
	he2 = he;

	//闭合生成2个loop
	he1->preh->nexth = he2->nexth;
	he2->nexth->preh = he1->preh;

	he1->nexth->preh = he2->preh;
	he2->preh->nexth = he1->nexth;

	//原out loop
	face->out_loop->he = he1->preh;

	//新的in loop
	in_loop->face = face;
	in_loop->he = he1->nexth;

	addLoop(solid->sface, in_loop);

	//删除两个半边
	delete he1;
	delete he2;
	return in_loop;
}

//kfmrh
//f1为外面的面--保留, f2为里面的面--删除
Solid* Euler::kfmrh(Face* f1, Face* f2) {
	Solid* solid = f2->solid;
	addLoop(f1, f2->out_loop);
	Face* face = solid->sface;
	if (face == f2) {
		solid->sface = face->nextf;
		solid->sface->pref = face->pref;
		delete face;
	}
	else {
		Face* f = face->nextf;
		while (f != f2) f = f->nextf;
		if (f == NULL) return solid;
		else {
			f->pref->nextf = f->nextf;
			if (f->nextf != NULL)	f->nextf->pref = f->pref;
			delete f;
		}
	}
	return solid;
}

//向边表中添加一条边
Solid* Euler::addEdge(Solid*& solid, Edge*& edge) {
	if (solid->edge == NULL)	solid->edge = edge;
	else {
		//尾插
		Edge* p = solid->edge;
		while (p->nexte != NULL) p = p->nexte;
		p->nexte = edge;
		edge->pree = p;
	}
	return solid;
}

//向面表中添加一个面
Solid* Euler::addFace(Solid*& solid, Face*& face) {
	face->solid = solid;
	if (solid->sface == NULL)		solid->sface = face;
	else {
		//头插
		Face* p = solid->sface;
		while (p->nextf != NULL) p = p->nextf;
		p->nextf = face;
		face->pref = p;
	}
	return solid;
}

//向loop 表中添加一个loop
Face* Euler::addLoop(Face*& face, Loop*& loop) {
	//out loop 只有一个，且必先有out loop
	loop->face = face;
	if (face->out_loop == NULL)		face->out_loop = loop;
	else {
		if (face->in_loop == NULL)	face->in_loop = loop;
		else {
			//头插
			Loop* p = face->in_loop;
			while (p->nextl != NULL) p = p->nextl;
			p->nextl = loop;
			loop->prel = p;
		}
	}
	return face;
}

//向Solid 表中添加一个solid
Solid* Euler::addSolid(Solid*& solid, Solid*& pSolid) {
	if (solid == NULL) solid = pSolid;
	else {
		//尾插
		Solid* p = solid;
		while (p->nexts != NULL) p = p->nexts;
		p->nexts = pSolid;
		pSolid->pres = p;
		pSolid->nexts = NULL;
	}
	return solid;
}

//向Solid中加入点
Solid* Euler::addVertex(Solid*& solid, Vertex* v) {
	if (solid->v == NULL)	solid->v == v;
	else {
		//头插
		v->nextv = solid->v->nextv;
		solid->v->nextv->prev = v;
		solid->v = v;
	}
	return solid;
}