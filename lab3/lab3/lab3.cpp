#include <iostream>
#include <exception>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <list>
#include <random>

using namespace std;

int numGenerator(int min, int max);
string fio();
int year();

template <typename T>
struct MyStack
{
    MyStack() : _top(NULL), _size(0){}

	struct Node
	{
		Node(T el) : data(el), next(NULL) {}
		Node(T el, Node* node) : data(el), next(node) {}
		Node* next;
		T data;
	};

	void push(const T);
	void pop();
	T& top();
	int size();
	bool empty();
	void printStack();
	void sorting();

	class Iterator
	{
	public:
		Iterator(Node* current_node = NULL) : current_node(current_node) {};
		~Iterator() {};

		Iterator& operator++() {
			current_node = current_node->next;
			return *this;
		}

		T& operator*() const {
			return current_node->data;
		}

		bool operator!=(const Iterator& other) const {
			return current_node != other.current_node;
		}


	private:
		Node* current_node;
	};

	Iterator Begin() { return Iterator(_top); }
	Iterator End() { return Iterator(); }

private:
	Node* _top;
	int _size;
};

template<typename T>
void MyStack<T>::push(const T el)
{
	try
	{
		if (_top != NULL) {
			Node* tmp = new Node(el, _top);
			_top = tmp;
		}
		else
			_top = new Node(el);

		_size++;
	}
	catch (bad_alloc)
	{
		cout << "bad_alloc" << endl;
	}

}

template<typename T>
void MyStack<T>::pop()
{
	try
	{
		if (empty())
			throw out_of_range("Error: Out of range");

		Node* tmp = _top;
		_top = _top->next;
		delete tmp;
		_size--;
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}

}

template<typename T>
T& MyStack<T>::top()
{
	try
	{
		if (empty())
			throw out_of_range("Error: MyStack is empty");

		return _top->data;

	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}

template<typename T>
int MyStack<T>::size()
{
	return _size;
}

template<typename T>
bool MyStack<T>::empty()
{
	return _size == 0 ? true : false;
}

template<typename T>
void MyStack<T>::printStack()
{
	//for (Iterator it = my_stack.Begin(); it != my_stack.End(); ++it) {
	//	cout << *it << endl;
	//}
	auto print = [](const T& n) { cout << n << endl; };
	for_each(Begin(), End(), print);
	
}

template<typename T>
void MyStack<T>::sorting()
{
	vector<T> v;

	while (!empty())
	{
		v.push_back(top());
		pop();
	}

	sort(v.begin(), v.end());

	for (int i = 0; i < v.size(); i++) {
		push(v[i]);
	}
}



void test1() {
	MyStack<int> my_stack;
	int N = 1000;
	int MIN = -1000;
	int MAX = 1000;

	int sum = 0, min_el = MAX, max_el = MIN;
	double avg;

	cout << "Тест 1" << endl << endl;

	for (int i = 0; i < N; i++)
	{
		int el = numGenerator(MIN, MAX);
		my_stack.push(el);
		sum += el;
		if (el > max_el) max_el = el;
		if (el < min_el) min_el = el;
	}

	avg = (double)sum / N;

	cout << "Размер стэка: " << my_stack.size() << endl;
	cout << "Минимальный элемент: " << min_el << endl;
	cout << "Максимальный элемент: " << max_el << endl;
	cout << "Среднее арифметическое: " << avg << endl;
	cout << "Сумма элементов: " << sum << endl << endl << endl;
}

void test2() {
	MyStack<string> my_stack;
	vector<string> bank = { "Москва" , "Омск" , "Калуга", "Коломна", "Челябинск", "Балашиха", "Егорьевск", "Воскресенск", "Гжель", "Серпухов" };

	cout << "Тест 2" << endl << endl;

	for (int i = 0; i < bank.size(); i++)
	{
		my_stack.push(bank[i]);
	}

	cout << "Элементы стека:" << endl;
	my_stack.printStack();
	cout << "Размер стэка: " << my_stack.size() << endl;

	cout << "\nУдалили 2 элемента с начала стека:" << endl;
	my_stack.pop();
	my_stack.pop();
	my_stack.printStack();
	cout << "Размер стэка: " << my_stack.size() << endl;

	cout << "\nДобавили 1 элемент (Воронеж): " << endl;
	my_stack.push("Воронеж");
	my_stack.printStack();

	cout << "Размер стэка: " << my_stack.size() << endl << endl << endl;
}

void test3() {

	struct Human
	{
		string fio;
		int year;
	};

	cout << "Тест 3" << endl;
	int N = 100;
	MyStack<Human> humanList;

	for (int i = 0; i < N; i++)
	{
		Human newhuman;
		newhuman.fio = fio();
		newhuman.year = year();
		humanList.push(newhuman);
	}

	MyStack<Human> y20;
	MyStack<Human> o30;
	int ageError = 0;

	while (!humanList.empty())
	{
		auto el = humanList.top();

		if (2020 - el.year < 20) y20.push(el);
		else if (2020 - el.year > 30) o30.push(el);

		humanList.pop();
	}


	while (!y20.empty()) {
		auto el = y20.top();

		if (el.year < 2000) ageError++;

		y20.pop();
	}
	
	cout << "Число ошибок в стеке людей младше 20: " << ageError << endl;

	ageError = 0;

	while (!o30.empty()) {
		auto el = o30.top();

		if (el.year > 1990) ageError++;

		o30.pop();
	}
	cout << "Число ошибок в стеке людей старше 30: " << ageError << endl << endl;

}

string fio() {

	const int N = 2;
	string fio;

	string firstName[N] = { "Артем", "Максим" };
	string secondName[N] = { "Петров", "Иванов" };
	string patronymic[N] = { "Андреевич", "Степанович" };

	int randomName[3] = { rand() % ((N - 1) + 1), rand() % ((N - 1) + 1), rand() % ((N - 1) + 1) };
	return fio = secondName[randomName[0]] + " " + firstName[randomName[0]] + " " + patronymic[randomName[0]];
};

int year() {
	return numGenerator(1990, 2020);
}

void test4() {
	MyStack<int> my_stack;
	//stack<int> defolt_stack;
	int N = 10;
	int MIN = -10;
	int MAX = 10;

	cout << "Тест 4" << endl;

	for (int i = 0; i < N; i++)
	{
		int el = numGenerator(MIN, MAX);
		my_stack.push(el);
		//defolt_stack.push(el);
	}
	cout << "\nИсходный стек:" << endl;
	my_stack.printStack();
	my_stack.sorting();
	cout << "\nОтсортированный стек:" << endl;
	my_stack.printStack();
	cout << endl << endl;

}

// Инверсировать содержимое контейнера заполненного отсортированными по возрастанию 
// элементами не используя операцию перемещения при помощи итератора, а только операторы изъятия и вставки
void test5() {
	MyStack<int> my_stack;
	MyStack<int> stack2;
	int N = 10;

	cout << "Тест 5" << endl;

	for (int i = 0; i < N; i++)
	{
		my_stack.push(i+1);
	}
	cout << "\nИсходный стек:" << endl;
	my_stack.printStack();

	while (!my_stack.empty())
	{
		stack2.push(my_stack.top());
		my_stack.pop();
	}

	my_stack = stack2;

	cout << "\nИнверсированный:" << endl;
	my_stack.printStack();
	cout << endl << endl;
}

int numGenerator(int min, int max) {
	return min + rand() % (max - min + 1);
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");

	//MyStack<int> my_stack;
	//int N = 10;

	//cout << my_stack.top() << endl;
	//my_stack.pop();


	//for (int i = 0; i < N; i++)
	//{
	//	my_stack.push(i+1);
	//}

	//cout << "Размер стэка: " << my_stack.size() << endl;



	////while (!my_stack.empty())
	////{
	////	cout << my_stack.top() << endl;
	////	my_stack.pop();
	////}

	//my_stack.printStack(my_stack);

	//cout << "Размер стэка: " << my_stack.size() << endl;

	//cout << my_stack.top() << endl;
	//my_stack.pop();

	test1();
	test2();
	test3();
	test4();
	test5();
	//my_stack.sorting();
}