#include"graph.h"
#include<fstream>
#include<queue>
using namespace std;

void graph::initial(int n,int m,int Pmax) {//���ó�ʼ��ѹ,�ڵ�ͱߵ�����
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
	nodes[x].outs.push_back(oe);//�ڵ�x��һ����x��y�ĳ���,Ȩ��Ϊc
	indegree[y]++;//y���+1
	inedge ie(x, c);
	nodes[y].ins.push_back(ie);//�ڵ�y��һ����x��y�����,Ȩ��Ϊc
	nodes[x].mcost = max(nodes[x].mcost, c);//��¼x��������
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
	int pNode = topnums[level];//�����������н���
	space += sizeof(pNode);
	if (level >= n) {//ȡ�������ĵ�,����
		ans = min(cnt,ans);//ansȡ���з�����������С��
		return;
	}
	if (level == 1) {//Դ�㲻���÷Ŵ���
		solution1(level + 1, 0); space += 4;
	}
	else {
		bool nd_to_set = 0;//�ж��Ƿ��Ǳ������
		space += 5;
		int prep = INT_MAX;
		for (int k = 1; k < level; k++) {//�������������ڵ�ǰ�ڵ�ǰ��Ľڵ�
			for (auto it = nodes[topnums[k]].outs.begin(); it != nodes[topnums[k]].outs.end(); it++) {
				if ((*it).to == pNode) {//����ýڵ��г���ָ��ǰ�ڵ�
					//��ѹȡ������ǰ�ڵ����ѹ����Сֵ,��prep����������ڵ��ȥ����Ȩ�ص���Сֵ
					prep = min(prep, nodes[topnums[k]].pressure - (*it).cost);
				}
			}
		}
		int npre = prep - nodes[pNode].mcost;//��ǰ�ڵ���һ���ڵ����С��ѹ
		space += 4;
		if (npre < 0)//�����С��ѹС��0,��˵����һ���ڵ����ѹһ��С��pmin,��ǰ�ڵ�һ��Ҫ���÷Ŵ���
			nd_to_set = 1;
		if (nd_to_set == 0) {//�������Ҫ�Ŵ���,����������ѡ��,�Ż��߲���
			//���ŷŴ���,��ǰ�ڵ���ѹΪprep,cnt�ޱ仯
			nodes[pNode].pressure = prep;
			nodes[pNode].set = 0;
			solution1(level + 1, cnt);
			//���÷Ŵ���,��ǰ�ڵ���ѹΪpmax,cnt+1
			nodes[pNode].pressure = pmax;
			nodes[pNode].set = 1;
			//���м�֦�ж�,���cnt�����Ѿ�ȡ����ans,˵����ǰ�ķ��÷����������ŵ�,���ܼ������½���
			if (cnt + 1 > ans) return;
			solution1(level + 1, cnt+1);
			space += 10;
		}
		else {//�����
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
	bb_Node* pNode = new bb_Node(pmax, 2);//Դ�㲻���÷Ŵ���
	space += sizeof(pNode);
	pNode->parent = NULL;
	pNode->num = 0;
	pNode->set = 0;
	int level = 2;//�ӵڶ����㿪ʼ����
	space += sizeof(level);
	while (level <= n-1) {
		int cur = topnums[level];
		int prep = INT_MAX;
		bool nd_to_set = 0;
		space += 9;
		for (int k = 1; k < level; k++) {
			//����֮ǰ�Ľڵ�����г���
			//�����ָ��ýڵ��,���ŷ�֧�ڵ�ĸ��ڵ��ҵ��ڵ�ǰ�����ǰһ���ڵ����ѹ
			//����ѹ����Ϊ�����������Сֵ
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
			//�����÷Ŵ���
			bb_Node* notput=new bb_Node(prep,level+1);
			notput->parent = pNode;
			notput->num = pNode->num;
			bb_q.push(notput);
			//���÷Ŵ���
			bb_Node* put = new bb_Node(pmax, level + 1);
			put->parent = pNode;
			put->set = 1;
			put->num = pNode->num+1;
			bb_q.push(put);
			space += 64;
		}
		else {//������÷Ŵ���
			bb_Node* put = new bb_Node(pmax, level + 1);
			put->parent = pNode;
			put->num = pNode->num + 1;
			put->set = 1;
			bb_q.push(put);
			space += 32;
		}
		pNode = bb_q.top();//��ָ��ǰ��֧�ڵ��ָ�����Ϊ�Ŵ����������ٵ����
		bb_q.pop();
		level = pNode->level;//��һ���ڵ�
	}
	ans = pNode->num;	
}