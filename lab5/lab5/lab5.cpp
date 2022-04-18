#include <iostream>
#include <time.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;

int numGenerator(int min, int max);

vector<vector<int>> GenerateGraphs(int vertex, int min_num_edge_one_vortex, int min_weight, int max_weight, bool is_directed) {

	cout << "Число вершин: " << vertex << endl;

	vector<vector<int>> graph(vertex, vector<int>(vertex));

	for (int i = 0; i < vertex; i++)
	{
		int count_existing_edge = 0; //количество существующих ребер
		vector<int> not_adjacency_list; //вектор несмежных вершин

		//проверяем число связей вершины и добавляем не связанные вершины в вектор
		for (int j = 0; j < vertex; j++)
		{
			if (j != i) {

				if (graph[j][i] > 0)
					count_existing_edge++;
				else
					not_adjacency_list.push_back(j);
			}
		}

		int num_edges = numGenerator(min_num_edge_one_vortex, vertex - 1);

		if (count_existing_edge < num_edges)
			for (int k = 0; k < num_edges - count_existing_edge; k++)
			{
				int random_index = rand() % not_adjacency_list.size();
				int v = not_adjacency_list[random_index];
				not_adjacency_list.erase(not_adjacency_list.begin() + random_index);
				int weight = numGenerator(min_weight, max_weight);

				if (is_directed)
					graph[i][v] = weight;
				else
					graph[i][v] = graph[v][i] = weight;

			}
	}

	cout << "\nМатрица смежности" << endl;

	for (int i = 0; i < vertex; i++) {
		for (int j = 0; j < vertex; j++) {
			cout << setw(5) << graph[i][j] << "\t";
		}

		cout << endl;
	}

	cout << endl;

	return graph;
}

void algDijkstra(vector<vector<int>> graph, int start_ind, int finish_ind) {

	int num_vertex = graph.size();
	int max_value = 2e5;
	vector<bool> visited(num_vertex, false); // посещенные вершины
	vector<int> min_distance(num_vertex, max_value); // минимальное расстояние (изначально равно "максимуму")
	int temp, minindex, min;

	min_distance[start_ind] = 0;
	// Шаг алгоритма
	do {
		minindex = max_value;
		min = max_value;
		for (int i = 0; i < num_vertex; i++)
		{ // Если вершину ещё не обошли и вес меньше min
			if (!visited[i] && (min_distance[i] < min))
			{ // Переприсваиваем значения
				min = min_distance[i];
				minindex = i;
			}
		}
		// Добавляем найденный минимальный вес
		// к текущему весу вершины
		// и сравниваем с текущим минимальным весом вершины
		if (minindex != max_value)
		{
			for (int i = 0; i < num_vertex; i++)
			{
				if (graph[minindex][i] > 0)
				{
					temp = min + graph[minindex][i];
					if (temp < min_distance[i])
					{
						min_distance[i] = temp;
					}
				}
			}
			visited[minindex] = true;
		}
	} while (minindex < max_value);

	// Вывод кратчайших расстояний до вершин
	cout << "Кратчайшие расстояния до вершин:" << endl;
	for (int i = 0; i < num_vertex; i++)
		cout << setw(5) <<  min_distance[i] << "\t";

	// Восстановление пути
	vector<int> visited_vertex(num_vertex);
	int end = finish_ind;
	visited_vertex[0] = finish_ind;
	int k = 1; // индекс предыдущей вершины
	int weight = min_distance[finish_ind]; // вес конечной вершины

	while (end != start_ind) // пока не дошли до начальной вершины
	{
		for (int i = 0; i < num_vertex; i++) // просматриваем все вершины
			if (graph[i][end] != 0)   // если связь есть
			{
				int temp = weight - graph[i][end]; // определяем вес пути из предыдущей вершины
				if (temp == min_distance[i]) // если вес совпал с рассчитанным
				{                 // значит из этой вершины и был переход
					weight = temp; // сохраняем новый вес
					end = i;       // сохраняем предыдущую вершину
					visited_vertex[k] = i; // и записываем ее в массив
					k++;
				}
			}
	}
	// Вывод пути (начальная вершина оказалась в конце массива из k элементов)
	cout << "\nВывод кратчайшего пути:" << endl;
	for (int i = k - 1; i >= 0; i--)
		cout << setw(5) << visited_vertex[i] << "\t";

	cout << endl;
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");

	const int NUM_TEST = 10;
	const int MIN_WEIGHT = 1;
	const int MAX_WEIGHT = 20;

	vector<int> NUM_VERTEX = { 10, 20, 50, 100, 250, 500 };
	vector<int> MIN_NUM_ADJ = { 3, 4, 10, 20, 50, 100 };

	double time[NUM_TEST] = { 0 };

	ofstream tout("Time.txt");

	if (tout.is_open()) {
		for (int i = 0; i < NUM_VERTEX.size(); i++)
		{
			double sum_time = 0;

			tout << "Число вершин: " << NUM_VERTEX[i] << endl;
			tout << "Время поиска кратчайших путей (мс):" << endl;

			for (int j = 0; j < NUM_TEST; j++)
			{
				vector<vector<int>> v = GenerateGraphs(NUM_VERTEX[i], MIN_NUM_ADJ[i], MIN_WEIGHT, MAX_WEIGHT, false);

				chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
				algDijkstra(v, 0, v.size() - 1);
				chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
				chrono::duration<double, milli> milli_diff = end - start;

				time[j] = milli_diff.count();
				tout << time[j] << " ; ";

				sum_time += time[j];

				cout << "Время поиска кратчайших путей (мс): " << time[j] << endl;
				cout << endl << endl;

			}

			tout << endl;

			double min_t = time[0];
			double max_t = time[0];

			for (int j = 0; j < NUM_TEST; j++) {
				if (max_t < time[j]) max_t = time[j];
				if (min_t > time[j]) min_t = time[j];
			}

			tout << endl << "Минимальное: " << min_t << endl << "Максимальное: " << max_t << endl << "Среднее: " << sum_time / NUM_TEST << endl << endl;
			tout << endl << endl;
		}

		tout.close();
	}

}

int numGenerator(int min, int max) {
	return min + rand() % (max - min + 1);
}