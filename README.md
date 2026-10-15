# ЛР №5 (S3P13) — Моделирование устройств с помощью класса

Вариант 3: **«Электронная кофеварка»**. Исполнитель: Быстряков Р. К., ИКТС-2512.

## Модель
Свойства (private): `power` (питание, bool), `water` (вода, bool), `coffee` (кофе, bool), `mode` (Ожидание / Варка / Подогрев).
Методы-события (public): `change()`, `addWater()`, `addCoffee()`, `startBrew()`, `finishBrew()`, `pourCoffee()`.
10 допустимых состояний, 21 переход — см. `docs/state_diagram.png`.

## Файлы
| Файл | Назначение |
|---|---|
| `CoffeeMaker.h` | интерфейс модуля класса |
| `CoffeeMaker.cpp` | реализация методов |
| `main.cpp` | консольное приложение, меню в цикле |
| `tests.cpp` | тесты соответствия диаграмме состояний |
| `coffee.exe`, `tests.exe` | готовые сборки для Windows (статические) |
| `docs/` | диаграммы состояний и компонентов, логи запуска |

## Сборка и запуск
```
g++ -std=c++17 -o coffee.exe main.cpp CoffeeMaker.cpp
g++ -std=c++17 -o tests.exe tests.cpp CoffeeMaker.cpp
coffee.exe
tests.exe
```
