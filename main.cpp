#include <iostream>
#include "Keeper.h"
#include "Car.h"
#include "Motorcycle.h"
#include "Bus.h"
#include "Utils.h"
#include "Exceptions.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

using namespace std;

static void printMenu() {
    cout << "\n===== ГАРАЖ =====\n"
            " 1 - Добавить транспорт\n"
            " 2 - Показать весь гараж\n"
            " 3 - Показать транспорт по типу\n"
            " 4 - Изменить объект\n"
            " 5 - Удалить объект\n"
            " 6 - Клонировать объект (конструктор копирования)\n"
            " 7 - Скопировать весь гараж (конструктор копирования Keeper)\n"
            " 8 - Сохранить гараж в файл\n"
            " 9 - Загрузить гараж из файла\n"
            "10 - Статистика\n"
            " 0 - Выход\n";
}

static int chooseType() {
    cout << "Тип транспорта:\n 1 - Автомобиль\n 2 - Мотоцикл\n 3 - Автобус\n";
    return readInt(cin, "Выбор: ", 1, 3);
}

// Создание объекта конструктором по умолчанию (поля вводятся потом через >>)
static Base* createDefault(int type) {
    switch (type) {
    case TYPE_CAR: return new Car();
    case TYPE_MOTORCYCLE: return new Motorcycle();
    default: return new Bus();
    }
}

// Ввод всех параметров с клавиатуры и создание объекта конструктором с параметрами
static Base* createWithParams(int type) {
    char brand[BUF_SIZE], model[BUF_SIZE], text[BUF_SIZE], text2[BUF_SIZE];
    readLine(cin, "Марка: ", brand, BUF_SIZE);
    readLine(cin, "Модель: ", model, BUF_SIZE);
    switch (type) {
    case TYPE_CAR: {
        double volume = readDouble(cin, "Объём двигателя (л): ", 0, 100);
        readLine(cin, "Цвет: ", text, BUF_SIZE);
        readLine(cin, "Тип КПП: ", text2, BUF_SIZE);
        return new Car(brand, model, volume, text, text2);
    }
    case TYPE_MOTORCYCLE: {
        double volume = readDouble(cin, "Объём двигателя (л): ", 0, 100);
        double power = readDouble(cin, "Мощность двигателя (л.с.): ", 0, 5000);
        readLine(cin, "Местность (дорога, бездорожье и т.п.): ", text, BUF_SIZE);
        return new Motorcycle(brand, model, volume, power, text);
    }
    default: {
        int seats = readInt(cin, "Количество сидячих мест: ", 1, 1000);
        int total = readInt(cin, "Общее количество мест: ", 1, 1000);
        readLine(cin, "Конечный пункт: ", text, BUF_SIZE);
        return new Bus(brand, model, seats, total, text);
    }
    }
}

static void addVehicle(Keeper& garage) {
    int type = chooseType();
    cout << "Способ создания:\n 1 - конструктор по умолчанию + ввод полей (оператор >>)\n"
            " 2 - конструктор с параметрами\n";
    int mode = readInt(cin, "Выбор: ", 1, 2);

    Base* obj = nullptr;
    if (mode == 1) {
        obj = createDefault(type);
        try {
            cin >> *obj;
        } catch (...) {
            delete obj;
            throw;
        }
    } else {
        obj = createWithParams(type);
    }
    try {
        garage += obj;
    } catch (...) {
        delete obj;
        throw;
    }
    cout << "Объект добавлен.\n";
}

static int selectIndex(const Keeper& garage) {
    if (garage.getCount() == 0) throw EmptyGarageException();
    garage.showAll(cout);
    return readInt(cin, "Номер объекта: ", 1, garage.getCount()) - 1;
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    Keeper garage;
    bool running = true;

    while (running) {
        printMenu();
        try {
            int choice = readInt(cin, "Ваш выбор: ", 0, 10);
            switch (choice) {
            case 1:
                addVehicle(garage);
                break;
            case 2:
                cout << garage;
                break;
            case 3: {
                int type = chooseType();
                garage.showByType(type, cout);
                break;
            }
            case 4: {
                int idx = selectIndex(garage);
                garage[idx].edit();
                cout << "Текущее состояние: " << garage[idx] << '\n';
                break;
            }
            case 5: {
                int idx = selectIndex(garage);
                garage.remove(idx);
                cout << "Объект удалён.\n";
                break;
            }
            case 6: {
                int idx = selectIndex(garage);
                Base* copy = garage[idx].clone();
                try {
                    garage += copy;
                } catch (...) {
                    delete copy;
                    throw;
                }
                cout << "Копия добавлена в конец гаража.\n";
                break;
            }
            case 7: {
                Keeper copy(garage);
                cout << "Копия гаража (объектов: " << copy.getCount() << "):\n" << copy;
                cout << "Копия будет удалена при выходе из этого пункта меню.\n";
                break;
            }
            case 8: {
                char name[BUF_SIZE];
                readLine(cin, "Имя файла для сохранения: ", name, BUF_SIZE);
                garage.saveToFile(name);
                cout << "Гараж сохранён.\n";
                break;
            }
            case 9: {
                char name[BUF_SIZE];
                readLine(cin, "Имя файла для загрузки: ", name, BUF_SIZE);
                garage.loadFromFile(name);
                cout << "Гараж загружен, объектов: " << garage.getCount() << '\n';
                break;
            }
            case 10:
                cout << "Всего объектов: " << garage.getCount()
                     << " (вместимость массива: " << garage.getCapacity() << ")\n"
                     << "Автомобилей: " << garage.countByType(TYPE_CAR) << '\n'
                     << "Мотоциклов: " << garage.countByType(TYPE_MOTORCYCLE) << '\n'
                     << "Автобусов: " << garage.countByType(TYPE_BUS) << '\n';
                break;
            case 0:
                running = false;
                break;
            }
        } catch (const GarageException& e) {
            cout << "\n[Ошибка] " << e.what() << '\n';
            if (cin.eof()) running = false;
        } catch (const std::exception& e) {
            cout << "\n[Системная ошибка] " << e.what() << '\n';
        }
    }
    cout << "Завершение работы.\n";
    return 0;
}
