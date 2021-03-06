#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<stack>
#include<set>
#include<random>
#include<chrono>
#include<algorithm>

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
	int connectGraph(mt19937& randomGenerator, int V);
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

int Graph::connectGraph(mt19937& randomGenerator, int V) {
	graph.resize(V+1);
	vector<int> a;
	for (int i = 1; i < V; ++i)
		a.push_back(i);
	shuffle(a.begin(), a.end(), randomGenerator);
	int from = 1;
	int to, weight, count = 0;
	while (from < V){
		to = a.back();
		//no self loop
		if (isEdge(to, from) || (from == to)) {
			if (a.size() != 1)
				swap(a[0], a[a.size() - 1]);
			else {
				graph[from].push_back(make_pair(V, weight));
				graph[V].push_back(make_pair(from, weight));
				count+=2;
				from++;
			}
		}
		else{
			weight = randomInt(randomGenerator, 1, 10);
			if (to == 1) {
				graph[from].push_back(make_pair(V, weight));
				count++;
				a.pop_back();
				from++;
			}
			else {
				graph[from].push_back(make_pair(to, weight));
				count++;
				a.pop_back();
				from++;
			}
		}
	}
	if (from == V) {
		weight = randomInt(randomGenerator, 1, 10);
		graph[from].push_back(make_pair(1, weight));
		count++;
	}
	cout << endl;
	return count;
}

void Graph::createRandomEdge(mt19937& randomGenerator, int V, int E) {
	int edgesLeft = E - connectGraph(randomGenerator, V);
	// construct random weighted graph
	for (int i = 0; i < edgesLeft;) {
		int from = randomInt(randomGenerator, 1, V), to = randomInt(randomGenerator, 1, V), weight = randomInt(randomGenerator, 1, 10);
		//check if same edge exists
		if (from != to && !isEdge(from, to)) {
			graph[from].push_back(make_pair(to, weight));
		}
		else
			continue;
		i++;
	}
}

///////IDDFS Algorithm////////
void getNeighborsDFS(Graph& g, set<int>& visited, stack<pair<int, int>>& vertices, int current, int depth) {
    vector<int> neighbors = g.getAdjacent(current);
    for(int e : neighbors) {
        if(visited.find(e) == visited.end()) {
            visited.insert(e);
            vertices.push(make_pair(e, depth + 1)); //Push in the neighbor and a depth one deeper
        }
    }
}

bool DFS(Graph& g, int source, int destination, int maxDepth) {
    set<int> visited;
    stack<pair<int, int>> vertices; //Each vertex is also going to store its depth from the source

    int depth = 0;
    visited.insert(source);
    vertices.push(make_pair(source, depth));

    while(!vertices.empty()) {
        pair<int, int> currentVertex = vertices.top();
        vertices.pop();

        if(currentVertex.first == destination) {
            return true;
        }

        if(currentVertex.second < maxDepth) {
            getNeighborsDFS(g, visited, vertices, currentVertex.first, currentVertex.second);
        }
    }

    return false;
}

bool IDDFS(Graph& g, int source, int destination, int maxDepth) {
    for(int i = 0; i <= maxDepth; i++) {
        if(DFS(g, source, destination, i)) {
            return true;
        }
    }

    return false;
}

///////Dijkstra's Algorithm////////
int findSmallestWeight(set<int>& notVisited, vector<int>& distance) {
    int vertex = -1;
    int minDist = INT_MAX;
    for(auto iter = notVisited.begin(); iter != notVisited.end(); iter++) {
        if(distance[*iter] < minDist) {
            minDist = distance[*iter];
            vertex = *iter;
        }
    }

    return vertex;
}

bool DijkstraPath(Graph& g, int numGraphVertices, int source, int destination) {
    set<int> visited;
    set<int> notVisited;

    visited.insert(source); //Load the starting vertex
    for(int i = 1; i <= numGraphVertices; i++) {
        if(visited.find(i) == visited.end()) {
            notVisited.insert(i); //Load all the other vertices
        }
    }

    vector<int> parent(numGraphVertices + 1, -1); //Load the initial parents
    vector<int> distance(numGraphVertices + 1, INT_MAX); //Load the initial "best distances"
    distance[source] = 0; //The source has a distance 0 from itself

    //Load the distances from the source to its neighbors
    for(auto iter = notVisited.begin(); iter != notVisited.end(); iter++) {
		parent[*iter] = source;
        if(g.isEdge(source, *iter)) {
            distance[*iter] = g.getWeight(source, *iter); //A path exists between these vertices
        }
        else {
            distance[*iter] = INT_MAX; //No path
        }
    }

    //Now keep processing all the other vertices and relaxing edges
    while(!notVisited.empty()) {
	//Get the smallest distance vertex
        int smallestDist = findSmallestWeight(notVisited, distance);
        notVisited.erase(smallestDist);
        visited.insert(smallestDist);

	//**In this modified Dijkstra's touching the destination vertex confirms that a path exists from the source vertex
	//So the code can terminate as soon as this vertex is found
	if(smallestDist == destination) {
		return true;
	}

        //Look through all of smallestDist's neighbors and see if taking this path results int a lower cost of travel
        //If so, reassign the weights to the sum of the current weight + new edge
        //Reassign the parent of the next vertex back to smallestDist
        for(auto iter = notVisited.begin(); iter != notVisited.end(); iter++) {
            if(g.isEdge(smallestDist, *iter)) {
                if(distance[smallestDist] + g.getWeight(smallestDist, *iter) < distance[*iter]) {
                    distance[*iter] = distance[smallestDist] + g.getWeight(smallestDist, *iter);
                    parent[*iter] = smallestDist;
                }
            }
        }
    }

    return false;
}

int main()
{
	//insert number of vertex
	int vertex;
	srand(time(NULL));

	mt19937 randomGenerator(time(NULL));

	cout << "Input number of vertices: ";
	cin >> vertex;
	
	//verify input for numbers
	while(!cin || vertex < 0) {
		cout << "Enter a positive number of vertices." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> vertex;
	}

	//randomly assign total number of edges
	long long edge_max = (vertex * (vertex - 1))/2;
	int edge_min = vertex - 1;
	int edge = edge_min + rand() % (edge_max + 1 - edge_min);
	cout << "\nThe graph has " << vertex << " vertices.";
	cout << "\nThe graph has " << edge << " edges." << endl;

	Graph g;
	g.createRandomEdge(randomGenerator, vertex, edge);
	
	//Displays the contents of the graph in adjacency list representation
	//g.printGraph();

	int source = randomInt(randomGenerator, 1, vertex);
	int destination = randomInt(randomGenerator, 1, vertex);

	cout << "The source is vertex " << source << "." << endl;
	cout << "The destination is vertex " << destination << "." << endl;
	cout << endl;

	auto timeBegin = chrono::high_resolution_clock::now();
	if(IDDFS(g, source, destination, vertex)) {
		cout << "A path was found from vertex " << source << " to vertex " << destination << " with IDDFS";
	}	
	else {
		cout << "A path was not found with IDDFS?" << endl;
		exit(1);
	}
	auto timeEnd =  chrono::high_resolution_clock::now();
	auto timeElapsed = chrono::duration_cast<chrono::nanoseconds>(timeEnd - timeBegin);
	cout << " in " << timeElapsed.count() * 1e-6 << " milliseconds (" << timeElapsed.count() * 1e-9 << " seconds)." << endl;
	
	timeBegin = chrono::high_resolution_clock::now();
	if(DijkstraPath(g, vertex, source, destination)) {
		cout << "A path was found from vertex " << source << " to vertex " << destination << " with Dijkstra's Algorithm";
	} 
	else {
		cout << "A path was not found with Dijkstra's Algorithm?" << endl;
		exit(1);
	}
	timeEnd =  chrono::high_resolution_clock::now();
	timeElapsed = chrono::duration_cast<chrono::nanoseconds>(timeEnd - timeBegin);
	cout << " in " << timeElapsed.count() * 1e-6 << " milliseconds (" << timeElapsed.count() * 1e-9 << " seconds)." << endl;
}
