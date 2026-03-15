#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <climits>

using namespace std;

const int SIZE = 10;

struct Edge 
{
    int u, v;       
    int weight;     

    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}

    bool operator<(const Edge& other) const 
    {
        return weight < other.weight;
    }
};

class Graph
{
private:
    vector<vector<int>> adjMatrix;

    vector<bool> visited;
    vector<Edge> edges;                

    vector<int> parent;
    vector<int> rank;

    void makeSet(int v) 
    {
        parent[v] = v;
        rank[v] = 0;
    }

    int findSet(int v) 
    {
        if (v == parent[v])
            return v;
        return parent[v] = findSet(parent[v]);
    }

    void unionSets(int a, int b) 
    {
        a = findSet(a);
        b = findSet(b);
        if (a != b) 
        {
            if (rank[a] < rank[b])
                swap(a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                rank[a]++;
        }
    }

    // Преобразование матрицы смежности в список рёбер
    void buildEdgeList() 
    {
        edges.clear();
        for (int i = 0; i < SIZE; i++) 
        {
            for (int j = i + 1; j < SIZE; j++) 
            { // только верхний треугольник
                if (adjMatrix[i][j] > 0) 
                { // если есть ребро
                    edges.push_back(Edge(i, j, adjMatrix[i][j]));
                }
            }
        }
    }

    bool isGraphConnected() 
    {
        fill(visited.begin(), visited.end(), false);

        int start = -1;
        for (int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                if (adjMatrix[i][j] > 0) 
                {
                    start = i;
                    break;
                }
            }
            if (start != -1) break;
        }

        if (start == -1) return false;

        vector<int> queue(SIZE * SIZE);
        int front = 0, rear = 0;

        queue[rear++] = start;
        visited[start] = true;
        int visitedCount = 1;

        while (front < rear) 
        {
            int current = queue[front++];

            for (int i = 0; i < SIZE; i++) 
            {
                if (adjMatrix[current][i] > 0 && !visited[i]) 
                {
                    visited[i] = true;
                    visitedCount++;
                    queue[rear++] = i;
                }
            }
        }

        return visitedCount == SIZE;
    }

public:
    Graph() : adjMatrix(SIZE, vector<int>(SIZE, 0)),
        visited(SIZE, false),
        parent(SIZE),
        rank(SIZE) 
    {
    }

    // 1. Ручной ввод
    void inputFromKeyboard()
    {
        cout << "Введите матрицу смежности 10x10 (веса рёбер):\n";
        cout << "0 означает отсутствие ребра\n";
        for (int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {

                cin >> adjMatrix[i][j];
            }
        }
        buildEdgeList();
    }

    // 2. Чтение из файла
    bool inputFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Не удалось открыть файл!\n";
            return false;
        }

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (!(file >> adjMatrix[i][j]))
                {
                    cout << "Ошибка чтения элемента [" << i << "][" << j << "]\n";
                    return false;
                }
            }
        }

        file.close();
        buildEdgeList();
        return true;
    }

    // 3. Случайная генерация
    void generateRandom()
    {
        srand(time(0));
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = i + 1; j < SIZE; j++)
            {
                if (rand() % 100 < 40) 
                {
                    int weight = rand() % 20 + 1;
                    adjMatrix[i][j] = weight;
                    adjMatrix[j][i] = weight; 
                }
                else 
                {
                    adjMatrix[i][j] = 0;
                    adjMatrix[j][i] = 0;
                }
            }
        }
        buildEdgeList();
        cout << "Случайный граф сгенерирован.\n";
    }

    // АЛГОРИТМ ПРИМА
    void primAlgorithm() 
    {
        cout << "\n АЛГОРИТМ ПРИМА \n";

        if (!isGraphConnected()) 
        {
            cout << "Граф несвязный! Невозможно построить остовное дерево.\n";
            return;
        }

        vector<int> minEdge(SIZE, INT_MAX);  // минимальное ребро до вершины
        vector<int> selected(SIZE, -1);      // из какой вершины пришли
        vector<bool> inMST(SIZE, false);     // включена ли вершина в матрицу
        vector<Edge> mstEdges;                // рёбра остовного дерева

        minEdge[0] = 0;

        for (int i = 0; i < SIZE; i++) 
        {
            int v = -1;
            for (int j = 0; j < SIZE; j++) 
            {
                if (!inMST[j] && (v == -1 || minEdge[j] < minEdge[v])) 
                {
                    v = j;
                }
            }

            inMST[v] = true;

            if (selected[v] != -1) 
            {
                mstEdges.push_back(Edge(selected[v], v, minEdge[v]));
            }

            for (int to = 0; to < SIZE; to++) 
            {
                if (adjMatrix[v][to] > 0 && !inMST[to] && adjMatrix[v][to] < minEdge[to]) 
                {
                    minEdge[to] = adjMatrix[v][to];
                    selected[to] = v;
                }
            }
        }

        int totalWeight = 0;
        cout << "Рёбра минимального остовного дерева (алгоритм Прима):\n";
        for (const Edge& e : mstEdges) 
        {
            cout << "  " << e.u + 1 << " -- " << e.v + 1 << " : вес = " << e.weight << "\n";
            totalWeight += e.weight;
        }
        cout << "Общий вес остовного дерева: " << totalWeight << "\n";
    }

    void kruskalAlgorithm() 
    {
        cout << "\n АЛГОРИТМ КРАСКАЛА \n";

        if (!isGraphConnected()) 
        {
            cout << "Граф несвязный! Невозможно построить остовное дерево.\n";
            return;
        }

        for (int i = 0; i < SIZE; i++) 
        {
            makeSet(i);
        }

        // Сортировка рёбер по весу
        sort(edges.begin(), edges.end());

        vector<Edge> mstEdges;
        int totalWeight = 0;


        for (const Edge& e : edges) 
        {
            if (findSet(e.u) != findSet(e.v))
            {
                unionSets(e.u, e.v);
                mstEdges.push_back(e);
                totalWeight += e.weight;
            }
        }

        cout << "Рёбра минимального остовного дерева (алгоритм Краскала):\n";
        for (const Edge& e : mstEdges) 
        {
            cout << "  " << e.u + 1 << " -- " << e.v + 1 << " : вес = " << e.weight << "\n";
        }
        cout << "Общий вес остовного дерева: " << totalWeight << "\n";
    }

    void printMatrix() 
    {
        cout << "\nИсходная матрица смежности с весами:\n";
        for (int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                cout << setw(3) << adjMatrix[i][j];
                if (j < SIZE - 1) cout << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // Вывод списка всех рёбер
    void printEdges() 
    {
        cout << "Список всех рёбер:\n";
        for (const Edge& e : edges) 
        {
            cout << "  " << e.u + 1 << " -- " << e.v + 1 << " : вес = " << e.weight << "\n";
        }
        cout << "Всего рёбер: " << edges.size() << "\n\n";
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    Graph graph;

    cout << "Программа нахождения минимального остовного дерева\n";
    cout << "Алгоритмы: Прима и Краскала\n\n";

    cout << "Выберите способ ввода:\n";
    cout << "1 - Ручной ввод с клавиатуры\n";
    cout << "2 - Чтение из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        graph.inputFromKeyboard();
        break;
    case 2:
    {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;
        if (!graph.inputFromFile(filename))
        {
            cout << "Генерируем случайный граф...\n";
            graph.generateRandom();
        }
        break;
    }
    case 3:
        graph.generateRandom();
        break;
    default:
        cout << "Неверный выбор. Генерируем случайный граф...\n";
        graph.generateRandom();
    }

    graph.printMatrix();
    graph.printEdges();

    graph.primAlgorithm();
    graph.kruskalAlgorithm();

    return 0;
}