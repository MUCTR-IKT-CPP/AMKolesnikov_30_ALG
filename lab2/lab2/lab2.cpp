#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;

void quSort(vector<double>& vector, int left, int right, int& callCount);

vector <double> vectorGenerator(int N);

double numGenerator(double min_r, double max);

int measurements(vector<double>& vec, double* time);

vector <double> sameVector(vector<double> vec);

vector <double> midBadVector(vector<double> vec);

bool comparator(const pair<double, int>& left, const pair<double, int>& right);

vector <double> determVector(int lenght);

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Rus");

    const int NUMS_COUNT = 8;
    const int NUM_TRIES = 20;
    int call[NUM_TRIES] = { 0 };
    double time[NUM_TRIES] = { 0 };
    double time_sort[NUMS_COUNT] = { 0 };
    double time_same[NUMS_COUNT] = { 0 };
    double time_midle[NUMS_COUNT] = { 0 };
    double time_determ[NUMS_COUNT] = { 0 };

    int N[] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 };

    vector <double> sortVec;

    ofstream rout("Recursions.txt");
    ofstream tout("Time.txt");

    if (rout.is_open()) {
        for (int i = 0; i < NUMS_COUNT; i++) {
            double sum_call = 0;
            double sum_time = 0;
            rout << "Кол-во чисел: " << N[i] << endl << endl << "Кол-во вызовов: ";
            tout << "Количество чисел: " << N[i] << endl << "Время (мс): ";

            for (int j = 0; j < NUM_TRIES; j++) {
                sortVec = vectorGenerator(N[i]);
                call[j] = measurements(sortVec, &time[j]);
                rout << call[j] << "  ;  ";
                tout << time[j] << " ; ";

                sum_call += call[j];
                sum_time += time[j];
            }

            rout << endl;
            tout << endl;
            double min_r = call[0];
            double max_r = call[0];

            double min_t = time[0];
            double max_t = time[0];

            for (int j = 0; j < NUM_TRIES; j++) {
                if (max_r < call[j]) max_r = call[j];
                if (min_r > call[j]) min_r = call[j];

                if (max_t < time[j]) max_t = time[j];
                if (min_t > time[j]) min_t = time[j];
            }

            rout << endl << "Минимальное: " << min_r << endl << "Максимальное: " << max_r << endl << "Среднее: " << sum_call / NUM_TRIES << endl << endl;
            tout << endl << "Минимальное: " << min_t << endl << "Максимальное: " << max_t << endl << "Среднее: " << sum_time / NUM_TRIES << endl << endl;

            rout << "Отсортированный массив: ";
            tout << "Отсортированный массив: ";

            rout << measurements(sortVec, &time_sort[i]) << endl;
            tout << time_sort[i] << endl;

            rout << "Массив с одинаковыми элементами: ";
            tout << "Массив с одинаковыми элементами: ";

            vector <double> sameVec = sameVector(sortVec);
            rout << measurements(sameVec, &time_same[i]) << endl;
            tout << time_same[i] << endl;

            rout << "Массив с максимальным количеством сравнений при выборе среднего элемента в качестве опорного: ";
            tout << "Массив с максимальным количеством сравнений при выборе среднего элемента в качестве опорного: ";

            vector <double> midVec = midBadVector(sortVec);
            rout << measurements(midVec, &time_midle[i]) << endl;
            tout << time_midle[i] << endl;

            rout << "Массив с максимальным количеством сравнений при детерминированном выборе опорного элемента: ";
            tout << "Массив с максимальным количеством сравнений при детерминированном выборе опорного элемента: ";

            vector <double> determVec = determVector(N[i]);
            rout << measurements(determVec, &time_determ[i]) << endl;
            tout << time_determ[i] << endl;

            tout << endl << endl;
            rout << endl << endl;


        }

        rout.close();
        tout.close();
    }

}

void quSort(vector<double>& vector, int left, int right, int& callCount) {


    callCount++;

    //Указатели в начало и в конец массива
    int i = left;
    int j = right;

    //Центральный элемент массива
    auto mid = vector[(left + right) / 2];

    //Делим массив
    do {
        //Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        //В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
        while (vector[i] < mid) {
            i++;
        }
        //В правой части пропускаем элементы, которые больше центрального
        while (vector[j] > mid) {
            j--;
        }

        //Меняем элементы местами
        if (i <= j) {

            swap(vector[i], vector[j]);

            i++;
            j--;
        }
    } while (i < j);


    //Рекурсивные вызовы, если осталось, что сортировать
    if (left < j)
        quSort(vector, left, j, callCount);

    if (i < right)
        quSort(vector, i, right, callCount);

}

int measurements(vector<double>& vec, double* time) {
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    int callCount = 0;

    quSort(vec, 0, vec.size() - 1, callCount);

    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> milli_diff = end - start;

    *time = milli_diff.count();
    return callCount;
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

vector <double> sameVector(vector<double> vec) {
    for (int i = 0; i < vec.size(); i++)
        vec[i] = vec[0];
    return vec;
}

vector <double> midBadVector(vector<double> vec) {
    for (int i = 0; i < vec.size(); i++)
        swap(vec[i], vec[i / 2]);
    return vec;
}

bool comparator(const pair<double, int>& left, const pair<double, int>& right) {
    return (left.second < right.second);
}

vector <double> determVector(int lenght) {
    int n = lenght;
    vector <double> determVec;
    determVec.reserve(lenght);

    vector<pair<double, int>> pairVec;
    pairVec.reserve(lenght);

    for (int i = 0; i < lenght; i++) {
        pairVec.emplace_back(0, i + 1);
    }

    int step = 1;
    const int left = 0;
    int decrease = 1;
    while (lenght > 0) {
        int right = pairVec.size() - (decrease++);
        pairVec[(left + right) / 2] = make_pair((n - step + 1), (pairVec[(left + right) / 2]).second);

        pair<double, int> pivot = pairVec[(left + right) / 2];
        swap(pairVec[(left + right) / 2], pairVec[right]);
        lenght--;
        step++;
    }

    sort(pairVec.begin(), pairVec.end(), comparator);

    for (int i = 0; i < n; i++) {
        determVec.emplace_back(pairVec[i].first);
    }
    return determVec;
}
