
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

//The graph class
template <class T>
class MGraph {

	vector<map<unsigned int, double>> v_m;
	vector<T> v;
	map<T, unsigned int> m;

public:
	MGraph() {};

	//This function returns the number of nodes in the graph
	inline unsigned int numberOfNodes() const { return v.size(); }
	// This function tells whether a provided label occurs or not
	inline bool labelOccurs(const T& label) const { return m.find(label) != m.end(); }
	//This function returns a label  for a input label number
	inline T numToLabel(const unsigned int& num) const { return v[num]; }
	//This function returns the label number for the input label 
	inline unsigned int labelToNum(const T& label) const { return m.at(label); }
	//This function compares the "value" of 2 given pairs. 
	inline static bool comparePair(pair<unsigned int, double> i, pair<unsigned int, double> j) { return i.second < j.second; }
	//This function adds a label to the graph
	void addLabel(const T& label);
	//This function adds a edge to the graph after checking whether the occurence of atleast 1 label in the graph
	void addEdgeAfterCheck(const T& label1, const T& label2, const double& cost);
	//This function adds an edge to the graph
	void addEdgeFast(const T& label1, const T& label2, const double& cost);
	//This function checks whether an edge exists between 2 labels or not
	bool edgeExists(const T& label1, const  T& label2) const;
	//This function determines the cost of an edge after checking whether the edge exists or not
	double determineCost(const unsigned int& node1, const unsigned int& node2) const;
	// This function returns the shortest path using the Dijkstra's algorithm
	stack<unsigned int> returnShortestPath(const unsigned int& node1, const unsigned int& node2) const;
	~MGraph() {};
};

//This function adds a node to the graph. The node is identified by the label
template <class T> void MGraph<T>::addLabel(const T& label) {
	if (!labelOccurs(label)) {
		m[label] = v.size();
		v.push_back(label);
		v_m.push_back(map<unsigned int, double>());
	}
}

//This function adds a edge to the graph after checking whether the occurence of atleast 1 label in the graph
template <class T> void MGraph<T>::addEdgeAfterCheck(const T& label1, const T& label2, const double& cost) {

	char input;
	if (labelOccurs(label1) || labelOccurs(label2)) addEdgeFast(label1, label2, cost);
	else {
		cout << "none of the 2 labels exist. Do you still need to add(Y/N)?";
		cin >> input;
		if (input == 'Y') addEdgeFast(label1, label2, cost);
	}
}

//This function adds an edge to the graph
template <class T> void MGraph<T>::addEdgeFast(const T& label1, const T& label2, const double& cost) {

	addLabel(label1);
	addLabel(label2);
	unsigned int x = labelToNum(label1);
	unsigned int y = labelToNum(label2);
	v_m[x][y] = cost;
}

//This function determines the cost of an edge after checking whether the edge exists or not
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

// This function returns the shortest path using the Dijkstra's algorithm
template <class T> stack<unsigned int> MGraph<T>::returnShortestPath(const unsigned int& node1, const unsigned int& node2) const {

	//The return type of the function is a stack
	stack<unsigned int> sP;
	unsigned int vsize = v.size();

	//If the node number is greater than the number of nodes in the graph the function outputs the following
	if (node1 > vsize - 1 || node2 > vsize - 1) {
		cout << "One of the node number you entered is greater than the total number of nodes in the graph.";
		cout << "Try again in the range " << 0 << "-" << vsize - 1 << "\n";
		return sP;
	}

	//If both the node numbers are same 
	if (node1 == node2) {
		cout << "Same node, 0 distance \n";
		return sP;
	}

	//The end node
	unsigned int target = node2;
	//This map stores the distance between each node and node1 (the starting node)
	map<unsigned int, double> distance;
	//This map stores the nodes that will be deleted from the distance map
	map<unsigned int, double> deletedDist;
	//This map stores the previous node for the current node 
	map<unsigned int, unsigned int> previous;

	//Initializing all the values in the distance map to Infinity because of unknown distance
	for (int it = 0; it < vsize; it++) distance[it] = INFINITY;

	//Distance from nodel1 to node1 (source->source)	
	distance[node1] = 0;

	while (!distance.empty()) {

		//Finding the node which has the minimun distance from the source
		pair<unsigned int, double> minValue = *min_element(distance.begin(), distance.end(), &MGraph<T>::comparePair);


		if (minValue.second == INFINITY) {

			cout << "No path exists \n";
			return sP;
		}

		double d = minValue.second;
		deletedDist.insert(minValue);
		distance.erase(minValue.first);

		//If the node deleted from the distance map is the target node break from the while loop
		if (minValue.first == node2) break;

		//Iterating over all the neighbouring nodes of the node which is currently selected ( that is node with minimum distance from source)
		for (auto iter_map = v_m[minValue.first].begin(); iter_map != v_m[minValue.first].end(); iter_map++) {
			//Do not iterate over the neighbouring node if it has been deleted
			if (deletedDist.find(iter_map->first) == deletedDist.end()) {
				double dis = d + v_m[minValue.first].at(iter_map->first);
				//If shorter distance than the one stored in the distance map is found
				if (dis < distance[iter_map->first]) {
					//Update the "distance" map and the "previous" map
					distance[iter_map->first] = dis;
					previous[iter_map->first] = minValue.first;
				}
			}
		}
	}

	//Save the shortest path in a stack
	sP.push(node2);
	while (previous[target] != node1) {

		target = previous[target];
		sP.push(target);
	}
	sP.push(node1);

	return sP;
}

//This function checks whether an edge exists between 2 labels or not
template <class T> bool MGraph<T>::edgeExists(const T& label1, const  T& label2) const {

	unsigned int x = labelToNum(label1);
	unsigned int y = labelToNum(label2);
	return (v_m[x].find(y) != v_m[x].end());
}

//This function reads a file and creates a graph
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

//The main function
int main() {

	//Specify the filename you want to read
	const string fileName = "simple.txt";
	//Specify the type of graph you would like to create according to the label names stores in your "filename" above
	typedef string typeSpecified;

	MGraph<typeSpecified> g1;
	readGraph(fileName, g1);

	typeSpecified label1;
	typeSpecified label2;

	//Enter the label names 
	cout << "Enter the 1st node label \n";
	cin >> label1;
	cout << "Enter the 2nd node label \n";
	cin >> label2;

	//If those labels exist
	if (g1.labelOccurs(label1) && g1.labelOccurs(label2)) {

		double sum = 0.0;
		unsigned int node1 = g1.labelToNum(label1);
		unsigned int node2 = g1.labelToNum(label2);

		stack<unsigned int> ss = g1.returnShortestPath(node1, node2);
		typeSpecified previosLabel = label1;
		typeSpecified nextLabel;

		//Display the path on the screen along with the sum
		if (!ss.empty()) cout << "The path is: \n";
		while (!ss.empty()) {

			nextLabel = g1.numToLabel(ss.top());
			sum += g1.determineCost(g1.labelToNum(previosLabel), g1.labelToNum(nextLabel));
			cout << sum << " " << nextLabel << "\n";
			ss.pop();
			previosLabel = nextLabel;
		}
	}
	else cout << "One of the two labels does not exist OR you have not loaded the file correctly \n";
	return 0;
}



