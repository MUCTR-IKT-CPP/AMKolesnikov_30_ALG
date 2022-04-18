#include<iostream>
#include<stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>


using namespace std;

int numGenerator(int min, int max);

vector<vector<int>> GenerateGraphs(int min_vertex, int max_vertex, int min_edge, int max_edge, int max_num_edge_one_vortex, bool is_directed) {

	int vertex, edges;

	vertex = numGenerator(min_vertex, max_vertex);
	int max_possible_num_edges = vertex * (vertex - 1) / 2;
	if ((min_edge <= max_possible_num_edges) && (max_edge <= max_possible_num_edges))
		edges = numGenerator(min_edge, max_edge);
	else if (min_edge <= ((vertex * (vertex - 1)) / 2)) {
		cout << "Максимальное число ребер превышает возможное значение ребер!" << endl;
		edges = numGenerator(min_edge, max_possible_num_edges);
	}
	else {
		cout << "Минимальное число ребер превышает возможное значение ребер! Поэтому число ребер будет равно максимально возможному: " << max_possible_num_edges << endl;
		edges = max_possible_num_edges;
	}

	cout << "\n\nЧисло вершин: " << vertex << endl;
		
	vector<vector<int>> graph(vertex, vector<int>(vertex));

	for (int i = 0; i < edges; i++)
	{
		int u, v;
		bool flag = false;

		do {
			u = rand() % vertex;
			v = rand() % vertex;

			int sum1 = 0;
			int sum2 = 0;
			flag = false;

			for (int i = 0; i < vertex; i++)
			{
				sum1 += graph[u][i];
				sum2 += graph[v][i];
			}

			if (u == v)
				flag = true;


		} while (flag);
		
		if (is_directed)
			graph[u][v] = 1;
		else
			graph[u][v] = graph[v][u] = 1;

	}

	return graph;

}

class GraphViews
{
public:
	GraphViews(vector<vector<int>> G) {
		adjMatrix = G;
		N = G.size();
		for (int i = 0; i < N; i++)
		{
			used.push_back(0);
		}
	};
	~GraphViews() {};

	vector<vector<int>> adjacencyMatrix() {

		cout << "\nМатрица смежности" << endl;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << adjMatrix[i][j] << "\t";
			}

			cout << endl;
		}

		cout << endl;

		return adjMatrix;
	}

	vector<vector<int>> adjacencyList() {
		vector<vector<int>> g(N);
		for (int i = 0; i < N;++i)
			for (int j = 0;j < N;++j) {
				if (adjMatrix[i][j]) 
					g[i].push_back(j);
			}

		cout << "\nСписок смежности" << endl;

		for (int i = 0; i < N; i++)
		{
			for (int n : g[i])
				cout << n << "\t";

			cout << endl;
		}

		cout << endl;

		return g;
	}

	vector<vector<int>> edgeList() {

		vector<vector<int>> edge;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++) {

				if (adjMatrix[i][j]) {
					vector<int> v(2);
					v[0] = i;
					v[1] = j;
					edge.push_back(v);
				}
			}
		}

		cout << "\nСписок ребер" << endl;

		for (int i = 0; i < edge.size(); i++)
		{
			for (int n : edge[i])
				cout << n << "\t";

			cout << endl;

		}

		cout << endl;

		return edge;
	}

	vector<vector<int>> incidenceMatrix() {

		int num_edge = 0, j_b = 0, col_nodirect = 0, col_direct = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if ((adjMatrix[i][j] == adjMatrix[j][i]) && (adjMatrix[i][j]))
					col_nodirect++;
				else if(adjMatrix[i][j])
					col_direct++;
			}
		}

		num_edge = col_direct + col_nodirect / 2;
		
		vector<vector<int>> matrix_incidence(N, vector<int>(num_edge));

		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if ((adjMatrix[i][j] == adjMatrix[j][i]) && (adjMatrix[i][j]))
				{
					matrix_incidence[i][j_b] = 1;
					matrix_incidence[j][j_b] = 1;
					j_b++;
				}
				else if ((adjMatrix[i][j] == 1) && (adjMatrix[j][i] == 0))
				{
					matrix_incidence[i][j_b] = 1;
					matrix_incidence[j][j_b] = -1;
					j_b++;
				}
				else if ((adjMatrix[i][j] == 0) && (adjMatrix[j][i] == 1))
				{
					matrix_incidence[i][j_b] = -1;
					matrix_incidence[j][j_b] = 1;
					j_b++;
				}
			}
		}

		cout << "\nМатрица инцидентности" << endl;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < matrix_incidence[0].size(); j++)
			{
				cout << matrix_incidence[i][j] << "\t";
			}
			cout << endl;
		}

		cout << endl;

		return matrix_incidence;
	}

	struct Edge {
		int begin;
		int end;
	};

	void bfs(int start_ind, int finish_ind) {

		queue<int> Queue;
		stack<Edge> Edges;
		Edge e;
		vector<int> nodes(N); // вершины графа
		int node;
		bool flag = false;
		Queue.push(start_ind); // помещаем в очередь первую вершину

		while (!Queue.empty())
		{ // пока очередь не пуста
			node = Queue.front(); // извлекаем вершину
			Queue.pop();
			nodes[node] = 2; // отмечаем ее как посещенную

			if (node == finish_ind) {
				flag = true;
				break;
			}

			for (int j = 0; j < N; j++)
			{ // проверяем для нее все смежные вершины
				if (adjMatrix[node][j] == 1 && nodes[j] == 0)
				{ // если вершина смежная и не обнаружена
					Queue.push(j); // добавляем ее в очередь
					nodes[j] = 1; // отмечаем вершину как обнаруженную
					e.begin = node; e.end = j;
					Edges.push(e);
				}
			}
		}

		cout << "Поиск в ширину:" << endl;

		if (flag) {
			cout << "Путь между вершинами " << start_ind << " и " << finish_ind << " найден!" << endl;
			cout << finish_ind;

			int ind_edge = finish_ind;

			while (!Edges.empty()) {
				e = Edges.top();
				Edges.pop();
				if (e.end == ind_edge) {
					ind_edge = e.begin;
					cout << " <- " << ind_edge;
				}
			}

			cout << endl;
		}
		else
			cout << "Пути между вершинами " << start_ind << " и " << finish_ind << " нет!" << endl;

	}

	void dfs(int start_ind, int finish_ind) {

		stack<int> Stack;
		stack<Edge> Edges;
		Edge e;
		vector<int> nodes(N); // вершины графа
		int node;
		bool flag = false;

		Stack.push(start_ind); // помещаем в очередь первую вершину
		while (!Stack.empty())
		{ // пока стек не пуст

			node = Stack.top(); // извлекаем вершину
			Stack.pop();
			if (nodes[node] == 2) continue;
			nodes[node] = 2; // отмечаем ее как посещенную

			if (node == finish_ind) {
				flag = true;
				break;
			}

			for (int j = N-1; j >= 0; j--)
			{ // проверяем для нее все смежные вершины
				if (adjMatrix[node][j] == 1 && nodes[j] != 2)
				{ // если вершина смежная и не обнаружена
					Stack.push(j); // добавляем ее в cтек
					nodes[j] = 1; // отмечаем вершину как обнаруженную
					e.begin = node; e.end = j;
					Edges.push(e);
				}
			}
		}

		cout << "Поиск в глубину:" << endl;

		if (flag) {
			cout << "Путь между вершинами " << start_ind << " и " << finish_ind << " найден!" << endl;
			cout << finish_ind;

			int ind_edge = finish_ind;

			while (!Edges.empty()) {
				e = Edges.top();
				Edges.pop();
				if (e.end == ind_edge) {
					ind_edge = e.begin;
					cout << " <- " << ind_edge;
				}
			}

			cout << endl;
		}
		else
			cout << "Пути между вершинами " << start_ind << " и " << finish_ind << " нет!" << endl;
	}

private:
	vector<vector<int>> adjMatrix;
	int N;
	vector<bool> used;
};



int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");

	int num_test = 10;

	for (int i = 0; i < num_test; i++)
	{
		int min_vertex = 2 * (i + 1);
		int max_vertex = 4 * (i + 1);
		vector<vector<int>> graph = GenerateGraphs(min_vertex, max_vertex, 2 * (i + 1), 5 * (i + 1), 1, false);
		GraphViews g(graph);
		g.adjacencyMatrix();
		g.incidenceMatrix();
		g.adjacencyList();
		g.edgeList();

		int rand_num = numGenerator(0, min_vertex - 1);

		chrono::high_resolution_clock::time_point start_dfs = chrono::high_resolution_clock::now();
		g.dfs(rand_num, min_vertex);
		chrono::high_resolution_clock::time_point end1 = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> milli_diff_dfs = end1 - start_dfs;
		cout << "Время поиска в глубину: " << milli_diff_dfs.count() << " мс" << endl << endl;

		chrono::high_resolution_clock::time_point start_bfs = chrono::high_resolution_clock::now();
		g.bfs(rand_num, min_vertex);
		chrono::high_resolution_clock::time_point end2 = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> milli_diff_bfs = end2 - start_bfs;
		cout << "Время поиска в ширину: " << milli_diff_bfs.count() << " мс" << endl;

	}

}

int numGenerator(int min, int max) {
	return min + rand() % (max - min + 1);
}