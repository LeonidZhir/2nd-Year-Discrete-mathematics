#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <functional>
#include <ctime>

using namespace std;

// Классы Поста
enum PostClass
{
    T0 = 0, // Сохраняет 0
    T1 = 1, // Сохраняет 1
    S = 2, // Самодвойственная
    M = 3, // Монотонная
    L = 4  // Линейная
};

// Функция для проверки принадлежности классу T0
bool preservesZero(const vector<int>& truthTable)
{
    return !truthTable.empty() && truthTable[0] == 0;
}

// Функция для проверки принадлежности классу T1
bool preservesOne(const vector<int>& truthTable, int vars)
{
    if (truthTable.empty()) return false;
    int lastIndex = (1 << vars) - 1;
    return lastIndex < truthTable.size() && truthTable[lastIndex] == 1;
}

// Функция для получения двойственной функции
vector<int> getDual(const vector<int>& truthTable, int vars)
{
    vector<int> dual(truthTable.size());
    int size = truthTable.size();
    for (int i = 0; i < size; i++)
    {
        int dualIndex = size - 1 - i;
        dual[i] = 1 - truthTable[dualIndex];
    }
    return dual;
}

// Функция для проверки самодвойственности
bool isSelfDual(const vector<int>& truthTable, int vars)
{
    if (truthTable.empty()) return false;
    vector<int> dual = getDual(truthTable, vars);
    return truthTable == dual;
}

// Функция для проверки монотонности
bool isMonotonic(const vector<int>& truthTable, int vars)
{
    if (truthTable.empty()) return false;
    int size = truthTable.size();

    // Создаем маску для сравнения наборов
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Проверяем, что если i "покрывает" j (все единицы j есть в i),
            // то f(j) <= f(i)
            if ((i & j) == j) { // j покрывается i
                if (truthTable[j] > truthTable[i])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// Функция для проверки линейности (представление в виде полинома Жегалкина)
bool isLinear(const vector<int>& truthTable, int vars)
{
    if (truthTable.empty()) return false;

    // Строим полином Жегалкина методом треугольника
    vector<int> coeffs = truthTable;
    int size = coeffs.size();

    // Применяем преобразование для получения коэффициентов полинома Жегалкина
    for (int step = 0; step < vars; step++)
    {
        int stepSize = 1 << step;
        for (int i = 0; i < size; i += (stepSize * 2))
        {
            for (int j = 0; j < stepSize; j++)
            {
                if (i + stepSize + j < size)
                {
                    coeffs[i + stepSize + j] ^= coeffs[i + j];
                }
            }
        }
    }

    // Проверяем, что в полиноме нет конъюнкций размера > 1
    for (int i = 0; i < size; i++)
    {
        // Если коэффициент при конъюнкции ненулевой, проверяем вес
        if (coeffs[i] != 0)
        {
            int weight = 0;
            int temp = i;
            while (temp > 0)
            {
                weight += temp & 1;
                temp >>= 1;
            }
            if (weight > 1)
            {
                return false;
            }
        }
    }
    return true;
}

// Функция для анализа одной функции
vector<bool> analyzeFunction(const vector<int>& truthTable, int vars)
{
    vector<bool> result(5, false);

    if (truthTable.empty()) return result;

    result[T0] = preservesZero(truthTable);
    result[T1] = preservesOne(truthTable, vars);
    result[S] = isSelfDual(truthTable, vars);
    result[M] = isMonotonic(truthTable, vars);
    result[L] = isLinear(truthTable, vars);

    return result;
}

// Функция для проверки полноты системы
bool isCompleteSystem(const vector<vector<bool>>& classesMatrix)
{
    if (classesMatrix.empty()) return false;

    // Система полна, если для каждого класса есть функция, ему не принадлежащая
    vector<bool> hasNonT0(5, false);

    for (const auto& funcClasses : classesMatrix)
    {
        if (funcClasses.size() != 5) continue;

        for (int i = 0; i < 5; i++)
        {
            if (!funcClasses[i])
            {
                hasNonT0[i] = true;
            }
        }
    }

    // Проверяем, что для всех 5 классов есть функции, им не принадлежащие
    for (int i = 0; i < 5; i++)
    {
        if (!hasNonT0[i])
        {
            return false;
        }
    }
    return true;
}

// Функция для проверки, является ли система базисом
bool isBasis(const vector<vector<bool>>& classesMatrix)
{
    if (classesMatrix.empty()) return false;

    if (!isCompleteSystem(classesMatrix))
    {
        return false;
    }

    // Проверяем, что система останется неполной при удалении любой функции
    for (size_t i = 0; i < classesMatrix.size(); i++)
    {
        vector<vector<bool>> reducedSystem;
        for (size_t j = 0; j < classesMatrix.size(); j++)
        {
            if (j != i)
            {
                reducedSystem.push_back(classesMatrix[j]);
            }
        }
        if (isCompleteSystem(reducedSystem))
        {
            return false; // Можно удалить функцию и система останется полной
        }
    }
    return true;
}

// Функция для ввода таблицы истинности
vector<int> inputTruthTable(int vars)
{
    int size = 1 << vars;
    vector<int> truthTable(size);

    cout << "Введите таблицу истинности (" << size << " значений 0 или 1 через пробел):" << endl;
    cout << "Наборы: ";
    for (int i = 0; i < size; i++)
    {
        // Выводим бинарное представление
        for (int j = vars - 1; j >= 0; j--)
        {
            cout << ((i >> j) & 1);
        }
        cout << " ";
    }
    cout << endl << "Значения: ";

    for (int i = 0; i < size; i++)
    {
        cin >> truthTable[i];
        if (truthTable[i] != 0 && truthTable[i] != 1)
        {
            cout << "Ошибка: введите только 0 или 1!" << endl;
            i--; // Повторяем ввод
        }
    }

    return truthTable;
}

// Функция для генерации случайной таблицы истинности
vector<int> generateRandomTruthTable(int vars)
{
    int size = 1 << vars;
    vector<int> truthTable(size);

    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < size; i++)
    {
        truthTable[i] = rand() % 2;
    }

    return truthTable;
}

// Функция для вывода матрицы принадлежности
void printClassesMatrix(const vector<vector<bool>>& classesMatrix,
    const vector<string>& functionNames)
{
    cout << "\n" << string(60, '=') << endl;
    cout << "МАТРИЦА ПРИНАДЛЕЖНОСТИ КЛАССАМ ПОСТА" << endl;

    // Заголовок таблицы
    printf("%-15s", "Функция");
    printf("%-8s", "T0");
    printf("%-8s", "T1");
    printf("%-8s", "S");
    printf("%-8s", "M");
    printf("%-8s", "L");
    cout << endl;
    cout << string(55, '-') << endl;

    // Данные таблицы
    for (size_t i = 0; i < classesMatrix.size(); i++)
    {
        printf("%-15s", functionNames[i].c_str());
        for (int j = 0; j < 5; j++)
        {
            if (j < classesMatrix[i].size()) 
            {
                printf("%-8s", classesMatrix[i][j] ? "+" : "-");
            }
            else 
            {
                printf("%-8s", "?");
            }
        }
        cout << endl;
    }
    cout << string(55, '-') << endl;

    // Легенда
    cout << "Легенда:" << endl;
    cout << "T0 - сохраняет 0, T1 - сохраняет 1, S - самодвойственная" << endl;
    cout << "M - монотонная, L - линейная" << endl;
    cout << "+ - принадлежит классу, - - не принадлежит" << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << " ИССЛЕДОВАНИЕ СИСТЕМЫ ФУНКЦИЙ НА ПОЛНОТУ " << endl;
    cout << "Анализ по 5 классам Поста: T0, T1, S, M, L" << endl;

    vector<vector<int>> functions;
    vector<int> variablesCount;
    vector<string> functionNames;
    vector<vector<bool>> classesMatrix;

    int choice;
    int funcCount = 0;

    do 
    {
        cout << "\n МЕНЮ " << endl;
        cout << "1. Добавить функцию" << endl;
        cout << "2. Сгенерировать случайную функцию" << endl;
        cout << "3. Показать текущие функции" << endl;
        cout << "4. Проанализировать систему" << endl;
        cout << "5. Очистить систему" << endl;
        cout << "6. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice)
        {
        case 1: 
        {
            int vars;
            cout << "Введите количество переменных (1-3): ";
            cin >> vars;

            if (vars < 1 || vars > 3)
            {
                cout << "Ошибка: количество переменных должно быть от 1 до 3!" << endl;
                break;
            }

            vector<int> truthTable = inputTruthTable(vars);
            functions.push_back(truthTable);
            variablesCount.push_back(vars);

            string name = "f" + to_string(++funcCount);
            functionNames.push_back(name);

            cout << "Функция " << name << " добавлена успешно!" << endl;
            break;
        }

        case 2:
        {
            int vars;
            cout << "Введите количество переменных (1-3): ";
            cin >> vars;

            if (vars < 1 || vars > 3)
            {
                cout << "Ошибка: количество переменных должно быть от 1 до 3!" << endl;
                break;
            }

            vector<int> truthTable = generateRandomTruthTable(vars);
            functions.push_back(truthTable);
            variablesCount.push_back(vars);

            string name = "f" + to_string(++funcCount);
            functionNames.push_back(name);

            cout << "Случайная функция " << name << " сгенерирована!" << endl;
            cout << "Таблица истинности: ";
            for (int val : truthTable)
            {
                cout << val << " ";
            }
            cout << endl;
            break;
        }

        case 3:
        {
            if (functions.empty())
            {
                cout << "Система функций пуста." << endl;
            }
            else
            {
                cout << "\nТекущие функции в системе:" << endl;
                for (size_t i = 0; i < functions.size(); i++)
                {
                    cout << functionNames[i] << " (";
                    cout << variablesCount[i] << " переменных): ";
                    for (int val : functions[i])
                    {
                        cout << val << " ";
                    }
                    cout << endl;
                }
            }
            break;
        }

        case 4:
        {
            if (functions.empty())
            {
                cout << "Система функций пуста. Добавьте функции для анализа." << endl;
                break;
            }

            // Анализируем все функции
            classesMatrix.clear();
            for (size_t i = 0; i < functions.size(); i++)
            {
                vector<bool> classes = analyzeFunction(functions[i], variablesCount[i]);
                classesMatrix.push_back(classes);
            }

            // Выводим матрицу принадлежности
            printClassesMatrix(classesMatrix, functionNames);

            // Проверяем полноту системы
            bool complete = isCompleteSystem(classesMatrix);
            bool basis = isBasis(classesMatrix);

            cout << "\nРЕЗУЛЬТАТЫ АНАЛИЗА:" << endl;
            cout << "Полнота системы: " << (complete ? "ДА" : "НЕТ") << endl;
            cout << "Является базисом: " << (basis ? "ДА" : "НЕТ") << endl;

            if (!complete)
            {
                cout << "\nСистема не является полной, так как отсутствуют функции," << endl;
                cout << "не принадлежащие следующим классам:" << endl;

                vector<string> classNames = { "T0", "T1", "S", "M", "L" };
                vector<bool> hasNonClass(5, false);

                for (const auto& funcClasses : classesMatrix)
                {
                    if (funcClasses.size() != 5) continue;

                    for (int i = 0; i < 5; i++)
                    {
                        if (!funcClasses[i])
                        {
                            hasNonClass[i] = true;
                        }
                    }
                }

                for (int i = 0; i < 5; i++)
                {
                    if (!hasNonClass[i])
                    {
                        cout << "- " << classNames[i] << endl;
                    }
                }
            }
            break;
        }

        case 5:
        {
            functions.clear();
            variablesCount.clear();
            functionNames.clear();
            classesMatrix.clear();
            funcCount = 0;
            cout << "Система функций очищена." << endl;
            break;
        }

        case 6:
            cout << "Выход из программы." << endl;
            break;

        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
            break;
        }

    } while (choice != 6);

    return 0;
}