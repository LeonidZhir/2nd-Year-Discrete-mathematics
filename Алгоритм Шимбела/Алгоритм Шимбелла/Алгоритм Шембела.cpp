#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <climits>
#include <string>
#include <cstring>

using namespace std;

const int SIZE = 6;
const long long INF = 1e18;

class ShimbelAlgorithm
{
private:
    vector<vector<int>> weightMatrix;
    int steps;
    bool findMax;

    void printMatrix(const vector<vector<long long>>& matrix, const string& type, bool showNote = false)
    {
        cout << "      ";
        for (int j = 0; j < SIZE; j++)
            cout << "   " << j + 1 << "    ";
        cout << "\n      ";
        for (int j = 0; j < SIZE; j++)
            cout << "-------";
        cout << "\n";

        for (int i = 0; i < SIZE; i++)
        {
            cout << "  " << i + 1 << "  |";
            for (int j = 0; j < SIZE; j++)
            {
                if (matrix[i][j] == INF)
                {
                    if (type == "MIN")
                        cout << "   B   ";
                    else
                        cout << "   M   ";
                }
                else if (matrix[i][j] == -INF)
                {
                    cout << "  -B   ";
                }
                else if (matrix[i][j] > 1e9 || matrix[i][j] < -1e9)
                {
                    cout << "   M   ";
                }
                else
                {
                    cout << "  " << setw(5) << matrix[i][j] << " ";
                }
            }
            cout << "\n";
        }

        // Вывод примечания только если запрошено
        if (showNote)
        {
            cout << "\nПримечание:\n";
            cout << "B - бесконечность (нет пути)\n";
            cout << "M - очень большое число (возможно переполнение или наличие положительных циклов)\n";
            cout << "-B - нет пути (для максимальных путей)\n";
        }
    }

public:
    ShimbelAlgorithm() : weightMatrix(SIZE, vector<int>(SIZE, 0)), steps(1), findMax(false)
    {
        for (int i = 0; i < SIZE; i++)
            weightMatrix[i][i] = 0;
    }

    void inputFromKeyboard()
    {
        cout << "Введите весовую матрицу 6x6:\n";
        cout << "Для обозначения отсутствия пути используйте 0\n\n";
        for (int i = 0; i < SIZE; i++)
        {
            cout << "Строка " << i + 1 << ": ";
            for (int j = 0; j < SIZE; j++)
            {
                int value;
                cin >> value;
                if (i != j && value == 0)
                    weightMatrix[i][j] = INT_MAX;
                else
                    weightMatrix[i][j] = value;
            }
        }
    }

    bool inputFromFile(const string& filename)
    {
        ifstream file(filename.c_str());
        if (!file.is_open())
        {
            cout << "Не удалось открыть файл!\n";
            return false;
        }
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int value;
                if (!(file >> value))
                {
                    cout << "Ошибка чтения элемента [" << i << "][" << j << "]\n";
                    return false;
                }
                if (i != j && value == 0)
                    weightMatrix[i][j] = INT_MAX;
                else
                    weightMatrix[i][j] = value;
            }
        }
        file.close();
        return true;
    }

    void generateRandom()
    {
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (i == j)
                    weightMatrix[i][j] = 0;
                else
                {
                    if (rand() % 100 < 60)
                        weightMatrix[i][j] = rand() % 21 - 5;
                    else
                        weightMatrix[i][j] = INT_MAX;
                }
            }
        }
        cout << "Случайная матрица весов сгенерирована.\n";
    }

    void inputParameters()
    {
        int direction;
        cout << "Введите направление поиска (1 - min, 2 - max): ";
        cin >> direction;

        while (direction != 1 && direction != 2)
        {
            cout << "Ошибка! Введите 1 (min) или 2 (max): ";
            cin >> direction;
        }

        findMax = (direction == 2);

        cout << "Введите количество переходов: ";
        cin >> steps;
        if (steps < 1) steps = 1;
    }

    void printWeightMatrix()
    {
        cout << "\nИСХОДНАЯ ВЕСОВАЯ МАТРИЦА:\n";
        cout << "      ";
        for (int j = 0; j < SIZE; j++)
            cout << "   " << j + 1 << "    ";
        cout << "\n      ";
        for (int j = 0; j < SIZE; j++)
            cout << "-------";
        cout << "\n";

        for (int i = 0; i < SIZE; i++)
        {
            cout << "  " << i + 1 << "  |";
            for (int j = 0; j < SIZE; j++)
            {
                if (weightMatrix[i][j] == INT_MAX)
                    cout << "  INF  ";
                else
                    cout << "  " << setw(5) << weightMatrix[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void shimbelAlgorithm()
    {
        vector<vector<long long>> cur(SIZE, vector<long long>(SIZE, findMax ? -INF : INF));

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (i == j)
                {
                    cur[i][j] = findMax ? -INF : INF;
                }
                else if (weightMatrix[i][j] != INT_MAX)
                {
                    cur[i][j] = weightMatrix[i][j];
                }
            }
        }

        cout << "Пути длины 1 (исходная матрица):\n";
        cout << "Шаг 1:\n";
        printMatrix(cur, findMax ? "MAX" : "MIN", false);  // без примечания

        if (steps == 1)
        {
            cout << "\nРЕЗУЛЬТАТ (пути длины 1):\n";
            cout << "Финальная матрица:\n";
            printMatrix(cur, findMax ? "MAX" : "MIN", true);  // с примечанием
            return;
        }

        vector<vector<long long>> res = cur;

        for (int step = 2; step <= steps; step++)
        {
            vector<vector<long long>> next(SIZE, vector<long long>(SIZE, findMax ? -INF : INF));

            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    for (int k = 0; k < SIZE; k++)
                    {
                        if (res[i][k] == (findMax ? -INF : INF)) continue;
                        if (cur[k][j] == (findMax ? -INF : INF)) continue;

                        long long val = res[i][k] + cur[k][j];

                        if (findMax)
                        {
                            if (val > next[i][j])
                                next[i][j] = val;
                        }
                        else
                        {
                            if (val < next[i][j])
                                next[i][j] = val;
                        }
                    }
            res = next;

            cout << "\nПути длины " << step << ":\n";
            cout << "Шаг " << step << ":\n";
            printMatrix(res, findMax ? "MAX" : "MIN", false);  // без примечания
        }

        cout << "\nРЕЗУЛЬТАТ (пути длины " << steps << "):\n";
        cout << "Финальная матрица:\n";
        printMatrix(res, findMax ? "MAX" : "MIN", true);  // с примечанием
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    ShimbelAlgorithm shimbel;

    cout << "АЛГОРИТМ ШИМБЕЛА (поиск путей заданной длины и циклов)\n";
    cout << "Выберите способ ввода весовой матрицы (6x6):\n";
    cout << "1 - Ручной ввод с клавиатуры\n";
    cout << "2 - Чтение из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        shimbel.inputFromKeyboard();
        break;
    case 2:
    {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;
        if (!shimbel.inputFromFile(filename))
        {
            cout << "Генерируем случайную матрицу...\n";
            shimbel.generateRandom();
        }
        break;
    }
    case 3:
        shimbel.generateRandom();
        break;
    default:
        cout << "Неверный выбор. Генерируем случайную матрицу...\n";
        shimbel.generateRandom();
    }

    shimbel.printWeightMatrix();
    shimbel.inputParameters();
    shimbel.shimbelAlgorithm();

    return 0;
}