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


class BST {

    struct node {
        int data;
        node* left;
        node* right;
    };

    node* root;

    void makeEmpty(node* t)
    {
        if (t == NULL)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    node* insert(int x, node* t)
    {
        if (t == NULL)
        {
            t = new node;
            t->data = x;
            t->left = t->right = NULL;
        }
        else if (x < t->data)
            t->left = insert(x, t->left);
        else if (x > t->data)
            t->right = insert(x, t->right);
        return t;
    }

    node* findMin(node* t)
    {
        if (t == NULL)
            return NULL;
        else if (t->left == NULL)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t) {
        if (t == NULL)
            return NULL;
        else if (t->right == NULL)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(int x, node* t) {
        node* temp;
        if (t == NULL)
            return NULL;
        else if (x < t->data)
            t->left = remove(x, t->left);
        else if (x > t->data)
            t->right = remove(x, t->right);
        else if (t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        else
        {
            temp = t;
            if (t->left == NULL)
                t = t->right;
            else if (t->right == NULL)
                t = t->left;
            delete temp;
        }

        return t;
    }

    void inorder(node* t) {
        if (t == NULL)
            return;
        inorder(t->left);
        cout << t->data << " ";
        inorder(t->right);
    }

    node* find(node* t, int x) {
        if (t == NULL)
            return NULL;
        else if (x < t->data)
            return find(t->left, x);
        else if (x > t->data)
            return find(t->right, x);
        else
            return t;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        makeEmpty(root);
    }

    void insert(int x) {
        root = insert(x, root);
    }

    void remove(int x) {
        root = remove(x, root);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    void search(int x) {
        root = find(root, x);
    }
};

class AVL
{
    struct node
    {
        int data;
        node* left;
        node* right;
        int height;
    };

    node* root;

    void makeEmpty(node* t)
    {
        if (t == NULL)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    node* insert(int x, node* t)
    {
        if (t == NULL)
        {
            t = new node;
            t->data = x;
            t->height = 0;
            t->left = t->right = NULL;
        }
        else if (x < t->data)
        {
            t->left = insert(x, t->left);
            if (height(t->left) - height(t->right) == 2)
            {
                if (x < t->left->data)
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if (x > t->data)
        {
            t->right = insert(x, t->right);
            if (height(t->right) - height(t->left) == 2)
            {
                if (x > t->right->data)
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right)) + 1;
        return t;
    }

    node* singleRightRotate(node*& t)
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(u->left), t->height) + 1;
        return u;
    }

    node* singleLeftRotate(node*& t)
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(t->right), t->height) + 1;
        return u;
    }

    node* doubleLeftRotate(node*& t)
    {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    node* doubleRightRotate(node*& t)
    {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    node* findMin(node* t)
    {
        if (t == NULL)
            return NULL;
        else if (t->left == NULL)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t)
    {
        if (t == NULL)
            return NULL;
        else if (t->right == NULL)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(int x, node* t)
    {
        node* temp;

        // Element not found
        if (t == NULL)
            return NULL;
        // Searching for element
        else if (x < t->data)
            t->left = remove(x, t->left);
        else if (x > t->data)
            t->right = remove(x, t->right);
        // Element found
        // With 2 children
        else if (t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        // With one or zero child
        else
        {
            temp = t;
            if (t->left == NULL)
                t = t->right;
            else if (t->right == NULL)
                t = t->left;
            delete temp;
        }
        if (t == NULL)
            return t;

        t->height = max(height(t->left), height(t->right)) + 1;

        // If node is unbalanced
        // If left node is deleted, right case
        if (height(t->left) - height(t->right) == 2)
        {
            // right right case
            if (height(t->left->left) - height(t->left->right) == 1)
                return singleLeftRotate(t);
            // right left case
            else
                return doubleLeftRotate(t);
        }
        // If right node is deleted, left case
        else if (height(t->right) - height(t->left) == 2)
        {
            // left left case
            if (height(t->right->right) - height(t->right->left) == 1)
                return singleRightRotate(t);
            // left right case
            else
                return doubleRightRotate(t);
        }
        return t;
    }

    int height(node* t)
    {
        return (t == NULL ? -1 : t->height);
    }

    int getBalance(node* t)
    {
        if (t == NULL)
            return 0;
        else
            return height(t->left) - height(t->right);
    }

    void inorder(node* t)
    {
        if (t == NULL)
            return;
        inorder(t->left);
        cout << t->data << " ";
        inorder(t->right);
    }

    node* find(node* t, int x) {
        if (t == NULL)
            return NULL;
        else if (x < t->data)
            return find(t->left, x);
        else if (x > t->data)
            return find(t->right, x);
        else
            return t;
    }

public:
    AVL()
    {
        root = NULL;
    }

    ~AVL() {
        makeEmpty(root);
    }

    void insert(int x)
    {
        insert(x, root);
    }

    void remove(int x)
    {
        remove(x, root);
    }

    void display()
    {
        inorder(root);
        cout << endl;
    }

    void search(int x) {
        find(root, x);
    }
};


int main() {
    srand(time(0));
    setlocale(LC_ALL, "Rus");

    const int NUM_TEST = 5;
    const int NUM_CYCLES = 10;
    const int NUM_OPERATIONS = 1000;

    vector<double> time_insert_binary_random(NUM_CYCLES);
    vector<double> time_insert_binary_sort(NUM_CYCLES);
    vector<double> time_insert_avl_random(NUM_CYCLES);
    vector<double> time_insert_avl_sort(NUM_CYCLES);

    //для поиска
    vector<vector<double>> time_search_binary_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_search_binary_sort(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    vector<vector<double>> time_search_avl_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_search_avl_sort(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    vector<vector<double>> time_search_array_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_search_array_sort(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    //для удаления
    vector<vector<double>> time_remove_binary_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_remove_binary_sort(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    vector<vector<double>> time_remove_avl_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_remove_avl_sort(NUM_CYCLES, vector<double>(NUM_OPERATIONS));


    ofstream tout("Time1.txt");

    if (tout.is_open()) {

        for (int i = 0; i < NUM_TEST; i++)
        {
            int num_el = pow(2, 10 + i);

            int MIN_VALUE = 0;
            int MAX_VALUE = num_el;

            double sum_time_insert_binary_random = 0;
            double sum_time_insert_binary_sort = 0;
            double sum_time_insert_avl_random = 0;
            double sum_time_insert_avl_sort = 0;

            //для поиска
            vector<double> sum_time_search_binary_random(NUM_CYCLES);
            double sum_time_search_all_cycles_binary_random = 0;
            vector<double> sum_time_search_binary_sort(NUM_CYCLES);
            double sum_time_search_all_cycles_binary_sort = 0;

            vector<double> sum_time_search_avl_random(NUM_CYCLES);
            double sum_time_search_all_cycles_avl_random = 0;
            vector<double> sum_time_search_avl_sort(NUM_CYCLES);
            double sum_time_search_all_cycles_avl_sort = 0;

            vector<double> sum_time_search_array_random(NUM_CYCLES);
            double sum_time_search_all_cycles_array_random = 0;
            vector<double> sum_time_search_array_sort(NUM_CYCLES);
            double sum_time_search_all_cycles_array_sort = 0;

            //для удаления
            vector<double> sum_time_remove_binary_random(NUM_CYCLES);
            double sum_time_remove_all_cycles_binary_random = 0;
            vector<double> sum_time_remove_binary_sort(NUM_CYCLES);
            double sum_time_remove_all_cycles_binary_sort = 0;

            vector<double> sum_time_remove_avl_random(NUM_CYCLES);
            double sum_time_remove_all_cycles_avl_random = 0;
            vector<double> sum_time_remove_avl_sort(NUM_CYCLES);
            double sum_time_remove_all_cycles_avl_sort = 0;

            
            vector<int> sorted_default(num_el);
            vector<int> sorted_random(num_el);

            for (int i = 0; i < num_el; i++)
            {
                sorted_default[i] = i;
            }

            for (int j = 0; j < NUM_CYCLES; j++)
            {

                for (int i = 0; i < num_el; i++)
                {
                    sorted_random[i] = numGenerator(MIN_VALUE, MAX_VALUE);
                }

                BST binary_tree_random;
                BST binary_tree_sort;
                AVL avl_tree_random;
                AVL avl_tree_sort;

                //ВСТАВКА РАНДОМНЫХ ЭЛЕМЕНТОВ

                //bst
                chrono::high_resolution_clock::time_point start_insert_binary_random = chrono::high_resolution_clock::now();
                for (int i = 0; i < num_el; i++)
                {
                    binary_tree_random.insert(sorted_random[i]);
                }
                chrono::high_resolution_clock::time_point end_insert_binary_random = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_binary_random = end_insert_binary_random - start_insert_binary_random;

                time_insert_binary_random[j] = milli_diff_insert_binary_random.count();
                sum_time_insert_binary_random += time_insert_binary_random[j];

                //avl
                chrono::high_resolution_clock::time_point start_insert_avl_random = chrono::high_resolution_clock::now();
                for (int i = 0; i < num_el; i++)
                {
                    avl_tree_random.insert(sorted_random[i]);
                }
                chrono::high_resolution_clock::time_point end_insert_avl_random = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_avl_random = end_insert_avl_random - start_insert_avl_random;

                time_insert_avl_random[j] = milli_diff_insert_avl_random.count();
                sum_time_insert_avl_random += time_insert_avl_random[j];

                //ВСТАВКА ОТСОРТИРОВАННЫХ ЭЛЕМЕНТОВ

                //bst
                chrono::high_resolution_clock::time_point start_insert_binary_sort = chrono::high_resolution_clock::now();
                for (int i = 0; i < num_el; i++)
                {
                    binary_tree_sort.insert(sorted_default[i]);
                }
                chrono::high_resolution_clock::time_point end_insert_binary_sort = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_binary_sort = end_insert_binary_sort - start_insert_binary_sort;

                time_insert_binary_sort[j] = milli_diff_insert_binary_sort.count();
                sum_time_insert_binary_sort += time_insert_binary_sort[j];

                //avl
                chrono::high_resolution_clock::time_point start_insert_avl_sort = chrono::high_resolution_clock::now();
                for (int i = 0; i < num_el; i++)
                {
                    avl_tree_sort.insert(sorted_default[i]);
                }
                chrono::high_resolution_clock::time_point end_insert_avl_sort = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_avl_sort = end_insert_avl_sort - start_insert_avl_sort;

                time_insert_avl_sort[j] = milli_diff_insert_avl_sort.count();
                sum_time_insert_avl_sort += time_insert_avl_sort[j];

                //ПОИСК

                for (int i = 0; i < NUM_OPERATIONS; i++)
                {
                    int rand_num = numGenerator(MIN_VALUE, MAX_VALUE);
                    bool flag = true;

                    //поиск в бинарном дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_search_binary_random = chrono::high_resolution_clock::now();
                    binary_tree_random.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_binary_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_binary_random = end_search_binary_random - start_search_binary_random;

                    time_search_binary_random[j][i] = milli_diff_search_binary_random.count();
                    sum_time_search_binary_random[j] += time_search_binary_random[j][i];

                    //поиск в бинарном дереве, заполненном из отсортированного массива
                    chrono::high_resolution_clock::time_point start_search_binary_sort = chrono::high_resolution_clock::now();
                    binary_tree_sort.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_binary_sort = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_binary_sort = end_search_binary_sort - start_search_binary_sort;

                    time_search_binary_sort[j][i] = milli_diff_search_binary_sort.count();
                    sum_time_search_binary_sort[j] += time_search_binary_sort[j][i];

                    //поиск в AVL дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_search_avl_random = chrono::high_resolution_clock::now();
                    avl_tree_random.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_avl_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_avl_random = end_search_avl_random - start_search_avl_random;

                    time_search_avl_random[j][i] = milli_diff_search_avl_random.count();
                    sum_time_search_avl_random[j] += time_search_avl_random[j][i];

                    //поиск в AVL дереве, заполненном из отсортированного массива
                    chrono::high_resolution_clock::time_point start_search_avl_sort = chrono::high_resolution_clock::now();
                    avl_tree_sort.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_avl_sort = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_avl_sort = end_search_avl_sort - start_search_avl_sort;

                    time_search_avl_sort[j][i] = milli_diff_search_avl_sort.count();
                    sum_time_search_avl_sort[j] += time_search_avl_sort[j][i];

                    //поиск в массиве со случайными элементами
                    chrono::high_resolution_clock::time_point start_search_array_random = chrono::high_resolution_clock::now();
                    int k = 0;
                    while (flag && k < num_el) {
                        if (sorted_random[k] == rand_num)
                            flag = false;
                        k++;
                    }
                    chrono::high_resolution_clock::time_point end_search_array_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_array_random = end_search_array_random - start_search_array_random;
                    time_search_array_random[j][i] = milli_diff_search_array_random.count();
                    sum_time_search_array_random[j] += time_search_array_random[j][i];

                    //поиск в массиве c отсортированными элементами
                    chrono::high_resolution_clock::time_point start_search_array_sort = chrono::high_resolution_clock::now();
                    k = 0;
                    while (flag && k < num_el) {
                        if (sorted_default[k] == rand_num)
                            flag = false;
                        k++;
                    }
                    chrono::high_resolution_clock::time_point end_search_array_sort = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_array_sort = end_search_array_sort - start_search_array_sort;
                    time_search_array_sort[j][i] = milli_diff_search_array_sort.count();
                    sum_time_search_array_sort[j] += time_search_array_sort[j][i];

                }

                sum_time_search_all_cycles_binary_random += sum_time_search_binary_random[j];
                sum_time_search_all_cycles_binary_sort += sum_time_search_binary_sort[j];
                sum_time_search_all_cycles_avl_random += sum_time_search_avl_random[j];
                sum_time_search_all_cycles_avl_sort += sum_time_search_avl_sort[j];
                sum_time_search_all_cycles_array_random += sum_time_search_array_random[j];
                sum_time_search_all_cycles_array_sort += sum_time_search_array_sort[j];

                //УДАЛЕНИЕ

                for (int i = 0; i < NUM_OPERATIONS; i++)
                {
                    int rand_num = numGenerator(MIN_VALUE, MAX_VALUE);

                    //удаление случайного элемента в бинарном дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_remove_binary_random = chrono::high_resolution_clock::now();
                    binary_tree_random.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_binary_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_binary_random = end_remove_binary_random - start_remove_binary_random;
                    time_remove_binary_random[j][i] = milli_diff_remove_binary_random.count();
                    sum_time_remove_binary_random[j] += time_remove_binary_random[j][i];

                    //удаление случайного элемента в бинарном дереве, заполненном из отсортированного массива
                    chrono::high_resolution_clock::time_point start_remove_binary_sort = chrono::high_resolution_clock::now();
                    binary_tree_sort.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_binary_sort = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_binary_sort = end_remove_binary_sort - start_remove_binary_sort;
                    time_remove_binary_sort[j][i] = milli_diff_remove_binary_sort.count();
                    sum_time_remove_binary_sort[j] += time_remove_binary_sort[j][i];

                    //удаление случайного элемента в AVL дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_remove_avl_random = chrono::high_resolution_clock::now();
                    avl_tree_random.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_avl_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_avl_random = end_remove_avl_random - start_remove_avl_random;
                    time_remove_avl_random[j][i] = milli_diff_remove_avl_random.count();
                    sum_time_remove_avl_random[j] += time_remove_avl_random[j][i];

                    //удаление случайного элемента в AVL дереве, заполненном из отсортированного массива
                    chrono::high_resolution_clock::time_point start_remove_avl_sort = chrono::high_resolution_clock::now();
                    avl_tree_sort.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_avl_sort = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_avl_sort = end_remove_avl_sort - start_remove_avl_sort;
                    time_remove_avl_sort[j][i] = milli_diff_remove_avl_sort.count();
                    sum_time_remove_avl_sort[j] += time_remove_avl_sort[j][i];
                }

                sum_time_remove_all_cycles_binary_random += sum_time_remove_binary_random[j];
                sum_time_remove_all_cycles_binary_sort += sum_time_remove_binary_sort[j];
                sum_time_remove_all_cycles_avl_random += sum_time_remove_avl_random[j];
                sum_time_remove_all_cycles_avl_sort += sum_time_remove_avl_sort[j];
            }

            tout << "Тест " << i + 1 << endl;
            tout << "Число элементов: " << num_el << endl;
            tout << "\n<Массив случайных чисел>" << endl;
            tout << "Время операций (мс):" << endl ;

            tout << "\nВставка: " << endl;
            tout << "BST: " << sum_time_insert_binary_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_insert_avl_random / NUM_CYCLES << endl;

            tout << "\nПоиск: " << endl;
            tout << "BST: " << sum_time_search_all_cycles_binary_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_search_all_cycles_avl_random / NUM_CYCLES << endl;
            tout << "Массив: " << sum_time_search_all_cycles_array_random / NUM_CYCLES << endl;

            tout << "\nПоиск (1 операция): " << endl;
            tout << "BST: " << sum_time_search_all_cycles_binary_random / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "AVL: " << sum_time_search_all_cycles_avl_random / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "Массив: " << sum_time_search_all_cycles_array_random / NUM_CYCLES / NUM_OPERATIONS << endl;

            tout << "\nУдаление: " << endl;
            tout << "BST: " << sum_time_remove_all_cycles_binary_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_remove_all_cycles_avl_random / NUM_CYCLES << endl;

            tout << "\nУдаление (1 операция): " << endl;
            tout << "BST: " << sum_time_remove_all_cycles_binary_random / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "AVL: " << sum_time_remove_all_cycles_avl_random / NUM_CYCLES / NUM_OPERATIONS << endl;

            //////////////////////////////////////////

            tout << "\n<Массив отсортированных чисел>" << endl;
            tout << "Время операций (мс):" << endl;

            tout << "\nВставка: " << endl;
            tout << "BST: " << sum_time_insert_binary_sort / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_insert_avl_sort / NUM_CYCLES << endl;

            tout << "\nПоиск: " << endl;
            tout << "BST: " << sum_time_search_all_cycles_binary_sort / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_search_all_cycles_avl_sort / NUM_CYCLES << endl;
            tout << "Массив: " << sum_time_search_all_cycles_array_sort / NUM_CYCLES << endl;

            tout << "\nПоиск (1 операция): " << endl;
            tout << "BST: " << sum_time_search_all_cycles_binary_sort / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "AVL: " << sum_time_search_all_cycles_avl_sort / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "Массив: " << sum_time_search_all_cycles_array_sort / NUM_CYCLES / NUM_OPERATIONS << endl;

            tout << "\nУдаление: " << endl;
            tout << "BST: " << sum_time_remove_all_cycles_binary_sort / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_remove_all_cycles_avl_sort / NUM_CYCLES << endl;

            tout << "\nУдаление (1 операция): " << endl;
            tout << "BST: " << sum_time_remove_all_cycles_binary_sort / NUM_CYCLES / NUM_OPERATIONS << endl;
            tout << "AVL: " << sum_time_remove_all_cycles_avl_sort / NUM_CYCLES / NUM_OPERATIONS << endl;

            tout << endl << "-----------------------------------" << endl;
        }

        tout.close();
    }
}

int numGenerator(int min, int max) {
    return min + rand() % (max - min + 1);
}