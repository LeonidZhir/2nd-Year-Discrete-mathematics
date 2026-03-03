#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

const int SIZE = 10;

class Graph
{
private:
    vector<vector<int>> adjMatrix;
    vector<vector<int>> reachMatrix;
    vector<bool> visited;
    vector<vector<int>> components;

    void bfs(int start, vector<int>& component)
    {
        vector<int> queue(SIZE * SIZE);
        int front = 0, rear = 0;

        queue[rear++] = start;
        visited[start] = true;

        while (front < rear)
        {
            int current = queue[front++];
            component.push_back(current);

            for (int i = 0; i < SIZE; i++)
            {
                if (adjMatrix[current][i] == 1 && !visited[i])
                {
                    visited[i] = true;
                    queue[rear++] = i;
                }
            }
        }
    }

    bool isDirected()
    {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (adjMatrix[i][j] != adjMatrix[j][i])
                    return true;
        return false;
    }

    vector<vector<int>> addMatrices(const vector<vector<int>>& a, const vector<vector<int>>& b)
    {
        vector<vector<int>> result(SIZE, vector<int>(SIZE, 0));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++) 
            {
                result[i][j] = a[i][j] + b[i][j];
                if (result[i][j] > 1) result[i][j] = 1;
            }
        return result;
    }

    vector<vector<int>> multiplyMatrices(const vector<vector<int>>& a, const vector<vector<int>>& b)
    {
        vector<vector<int>> result(SIZE, vector<int>(SIZE, 0));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for (int k = 0; k < SIZE; k++)
                    if (a[i][k] && b[k][j]) 
                    {
                        result[i][j] = 1;
                        break;
                    }
        return result;
    }

    bool compareMatrices(const vector<vector<int>>& a, const vector<vector<int>>& b)
    {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (a[i][j] != b[i][j])
                    return false;
        return true;
    }

    void printMatrix(const vector<vector<int>>& matrix, const string& name)
    {
        cout << name << ":\n";
        for (int i = 0; i < SIZE; i++) 
        {
            for (int j = 0; j < SIZE; j++) 
            {
                cout << matrix[i][j];
                if (j < SIZE - 1) cout << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

public:
    Graph() : adjMatrix(SIZE, vector<int>(SIZE, 0)),
        reachMatrix(SIZE, vector<int>(SIZE, 0)),
        visited(SIZE, false) {
    }

    void inputFromKeyboard()
    {
        cout << "Введите матрицу смежности 10x10:\n";
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                cin >> adjMatrix[i][j];
    }

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
        return true;
    }

    void generateRandom()
    {
        srand(time(0));
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                adjMatrix[i][j] = (i != j) ? (rand() % 2) : 0;
    }

    void findComponents()
    {
        fill(visited.begin(), visited.end(), false);
        components.clear();

        for (int i = 0; i < SIZE; i++) 
        {
            if (!visited[i]) {
                vector<int> component;
                bfs(i, component);
                components.push_back(component);
            }
        }
    }

    void findWeakComponents()
    {
        vector<vector<int>> undirectedMatrix(SIZE, vector<int>(SIZE, 0));

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                if (adjMatrix[i][j] == 1) {
                    undirectedMatrix[i][j] = 1;
                    undirectedMatrix[j][i] = 1;
                }

        vector<vector<int>> temp = adjMatrix;
        adjMatrix = undirectedMatrix;
        findComponents();
        adjMatrix = temp;
    }

    // функция вычисления матрицы достижимости
    void computeReachabilityMatrix()
    {
        vector<vector<int>> current = adjMatrix;
        vector<vector<int>> previous;
        vector<vector<int>> sum = adjMatrix;

        int maxIterations = SIZE; // Максимум SIZE итераций для графа из SIZE вершин
        int iteration = 1;

        while (iteration < maxIterations)
        {
            previous = current;
            current = multiplyMatrices(current, adjMatrix);
            sum = addMatrices(sum, current);

            if (compareMatrices(current, previous))
                break;

            iteration++;
        }

        // Добавляем единичную матрицу (достижимость самой себя)
        for (int i = 0; i < SIZE; i++)
            sum[i][i] = 1;

        reachMatrix = sum;
    }

    void printResults()
    {
        cout << "\n РЕЗУЛЬТАТЫ \n";

        printMatrix(adjMatrix, "Исходная матрица смежности");

        bool directed = isDirected();
        cout << "Граф " << (directed ? "ориентированный" : "неориентированный") << "\n\n";

        if (!directed)
        {
            findComponents();
            cout << "Количество компонент связности: " << components.size() << "\n";
            for (size_t i = 0; i < components.size(); i++) 
            {
                cout << "Компонента " << i + 1 << ": ";
                for (int v : components[i])
                    cout << v + 1 << " ";
                cout << "\n";
            }
        }
        else 
        {
            cout << "Анализ связности ориентированного графа:\n";
            cout << "\nСлабые компоненты:\n";
            findWeakComponents();
            cout << "Количество слабых компонент: " << components.size() << "\n";
            for (size_t i = 0; i < components.size(); i++) 
            {
                cout << "Слабая компонента " << i + 1 << ": ";
                for (int v : components[i])
                    cout << v + 1 << " ";
                cout << "\n";
            }
        }

        cout << "\nВычисление матрицы достижимости...\n";
        computeReachabilityMatrix();
        printMatrix(reachMatrix, "Матрица достижимости");
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    Graph graph;

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
        cout << "Случайный граф сгенерирован.\n";
        break;
    default:
        cout << "Неверный выбор. Генерируем случайный граф...\n";
        graph.generateRandom();
    }

    graph.printResults();
    return 0;
}