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
void swap(int* x, int* y);

class BinaryHeap
{
    int* harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    BinaryHeap(int capacity);

    // to heapify a subtree with the root at given index
    void BinaryHeapify(int);

    int parent(int i) { return (i - 1) / 2; }

    // to get index of left child of node at index i
    int left(int i) { return (2 * i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2 * i + 2); }

    // to extract the root which is the minimum element
    int extractMin();

    // Returns the minimum key (key at root) from min heap
    int getMin() { return harr[0]; }

    // Inserts a new key 'k'
    void insertKey(int k);
};

// Constructor: Builds a heap from a given array a[] of given size
BinaryHeap::BinaryHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new int[cap];
}

// Inserts a new key 'k'
void BinaryHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}


// Method to remove minimum element (or root) from min heap
int BinaryHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    // Store the minimum value, and remove it from heap
    int root = harr[0];
    harr[0] = harr[heap_size - 1];
    heap_size--;
    BinaryHeapify(0);

    return root;
}


// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void BinaryHeap::BinaryHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        BinaryHeapify(smallest);
    }
}

 //A utility function to swap two elements
void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

struct BinNode
{
    int key;
    int degree;
    BinNode* f, * p, * c;

    BinNode()
    {
        this->key = 0;
        this->degree = 0;
        this->f = this->p = this->c = NULL;
    }

    BinNode(int key)
    {
        this->key = key;
        this->degree = 0;
        this->f = this->p = this->c = NULL;
    }
};

class BinomialHeap
{
private:
    BinNode* roots;
    BinNode* min;
    void linkTrees(BinNode*, BinNode*);
    BinNode* mergeRoots(BinomialHeap*, BinomialHeap*);

public:
    BinomialHeap();
    BinomialHeap(BinNode*);
    bool isEmpty();
    void insert(int);
    void merge(BinomialHeap*);
    BinNode* first();
    int getMin() { return first()->key; }
    BinNode* extractMin();
};

BinomialHeap::BinomialHeap()
{
    this->roots = NULL;
}

BinomialHeap::BinomialHeap(BinNode* x)
{
    this->roots = x;
}

bool BinomialHeap::isEmpty()
{
    return (this->roots == NULL);
}

void BinomialHeap::insert(int x)
{
    this->merge(new BinomialHeap(new BinNode(x)));
}

void BinomialHeap::linkTrees(BinNode* y, BinNode* z)
{
    // Precondition: y -> key >= z -> key
    y->p = z;
    y->f = z->c;
    z->c = y;
    z->degree = z->degree + 1;
}

BinNode* BinomialHeap::mergeRoots(BinomialHeap* x, BinomialHeap* y)
{
    BinNode* ret = new BinNode();
    BinNode* end = ret;

    BinNode* L = x->roots;
    BinNode* R = y->roots;
    if (L == NULL) return R;
    if (R == NULL) return L;
    while (L != NULL || R != NULL)
    {
        if (L == NULL)
        {
            end->f = R;
            end = end->f;
            R = R->f;
        }
        else if (R == NULL)
        {
            end->f = L;
            end = end->f;
            L = L->f;
        }
        else
        {
            if (L->degree < R->degree)
            {
                end->f = L;
                end = end->f;
                L = L->f;
            }
            else
            {
                end->f = R;
                end = end->f;
                R = R->f;
            }
        }
    }
    return (ret->f);
}

void BinomialHeap::merge(BinomialHeap* bh)
{
    BinomialHeap* H = new BinomialHeap();
    H->roots = mergeRoots(this, bh);

    if (H->roots == NULL)
    {
        this->roots = NULL;
        this->min = NULL;
        return;
    }

    BinNode* prevX = NULL;
    BinNode* x = H->roots;
    BinNode* nextX = x->f;
    while (nextX != NULL)
    {
        if (x->degree != nextX->degree || (nextX->f != NULL && nextX->f->degree == x->degree))
        {
            prevX = x;
            x = nextX;
        }
        else if (x->key <= nextX->key)
        {
            x->f = nextX->f;
            linkTrees(nextX, x);
        }
        else
        {
            if (prevX == NULL) H->roots = nextX;
            else prevX->f = nextX;
            linkTrees(x, nextX);
            x = nextX;
        }
        nextX = x->f;
    }

    this->roots = H->roots;
    this->min = H->roots;
    BinNode* cur = this->roots;
    while (cur != NULL)
    {
        if (cur->key < this->min->key) this->min = cur;
        cur = cur->f;
    }
}

BinNode* BinomialHeap::first()
{
    return this->min;
}



BinNode* BinomialHeap::extractMin()
{
    BinNode* ret = this->first();

    // delete ret from the list of roots
    BinNode* prevX = NULL;
    BinNode* x = this->roots;
    while (x != ret)
    {
        prevX = x;
        x = x->f;
    }
    if (prevX == NULL) this->roots = x->f;
    else prevX->f = x->f;

    // reverse the list of ret's children
    BinNode* revChd = NULL;
    BinNode* cur = ret->c;
    while (cur != NULL)
    {
        BinNode* next = cur->f;
        cur->f = revChd;
        revChd = cur;
        cur = next;
    }

    // merge the two lists
    BinomialHeap* H = new BinomialHeap();
    H->roots = revChd;
    this->merge(H);

    return ret;
}


int main()
{

    srand(time(0));
    setlocale(LC_ALL, "Rus");

    const int NUM_TEST = 5;
    const int NUM_OPERATIONS = 1000;

    vector<vector<double>> time_search_binary(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_search_binomial(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<double> sum_time_search_binary(NUM_TEST);
    vector<double> sum_time_search_binomial(NUM_TEST);

    vector<vector<double>> time_insert_binary(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_insert_binomial(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<double> sum_time_insert_binary(NUM_TEST);
    vector<double> sum_time_insert_binomial(NUM_TEST);

    vector<vector<double>> time_remove_binary(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_remove_binomial(NUM_TEST, vector<double>(NUM_OPERATIONS));
    vector<double> sum_time_remove_binary(NUM_TEST);
    vector<double> sum_time_remove_binomial(NUM_TEST);

    ofstream tout("Time2.txt");

    if (tout.is_open()) {
        for (int i = 0; i < NUM_TEST; i++)
        {
            int num_el = pow(10, 3 + i);

            BinomialHeap binomial;
            BinaryHeap binary(num_el);

            for (int i = 0; i < num_el; i++)
            {
                int num = numGenerator(-num_el, num_el);
                binary.insertKey(num);
                binomial.insert(num);
            }
            
            //ПОИСК МИНИМАЛЬНОГО
            for (int j = 0; j < NUM_OPERATIONS; j++)
            {
                chrono::high_resolution_clock::time_point start_search_binary = chrono::high_resolution_clock::now();
                binary.getMin();
                chrono::high_resolution_clock::time_point end_search_binary = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_search_binary = end_search_binary - start_search_binary;
                time_search_binary[i][j] = milli_diff_search_binary.count();
                sum_time_search_binary[i] += milli_diff_search_binary.count();

                chrono::high_resolution_clock::time_point start_search_binomial = chrono::high_resolution_clock::now();
                binomial.getMin();
                chrono::high_resolution_clock::time_point end_search_binomial = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_search_binomial = end_search_binomial - start_search_binomial;
                time_search_binomial[i][j] = milli_diff_search_binomial.count();
                sum_time_search_binomial[i] += milli_diff_search_binomial.count();
            }

            //УДАЛЕНИЕ МИНИМАЛЬНОГО
            for (int j = 0; j < NUM_OPERATIONS; j++)
            {
                chrono::high_resolution_clock::time_point start_remove_binary = chrono::high_resolution_clock::now();
                binary.extractMin();
                chrono::high_resolution_clock::time_point end_remove_binary = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_remove_binary = end_remove_binary - start_remove_binary;
                time_remove_binary[i][j] = milli_diff_remove_binary.count();
                sum_time_remove_binary[i] += milli_diff_remove_binary.count();

                chrono::high_resolution_clock::time_point start_remove_binomial = chrono::high_resolution_clock::now();
                binomial.extractMin();
                chrono::high_resolution_clock::time_point end_remove_binomial = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_remove_binomial = end_remove_binomial - start_remove_binomial;
                time_remove_binomial[i][j] = milli_diff_remove_binomial.count();
                sum_time_remove_binomial[i] += milli_diff_remove_binomial.count();
            }

            //ВСТАВКА
            for (int j = 0; j < NUM_OPERATIONS; j++)
            {
                int num = numGenerator(-num_el, num_el);

                chrono::high_resolution_clock::time_point start_insert_binary = chrono::high_resolution_clock::now();
                binary.insertKey(num);
                chrono::high_resolution_clock::time_point end_insert_binary = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_binary = end_insert_binary - start_insert_binary;
                time_insert_binary[i][j] = milli_diff_insert_binary.count();
                sum_time_insert_binary[i] += milli_diff_insert_binary.count();

                chrono::high_resolution_clock::time_point start_insert_binomial = chrono::high_resolution_clock::now();
                binomial.insert(num);
                chrono::high_resolution_clock::time_point end_insert_binomial = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_binomial = end_insert_binomial - start_insert_binomial;
                time_insert_binomial[i][j] = milli_diff_insert_binomial.count();
                sum_time_insert_binomial[i] += milli_diff_insert_binomial.count();
            }


            double max_time_search_binary = time_search_binary[i][0];
            double max_time_search_binomial = time_search_binomial[i][0];
            double max_time_remove_binary = time_remove_binary[i][0];
            double max_time_remove_binomial = time_remove_binomial[i][0];
            double max_time_insert_binary = time_insert_binary[i][0];
            double max_time_insert_binomial = time_insert_binomial[i][0];

            for (int j = 0; j < NUM_OPERATIONS; j++)
            {
                if (max_time_search_binary < time_search_binary[i][j])
                    max_time_search_binary = time_search_binary[i][j];
                if (max_time_search_binomial < time_search_binomial[i][j])
                    max_time_search_binomial = time_search_binomial[i][j];
                if (max_time_remove_binary < time_remove_binary[i][j])
                    max_time_remove_binary = time_remove_binary[i][j];
                if (max_time_remove_binomial < time_remove_binomial[i][j])
                    max_time_remove_binomial = time_remove_binomial[i][j];
                if (max_time_insert_binary < time_insert_binary[i][j])
                    max_time_insert_binary = time_insert_binary[i][j];
                if (max_time_insert_binomial < time_insert_binomial[i][j])
                    max_time_insert_binomial = time_insert_binomial[i][j];
            }

            tout << "Тест " << i + 1 << endl;
            tout << "Число элементов: " << num_el << endl;
            tout << "\nВремя 1000 операций (мс):" << endl;
            tout << "Поиск минимума:" << endl;
            tout << "Бинарная: " << sum_time_search_binary[i] << endl;
            tout << "Биномиальная: " << sum_time_search_binomial[i] << endl;
            tout << "Удаление минимума:" << endl;
            tout << "Бинарная: " << sum_time_remove_binary[i] << endl;
            tout << "Биномиальная: " << sum_time_remove_binomial[i] << endl;
            tout << "Вставка:" << endl;
            tout << "Бинарная: " << sum_time_insert_binary[i] << endl;
            tout << "Биномиальная: " << sum_time_insert_binomial[i] << endl;

            tout << "\nСреднее время 1 операции (мс):" << endl;
            tout << "Поиск минимума:" << endl;
            tout << "Бинарная: " << sum_time_search_binary[i] / NUM_OPERATIONS << endl;
            tout << "Биномиальная: " << sum_time_search_binomial[i] / NUM_OPERATIONS << endl;
            tout << "Удаление минимума:" << endl;
            tout << "Бинарная: " << sum_time_remove_binary[i] / NUM_OPERATIONS << endl;
            tout << "Биномиальная: " << sum_time_remove_binomial[i] / NUM_OPERATIONS << endl;
            tout << "Вставка:" << endl;
            tout << "Бинарная: " << sum_time_insert_binary[i] / NUM_OPERATIONS << endl;
            tout << "Биномиальная: " << sum_time_insert_binomial[i] / NUM_OPERATIONS << endl;

            tout << "\nМаксимальное время 1 операции (мс):" << endl;
            tout << "Поиск минимума:" << endl;
            tout << "Бинарная: " << max_time_search_binary << endl;
            tout << "Биномиальная: " << max_time_search_binomial << endl;
            tout << "Удаление минимума:" << endl;
            tout << "Бинарная: " << max_time_remove_binary << endl;
            tout << "Биномиальная: " << max_time_remove_binomial << endl;
            tout << "Вставка:" << endl;
            tout << "Бинарная: " << max_time_insert_binary << endl;
            tout << "Биномиальная: " << max_time_insert_binomial << endl << endl;
            tout << "--------------------------------------------------------------------------" << endl << endl;

        }

        tout.close();
    }
}

int numGenerator(int min, int max) {
    return min + rand() % (max - min + 1);
}