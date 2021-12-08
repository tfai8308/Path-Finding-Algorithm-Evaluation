#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<random>

using namespace std;

int randomInt(mt19937& randomGenerator, int min, int max) {
	uniform_int_distribution<int> distance(min, max);
	return distance(randomGenerator);
}

class Graph
{
private:
	vector<vector<pair<int, int>>> graph;
public:
	void createRandomEdge(mt19937& randomGenerator, int V, int E);
	void connectGraph(int V);
	bool isEdge(int from, int to);
	int getWeight(int from, int to);
	vector<int> getAdjacent(int vertex);
	void printGraph();
};

bool Graph::isEdge(int from, int to)
{
	for (auto it = graph[from].begin(); it != graph[from].end(); it++) {
		if (it->first == to)
			return true;
	}
	return false;
}

int Graph::getWeight(int from, int to)
{
	for (auto it = graph[from].begin(); it != graph[from].end(); it++) {
		if (it->first == to)
			return it->second;
	}
	return 0;
}

vector<int> Graph::getAdjacent(int vertex)
{
	vector<int> v;
	for (auto it = graph[vertex].begin(); it != graph[vertex].end(); it++) {
		v.push_back(it->first);
	}
	return v;
}

void Graph::printGraph()
{
	for (int i = 1; i < graph.size(); i++) {
		cout << i << " - ";
		for (int j = 0; j < graph[i].size(); j++) {
			cout << graph[i][j].first << "(" << graph[i][j].second << ") ";
		}
		cout << endl;
	}
}
void Graph::connectGraph(int V) {
	graph.resize(V + 1);
	for (int from = 1; from <= V; from++) {
		int weight = 1 + rand() % 10;
		if (from == V){
			graph[from].push_back(make_pair(1, weight));
			break;
		}
		int to = from + 1;
		graph[from].push_back(make_pair(to, weight));
	}
}
void Graph::createRandomEdge(mt19937& randomGenerator, int V, int E) {
	connectGraph(V);
	int count = 0;
	// construct random weighted graph
	for (int i = 0; i < E - V;) {
		int from = randomInt(randomGenerator, 1, V), to = randomInt(randomGenerator, 1, V), weight = randomInt(randomGenerator, 1, 10);
		//cout << from << " " << to << " " << weight << endl;

		//check if same edge exists
		if (from != to && !isEdge(from, to)) {
			graph[from].push_back(make_pair(to, weight));
		}
		else
			continue;
		i++;
	}
}


int main()
{
	//insert number of vertex
	int vertex;
	srand(time(NULL));

	mt19937 randomGenerator(time(NULL));

	cout << "Input number of vertices: ";
	cin >> vertex;

	//randomly assign total number of edges
	int edge_max = (vertex * (vertex - 1));
	int edge_min = vertex - 1;
	int edge = edge_min + rand() % (edge_max + 1 - edge_min);
	cout << "\nThe graph has " << vertex << " vertices.";
	cout << "\nThe graph has " << edge << " edges." << endl;

	Graph g;
	g.createRandomEdge(randomGenerator, vertex, edge);
	g.printGraph();
}
