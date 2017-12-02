// Graph_work_2.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <ctime>

using namespace std;

template <class T>
class MGraph {
	
	vector<map<unsigned int, double>> v_m;
	vector<T> v;
	map<T, unsigned int> m;

public:
	MGraph() {};
	inline bool labelOccurs(const T& label) const { return m.find(label) != m.end(); }
	inline T numToLabel(const unsigned int& num) const { return v[num]; }
	inline unsigned int labelToNum(const T& label) const { return m.at(label); }
	inline static bool comparePair(pair<unsigned int, double> i, pair<unsigned int, double> j) { return i.second < j.second; }
	void addLabel(const T& label);
	void addEdgeAfterCheck(const T& label1, const T& label2, const double& cost);
	void addEdgeFast(const T& label1, const T& label2, const double& cost);
	bool edgeExists(const T& label1,const  T& label2) const;
	double determineCost(const unsigned int& node1, const unsigned int& node2) const;
	stack<unsigned int> returnShortestPath(const unsigned int& node1, const unsigned int& node2) const;
	~MGraph() {};
};

template <class T> void MGraph<T>::addLabel(const T& label) {
	if (!labelOccurs(label)) {
		m[label] = v.size();
		v.push_back(label);
		v_m.push_back(map<unsigned int,double>());
	}
}

template <class T> void MGraph<T>::addEdgeAfterCheck(const T& label1, const T& label2, const double& cost) {

	char input;
	if (labelOccurs(label1) || labelOccurs(label2)) addEdgeFast(label1, label2, cost);
	else {
		cout << "none of the 2 labels exist. Do you still need to add(Y/N)?";
		cin >> input;
		if (input == 'Y') addEdgeFast(label1, label2, cost);
	}
}

template <class T> void MGraph<T>::addEdgeFast(const T& label1, const T& label2, const double& cost) {

	addLabel(label1);
	addLabel(label2);
	unsigned int x = labelToNum(label1);
	unsigned int y = labelToNum(label2);
	v_m[x][y] = cost;
}

template <class T> double MGraph<T>::determineCost(const unsigned int& node1, const unsigned int& node2) const {

	if (node1 == node2) return 0.0;

	T label1 = numToLabel(node1);
	T label2 = numToLabel(node2);
	if (edgeExists(label1, label2)) return v_m[node1].at(node2);
	else {
		cout << "The edge does not exist";
		return NULL;
	}
}

template <class T> stack<unsigned int> MGraph<T>::returnShortestPath(const unsigned int& node1, const unsigned int& node2) const{

	stack<unsigned int> sP;
	unsigned int vsize = v.size();
	if (node1 > vsize - 1 || node2 > vsize - 1) {
		cout << "One of the node number you entered is greater than the total number of nodes in the graph.";
		cout << "Try again in the range " << 0 << "-" << vsize - 1 << "\n";
		return sP;
	}

	if (node1 == node2) {
		cout << "Same node, 0 distance \n";
		return sP;
	}

	unsigned int target = node2;
	map<unsigned int, double> distance;
	map<unsigned int, double> deletedDist;
	map<unsigned int, unsigned int> previous;

	for (int it = 0; it < vsize; it++) distance[it] = INFINITY;
	distance[node1] = 0;

	while (!distance.empty()) {

		auto minValue = *min_element(distance.begin(), distance.end(), &MGraph<T>::comparePair);
		if (minValue.second == INFINITY) {
			
			cout << "No path exists \n";
			return sP;
		}

		double d = minValue.second;
		deletedDist.insert(minValue);
		distance.erase(minValue.first);
		if (minValue.first == node2) break;

		for (auto iter_map = v_m[minValue.first].begin(); iter_map != v_m[minValue.first].end(); iter_map++) {
			// do not iterate over the node if it has been deleted
			if (deletedDist.find(iter_map->first) == deletedDist.end()) {
				double dis = d + v_m[minValue.first].at(iter_map->first);
				if (dis < distance[iter_map->first]) {
					distance[iter_map->first] = dis;
					previous[iter_map->first] = minValue.first;
				}
			}
		}
	}

	sP.push(node2);
	while (previous[target] != node1) {

		target = previous[target];
		sP.push(target);
	}
	sP.push(node1);

	return sP;
}

template <class T> bool MGraph<T>::edgeExists( const T& label1,const  T& label2) const {

 	unsigned int x = labelToNum(label1);
	unsigned int y = labelToNum(label2);
	return (v_m[x].find(y) != v_m[x].end());
}

template <typename T>
void readGraph(const string& s, MGraph<T>& g1) {

	ifstream file(s);

	if (file.is_open()) {

		T label1, label2;
		double cost;
		while (file >> label1 >> label2 >> cost) g1.addEdgeFast(label1, label2, cost);	
	}
	file.close();
}

int main() {

	const string fileName = "simple.txt";
	typedef string typeSpecified;

	MGraph<typeSpecified> g1;
	readGraph(fileName, g1);
	typeSpecified label1;
	typeSpecified label2;
	cout << "Enter the 1st node label \n";
	cin >> label1;
	cout << "Enter the 2nd node label \n";
	cin >> label2;
	
	if (g1.labelOccurs(label1) && g1.labelOccurs(label2)) {
	
		double sum = 0.0;
		unsigned int node1 = g1.labelToNum(label1);
		unsigned int node2 = g1.labelToNum(label2);
	
		stack<unsigned int> ss = g1.returnShortestPath(node1, node2);
		typeSpecified previosLabel = label1;
		typeSpecified nextLabel;
	
		if(!ss.empty()) cout << "The path is: \n";
		while (!ss.empty()) {
	
			nextLabel = g1.numToLabel(ss.top());
			sum += g1.determineCost(g1.labelToNum(previosLabel), g1.labelToNum(nextLabel));
			cout << sum << " " << nextLabel << "\n";
			ss.pop();
			previosLabel = nextLabel;
		}
	}
	else cout << "One of the two labels does not exist \n";
	return 0;
}
	
	
	
