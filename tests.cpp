// tests.cpp — тесты соответствия класса CoffeeMaker диаграмме состояний
// Для каждого из 10 состояний применяется каждое из 6 событий,
// результат сравнивается с ожидаемым состоянием по State Machine Diagram.
#include <iostream>
#include "CoffeeMaker.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Привести объект в состояние s (1..10) только через конструктор и публичные методы
CoffeeMaker make(int s)
{
    if (s <= 8)
    {
        int k = s - 1;            // 0..7: биты — вода(1), кофе(2), питание(4)
        return CoffeeMaker(k & 4, k & 1, k & 2);
    }
    CoffeeMaker cm(true, true, true);   // состояние 8
    cm.startBrew();                     // -> 9
    if (s == 10) cm.finishBrew();       // -> 10
    return cm;
}

const char *eventName[6] = {"change", "addWater", "addCoffee", "startBrew", "finishBrew", "pourCoffee"};

bool apply(CoffeeMaker &cm, int e)
{
    switch (e)
    {
    case 0: return cm.change();
    case 1: return cm.addWater();
    case 2: return cm.addCoffee();
    case 3: return cm.startBrew();
    case 4: return cm.finishBrew();
    default: return cm.pourCoffee();
    }
}

// Таблица переходов по диаграмме: expected[состояние-1][событие] — новое состояние.
// Если новое состояние совпадает со старым — событие в этом состоянии запрещено.
const int expected[10][6] = {
    //  change addWater addCoffee startBrew finishBrew pourCoffee
    {   5,     2,       3,        1,        1,         1 },  // 1
    {   6,     2,       4,        2,        2,         2 },  // 2
    {   7,     4,       3,        3,        3,         3 },  // 3
    {   8,     4,       4,        4,        4,         4 },  // 4
    {   1,     6,       7,        5,        5,         5 },  // 5
    {   2,     6,       8,        6,        6,         6 },  // 6
    {   3,     8,       7,        7,        7,         7 },  // 7
    {   4,     8,       8,        9,        8,         8 },  // 8
    {   4,     9,       9,        9,       10,         9 },  // 9
    {   1,    10,      10,       10,       10,         5 },  // 10
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    int passed = 0, failed = 0;

    // Проверка конструктора по умолчанию: начальное состояние 1
    CoffeeMaker def;
    if (def.getStateNumber() == 1) passed++;
    else { failed++; cout << "FAIL: конструктор по умолчанию\n"; }

    for (int s = 1; s <= 10; s++)
    {
        if (make(s).getStateNumber() != s)
        {
            failed++;
            cout << "FAIL: не удалось получить состояние " << s << "\n";
            continue;
        }
        for (int e = 0; e < 6; e++)
        {
            CoffeeMaker cm = make(s);
            bool ok = apply(cm, e);
            int to = cm.getStateNumber();
            int exp = expected[s - 1][e];
            bool expOk = (exp != s);   // change() всегда успешен и всегда меняет состояние
            if (to == exp && ok == expOk)
            {
                passed++;
                if (exp != s)
                    cout << "PASS  " << s << " --" << eventName[e] << "--> " << to << "\n";
            }
            else
            {
                failed++;
                cout << "FAIL  " << s << " --" << eventName[e] << "--> " << to
                     << " (ожидалось " << exp << ", результат метода " << ok << ")\n";
            }
        }
    }
    cout << "\nЗапрещённые события (состояние не меняется, метод вернул false) тоже проверены.\n";
    cout << "Итого: пройдено " << passed << ", ошибок " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
