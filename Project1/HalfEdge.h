#pragma once

#include <iostream>

//给相关结构体赋一个唯一的ID
static int vertexIdx = 0;
static int heIdx = 0;

//半边数据结构
struct Solid;
struct Face;
struct Loop;
struct HalfEdge;
struct Vertex;
struct Edge;

struct Point {
	double x, y, z;
	Point() {};
	Point(double xx, double yy, double zz) :x(xx), y(yy), z(zz) {};
	friend Point operator-(Point& a, Point& b) {
		return Point(a.x - b.x, a.y - b.y, a.z - b.z);
	}
};
//Solid

struct Solid {
	// int id;
	Solid* pres, * nexts;
	Face* sface; //构造该实体的首面
	Edge* edge; //构造该实体的首边
	Vertex* v;

	Solid() : pres(NULL), nexts(NULL), sface(NULL), edge(NULL), v(NULL) {};
};
//Face
struct Face {
	// int id;
	Face* pref, * nextf;
	Solid* solid;//该面属于哪个实体
	Loop* in_loop, * out_loop; //内环和外环
	// int innum; //内环的数目
	Face() : pref(NULL), nextf(NULL), solid(NULL), in_loop(NULL), out_loop(NULL) {};
};

//Loop
struct Loop {
	// int id;
	Loop* prel, * nextl;
	Face* face; //Loop构造的面
	HalfEdge* he;//构造该Loop的所有半边

	Loop() : prel(NULL), nextl(NULL), face(NULL), he(NULL) {};
};

//HalfEdge
struct HalfEdge {
	HalfEdge* preh, * nexth;
	Loop* loop;//半边所属的Loop
	Vertex* sv, * ev;//半边的起始点和终点
	Edge* edge;//半边对应的实体边
	// HalfEdge* brother; //指向另一条方向相反的半边
	int id;

	HalfEdge(Vertex* sv_, Vertex* ev_) : preh(NULL), nexth(NULL), loop(NULL), edge(NULL) {
		sv = sv_;
		ev = ev_;
		id = heIdx++;
	}

	friend bool operator==(HalfEdge& a, HalfEdge& b) {
		if (a.id == b.id)	return true;
		else   return false;
	}
};

//Edge
struct Edge {
	Edge* pree, * nexte;
	HalfEdge* he1, * he2;//实体边对应的两条半边
	Vertex* v1, * v2;

	Edge(Vertex* v1_, Vertex* v2_, HalfEdge* he1_, HalfEdge* he2_) : pree(NULL), nexte(NULL) {
		v1 = v1_, v2 = v2_;
		he1 = he1_, he2 = he2_;
	}
};

//Vertex
struct Vertex {
	Vertex* prev, * nextv;
	int id;

	Point cdt;//点的三维坐标

	Vertex(double x, double y, double z) : prev(NULL), nextv(NULL) {
		cdt.x = x, cdt.y = y, cdt.z = z;
		id = vertexIdx++;
	}

	friend bool operator==(Vertex& a, Vertex& b) {
		if (a.id == b.id)	return true;
		else   return false;
	}
};