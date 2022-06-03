#include <string>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <vector>
#include <fstream>  
#include <algorithm>
#include <stack>
#include <numeric>

using namespace std;

int numGenerator(int min, int max);
string getLastNums(vector<int> toss, int num_last);
vector<int> generateToss(int num_toss);


int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");

	vector<string> A = { "000", "001", "010", "011", "100", "101", "110", "111" };
	vector<string> B = { "000", "001", "010", "011", "100", "101", "110", "111" };

	vector<vector<double>> win_probability_A(A.size(), vector<double>(B.size()));

	vector<int> NUM_GAMES = { 10, 100, 1000, 10000, 100000 };
	int NUM_LAST = 3;
	int NUM_TOSS = 15;

	ofstream rout("Results.txt");

	if (rout.is_open()) {
		for (int n = 0; n < NUM_GAMES.size(); n++)
		{
			cout << "Тест " << n + 1 << endl << endl;

			for (int i = 0; i < A.size(); i++)
			{

				for (int j = 0; j < B.size(); j++)
				{
					if (A[i] == B[j])
						continue;

					vector<int> wins_A_B(NUM_GAMES[n]);

					for (int k = 0; k < NUM_GAMES[n]; k++)
					{
						vector<int> toss = generateToss(NUM_TOSS);
						string last_num = getLastNums(toss, NUM_LAST);

						bool flag_A = A[i] == last_num;
						bool flag_B = B[j] == last_num;

						if (flag_A)
							wins_A_B[k] = 1;

						if (!flag_A && !flag_B) {
							k--;
							continue;
						}
					}

					double probability = (double)accumulate(wins_A_B.begin(), wins_A_B.end(), 0) / NUM_GAMES[n];
					win_probability_A[i][j] = probability;
				}
			}

			rout << "Тест " << n + 1 << endl;
			rout << "Число игр на каждую комбинацию: " << NUM_GAMES[n] << endl << endl;
			rout << setw(10) << "A\\B" << "\t";

			for (int i = 0; i < B.size(); i++)
			{
				rout << setw(10) << B[i] << "\t";
			}
			rout << "\n";

			double sum_win_probability_A = 0;
			double avg_win_probability_A = 0;
			int count_probabilities = 0;

			for (int i = 0; i < A.size(); i++)
			{
				rout << setw(10) << A[i] << "\t";
				for (int j = 0; j < B.size(); j++) {
					rout << setw(10) << win_probability_A[i][j] << "\t";
					sum_win_probability_A += win_probability_A[i][j];
					if (win_probability_A[i][j] != 0)
						count_probabilities++;
				}

				rout << "\n";
			}

			avg_win_probability_A = sum_win_probability_A / count_probabilities;

			rout << "\nCуммарный средний шанс выйгрыша игрока А: " << avg_win_probability_A << endl;
			rout << "Cуммарный средний шанс выйгрыша игрока B: " << 1 - avg_win_probability_A << endl << endl << endl;
		}

		rout.close();
	}

}


int numGenerator(int min, int max) {
	return min + rand() % (max - min + 1);
}


vector<int> generateToss(int num_toss) {

	vector<int> toss;

	for (int j = 0; j < num_toss; j++)
		toss.push_back(numGenerator(0, 1));

	return toss;
}


string getLastNums(vector<int> toss, int num_last) {

	string str;

	for (int i = num_last; i > 0 ; i--)
		str += to_string(toss[toss.size() - i]);
	
	return str;
}