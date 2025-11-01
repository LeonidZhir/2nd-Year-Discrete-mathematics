#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

// Класс для работы с матрицей бинарного отношения и анализа её свойств
class RelationMatrix
{
private:
    std::vector<std::vector<int>> matrix; // Двумерный вектор для хранения матрицы
    int size; // Размер матрицы (по умолчанию 6x6)

    // Проверка рефлексивности: все элементы главной диагонали должны быть равны 1
    bool isReflexive()
    {
        for (int i = 0; i < size; i++)
        {
            // Если найден элемент на диагонали не равный 1 - отношение не рефлексивно
            if (matrix[i][i] != 1) return false;
        }
        return true;
    }

    // Проверка антирефлексивности: все элементы главной диагонали должны быть равны 0
    bool isIrreflexive()
    {
        for (int i = 0; i < size; i++)
        {
            // Если найден элемент на диагонали не равный 0 - отношение не антирефлексивно
            if (matrix[i][i] != 0) return false;
        }
        return true;
    }

    // Проверка симметричности: матрица должна быть симметрична относительно главной диагонали
    bool isSymmetric()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Если элемент [i][j] не равен симметричному элементу [j][i] - отношение не симметрично
                if (matrix[i][j] != matrix[j][i]) return false;
            }
        }
        return true;
    }

    // Проверка асимметричности: если aRb, то не должно быть bRa (и диагональ должна быть нулевой)
    bool isAsymmetric()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Если существует связь в обе стороны - отношение не асимметрично
                if (matrix[i][j] == 1 && matrix[j][i] == 1)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Проверка антисимметричности: если aRb и bRa, то a = b
    bool isAntisymmetric()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Если для i ≠ j существуют обе связи (i,j) и (j,i) - отношение не антисимметрично
                if (i != j && matrix[i][j] == 1 && matrix[j][i] == 1)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Проверка транзитивности: если aRb и bRc, то должно выполняться aRc
    bool isTransitive()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    // Если есть связи (i,j) и (j,k), но нет связи (i,k) - отношение не транзитивно
                    if (matrix[i][j] == 1 && matrix[j][k] == 1 && matrix[i][k] != 1)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // Проверка связности: для любых двух различных элементов a и b выполняется aRb или bRa
    bool isConnected()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (i != j && matrix[i][j] == 0 && matrix[j][i] == 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Проверка эквивалентности: отношение должно быть рефлексивным, симметричным и транзитивным
    bool isEquivalence()
    {
        return isReflexive() && isSymmetric() && isTransitive();
    }

    // Проверка частичного порядка: рефлексивное, антисимметричное, транзитивное
    bool isPartialOrder()
    {
        return isReflexive() && isAntisymmetric() && isTransitive();
    }

    // Проверка строгого порядка: антирефлексивное, асимметричное, транзитивное
    bool isStrictOrder()
    {
        return isIrreflexive() && isAsymmetric() && isTransitive();
    }

    // Проверка линейного (полного) порядка: частичный порядок + связность
    bool isLinearOrder()
    {
        return isPartialOrder() && isConnected();
    }

    // Проверка строгого линейного порядка: строгий порядок + связность
    bool isStrictLinearOrder()
    {
        return isStrictOrder() && isConnected();
    }

    // Нахождение порядка отношения (количество элементов в множестве)
    int findOrder()
    {
        return size;
    }

public:
    // Конструктор класса: инициализирует матрицу заданного размера нулями
    RelationMatrix(int n = 6) : size(n)
    {
        matrix.resize(size, std::vector<int>(size, 0));
    }

    // Ввод матрицы вручную с клавиатуры
    void inputManual()
    {
        std::cout << "Введите матрицу " << size << "x" << size << " (0 или 1, разделитель - пробел):\n";
        for (int i = 0; i < size; i++)
        {
            std::cout << "Строка " << i + 1 << ": ";
            for (int j = 0; j < size; j++)
            {
                std::cin >> matrix[i][j];
                // Проверка на корректность ввода: допускаются только 0 и 1
                if (matrix[i][j] != 0 && matrix[i][j] != 1)
                {
                    std::cout << "Ошибка: вводите только 0 или 1!\n";
                    j--; // Повторяем ввод для этого элемента
                }
            }
        }
    }

    // Генерация случайной матрицы с элементами 0 и 1
    void generateRandom()
    {
        // Инициализация генератора случайных чисел
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        // Заполнение матрицы случайными значениями 0 или 1
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                matrix[i][j] = dis(gen);
            }
        }
        std::cout << "Случайная матрица сгенерирована.\n";
    }

    // Чтение матрицы из файла
    bool readFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        // Проверка успешности открытия файла
        if (!file.is_open())
        {
            std::cout << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return false;
        }

        // Чтение данных из файла
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Проверка корректности чтения данных
                if (!(file >> matrix[i][j]))
                {
                    std::cout << "Ошибка: файл содержит некорректные данные или недостаточно данных.\n";
                    return false;
                }
                // Проверка допустимых значений (только 0 и 1)
                if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                    std::cout << "Ошибка: файл содержит значения, отличные от 0 и 1.\n";
                    return false;
                }
            }
        }
        std::cout << "Матрица успешно загружена из файла " << filename << std::endl;
        return true;
    }

    // Вывод матрицы на экран
    void printMatrix()
    {
        std::cout << "\nМатрица отношения " << size << "x" << size << ":\n";
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Анализ всех свойств матрицы отношения
    void analyzeProperties()
    {
        std::cout << "\n*** АНАЛИЗ СВОЙСТВ ОТНОШЕНИЯ ***\n";

        // Проверка всех базовых свойств
        bool reflexive = isReflexive();
        bool irreflexive = isIrreflexive();
        bool symmetric = isSymmetric();
        bool asymmetric = isAsymmetric();
        bool antisymmetric = isAntisymmetric();
        bool transitive = isTransitive();
        bool connected = isConnected();
        bool equivalence = isEquivalence();
        bool partialOrder = isPartialOrder();
        bool strictOrder = isStrictOrder();
        bool linearOrder = isLinearOrder();
        bool strictLinearOrder = isStrictLinearOrder();
        int order = findOrder();

        // Вывод результатов проверки свойств
        std::cout << "Рефлексивность: " << (reflexive ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Антирефлексивность: " << (irreflexive ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Симметричность: " << (symmetric ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Асимметричность: " << (asymmetric ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Антисимметричность: " << (antisymmetric ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Транзитивность: " << (transitive ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Связность: " << (connected ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Порядок отношения: " << order << std::endl;

        // Дополнительная информация о типе отношения
        std::cout << "\n*** АНАЛИЗ ПОРЯДКА И ТИПОВ ОТНОШЕНИЙ ***\n";
        if (equivalence)
        {
            std::cout << "Отношение является отношением ЭКВИВАЛЕНТНОСТИ\n";
        }
        if (partialOrder)
        {
            std::cout << "Отношение является отношением ЧАСТИЧНОГО ПОРЯДКА\n";
        }
        if (strictOrder)
        {
            std::cout << "Отношение является отношением СТРОГОГО ПОРЯДКА\n";
        }
        if (linearOrder)
        {
            std::cout << "Отношение является отношением ЛИНЕЙНОГО (ПОЛНОГО) ПОРЯДКА\n";
        }
        if (strictLinearOrder)
        {
            std::cout << "Отношение является отношением СТРОГОГО ЛИНЕЙНОГО ПОРЯДКА\n";
        }

        // Если не определено ни одного специального типа
        if (!equivalence && !partialOrder && !strictOrder && !linearOrder && !strictLinearOrder)
        {
            std::cout << "Отношение не принадлежит к основным типам отношений порядка или эквивалентности\n";
        }

        // Анализ комбинаций свойств для порядков
        std::cout << "\n*** КОМБИНАЦИИ СВОЙСТВ ДЛЯ АНАЛИЗА ПОРЯДКА ***\n";
        std::cout << "Рефлексивное + Антисимметричное + Транзитивное: "
            << (reflexive && antisymmetric && transitive ? "ДА" : "НЕТ")
            << " (частичный порядок)\n";
        std::cout << "Антирефлексивное + Асимметричное + Транзитивное: "
            << (irreflexive && asymmetric && transitive ? "ДА" : "НЕТ")
            << " (строгий порядок)\n";
        std::cout << "Частичный порядок + Связность: "
            << (partialOrder && connected ? "ДА" : "НЕТ")
            << " (линейный порядок)\n";
        std::cout << "Строгий порядок + Связность: "
            << (strictOrder && connected ? "ДА" : "НЕТ")
            << " (строгий линейный порядок)\n";
    }
};

// Главная функция программы
int main()
{
    // Установка локали для поддержки русского языка
    setlocale(LC_ALL, "Russian");

    // Создание объекта матрицы отношения размером 6x6
    RelationMatrix relation(6);
    int choice; // Переменная для выбора пользователя

    std::cout << "*** АНАЛИЗАТОР СВОЙСТВ ОТНОШЕНИЙ ***\n";
    std::cout << "Матрица: 6x6, элементы: 0 и 1\n\n";

    // Главный цикл меню
    do {
        std::cout << "\n*** ГЛАВНОЕ МЕНЮ ***\n";
        std::cout << "1. Ввод матрицы вручную\n";
        std::cout << "2. Генерация случайной матрицы\n";
        std::cout << "3. Чтение матрицы из файла\n";
        std::cout << "4. Вывод текущей матрицы\n";
        std::cout << "5. Анализ свойств матрицы\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        // Обработка выбора пользователя
        switch (choice)
        {
        case 1:
            // Ручной ввод матрицы
            relation.inputManual();
            break;
        case 2:
            // Генерация и вывод случайной матрицы
            relation.generateRandom();
            relation.printMatrix();
            break;
        case 3:
        {
            // Чтение матрицы из файла
            std::string filename;
            std::cout << "Введите имя файла: ";
            std::cin >> filename;
            if (relation.readFromFile(filename))
            {
                relation.printMatrix();
            }
            break;
        }
        case 4:
            // Вывод текущей матрицы
            relation.printMatrix();
            break;
        case 5:
            // Анализ и вывод свойств матрицы
            relation.printMatrix();
            relation.analyzeProperties();
            break;
        case 0:
            // Выход из программы
            std::cout << "Выход из программы.\n";
            break;
        default:
            // Обработка неверного выбора
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0); // Продолжаем цикл пока пользователь не выберет выход

    return 0;
}