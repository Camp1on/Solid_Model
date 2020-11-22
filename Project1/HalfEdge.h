#pragma once

#include <iostream>

//����ؽṹ�帳һ��Ψһ��ID
static int vertexIdx = 0;
static int heIdx = 0;

//������ݽṹ
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
	Face* sface; //�����ʵ�������
	Edge* edge; //�����ʵ����ױ�
	Vertex* v;

	Solid() : pres(NULL), nexts(NULL), sface(NULL), edge(NULL), v(NULL) {};
};
//Face
struct Face {
	// int id;
	Face* pref, * nextf;
	Solid* solid;//���������ĸ�ʵ��
	Loop* in_loop, * out_loop; //�ڻ����⻷
	// int innum; //�ڻ�����Ŀ
	Face() : pref(NULL), nextf(NULL), solid(NULL), in_loop(NULL), out_loop(NULL) {};
};

//Loop
struct Loop {
	// int id;
	Loop* prel, * nextl;
	Face* face; //Loop�������
	HalfEdge* he;//�����Loop�����а��

	Loop() : prel(NULL), nextl(NULL), face(NULL), he(NULL) {};
};

//HalfEdge
struct HalfEdge {
	HalfEdge* preh, * nexth;
	Loop* loop;//���������Loop
	Vertex* sv, * ev;//��ߵ���ʼ����յ�
	Edge* edge;//��߶�Ӧ��ʵ���
	// HalfEdge* brother; //ָ����һ�������෴�İ��
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
	HalfEdge* he1, * he2;//ʵ��߶�Ӧ���������
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

	Point cdt;//�����ά����

	Vertex(double x, double y, double z) : prev(NULL), nextv(NULL) {
		cdt.x = x, cdt.y = y, cdt.z = z;
		id = vertexIdx++;
	}

	friend bool operator==(Vertex& a, Vertex& b) {
		if (a.id == b.id)	return true;
		else   return false;
	}
};