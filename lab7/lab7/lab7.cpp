#include <iostream>
#include <time.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <iomanip>
#include <fstream>    
#include <numeric>
using namespace std;

int numGenerator(int min, int max);

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


    int depth(node* t)
    {
        int ret = 0;
        if (t)
        {
            int lDepth = depth(t->left);
            int rDepth = depth(t->right);
            ret = std::max(lDepth + 1, rDepth + 1);
        }
        return ret;
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
        root = insert(x, root);
    }

    void remove(int x)
    {
        root = remove(x, root);
    }

    void display()
    {
        inorder(root);
        cout << endl;
    }

    void search(int x) {
        root = find(root, x);
    }


    int max_deep() {
        return depth(root);
    }
};


class Treap
{
private:

    // Определение узла декартового дерева
    struct Node
    {
        int x; // Хранимый ключ
        int y; // "Порядок" вершины
        int sz; // Размер поддерева (количество узлов, находящихся ниже данного)
        Node* l, * r; // Указатели на потомков данного узла

        Node(int _x)
        {
            x = _x;
            y = rand();
            sz = 1;
            l = r = nullptr;
        }
    };

    Node* root;


    void makeEmpty(Node* t)
    {
        if (t == nullptr)
            return;
        makeEmpty(t->l);
        makeEmpty(t->r);
        delete t;
    }

    // --- Основные операции - слияние (merge) и разделение (split) деревьев
    // Важно! При выполнении этих операций исходные деревья могут перестать быть
    // валидными!

    // Слияние двух деревьев (t1, t2). Работает корректно тогда и только тогда,
    // когда max(t1) < min(t2). Результат - корень нового дерева.
    Node* merge(Node* t1, Node* t2)
    {
        // Пустое дерево - то, у которого корень - nullptr.
        // Считаем, что слияние дерева с пустым деревом - это исходное дерево.
        if (t1 == nullptr) { return t2; }
        if (t2 == nullptr) { return t1; }

        // Дерево с большим значением y становится новым корнем
        if (t1->y > t2->y)
        {
            // Сливаем правое поддерево первого дерева со вторым деревом и ставим
            // результат в правое поддерево первого дерева.
            t1->r = merge(t1->r, t2);
            // Поскольку теперь дерево t1 поменялось, запускаем в нём обновление.
            update(t1);
            // Новый корень дерева - t1.
            return t1;
        }
        else
        {
            // Сливаем всё первое дерево с левым поддеревом второго дерева
            // (обязательно именно в таком порядке!!) и ставим результат в левое поддерево
            // второго дерева.
            t2->l = merge(t1, t2->l);
            // Пересчитываем значение в новом корне дерева
            update(t2);
            return t2;
        }
    }

    // Разделение дерева на два по заданному ключу. После этого в первом дереве
    // будут значения (-inf, x), во втором - [x, +inf)
    void split(Node* t, int x, Node*& t1, Node*& t2)
    {
        // Пустое дерево будет в любом случае разделено на два пустых поддерева
        if (t == nullptr)
        {
            t1 = t2 = nullptr;
            return;
        }
        // Если текущая вершина содержит значение, меньшее, чем заданное,
        // то она отправляется в первое дерево. Правое поддерево данной вершины
        // в принципе может оказаться больше или равно x, так что его тоже разрезаем,
        // и записываем первое дерево-результат на его место.
        if (t->x < x)
        {
            split(t->r, x, t->r, t2);
            t1 = t;
        }
        else
        {
            // В противном случае "режем" левое поддерево. Рассуждения остаются такими
            // же (текущая вершина отправляется во второе дерево-результат)
            split(t->l, x, t1, t->l);
            t2 = t;
        }
        // В процессе отрезания мы модифицируем дерево, поэтому для данной вершины надо
        // пересчитать хранимое выражение
        update(t);
    }

    // Безопасное получение размера поддерева
    int get_sz(Node* t)
    {
        // Размер пустых деревьев считаем равным нулю
        if (t == nullptr) { return 0; }
        // У непустых поддеревьев размер хранится в корне
        return t->sz;
    }

    // Обновление размера поддерева
    void update(Node* t)
    {
        // Размер обновляем только для непустых деревьев
        if (t != nullptr)
        {
            t->sz = 1 + get_sz(t->l) + get_sz(t->r);
        }
    }


    // --- Операции, проводимые с деревом

    // Добавление нового элемента x в дерево t
    Node* add(Node*& t, int x)
    {
        Node* t1, * t2;
        // Для добавления делаем следующее:
        // - Разрезаем исходное дерево по ключу x. В левом поддереве все элементы меньше x,
        //   в правом - больше.
        split(t, x, t1, t2);
        // - Создаём новое дерево из одной вершины - собственно, x.
        Node* new_tree = new Node(x);
        // - Производим слияние левого поддерева с новым, потом слияние результата с правым
        //   Результат слияния - новый корень дерева.
        t = merge(merge(t1, new_tree), t2);
        return t;
    }

    // Удаление вершины из дерева
    Node* remove(Node*& t, int x)
    {
        Node* t1, * t2, * t3, * t4;
        // Для удаления делаем следующее:
        // - Разрезаем исходное дерево по ключу x.
        split(t, x, t1, t2);
        // - Разрезаем правое поддерево по ключу x + 1 
        split(t2, x + 1, t3, t4);
        // - Соединяем деревья t1 и t4, которые теперь не содержат ключа x
        //   (он остался в дереве t3)
        t = merge(t1, t4);

        delete t3;
        return t;
    }

    // Вывод элементов дерева на экран в отсортированном порядке (обход ЛКП)
    void print(Node* t)
    {
        // У пустых деревьев выводить нечего
        if (t != nullptr)
        {
            // Сначала выводим всё из левого поддерева, затем то, что хранится в
            // корне, затем всё из правого поддерева
            print(t->l);
            cout << t->x << " ";
            print(t->r);
        }
    }

    void print_deep(Node* t)
    {
        // У пустых деревьев выводить нечего
        if (t != nullptr)
        {
            // Сначала выводим всё из левого поддерева, затем то, что хранится в
            // корне, затем всё из правого поддерева
            print_deep(t->l);
            cout << t->sz << " ";
            print_deep(t->r);
        }
    }

    // Получение элемента, стоящего на k-м месте в дереве (начиная с единицы!)
    int get_k(Node* t, int k)
    {
        if (k < get_sz(t->l))
        {
            return get_k(t->l, k);
        }
        else if (k == get_sz(t->l))
        {
            return t->x;
        }
        else
        {
            return get_k(t->r, k - get_sz(t->l) - 1);
        }
    }

    Node* find(Node* t, int _x) {
        if (t == nullptr)
            return nullptr;
        else if (_x < t->x)
            return find(t->l, _x);
        else if (_x > t->x)
            return find(t->r, _x);
        else
            return t;
    }

    int depth(Node* t)
    {
        int ret = 0;
        if (t)
        {
            int lDepth = depth(t->l);
            int rDepth = depth(t->r);
            ret = std::max(lDepth + 1, rDepth + 1);
        }
        return ret;
    }

public:
    Treap() {
        root = nullptr;
    };
    ~Treap() {
        makeEmpty(root);
    };

    void insert(int x)
    {
        root = add(root, x);
    }

    void remove(int x)
    {
        root = remove(root, x);
    }

    void display()
    {
        print(root);
        cout << endl;
    }

    void display_deep()
    {
        print_deep(root);
        cout << endl;
    }

    void search(int x) {
        find(root, x);
    }

    int max_deep() {
        return depth(root);
    }
};


int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Rus");

    const int NUM_TEST = 9;
    const int NUM_CYCLES = 50;
    const int NUM_OPERATIONS = 1000;

    vector<double> time_insert_treap_random(NUM_CYCLES);
    vector<double> time_insert_avl_random(NUM_CYCLES);

    //для поиска
    vector<vector<double>> time_search_treap_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_search_avl_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    //для удаления
    vector<vector<double>> time_remove_treap_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));
    vector<vector<double>> time_remove_avl_random(NUM_CYCLES, vector<double>(NUM_OPERATIONS));

    ofstream tout("Time1.txt");

    if (tout.is_open()) {

        for (int i = 0; i < NUM_TEST; i++)
        {
            int num_el = pow(2, 10 + i);

            int MIN_VALUE = -5*num_el;
            int MAX_VALUE = 5*num_el;

            double sum_time_insert_treap_random = 0;
            double sum_time_insert_avl_random = 0;

            //для глубины

            vector<int> max_deep_avl(NUM_CYCLES);
            vector<int> max_deep_treap(NUM_CYCLES);

            //для поиска
            vector<double> sum_time_search_treap_random(NUM_CYCLES);
            double sum_time_search_all_cycles_treap_random = 0;

            vector<double> sum_time_search_avl_random(NUM_CYCLES);
            double sum_time_search_all_cycles_avl_random = 0;

            //для удаления
            vector<double> sum_time_remove_treap_random(NUM_CYCLES);
            double sum_time_remove_all_cycles_treap_random = 0;

            vector<double> sum_time_remove_avl_random(NUM_CYCLES);
            double sum_time_remove_all_cycles_avl_random = 0;


            vector<int> sorted_random(num_el);

            for (int j = 0; j < NUM_CYCLES; j++)
            {

                for (int i = 0; i < num_el; i++)
                {
                    sorted_random[i] = numGenerator(MIN_VALUE, MAX_VALUE);
                }

                AVL avl_tree_random;
                Treap treap_tree_random;

                //ВСТАВКА РАНДОМНЫХ ЭЛЕМЕНТОВ

                //Treap
                chrono::high_resolution_clock::time_point start_insert_treap_random = chrono::high_resolution_clock::now();
                for (int i = 0; i < num_el; i++)
                {
                    treap_tree_random.insert(sorted_random[i]);
                }
                chrono::high_resolution_clock::time_point end_insert_treap_random = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> milli_diff_insert_treap_random = end_insert_treap_random - start_insert_treap_random;

                time_insert_treap_random[j] = milli_diff_insert_treap_random.count();
                sum_time_insert_treap_random += time_insert_treap_random[j];

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

                //ГЛУБИНА

                max_deep_avl[j] = avl_tree_random.max_deep();
                max_deep_treap[j] = treap_tree_random.max_deep();

                //ПОИСК

                for (int i = 0; i < NUM_OPERATIONS; i++)
                {
                    int rand_num = numGenerator(MIN_VALUE, MAX_VALUE);

                    //поиск в декартовом дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_search_treap_random = chrono::high_resolution_clock::now();
                    treap_tree_random.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_treap_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_treap_random = end_search_treap_random - start_search_treap_random;

                    time_search_treap_random[j][i] = milli_diff_search_treap_random.count();
                    sum_time_search_treap_random[j] += time_search_treap_random[j][i];

                    //поиск в AVL дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_search_avl_random = chrono::high_resolution_clock::now();
                    avl_tree_random.search(rand_num);
                    chrono::high_resolution_clock::time_point end_search_avl_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_search_avl_random = end_search_avl_random - start_search_avl_random;

                    time_search_avl_random[j][i] = milli_diff_search_avl_random.count();
                    sum_time_search_avl_random[j] += time_search_avl_random[j][i];

                }

                sum_time_search_all_cycles_treap_random += sum_time_search_treap_random[j];
                sum_time_search_all_cycles_avl_random += sum_time_search_avl_random[j];

                //УДАЛЕНИЕ                

                for (int i = 0; i < NUM_OPERATIONS; i++)
                {
                    int rand_num = numGenerator(MIN_VALUE, MAX_VALUE);

                    //удаление случайного элемента в декартовом дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_remove_treap_random = chrono::high_resolution_clock::now();
                    treap_tree_random.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_treap_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_treap_random = end_remove_treap_random - start_remove_treap_random;
                    time_remove_treap_random[j][i] = milli_diff_remove_treap_random.count();
                    sum_time_remove_treap_random[j] += time_remove_treap_random[j][i];

                    //удаление случайного элемента в AVL дереве, заполненном из массива с рандомными элементами
                    chrono::high_resolution_clock::time_point start_remove_avl_random = chrono::high_resolution_clock::now();
                    avl_tree_random.remove(rand_num);
                    chrono::high_resolution_clock::time_point end_remove_avl_random = chrono::high_resolution_clock::now();
                    chrono::duration<double, milli> milli_diff_remove_avl_random = end_remove_avl_random - start_remove_avl_random;
                    time_remove_avl_random[j][i] = milli_diff_remove_avl_random.count();
                    sum_time_remove_avl_random[j] += time_remove_avl_random[j][i];

                }

                sum_time_remove_all_cycles_treap_random += sum_time_remove_treap_random[j];
                sum_time_remove_all_cycles_avl_random += sum_time_remove_avl_random[j];
            }

            int max_deep_a = max_deep_avl[0];
            int max_deep_tr = max_deep_treap[0];

            for (int i = 0; i < NUM_CYCLES; i++)
            {
                if (max_deep_avl[i] > max_deep_a)
                    max_deep_a = max_deep_avl[i];

                if (max_deep_treap[i] > max_deep_tr)
                    max_deep_tr = max_deep_treap[i];

            }

            tout << "Тест " << i + 1 << endl;
            tout << "Число элементов: " << num_el << endl;
            tout << "\n<Массив случайных чисел>" << endl;
            tout << "Время операций (мс):" << endl;

            tout << "\nВставка: " << endl;
            tout << "Treap: " << sum_time_insert_treap_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_insert_avl_random / NUM_CYCLES << endl;

            tout << "\nМаксимальная глубина: " << endl;
            tout << "Treap: " << max_deep_tr << endl;
            tout << "AVL: " << max_deep_a << endl;

            tout << "\nСреднее значение Максимальной глубины: " << endl;
            tout << "Treap: " << (double)accumulate(max_deep_treap.begin(), max_deep_treap.end(), 0) / NUM_CYCLES << endl;
            tout << "AVL: " << (double)accumulate(max_deep_avl.begin(), max_deep_avl.end(), 0) / NUM_CYCLES << endl;

            tout << "\nПоиск: " << endl;
            tout << "Treap: " << sum_time_search_all_cycles_treap_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_search_all_cycles_avl_random / NUM_CYCLES << endl;

            tout << "\nУдаление: " << endl;
            tout << "Treap: " << sum_time_remove_all_cycles_treap_random / NUM_CYCLES << endl;
            tout << "AVL: " << sum_time_remove_all_cycles_avl_random / NUM_CYCLES << endl;

            tout << endl << "-----------------------------------" << endl;
        }

        tout.close();
    }

}

int numGenerator(int min, int max) {
    return min + rand() % (max - min + 1);
}