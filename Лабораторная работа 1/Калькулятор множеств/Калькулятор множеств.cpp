#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <string>
#include <sstream>
#include <clocale>

using namespace std;

class SetCalculator
{
private:
    vector<set<int>> sets;
    set<int> universe;

    // Вспомогательные функции
    void initializeUniverse()
    {
        universe.clear();
        for (int i = -50; i <= 50; i++)
        {
            universe.insert(i);
        }
    }

    set<int> createSetFromConditions(const vector<pair<int, int>>& conditions)
    {
        set<int> result;
        for (int i = -50; i <= 50; i++)
        {
            bool allConditionsMet = true;

            for (const auto& condition : conditions)
            {
                int conditionType = condition.first;
                int n = condition.second;
                bool conditionMet = false;

                switch (conditionType)
                {
                case 1: // Четность/нечетность
                    if (n == 0) conditionMet = (i % 2 == 0); // Четные
                    else conditionMet = (i % 2 != 0); // Нечетные
                    break;
                case 2: // Знак
                    if (n == 0) conditionMet = (i > 0); // Положительные
                    else if (n == 1) conditionMet = (i < 0); // Отрицательные
                    else conditionMet = (i == 0); // Ноль
                    break;
                case 3: // Кратность
                    if (n != 0) conditionMet = (i % n == 0);
                    break;
                }

                if (!conditionMet)
                {
                    allConditionsMet = false;
                    break;
                }
            }

            if (allConditionsMet)
            {
                result.insert(i);
            }
        }
        return result;
    }

    void printSet(const set<int>& s, const string& name)
    {
        cout << name << " = {";
        bool first = true;
        for (int elem : s)
        {
            if (!first) cout << ", ";
            cout << elem;
            first = false;
        }
        cout << "}" << endl;
    }

    // Самостоятельно реализованные операции над множествами

    // Объединение множеств
    set<int> setUnion(const set<int>& set1, const set<int>& set2)
    {
        set<int> result;
        // Добавляем все элементы из первого множества
        for (int elem : set1)
        {
            result.insert(elem);
        }
        // Добавляем все элементы из второго множества
        for (int elem : set2)
        {
            result.insert(elem);
        }
        return result;
    }

    // Пересечение множеств
    set<int> setIntersection(const set<int>& set1, const set<int>& set2)
    {
        set<int> result;
        // Проходим по меньшему множеству для оптимизации
        const set<int>& smaller = (set1.size() <= set2.size()) ? set1 : set2;
        const set<int>& larger = (set1.size() > set2.size()) ? set1 : set2;

        for (int elem : smaller)
        {
            if (larger.find(elem) != larger.end())
            {
                result.insert(elem);
            }
        }
        return result;
    }

    // Разность множеств (set1 - set2)
    set<int> setDifference(const set<int>& set1, const set<int>& set2)
    {
        set<int> result;
        // Добавляем элементы из set1, которых нет в set2
        for (int elem : set1)
        {
            if (set2.find(elem) == set2.end())
            {
                result.insert(elem);
            }
        }
        return result;
    }

    // Симметричная разность
    set<int> setSymmetricDifference(const set<int>& set1, const set<int>& set2)
    {
        set<int> result;

        // Элементы из set1, которых нет в set2
        for (int elem : set1)
        {
            if (set2.find(elem) == set2.end())
            {
                result.insert(elem);
            }
        }

        // Элементы из set2, которых нет в set1
        for (int elem : set2)
        {
            if (set1.find(elem) == set1.end())
            {
                result.insert(elem);
            }
        }

        return result;
    }

    // Дополнение множества (до универсума) 
    set<int> setComplement(const set<int>& set1)
    {
        set<int> result;

        // Проходим по всем элементам универсума
        for (int elem : universe)
        {
            // Если элемента нет в исходном множестве, добавляем его в результат
            if (set1.find(elem) == set1.end())
            {
                result.insert(elem);
            }
        }

        return result;
    }

    set<int> evaluateFormula(const string& formula)
    {
        // Упрощенный парсинг формул без вложенных скобок
        // Формат: A+B, A*B, A-B, A^B, !A

        if (formula.empty()) return set<int>();

        // Проверяем унарные операции
        if (formula[0] == '!')
        {
            char setChar = formula[1];
            int setIndex = setChar - 'A';
            if (setIndex >= 0 && setIndex < sets.size())
            {
                return setComplement(sets[setIndex]);
            }
        }

        // Бинарные операции
        for (size_t i = 1; i < formula.length() - 1; i++)
        {
            char op = formula[i];
            if (op == '+' || op == '*' || op == '-' || op == '^')
            {
                char leftChar = formula[i - 1];
                char rightChar = formula[i + 1];

                int leftIndex = leftChar - 'A';
                int rightIndex = rightChar - 'A';

                if (leftIndex >= 0 && leftIndex < sets.size() &&
                    rightIndex >= 0 && rightIndex < sets.size())
                {

                    set<int> result;
                    switch (op)
                    {
                    case '+': // Объединение
                        result = setUnion(sets[leftIndex], sets[rightIndex]);
                        break;
                    case '*': // Пересечение
                        result = setIntersection(sets[leftIndex], sets[rightIndex]);
                        break;
                    case '-': // Разность
                        result = setDifference(sets[leftIndex], sets[rightIndex]);
                        break;
                    case '^': // Симметричная разность
                        result = setSymmetricDifference(sets[leftIndex], sets[rightIndex]);
                        break;
                    }
                    return result;
                }
            }
        }

        return set<int>();
    }

public:
    SetCalculator()
    {
        sets.resize(3);
        initializeUniverse();
    }

    void createSets()
    {
        cout << "=== СОЗДАНИЕ МНОЖЕСТВ ===" << endl;

        for (int i = 0; i < 3; i++)
        {
            cout << "\n--- Создание множества " << char('A' + i) << " ---" << endl;
            cout << "Выберите способ создания:" << endl;
            cout << "1. Ручной ввод" << endl;
            cout << "2. Генератор случайных чисел" << endl;
            cout << "3. Заполнение по условию" << endl;

            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                manualInput(i);
                break;
            }
            case 2:
            {
                randomGeneration(i);
                break;
            }
            case 3:
            {
                conditionalInput(i);
                break;
            }
            default:
            {
                cout << "Неверный выбор, используется ручной ввод." << endl;
                manualInput(i);
                break;
            }
            }

            // Вывод созданного множества
            printSet(sets[i], string(1, 'A' + i));
        }
    }

    void manualInput(int setIndex)
    {
        cout << "Введите 10 уникальных целых чисел от -50 до 50:" << endl;
        sets[setIndex].clear();

        for (int j = 0; j < 10; j++)
        {
            int num;
            cout << "Число " << (j + 1) << ": ";
            cin >> num;

            if (num < -50 || num > 50)
            {
                cout << "Число должно быть в диапазоне [-50, 50]. Попробуйте снова." << endl;
                j--;
                continue;
            }

            if (sets[setIndex].count(num) > 0)
            {
                cout << "Это число уже есть в множестве. Попробуйте снова." << endl;
                j--;
                continue;
            }

            sets[setIndex].insert(num);
        }
    }

    void randomGeneration(int setIndex)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(-50, 50);

        sets[setIndex].clear();

        while (sets[setIndex].size() < 10)
        {
            int num = dis(gen);
            sets[setIndex].insert(num);
        }

        cout << "Множество заполнено случайными числами." << endl;
    }

    void conditionalInput(int setIndex)
    {
        vector<pair<int, int>> conditions;

        cout << "=== ВЫБОР УСЛОВИЙ ДЛЯ МНОЖЕСТВА ===" << endl;

        while (true)
        {
            cout << "\nТекущие условия: " << conditions.size() << endl;
            cout << "Выберите действие:" << endl;
            cout << "1. Добавить условие" << endl;
            cout << "2. Удалить последнее условие" << endl;
            cout << "3. Просмотреть текущие условия" << endl;
            cout << "4. Завершить выбор условий" << endl;

            int action;
            cin >> action;

            if (action == 1) 
            {
                // Добавление нового условия
                cout << "\nВыберите тип условия:" << endl;
                cout << "1. Четность/нечетность" << endl;
                cout << "2. Знак (+ или -)" << endl;
                cout << "3. Кратность числу" << endl;

                int conditionType;
                cin >> conditionType;

                int param = 0;
                if (conditionType == 1)
                {
                    cout << "Выберите: 0 - четные, 1 - нечетные: ";
                    cin >> param;
                    conditions.push_back({ conditionType, param });
                    cout << "Добавлено условие: " << (param == 0 ? "четные" : "нечетные") << " числа" << endl;
                }
                else if (conditionType == 2)
                {
                    cout << "Выберите: 0 - положительные, 1 - отрицательные, 2 - ноль: ";
                    cin >> param;
                    conditions.push_back({ conditionType, param });
                    string signDesc;
                    if (param == 0) signDesc = "положительные";
                    else if (param == 1) signDesc = "отрицательные";
                    else signDesc = "ноль";
                    cout << "Добавлено условие: " << signDesc << " числа" << endl;
                }
                else if (conditionType == 3)
                {
                    cout << "Введите число для проверки кратности: ";
                    cin >> param;
                    conditions.push_back({ conditionType, param });
                    cout << "Добавлено условие: числа, кратные " << param << endl;
                }
                else
                {
                    cout << "Неверный тип условия!" << endl;
                }
            }
            else if (action == 2) 
            {
                // Удаление последнего условия
                if (!conditions.empty())
                {
                    conditions.pop_back();
                    cout << "Последнее условие удалено." << endl;
                }
                else {
                    cout << "Нет условий для удаления." << endl;
                }
            }
            else if (action == 3) 
            {
                // Просмотр текущих условий
                cout << "\nТекущие условия:" << endl;
                if (conditions.empty()) 
                {
                    cout << "Условия не заданы." << endl;
                }
                else {
                    for (size_t i = 0; i < conditions.size(); i++) 
                    {
                        int type = conditions[i].first;
                        int param = conditions[i].second;
                        cout << i + 1 << ". ";
                        if (type == 1) 
                        {
                            cout << (param == 0 ? "Четные числа" : "Нечетные числа");
                        }
                        else if (type == 2)
                        {
                            if (param == 0) cout << "Положительные числа";
                            else if (param == 1) cout << "Отрицательные числа";
                            else cout << "Ноль";
                        }
                        else if (type == 3) 
                        {
                            cout << "Числа, кратные " << param;
                        }
                        cout << endl;
                    }
                }
            }
            else if (action == 4) 
            {
                // Завершение выбора условий
                break;
            }
            else 
            {
                cout << "Неверный выбор!" << endl;
            }
        }

        // Создание множества на основе выбранных условий
        if (conditions.empty()) {
            cout << "Условия не заданы. Используется множество по умолчанию (все четные числа)." << endl;
            conditions.push_back({ 1, 0 }); // Четные по умолчанию
        }

        sets[setIndex] = createSetFromConditions(conditions);

        // Если множество слишком большое, берем первые 10 элементов
        if (sets[setIndex].size() > 10)
        {
            set<int> limitedSet;
            auto it = sets[setIndex].begin();
            for (int i = 0; i < 10 && it != sets[setIndex].end(); i++, it++)
            {
                limitedSet.insert(*it);
            }
            sets[setIndex] = limitedSet;
            cout << "Множество ограничено 10 элементами." << endl;
        }

        cout << "Множество создано с " << conditions.size() << " условием(ями)." << endl;
    }

    void performOperations()
    {
        cout << "\n=== ОСНОВНЫЕ ОПЕРАЦИИ НАД МНОЖЕСТВАМИ ===" << endl;

        // Вывод всех множеств
        for (int i = 0; i < 3; i++)
        {
            printSet(sets[i], string(1, 'A' + i));
        }

        while (true)
        {
            cout << "\nВыберите операцию:" << endl;
            cout << "1. Объединение" << endl;
            cout << "2. Пересечение" << endl;
            cout << "3. Дополнение" << endl;
            cout << "4. Разность" << endl;
            cout << "5. Симметричная разность" << endl;
            cout << "0. Перейти к формулам" << endl;

            int choice;
            cin >> choice;

            if (choice == 0) break;

            char set1, set2;
            set<int> result;

            switch (choice)
            {
            case 1: 
            { // Объединение
                cout << "Введите два множества (например, A B): ";
                cin >> set1 >> set2;
                int idx1 = set1 - 'A';
                int idx2 = set2 - 'A';
                result = setUnion(sets[idx1], sets[idx2]);
                printSet(result, string(1, set1) + " ∪ " + string(1, set2));
                break;
            }
            case 2:
            { // Пересечение
                cout << "Введите два множества (например, A B): ";
                cin >> set1 >> set2;
                int idx1 = set1 - 'A';
                int idx2 = set2 - 'A';
                result = setIntersection(sets[idx1], sets[idx2]);
                printSet(result, string(1, set1) + " ∩ " + string(1, set2));
                break;
            }
            case 3:
            { // Дополнение
                cout << "Введите множество (например, A): ";
                cin >> set1;
                int idx = set1 - 'A';
                result = setComplement(sets[idx]);
                printSet(result, "U \\ " + string(1, set1));
                break;
            }
            case 4:
            { // Разность
                cout << "Введите два множества (например, A B): ";
                cin >> set1 >> set2;
                int idx1 = set1 - 'A';
                int idx2 = set2 - 'A';
                result = setDifference(sets[idx1], sets[idx2]);
                printSet(result, string(1, set1) + " \\ " + string(1, set2));
                break;
            }
            case 5:
            { // Симметричная разность
                cout << "Введите два множества (например, A B): ";
                cin >> set1 >> set2;
                int idx1 = set1 - 'A';
                int idx2 = set2 - 'A';
                result = setSymmetricDifference(sets[idx1], sets[idx2]);
                printSet(result, string(1, set1) + " Δ " + string(1, set2));
                break;
            }
            default:
                cout << "Неверный выбор!" << endl;
            }
        }
    }

    void evaluateFormulas()
    {
        cout << "\n=== ВЫЧИСЛЕНИЕ ФОРМУЛ ===" << endl;
        cout << "Легенда операций:" << endl;
        cout << "+  - объединение (A+B)" << endl;
        cout << "*  - пересечение (A*B)" << endl;
        cout << "-  - разность (A-B)" << endl;
        cout << "^  - симметричная разность (A^B)" << endl;
        cout << "!  - дополнение (!A)" << endl;
        cout << "Примеры формул: A+B, A*B, A-B, A^B, !A" << endl;
        cout << "Для выхода введите 'exit'" << endl;

        cin.ignore(); // Очищаем буфер

        while (true)
        {
            cout << "\nВведите формулу: ";
            string formula;
            getline(cin, formula);

            if (formula == "exit") break;

            // Удаляем пробелы из формулы
            formula.erase(remove(formula.begin(), formula.end(), ' '), formula.end());

            set<int> result = evaluateFormula(formula);
            if (!result.empty())
            {
                printSet(result, formula);
            }
            else
            {
                cout << "Ошибка: неверный формат формулы или несуществующие множества!" << endl;
            }
        }
    }

    void run()
    {
        cout << "=== КАЛЬКУЛЯТОР МНОЖЕСТВ ===" << endl;
        createSets();
        performOperations();
        evaluateFormulas();
        cout << "Программа завершена." << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "RU");
    SetCalculator calculator;
    calculator.run();
    return 0;
}