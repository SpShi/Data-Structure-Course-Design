#pragma once
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

//���ڷ�֧����Ľڵ�
struct bb_Node {//32byte
	bb_Node* parent;//���ڵ�,������һ�����������
	int pressure;//��ѹ
	int level;//�㼶
	bool set;//�Ƿ����
	int num;//һ�����˼����Ŵ���
	int mcost;//������
	bb_Node(int the_p, int level) {
		parent = NULL;
		pressure = the_p;
		this->level = level;
		set = 0;
	}
	~bb_Node() {}
};

struct cmp{
	bool operator () (const bb_Node* p1, const bb_Node* p2) const {
		return p1->num > p2->num;
	}
};

struct outedge {//����
	int to;
	int cost;
	outedge(int tto, int ccost) {
		to = tto;
		cost = ccost;
	}
	~outedge(){}
};

struct inedge {//���
	int from;
	int cost;
	inedge(int ffrom, int ccost) {
		from = ffrom;
		cost = ccost;
	}
	~inedge() {}
};
struct Node {//ͼ�Ľڵ�
	vector<outedge> outs;//���г���
	vector<inedge> ins;//�������
	bool set = 0;//�Ƿ���÷Ŵ���
	int pressure=0;//�ڵ�ѹ��
	int mcost=0;//������
	int top=0;//���������е����
	Node() {
		outs.clear();
		ins.clear();
	}
	~Node() {}
};
class graph {
public:
	graph() {};
	void initial(int n, int m, int Pmax);//��ʼ��
	void topsort();//��������
	void addedge(int x,int y,int c);//��һ����
	void solution1(int level,int cnt);//����һ,����
	void solution2();//������,��֧����
	vector<int> test() { return topnums; }
	int getans() { return ans; }
	~graph() {
		while (!bb_q.empty()) {
			bb_Node* pNode = bb_q.top();
			delete pNode;
			bb_q.pop();
		}
	};
	int getroom() { return space; }
protected:
	int pmin=0, pmax=0;
	vector<Node> nodes;//���нڵ�
	vector<int> indegree;//�������,������������
	vector<int> topnums;//��������Ľ��
	priority_queue<bb_Node*, vector<bb_Node*>, cmp> bb_q;//���ڷ�֧����
	int n = 0;//�ڵ���
	int m = 0;//����
	int ans=INT_MAX;
	int space = 0;
};