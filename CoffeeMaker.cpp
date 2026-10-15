// CoffeeMaker.cpp — реализация модуля класса «Электронная кофеварка»
#include "CoffeeMaker.h"

// Конструктор по умолчанию: кофеварка выключена и пуста (состояние 1)
CoffeeMaker::CoffeeMaker()
{
    power = false;
    water = false;
    coffee = false;
    mode = IDLE;
}

// Конструктор с параметрами: режим всегда «Ожидание»,
// т. к. варка и подогрев возникают только в результате работы (состояния 1–8)
CoffeeMaker::CoffeeMaker(bool powerOn, bool water, bool coffee)
{
    this->power = powerOn;
    this->water = water;
    this->coffee = coffee;
    this->mode = IDLE;
}

// Кнопка питания. Выключение прерывает варку или подогрев.
bool CoffeeMaker::change()
{
    power = !power;
    if (!power)
        mode = IDLE;   // из «Варки» -> вода и кофе остаются; из «Подогрева» -> пустая
    return true;
}

// Воду можно залить, только если её нет и кофеварка не варит и не подогревает
bool CoffeeMaker::addWater()
{
    if (water || mode != IDLE)
        return false;
    water = true;
    return true;
}

bool CoffeeMaker::addCoffee()
{
    if (coffee || mode != IDLE)
        return false;
    coffee = true;
    return true;
}

// Варка начинается, только если кофеварка включена, есть вода и кофе
bool CoffeeMaker::startBrew()
{
    if (!power || !water || !coffee || mode != IDLE)
        return false;
    mode = BREWING;
    return true;
}

// Вода прошла через фильтр: вода и кофе израсходованы, включается подогрев
bool CoffeeMaker::finishBrew()
{
    if (mode != BREWING)
        return false;
    water = false;
    coffee = false;
    mode = WARMING;
    return true;
}

// Кофе налит в чашку: подогрев отключается, кофеварка ждёт
bool CoffeeMaker::pourCoffee()
{
    if (mode != WARMING)
        return false;
    mode = IDLE;
    return true;
}

bool CoffeeMaker::isOn() const { return power; }
bool CoffeeMaker::hasWater() const { return water; }
bool CoffeeMaker::hasCoffee() const { return coffee; }
CoffeeMaker::Mode CoffeeMaker::getMode() const { return mode; }

// Номер состояния по таблице состояний
int CoffeeMaker::getStateNumber() const
{
    if (mode == BREWING) return 9;
    if (mode == WARMING) return 10;
    // режим «Ожидание»: 1–4 — выключена, 5–8 — включена
    return (power ? 5 : 1) + (water ? 1 : 0) + (coffee ? 2 : 0);
}

std::string CoffeeMaker::modeName() const
{
    switch (mode)
    {
    case BREWING: return "Варка";
    case WARMING: return "Подогрев";
    default:      return "Ожидание";
    }
}

std::string CoffeeMaker::stateDescription() const
{
    switch (getStateNumber())
    {
    case 1:  return "Выключена, бачок и фильтр пусты";
    case 2:  return "Выключена, вода залита, кофе нет";
    case 3:  return "Выключена, кофе засыпан, воды нет";
    case 4:  return "Выключена, вода и кофе заправлены";
    case 5:  return "Включена, ожидание: нет ни воды, ни кофе";
    case 6:  return "Включена, ожидание: есть вода, нет кофе";
    case 7:  return "Включена, ожидание: есть кофе, нет воды";
    case 8:  return "Включена, готова к варке";
    case 9:  return "Идёт варка кофе";
    default: return "Кофе готов, включён подогрев";
    }
}
