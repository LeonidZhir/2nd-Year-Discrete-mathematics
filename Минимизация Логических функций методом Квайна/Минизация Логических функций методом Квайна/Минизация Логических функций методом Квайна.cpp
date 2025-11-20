#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;

// Функция для преобразования двоичного числа в буквенное представление
string binaryToLetters(const string& binary, const vector<char>& variables)
{
    string result;
    for (size_t i = 0; i < binary.length(); i++)
    {
        if (binary[i] == '0')
        {
            result += variables[i];
            result += "'";
        }
        else if (binary[i] == '1')
        {
            result += variables[i];
        }
        // '-' пропускается
    }
    return result;
}

// Функция для подсчета количества единиц в двоичной строке
int countOnes(const string& binary)
{
    int count = 0;
    for (char c : binary)
    {
        if (c == '1') count++;
    }
    return count;
}

// Функция для проверки возможности склейки двух термов (Терм - это логическое выражение, представляющее собой конъюнкцию переменных и/или их отрицаний)
bool canCombine(const string& a, const string& b)
{
    if (a.length() != b.length()) return false;

    int diffCount = 0;
    for (size_t i = 0; i < a.length(); i++)
    {
        if (a[i] != b[i])
        {
            diffCount++;
            if (diffCount > 1) return false;
        }
    }
    return diffCount == 1;
}

// Функция для склейки двух термов
string combineTerms(const string& a, const string& b)
{
    string result = a;
    for (size_t i = 0; i < a.length(); i++)
    {
        if (a[i] != b[i])
        {
            result[i] = '-';
            break;
        }
    }
    return result;
}

// Функция для проверки, покрывает ли импликанта минтерм            (Минтерм - конъюнкция ВСЕХ переменных функции)
bool covers(const string& implicant, const string& minterm)
{
    for (size_t i = 0; i < implicant.length(); i++)
    {
        if (implicant[i] != '-' && implicant[i] != minterm[i])
        {
            return false;
        }
    }
    return true;
}

// Основной класс для метода Квайна
class QuineMcCluskey
{
private:
    vector<string> minterms;
    vector<char> variables;
    int numVars;
    vector<string> primeImplicants; // Храним простые импликанты между этапами (Импликанта - это коньюнкция, которая при 1 обеспечивает значение 1 для всей функции)

public:
    QuineMcCluskey(int vars) : numVars(vars)
    {
        // Создаем имена переменных
        for (int i = 0; i < vars; i++)
        {
            variables.push_back('A' + i);
        }
    }

    // Метод для установки минтермов
    void setMinterms(const vector<string>& m)
    {
        minterms = m;
    }

    // Метод для получения текущей последовательности
    string getCurrentSequence()
    {
        string sequence(1 << numVars, '0');
        for (const string& minterm : minterms)
        {
            int index = stoi(minterm, nullptr, 2);
            sequence[index] = '1';
        }
        return sequence;
    }

    // Метод 1: Ручной ввод
    void manualInput()
    {
        cout << "Введите последовательность из " << (1 << numVars) << " символов (0 и 1): ";
        string sequence;
        cin >> sequence;

        if (sequence.length() != (1 << numVars))
        {
            cout << "Ошибка: неверная длина последовательности! Ожидается " << (1 << numVars) << " символов." << endl;
            return;
        }

        // Проверка на корректность символов
        for (char c : sequence)
        {
            if (c != '0' && c != '1')
            {
                cout << "Ошибка: последовательность должна содержать только символы '0' и '1'!" << endl;
                return;
            }
        }

        // Преобразуем последовательность в минтермы
        minterms.clear();
        for (int i = 0; i < (1 << numVars); i++)
        {
            if (sequence[i] == '1')
            {
                minterms.push_back(bitset<4>(i).to_string().substr(4 - numVars));
            }
        }

        cout << "Последовательность успешно введена: " << sequence << endl;
    }

    // Метод 2: Случайный ввод
    void randomInput() 
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        uniform_int_distribution<int> distribution(0, 1);

        string sequence;
        for (int i = 0; i < (1 << numVars); i++)
        {
            sequence += (distribution(generator) == 1) ? '1' : '0';
        }

        // Преобразуем последовательность в минтермы
        minterms.clear();
        for (int i = 0; i < (1 << numVars); i++)
        {
            if (sequence[i] == '1')
            {
                minterms.push_back(bitset<4>(i).to_string().substr(4 - numVars));
            }
        }

        cout << "Сгенерирована случайная последовательность: " << sequence << endl;
    }

    // Метод 3: Чтение из файла
    void readFromFile()
    {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;

        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Ошибка: не удалось открыть файл '" << filename << "'!" << endl;
            return;
        }

        string sequence;
        getline(file, sequence);

        // Удаляем пробелы и другие нежелательные символы
        sequence.erase(remove_if(sequence.begin(), sequence.end(),
            [](char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }),
            sequence.end());

        if (sequence.length() != (1 << numVars))
        {
            cout << "Ошибка: неверная длина последовательности в файле! Ожидается " << (1 << numVars) << " символов, получено " << sequence.length() << "." << endl;
            file.close();
            return;
        }

        // Проверка на корректность символов
        for (char c : sequence)
        {
            if (c != '0' && c != '1')
            {
                cout << "Ошибка: последовательность в файле должна содержать только символы '0' и '1'!" << endl;
                file.close();
                return;
            }
        }

        // Преобразуем последовательность в минтермы
        minterms.clear();
        for (int i = 0; i < (1 << numVars); i++)
        {
            if (sequence[i] == '1')
            {
                minterms.push_back(bitset<4>(i).to_string().substr(4 - numVars));
            }
        }

        file.close();
        cout << "Последовательность успешно загружена из файла: " << sequence << endl;
    }

    // Метод для сохранения последовательности в файл
    void saveToFile()
    {
        string filename;
        cout << "Введите имя файла для сохранения: ";
        cin >> filename;

        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "Ошибка: не удалось создать файл '" << filename << "'!" << endl;
            return;
        }

        string sequence = getCurrentSequence();
        file << sequence;
        file.close();

        cout << "Последовательность успешно сохранена в файл: " << sequence << endl;
    }

    // Вывод СДНФ и выполнение склеек
    void findPrimeImplicants()
    {
        if (minterms.empty())
        {
            cout << "Ошибка: последовательность не задана!" << endl;
            return;
        }

        cout << "\n I: ПОСТРОЕНИЕ СДНФ И СКЛЕЙКИ" << endl;

        // Вывод СДНФ
        cout << "СДНФ: ";
        vector<string> sdnfTerms;
        for (const string& minterm : minterms)
        {

            sdnfTerms.push_back(binaryToLetters(minterm, variables));
        }

        for (size_t i = 0; i < sdnfTerms.size(); i++)
        {
            cout << sdnfTerms[i];
            if (i != sdnfTerms.size() - 1)
            {
                cout << " + ";
            }
        }
        cout << endl;

        // Выполнение склеек
        vector<vector<string>> groups(numVars + 1);
        set<string> allTerms;

        // Группируем минтермы по количеству единиц
        for (const string& minterm : minterms)
        {
            int ones = countOnes(minterm);
            groups[ones].push_back(minterm);
            allTerms.insert(minterm);
        }

        cout << "\nРезультативность склеек:" << endl;
        int stage = 1;
        bool foundCombination = true;

        set<string> primeImplicantsSet;
        vector<set<string>> allStages;

        while (foundCombination)
        {
            foundCombination = false;
            set<string> used;
            set<string> newImplicants;
            vector<vector<string>> newGroups(numVars + 1);

            cout << "Склейка " << stage << ": ";

            for (int i = 0; i < numVars; i++)
            {
                for (const string& term1 : groups[i])
                {
                    for (const string& term2 : groups[i + 1])
                    {
                        if (canCombine(term1, term2)) 
                        {
                            string combined = combineTerms(term1, term2);
                            if (newImplicants.find(combined) == newImplicants.end())
                            {
                                newGroups[countOnes(combined)].push_back(combined);
                                newImplicants.insert(combined);
                                used.insert(term1);
                                used.insert(term2);
                                foundCombination = true;

                                cout << binaryToLetters(term1, variables) << " + "
                                    << binaryToLetters(term2, variables) << " = "
                                    << binaryToLetters(combined, variables) << "; ";
                            }
                        }
                    }
                }
            }

            // Добавляем непокрытые термы в простые импликанты
            for (int i = 0; i <= numVars; i++)
            {
                for (const string& term : groups[i])
                {
                    if (used.find(term) == used.end())
                    {
                        primeImplicantsSet.insert(term);
                    }
                }
            }

            allStages.push_back(newImplicants);
            groups = newGroups;
            stage++;

            if (!foundCombination)
            {
                cout << "нет склеек";
            }
            cout << endl;
        }

        // Добавляем оставшиеся термы из последней группы
        for (int i = 0; i <= numVars; i++)
        {
            for (const string& term : groups[i])
            {
                primeImplicantsSet.insert(term);
            }
        }

        vector<string> primeImplicants(primeImplicantsSet.begin(), primeImplicantsSet.end());

        cout << "\nПростые импликанты: ";
        for (size_t i = 0; i < primeImplicants.size(); i++)
        {
            cout << binaryToLetters(primeImplicants[i], variables);
            if (i != primeImplicants.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;

        // Сохраняем простые импликанты для использования в следующих этапах
        this->primeImplicants = primeImplicants;
    }

    // Построение импликантной матрицы
    void buildImplicantMatrix()
    {
        if (minterms.empty())
        {
            cout << "Ошибка: последовательность не задана!" << endl;
            return;
        }

        cout << "\n II: ИМПЛИКАНТНАЯ МАТРИЦА" << endl;

        // Используем найденные простые импликанты
        if (primeImplicants.empty())
        {
            primeImplicants = findAllPrimeImplicants();
        }

        // Подробный вывод информации о минтермах
        cout << "\nШАГ 1: Анализ минтермов функции:" << endl;
        for (size_t i = 0; i < minterms.size(); i++)
        {
            int decimal_value = bitset<4>(stoi(minterms[i], nullptr, 2)).to_ulong();
            string letter_form = binaryToLetters(minterms[i], variables);
            cout << "m" << i << " = " << minterms[i] << " (десятичный: " << decimal_value << ") = " << letter_form << endl;
        }

        // Подробный вывод информации о простых импликантах
        cout << "\nШАГ 2: Анализ простых импликант:" << endl;
        for (size_t i = 0; i < primeImplicants.size(); i++)
        {
            string letter_form = binaryToLetters(primeImplicants[i], variables);
            cout << "I" << i << " = " << primeImplicants[i] << " = " << letter_form << endl;
        }

        // Компактная матрица покрытий
        cout << "\nШАГ 3: Матрица покрытий (компактная форма):" << endl;
        cout << "X - импликанта покрывает минтерм" << endl;
        cout << "- - импликанта не покрывает минтерм" << endl;
        cout << endl;

        // Заголовок таблицы
        cout << "     ";
        for (size_t i = 0; i < minterms.size(); i++)
        {
            cout << "m" << i << "  ";
        }
        cout << endl;

        cout << "     ";
        for (size_t i = 0; i < minterms.size(); i++)
        {
            cout << "--- ";
        }
        cout << endl;

        // Заполнение матрицы
        for (size_t i = 0; i < primeImplicants.size(); i++)
        {
            cout << "I" << i << "  ";
            for (const string& minterm : minterms)
            {
                if (covers(primeImplicants[i], minterm))
                {
                    cout << " X  ";
                }
                else
                {
                    cout << " -  ";
                }
            }
            cout << endl;
        }

        // Анализ покрытий после матрицы
        cout << "\nШАГ 4: Анализ покрытий:" << endl;

        // Анализ покрытий минтермов
        cout << "Покрытия минтермов:" << endl;
        for (size_t i = 0; i < minterms.size(); i++)
        {
            cout << "m" << i << ": ";
            bool first = true;
            for (size_t j = 0; j < primeImplicants.size(); j++)
            {
                if (covers(primeImplicants[j], minterms[i]))
                {
                    if (!first) cout << ", ";
                    cout << "I" << j;
                    first = false;
                }
            }
            if (first)
            {
                cout << "нет покрытия";
            }
            cout << endl;
        }

        // Анализ существенных импликант
        cout << "\nПоиск существенных импликант:" << endl;
        vector<string> essential_implicants;

        for (size_t i = 0; i < minterms.size(); i++)
        {
            int coverCount = 0;
            size_t coveringIndex = 0;

            for (size_t j = 0; j < primeImplicants.size(); j++)
            {
                if (covers(primeImplicants[j], minterms[i]))
                {
                    coverCount++;
                    coveringIndex = j;
                }
            }

            if (coverCount == 1)
            {
                string essential_impl = primeImplicants[coveringIndex];
                if (find(essential_implicants.begin(), essential_implicants.end(), essential_impl) == essential_implicants.end())
                {
                    essential_implicants.push_back(essential_impl);
                    cout << "m" << i << " покрывается только I" << coveringIndex << " -> СУЩЕСТВЕННАЯ" << endl;
                }
            }
        }

        if (essential_implicants.empty())
        {
            cout << "Существенные импликанты не найдены" << endl;
        }

        // Сводная информация
        cout << "\nШАГ 5: Сводная информация:" << endl;
        cout << "Всего минтермов: " << minterms.size() << endl;
        cout << "Всего простых импликант: " << primeImplicants.size() << endl;
        cout << "Найдено существенных импликант: " << essential_implicants.size() << endl;

        // Проверка полноты покрытия
        set<string> covered_minterms;
        for (const string& impl : primeImplicants)
        {
            for (const string& minterm : minterms)
            {
                if (covers(impl, minterm))
                {
                    covered_minterms.insert(minterm);
                }
            }
        }

        if (covered_minterms.size() == minterms.size())
        {
            cout << "Все минтермы покрыты" << endl;
        }
        else
        {
            cout << "Покрыто " << covered_minterms.size() << " из " << minterms.size() << " минтермов" << endl;
        }
    }

    // Нахождение МДНФ
    void findMDNF()
    {
        if (minterms.empty())
        {
            cout << "Ошибка: последовательность не задана!" << endl;
            return;
        }

        cout << "\n III: ВЫЧИСЛЕНИЕ МДНФ" << endl;

        // Используем найденные простые импликанты
        if (primeImplicants.empty())
        {
            primeImplicants = findAllPrimeImplicants();
        }

        vector<string> essentialImplicants;

        // Находим существенные импликанты
        for (const string& minterm : minterms)
        {
            int coverCount = 0;
            string coveringImplicant;

            for (const string& implicant : primeImplicants)
            {
                if (covers(implicant, minterm))
                {
                    coverCount++;
                    coveringImplicant = implicant;
                }
            }

            if (coverCount == 1)
            {
                // Это существенная импликанта
                if (find(essentialImplicants.begin(), essentialImplicants.end(), coveringImplicant) == essentialImplicants.end())
                {
                    essentialImplicants.push_back(coveringImplicant);
                }
            }
        }

        cout << "Существенные импликанты: ";
        for (size_t i = 0; i < essentialImplicants.size(); i++)
        {
            cout << binaryToLetters(essentialImplicants[i], variables);
            if (i != essentialImplicants.size() - 1)
            {
                cout << " + ";
            }
        }
        cout << endl;

        // Удаляем покрытые минтермы
        vector<string> uncoveredMinterms;
        for (const string& minterm : minterms)
        {
            bool covered = false;
            for (const string& implicant : essentialImplicants)
            {
                if (covers(implicant, minterm))
                {
                    covered = true;
                    break;
                }
            }
            if (!covered)
            {
                uncoveredMinterms.push_back(minterm);
            }
        }

        // Алгоритм выбора дополнительных импликант
        vector<string> additionalImplicants;
        vector<string> remainingImplicants;

        // Собираем оставшиеся импликанты
        for (const string& implicant : primeImplicants)
        {
            if (find(essentialImplicants.begin(), essentialImplicants.end(), implicant) == essentialImplicants.end())
            {
                remainingImplicants.push_back(implicant);
            }
        }

        // Алгоритм выбора импликант с максимальным покрытием
        while (!uncoveredMinterms.empty())
        {
            string bestImplicant;
            int maxCoverage = 0;

            for (const string& implicant : remainingImplicants)
            {
                int coverage = 0;
                for (const string& minterm : uncoveredMinterms)
                {
                    if (covers(implicant, minterm))
                    {
                        coverage++;
                    }
                }
                if (coverage > maxCoverage)
                {
                    maxCoverage = coverage;
                    bestImplicant = implicant;
                }
            }

            if (!bestImplicant.empty())
            {
                additionalImplicants.push_back(bestImplicant);
                // Удаляем покрытые минтермы
                vector<string> newUncovered;
                for (const string& minterm : uncoveredMinterms)
                {
                    if (!covers(bestImplicant, minterm))
                    {
                        newUncovered.push_back(minterm);
                    }
                }
                uncoveredMinterms = newUncovered;
                // Удаляем выбранную импликанту из оставшихся
                remainingImplicants.erase(
                    remove(remainingImplicants.begin(), remainingImplicants.end(), bestImplicant),
                    remainingImplicants.end()
                );
            }
            else
            {
                break;
            }
        }

        // Формируем МДНФ
        vector<string> mdnf = essentialImplicants;
        mdnf.insert(mdnf.end(), additionalImplicants.begin(), additionalImplicants.end());

        cout << "Минимальная ДНФ (МДНФ): ";
        for (size_t i = 0; i < mdnf.size(); i++)
        {
            cout << binaryToLetters(mdnf[i], variables);
            if (i != mdnf.size() - 1)
            {
                cout << " + ";
            }
        }
        cout << endl;
    }

    // Метод для отображения текущего состояния
    void showCurrentState()
    {
        if (minterms.empty())
        {
            cout << "Последовательность не задана." << endl;
            return;
        }

        string sequence = getCurrentSequence();
        cout << "Текущая последовательность: " << sequence << endl;
        cout << "Количество минтермов: " << minterms.size() << endl;
    }

private:
    // Вспомогательная функция для нахождения всех простых импликант
    vector<string> findAllPrimeImplicants()
    {
        set<string> primeSet;
        vector<vector<string>> groups(numVars + 1);

        // Группируем минтермы по количеству единиц
        for (const string& minterm : minterms)
        {
            int ones = countOnes(minterm);
            groups[ones].push_back(minterm);
        }

        bool foundCombination = true;

        while (foundCombination)
        {
            foundCombination = false;
            set<string> used;
            vector<vector<string>> newGroups(numVars + 1);

            for (int i = 0; i < numVars; i++)
            {
                for (const string& term1 : groups[i])
                {
                    for (const string& term2 : groups[i + 1])
                    {
                        if (canCombine(term1, term2))
                        {
                            string combined = combineTerms(term1, term2);
                            newGroups[countOnes(combined)].push_back(combined);
                            used.insert(term1);
                            used.insert(term2);
                            foundCombination = true;
                        }
                    }
                }
            }

            // Добавляем непокрытые термы в простые импликанты
            for (int i = 0; i <= numVars; i++)
            {
                for (const string& term : groups[i])
                {
                    if (used.find(term) == used.end())
                    {
                        primeSet.insert(term);
                    }
                }
            }

            groups = newGroups;
        }

        // Добавляем оставшиеся термы
        for (int i = 0; i <= numVars; i++)
        {
            for (const string& term : groups[i])
            {
                primeSet.insert(term);
            }
        }

        return vector<string>(primeSet.begin(), primeSet.end());
    }
};

// Функция для отображения меню
void showMenu()
{
    cout << "\n МЕНЮ" << endl;
    cout << "1. Ручной ввод последовательности" << endl;
    cout << "2. Случайная генерация последовательности" << endl;
    cout << "3. Чтение последовательности из файла" << endl;
    cout << "4. Сохранение последовательности в файл" << endl;
    cout << "5. Показать текущую последовательность" << endl;
    cout << "6. Выполнить минимизацию (все этапы)" << endl;
    cout << "7. Выход" << endl;
    cout << "Выберите действие: ";
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "МИНИМИЗАЦИЯ ЛОГИЧЕСКИХ ФУНКЦИЙ МЕТОДОМ КВАЙНА" << endl;

    // Создаем объект для 4 переменных
    QuineMcCluskey qm(4);
    int choice;

    do
    {
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            qm.manualInput();
            break;
        case 2:
            qm.randomInput();
            break;
        case 3:
            qm.readFromFile();
            break;
        case 4:
            qm.saveToFile();
            break;
        case 5:
            qm.showCurrentState();
            break;
        case 6:
            qm.findPrimeImplicants();
            qm.buildImplicantMatrix();
            qm.findMDNF();
            break;
        case 7:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
            break;
        }
    } while (choice != 7);

    return 0;
}