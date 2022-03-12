#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <vector>
#include <fstream>
#include <time.h>

using namespace std;

double insertionSort(vector <double> v) {

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

	for (int i = 1; i < v.size(); i++)
	{
		double key = v[i];
		int j = i - 1;

		while (j >= 0 && v[j] > key)
		{
			v[j + 1] = v[j];
			v[j] = key;
			j--;
		}
	}

	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> milli_diff = end - start;


	return milli_diff.count();
}


vector <double> vectorGenerator(int N) {

	const int MIN = -1;
	const int MAX = 1;

	vector <double> v;
	v.resize(N);

	for (int i = 0; i < N; i++)
	{
		v[i] = numGenerator(MIN, MAX);
	}

	return v;
}

double numGenerator(double min, double max) {
	return min + ((double)rand() / RAND_MAX * (max - min));
}


int main()
{
	srand(time(0));

	vector <int> N = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 };
	const int NUM_TRIES = 20;
	double time[NUM_TRIES] = { 0 };

	ofstream fout("Results.txt");
	if (fout.is_open())
	{
		for (int i = 0; i < N.size(); i++) {
			double summ = 0;
			fout << "Количество чисел: " << N[i] << endl << "Время (мс): ";

			for (int j = 0; j < NUM_TRIES; j++) {
				time[j] = insertionSort(vectorGenerator(N[i]));
				fout << time[j] << " ; ";
				summ += time[j];
			}

			double min = time[0];
			double max = time[0];

			for (int j = 0; j < NUM_TRIES; j++) {
				if (max < time[j]) max = time[j];
				if (min > time[j]) min = time[j];
			}
			fout << endl << "Минимальное: " << min << endl << "Максимальное: " << max << endl << "Среднее: " << summ / NUM_TRIES << endl;
			fout << endl << endl;
		}

		fout.close();
	}

}
