#include <iostream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

// Функция проверки, является ли символ гласной
bool isVowel(char ch)
{
    return (ch == 'a' || ch == 'e');
}

// Функция проверки, является ли символ согласной
bool isConsonant(char ch)
{
    return (ch == 'b' || ch == 'c' || ch == 'd');
}

// Функция проверки допустимости символа
bool isValidChar(char ch)
{
    return (ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e');
}

// Основная функция проверки принадлежности слова языку
bool belongsToLanguage(const string& word) 
{
    int n = word.length();

    if (n == 0)
    {
        return true;
    }

    for (int i = 0; i < n; i++)
    {
        if (!isValidChar(word[i]))
        {
            cout << " Ошибка: символ '" << word[i] << "' не принадлежит алфавиту {a, b, c, d, e}" << endl;
            return false;
        }
    }

    for (int i = 0; i <= n - 3; i++)
    {
        string substring = word.substr(i, 3);
        if (substring == "aca" || substring == "ece")
        {
            cout << " Ошибка: найдено запрещенное подслово '" << substring << "' на позиции " << i << endl;
            return false;
        }
    }

    for (int i = 0; i < n - 1; i++)
    {
        bool firstIsVowel = isVowel(word[i]);
        bool secondIsVowel = isVowel(word[i + 1]);

        if (firstIsVowel == secondIsVowel)
        {
            cout << " Ошибка: нарушено чередование на позициях " << i << " и " << i + 1
                << " (" << word[i] << word[i + 1] << ")" << endl;
            return false;
        }
    }

    return true;
}

void analyzeWord(const string& word)
{
    cout << "\n Анализ слова: \"" << word << "\" (длина: " << word.length() << ")" << endl;

    bool result = belongsToLanguage(word);

    if (result)
    {
        cout << " Результат: СЛОВО ПРИНАДЛЕЖИТ языку" << endl;

        cout << "  Структура: ";
        for (int i = 0; i < word.length(); i++)
        {
            char ch = word[i];
            if (isVowel(ch))
            {
                cout << ch << "(V) ";
            }
            else
            {
                cout << ch << "(C) ";
            }
        }
        cout << endl;
    }
    else
    {
        cout << " Результат: СЛОВО НЕ ПРИНАДЛЕЖИТ языку" << endl;
    }
}

void runTests()
{
    cout << " ТЕСТИРОВАНИЕ" << endl;

    cout << " ГРУППА A: ПРИНАДЛЕЖАЩИЕ СЛОВА " << endl;

    vector<string> validWords;
    validWords.push_back("");
    validWords.push_back("a");
    validWords.push_back("e");
    validWords.push_back("b");
    validWords.push_back("c");
    validWords.push_back("d");
    validWords.push_back("ab");
    validWords.push_back("ba");
    validWords.push_back("aba");
    validWords.push_back("bab");
    validWords.push_back("abab");
    validWords.push_back("ed");
    validWords.push_back("de");
    validWords.push_back("eded");
    validWords.push_back("abed");
    validWords.push_back("abac");
    validWords.push_back("ec");
    validWords.push_back("ce");
    validWords.push_back("beb");
    validWords.push_back("cad");
    validWords.push_back("abacab");
    validWords.push_back("edededed");

    for (int i = 0; i < validWords.size(); i++)
    {
        analyzeWord(validWords[i]);
    }

    cout << " ГРУППА B: НЕПРИНАДЛЕЖАЩИЕ СЛОВА " << endl;

    vector<string> invalidWords;
    invalidWords.push_back("aa");
    invalidWords.push_back("ee");
    invalidWords.push_back("bb");
    invalidWords.push_back("cc");
    invalidWords.push_back("dd");
    invalidWords.push_back("abca");
    invalidWords.push_back("abcde");
    invalidWords.push_back("acbd");
    invalidWords.push_back("aca");
    invalidWords.push_back("ece");
    invalidWords.push_back("abaca");
    invalidWords.push_back("ecece");
    invalidWords.push_back("aac");
    invalidWords.push_back("abcaa");
    invalidWords.push_back("x");
    invalidWords.push_back("abz");
    invalidWords.push_back("AcA");
    invalidWords.push_back("abacae");
    invalidWords.push_back("ecec");
    invalidWords.push_back("baca");
    invalidWords.push_back("aeca");

    vector<string> reasons;
    reasons.push_back("две гласные подряд");
    reasons.push_back("две гласные подряд");
    reasons.push_back("две согласные подряд");
    reasons.push_back("две согласные подряд");
    reasons.push_back("две согласные подряд");
    reasons.push_back("нарушение чередования (bc)");
    reasons.push_back("нарушение чередования");
    reasons.push_back("нарушение чередования");
    reasons.push_back("запрещенное подслово 'aca'");
    reasons.push_back("запрещенное подслово 'ece'");
    reasons.push_back("содержит 'aca'");
    reasons.push_back("содержит 'ece'");
    reasons.push_back("две гласные + нарушение");
    reasons.push_back("несколько нарушений");
    reasons.push_back("символ не из алфавита");
    reasons.push_back("символ не из алфавита");
    reasons.push_back("недопустимые символы (регистр)");
    reasons.push_back("содержит 'aca'");
    reasons.push_back("содержит 'ece'");
    reasons.push_back("содержит 'aca'");
    reasons.push_back("нарушение чередования (ae) и возможно 'eca'");

    for (int i = 0; i < invalidWords.size(); i++)
    {
        cout << "\n  [Причина: " << reasons[i] << "]" << endl;
        analyzeWord(invalidWords[i]);
    }
}

void interactiveMode()
{
    string choice;
    string word;

    cout << " ИНТЕРАКТИВНЫЙ РЕЖИМ" << endl;
    cout << "\n  Вводите слова для проверки (только a,b,c,d,e)" << endl;

    do {
        cout << "\n Введите слово: ";
        cin >> word;

        for (int i = 0; i < word.length(); i++)
        {
            word[i] = tolower(word[i]);
        }

        analyzeWord(word);

        cout << "\n Продолжить? (y/n): ";
        cin >> choice;

    } while (choice == "y" || choice == "Y");
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "АВТОМАТ ДЛЯ РАСПОЗНАВАНИЯ ФОРМАЛЬНОГО ЯЗЫКА" << endl;
    cout << "\n Алфавит: {a, b, c, d, e}" << endl;
    cout << " Условия языка:" << endl;
    cout << " 1. Гласные (a, e) и согласные (b, c, d) чередуются" << endl;
    cout << " 2. Отсутствуют подслова \"aca\" и \"ece\"" << endl;
    cout << " 3. Допускаются слова любой длины (включая пустое)" << endl;

    while (true)
    {
        cout << "\n" << string(60, '-') << endl;
        cout << " МЕНЮ:" << endl;
        cout << " 1. Запустить тестирование" << endl;
        cout << " 2. Интерактивный режим" << endl;
        cout << " 0. Выход" << endl;
        cout << " Ваш выбор: ";

        int mode;
        cin >> mode;

        if (mode == 0)
        {
            break;
        }

        switch (mode)
        {
        case 1:
            runTests();
            break;
        case 2:
            interactiveMode();
            break;
        default:
            cout << "  Неверный выбор. Попробуйте снова." << endl;
        }
    }

    cout << "\n  Программа завершена." << endl;
    return 0;
}