// CoffeeMaker.h — интерфейсная часть модуля класса «Электронная кофеварка»
// Лабораторная работа S3P13, вариант 3. Исполнитель: Быстряков Р. К., ИКТС-2512
#ifndef COFFEEMAKER_H
#define COFFEEMAKER_H

#include <string>

class CoffeeMaker
{
public:
    // Режим работы кофеварки (свойство 4)
    enum Mode { IDLE = 0, BREWING = 1, WARMING = 2 };

    CoffeeMaker();                                    // конструктор по умолчанию
    CoffeeMaker(bool powerOn, bool water, bool coffee); // конструктор с параметрами

    // Методы, моделирующие поведение (меняют состояние)
    bool change();      // кнопка питания: вкл <-> выкл
    bool addWater();    // залить воду в бачок
    bool addCoffee();   // засыпать молотый кофе
    bool startBrew();   // кнопка «Старт»: начать варку
    bool finishBrew();  // событие «вода закончилась»: варка завершена
    bool pourCoffee();  // налить готовый кофе в чашку

    // Методы чтения свойств (состояние не меняют)
    bool isOn() const;
    bool hasWater() const;
    bool hasCoffee() const;
    Mode getMode() const;
    int getStateNumber() const;          // номер состояния по таблице (1..10)
    std::string modeName() const;
    std::string stateDescription() const;

private:
    bool power;   // свойство 1: питание (false — выкл, true — вкл)
    bool water;   // свойство 2: вода в бачке (false — нет, true — есть)
    bool coffee;  // свойство 3: кофе в фильтре (false — нет, true — есть)
    Mode mode;    // свойство 4: режим (ожидание, варка, подогрев)
};

#endif // COFFEEMAKER_H
