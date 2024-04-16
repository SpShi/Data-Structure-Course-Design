#define _HAS_STD_BYTE 0
#include"graph.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<ctime>
#include<Windows.h>
#include"matplotlibcpp.h"
#include<chrono>
//using namespace std;

namespace plttime = matplotlibcpp;
namespace pltspace = matplotlibcpp;
int main() {
	ifstream infile;
	ifstream cmpfile;
	vector < double > t1, t2;
	vector<int> r1, r2;
	for (int j = 1; j <= 100; j++) {
		graph g1, g2;
		infile.open("D://data_structure_data4/input" + to_string(j) + ".in", ios::in);
		cmpfile.open("D://data_structure_data4/" + to_string(j) + ".out", ios::in);
		int n, m, pmax, standard_ans, ans1, ans2;
		infile >> n >> m >> pmax;
		cmpfile >> standard_ans;
		g1.initial(n, m, pmax);
		g2.initial(n, m, pmax);
		for (int i = 0; i < m; i++) {
			int x, y, c;
			infile >> x >> y >> c;
			g1.addedge(x, y, c);
			g2.addedge(x, y, c);
		}
		g1.topsort();
		g2.topsort();
		auto bt_start_time = std::chrono::high_resolution_clock::now(); 
		g1.solution1(1, 0);
		auto bt_end_time = std::chrono::high_resolution_clock::now();
		auto bb_start_time = std::chrono::high_resolution_clock::now();
		g2.solution2();
		auto bb_end_time = std::chrono::high_resolution_clock::now();
		auto t_bt = std::chrono::duration_cast<std::chrono::nanoseconds>(bt_end_time - bt_start_time);
		auto t_bb = std::chrono::duration_cast<std::chrono::nanoseconds>(bb_end_time - bb_start_time);
		ans1 = g1.getans();
		ans2 = g2.getans();
		t1.push_back(t_bt.count());	
		t2.push_back(t_bb.count());
		r1.push_back(g1.getroom());
		r2.push_back(g2.getroom());
		cout<<"��"<<j << "����׼��Ϊ: " << standard_ans << " ����1:" << ans1 << " ����1ʱ��:" << t_bt.count() << " ����2:" << ans2 << " ����2ʱ��:" << t_bb.count() << endl;
		if (standard_ans == ans1 && standard_ans == ans2) cout << "�ɹ�";
		else cout << "ʧ��ʧ��ʧ��ʧ��ʧ��ʧ��ʧ��ʧ��ʧ��ʧ��";
		cout << endl;
		cmpfile.close();
		infile.close();
	}
	int choice;
	cout << "�Ƿ�Ƚ�ʱ��ռ�����?  1-time;2-room;3-quit" << endl;
	while (cin >> choice) {
		if (choice == 1) {
			plttime::title("time");
			plttime::plot(t1, "b");
			plttime::plot(t2, "r");
			plttime::show();
		}
		else if (choice == 2) {
			pltspace::title("room");
			pltspace::plot(r1, "b");
			pltspace::plot(r2, "r");
			pltspace::show();
		}
		else break;
	}
}