#pragma once
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

//用于分支定界的节点
struct bb_Node {//32byte
	bb_Node* parent;//父节点,即从哪一种情况产生的
	int pressure;//油压
	int level;//层级
	bool set;//是否放置
	int num;//一共放了几个放大器
	int mcost;//最大出边
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

struct outedge {//出边
	int to;
	int cost;
	outedge(int tto, int ccost) {
		to = tto;
		cost = ccost;
	}
	~outedge(){}
};

struct inedge {//入边
	int from;
	int cost;
	inedge(int ffrom, int ccost) {
		from = ffrom;
		cost = ccost;
	}
	~inedge() {}
};
struct Node {//图的节点
	vector<outedge> outs;//所有出边
	vector<inedge> ins;//所有入边
	bool set = 0;//是否放置放大器
	int pressure=0;//节点压力
	int mcost=0;//最大出边
	int top=0;//拓扑序列中的序号
	Node() {
		outs.clear();
		ins.clear();
	}
	~Node() {}
};
class graph {
public:
	graph() {};
	void initial(int n, int m, int Pmax);//初始化
	void topsort();//拓扑排序
	void addedge(int x,int y,int c);//加一条边
	void solution1(int level,int cnt);//方法一,回溯
	void solution2();//方法二,分支定界
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
	vector<Node> nodes;//所有节点
	vector<int> indegree;//储存入度,用于拓扑排序
	vector<int> topnums;//拓扑排序的结果
	priority_queue<bb_Node*, vector<bb_Node*>, cmp> bb_q;//用于分支定界
	int n = 0;//节点数
	int m = 0;//边数
	int ans=INT_MAX;
	int space = 0;
};