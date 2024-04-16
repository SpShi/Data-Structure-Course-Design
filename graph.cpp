#include"graph.h"
#include<fstream>
#include<queue>
using namespace std;

void graph::initial(int n,int m,int Pmax) {//设置初始油压,节点和边的数量
	this->n = n;
	this->m = m;
	this->pmax = Pmax;
	pmin = 0;
	nodes.clear();
	indegree.resize(n + 1, 0);
	for (int i = 0; i <= n; i++)
		nodes.push_back(Node());
	nodes[1].pressure = pmax;
	topnums.push_back(0);
}

void graph::addedge(int x, int y, int c) {
	outedge oe(y, c);
	nodes[x].outs.push_back(oe);//节点x加一条从x到y的出边,权重为c
	indegree[y]++;//y入度+1
	inedge ie(x, c);
	nodes[y].ins.push_back(ie);//节点y加一条从x到y的入边,权重为c
	nodes[x].mcost = max(nodes[x].mcost, c);//记录x的最大出边
}

void graph::topsort() {
	int topnum = 1;
	bool* is_pushed = new bool[n+1];
	for (int i = 0; i <= n; i++)
		is_pushed[i] = 0;
	priority_queue<int, vector<int>, greater<int>> q;
	for (int i = 1; i <= n; i++) {
		if (indegree[i] == 0) {
			q.push(i);
			is_pushed[i] = 1;
		}
	}
	while (!q.empty()) {
		int temp = q.top();
		q.pop();
		nodes[temp].top = topnum++;
		topnums.push_back(temp);
		for (auto it = nodes[temp].outs.begin(); it != nodes[temp].outs.end(); it++) {
			indegree[(*it).to]--;
			if (indegree[(*it).to] == 0&&is_pushed[(*it).to]==0) {
				q.push((*it).to);
				is_pushed[(*it).to] = 1;
			}
		}
	}
}

void graph::solution1(int level, int cnt) {
	int pNode = topnums[level];//按照拓扑序列进行
	space += sizeof(pNode);
	if (level >= n) {//取到了最后的点,结束
		ans = min(cnt,ans);//ans取所有放置数量中最小的
		return;
	}
	if (level == 1) {//源点不放置放大器
		solution1(level + 1, 0); space += 4;
	}
	else {
		bool nd_to_set = 0;//判断是否是必须放置
		space += 5;
		int prep = INT_MAX;
		for (int k = 1; k < level; k++) {//遍历拓扑序列在当前节点前面的节点
			for (auto it = nodes[topnums[k]].outs.begin(); it != nodes[topnums[k]].outs.end(); it++) {
				if ((*it).to == pNode) {//如果该节点有出边指向当前节点
					//油压取流到当前节点的油压的最小值,即prep本身和其他节点减去出边权重的最小值
					prep = min(prep, nodes[topnums[k]].pressure - (*it).cost);
				}
			}
		}
		int npre = prep - nodes[pNode].mcost;//当前节点下一个节点的最小油压
		space += 4;
		if (npre < 0)//如果最小油压小于0,则说明下一个节点的油压一定小于pmin,当前节点一定要放置放大器
			nd_to_set = 1;
		if (nd_to_set == 0) {//如果不需要放大器,可以有两种选择,放或者不放
			//不放放大器,当前节点油压为prep,cnt无变化
			nodes[pNode].pressure = prep;
			nodes[pNode].set = 0;
			solution1(level + 1, cnt);
			//放置放大器,当前节点油压为pmax,cnt+1
			nodes[pNode].pressure = pmax;
			nodes[pNode].set = 1;
			//进行剪枝判断,如果cnt大于已经取到的ans,说明当前的放置方法不是最优的,不能继续向下进行
			if (cnt + 1 > ans) return;
			solution1(level + 1, cnt+1);
			space += 10;
		}
		else {//必须放
			cnt++;
			if (cnt > ans) return;
			nodes[pNode].pressure = pmax;
			nodes[pNode].set = 1;
			solution1(level + 1, cnt);
			space += 6;
		}
	}
}

void graph::solution2() {
	bb_Node* pNode = new bb_Node(pmax, 2);//源点不放置放大器
	space += sizeof(pNode);
	pNode->parent = NULL;
	pNode->num = 0;
	pNode->set = 0;
	int level = 2;//从第二个点开始进行
	space += sizeof(level);
	while (level <= n-1) {
		int cur = topnums[level];
		int prep = INT_MAX;
		bool nd_to_set = 0;
		space += 9;
		for (int k = 1; k < level; k++) {
			//遍历之前的节点的所有出边
			//如果有指向该节点的,沿着分支节点的父节点找到在当前情况下前一级节点的油压
			//将油压更新为所有情况的最小值
			for (auto it = nodes[topnums[k]].outs.begin(); it != nodes[topnums[k]].outs.end(); it++) {
				if ((*it).to == cur) {
					bb_Node* tempp = pNode;
					for (int j = 0; j <level-k-1; j++)
						tempp = tempp->parent;
					prep = min(prep, tempp->pressure - (*it).cost);
				}
			}
		}
		if (prep - nodes[cur].mcost < 0)
			nd_to_set = 1;
		if (!nd_to_set) {
			//不放置放大器
			bb_Node* notput=new bb_Node(prep,level+1);
			notput->parent = pNode;
			notput->num = pNode->num;
			bb_q.push(notput);
			//放置放大器
			bb_Node* put = new bb_Node(pmax, level + 1);
			put->parent = pNode;
			put->set = 1;
			put->num = pNode->num+1;
			bb_q.push(put);
			space += 64;
		}
		else {//必须放置放大器
			bb_Node* put = new bb_Node(pmax, level + 1);
			put->parent = pNode;
			put->num = pNode->num + 1;
			put->set = 1;
			bb_q.push(put);
			space += 32;
		}
		pNode = bb_q.top();//将指向当前分支节点的指针更新为放大器数量最少的情况
		bb_q.pop();
		level = pNode->level;//下一级节点
	}
	ans = pNode->num;	
}