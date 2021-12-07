#include<iostream>
#include<vector>
#include<algorithm>
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
	vector<int> a;
	for (int i = 1; i < graph.size(); i++) {
		if (find(a.begin(), a.end(), i) != a.end() || !graph[i].empty()) {
			if (i != 1)
				cout << "\n" << i << " - ";
			else
				cout << i << " - ";
		}
		for (auto it = graph[i].begin(); it != graph[i].end(); it++) {
			a.push_back(it->first);
			if (!graph[i].empty())
				cout << " ";
			cout << it->first << "(" << it->second << ")";
		}
	}

	// for(int i = 1; i < graph.size(); i++) {
	// 	cout << i << " - ";
	// 	for(int j = 0; j < graph[i].size(); j++) {
	// 		cout << graph[i][j].first << "(" << graph[i][j].second << ") ";
	// 	}
	// 	cout << endl;
	// }
}

void Graph::createRandomEdge(mt19937& randomGenerator, int V, int E) {
	graph.resize(V+1);
	int count = 0;
	// construct random weighted graph
	for (int i = 0; i < E;) {
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
	//Generate a random edge between any two nodes in the graph.
	//int from = 1 + rand() % V, to = 1 + rand() % V;
/*	
	def add_edge(self, edge):
		"""Add the edge if the graph type allows it."""
		if self.multigraph or edge not in self.edge_set:
			self.edges.append(edge)
			self.edge_set.add(edge)
			if not self.digraph:
				self.edge_set.add(edge[::-1])  # add other direction to set.
			return True
		return False

	def make_random_edge(self):
		if self.loops:
			With replacement.
			random_edge = (random.choice(self.nodes), random.choice(self.nodes))
		else:
			Without replacement.
			random_edge = tuple(random.sample(self.nodes, 2))
		return random_edge
	def add_random_edges(self, total_edges):
		"""Add random edges until the number of desired edges is reached."""
		while len(self.edges) < total_edges:
			self.add_edge(self.make_random_edge())
*/
}


int main()
{
	//insert number of vertex
	long long vertex;
	srand(time(NULL));

	mt19937 randomGenerator(time(NULL));

	cout << "Input number of vertices: ";
	cin >> vertex;

	//randomly assign total number of edges
	long long edge_max = (vertex * (vertex - 1));
	long long edge_min = vertex - 1;
	int edge = edge_min + rand() % (edge_max+1 - edge_min);
	cout << "\nThe graph has " << vertex << " vertices.";
	cout << "\nThe graph has " << edge << " edges." << endl;

	Graph g;
	g.createRandomEdge(randomGenerator, vertex, edge);
	g.printGraph();
}
