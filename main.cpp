// main.cpp — главный модуль: управление объектом «Электронная кофеварка» через меню
// Лабораторная работа S3P13, вариант 3. Исполнитель: Быстряков Р. К., ИКТС-2512
#include <iostream>
#include <limits>
#include "CoffeeMaker.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Вывод текущего состояния объекта
void showState(const CoffeeMaker &cm)
{
    cout << "--------------------------------------------\n";
    cout << " Состояние №" << cm.getStateNumber() << ": " << cm.stateDescription() << "\n";
    cout << "  Питание: " << (cm.isOn() ? "вкл" : "выкл")
         << " | Вода: " << (cm.hasWater() ? "есть" : "нет")
         << " | Кофе: " << (cm.hasCoffee() ? "есть" : "нет")
         << " | Режим: " << cm.modeName() << "\n";
    cout << "--------------------------------------------\n";
}

// Сообщение о результате действия
void report(bool ok, const char *success, const char *fail)
{
    cout << (ok ? "[OK] " : "[Нельзя] ") << (ok ? success : fail) << "\n";
}

// Ввод ответа да/нет (1/0)
bool askBool(const char *question)
{
    int v;
    while (true)
    {
        cout << question << " (1 - да, 0 - нет): ";
        if (cin >> v && (v == 0 || v == 1))
            return v == 1;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите 0 или 1.\n";
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    CoffeeMaker cm;   // создание объекта конструктором по умолчанию
    cout << "Объект создан конструктором по умолчанию.\n";
    showState(cm);

    int work = 1, command;
    while (work)
    {
        cout << "\n=== Электронная кофеварка: меню ===\n"
             << "1 - Показать состояние\n"
             << "2 - Кнопка питания (вкл/выкл)\n"
             << "3 - Залить воду\n"
             << "4 - Засыпать кофе\n"
             << "5 - Кнопка «Старт» (начать варку)\n"
             << "6 - Вода закончилась (завершить варку)\n"
             << "7 - Налить кофе в чашку\n"
             << "8 - Новая кофеварка (конструктор с параметрами)\n"
             << "0 - Выход\n"
             << "Введите номер команды -> ";
        if (!(cin >> command))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка ввода: нужно ввести число.\n";
            continue;
        }

        switch (command)
        {
        case 1:
            break;
        case 2:
        {
            bool wasBrewing = cm.getMode() == CoffeeMaker::BREWING;
            cm.change();
            if (cm.isOn())
                cout << "[OK] Кофеварка включена.\n";
            else if (wasBrewing)
                cout << "[OK] Кофеварка выключена, варка прервана.\n";
            else
                cout << "[OK] Кофеварка выключена.\n";
            break;
        }
        case 3:
            report(cm.addWater(), "Вода залита.",
                   cm.hasWater() ? "Вода уже залита." : "Во время варки или подогрева воду не доливают.");
            break;
        case 4:
            report(cm.addCoffee(), "Кофе засыпан.",
                   cm.hasCoffee() ? "Кофе уже засыпан." : "Во время варки или подогрева кофе не засыпают.");
            break;
        case 5:
            report(cm.startBrew(), "Варка началась.",
                   "Для варки кофеварка должна быть включена, в режиме ожидания, с водой и кофе.");
            break;
        case 6:
            report(cm.finishBrew(), "Варка завершена, кофе готов. Включён подогрев.",
                   "Сейчас варка не идёт.");
            break;
        case 7:
            report(cm.pourCoffee(), "Кофе налит в чашку. Приятного аппетита!",
                   "Готового кофе нет.");
            break;
        case 8:
        {
            bool p = askBool("Питание включено?");
            bool w = askBool("Вода залита?");
            bool c = askBool("Кофе засыпан?");
            cm = CoffeeMaker(p, w, c);   // создание объекта конструктором с параметрами
            cout << "[OK] Объект создан конструктором с параметрами.\n";
            break;
        }
        case 0:
            work = 0;
            cout << "Выход из программы.\n";
            continue;
        default:
            cout << "Нет такой команды.\n";
            continue;
        }
        showState(cm);
    }
    return 0;
}
