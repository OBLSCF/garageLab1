#include "Car.h"
#include "Utils.h"
#include "Exceptions.h"

Car::Car() : Base(), engineVolume(0), color(dupStr("Не указано")), gearbox(dupStr("Не указано")) {
    std::cout << "[Car] Конструктор по умолчанию\n";
}

Car::Car(const char* brandValue, const char* modelValue, double volume,
         const char* colorValue, const char* gearboxValue)
    : Base(brandValue, modelValue), engineVolume(0), color(nullptr), gearbox(nullptr) {
    try {
        setEngineVolume(volume);
        setColor(colorValue);
        setGearbox(gearboxValue);
    } catch (...) {
        delete[] color;
        delete[] gearbox;
        throw;
    }
    std::cout << "[Car] Конструктор с параметрами\n";
}

Car::Car(const Car& other)
    : Base(other), engineVolume(other.engineVolume),
      color(dupStr(other.color)), gearbox(dupStr(other.gearbox)) {
    std::cout << "[Car] Конструктор копирования\n";
}

Car::~Car() {
    std::cout << "[Car] Деструктор\n";
    delete[] color;
    delete[] gearbox;
}

Car& Car::operator=(const Car& other) {
    if (this != &other) {
        Base::operator=(other);
        char* c = dupStr(other.color);
        char* g = dupStr(other.gearbox);
        delete[] color;
        delete[] gearbox;
        color = c;
        gearbox = g;
        engineVolume = other.engineVolume;
    }
    return *this;
}

double Car::getEngineVolume() const { return engineVolume; }
const char* Car::getColor() const { return color; }
const char* Car::getGearbox() const { return gearbox; }

void Car::setEngineVolume(double value) {
    if (value <= 0) throw InvalidValueException("Объём двигателя должен быть больше нуля");
    engineVolume = value;
}

void Car::setColor(const char* value) {
    char* t = dupChecked(value, "Цвет");
    delete[] color;
    color = t;
}

void Car::setGearbox(const char* value) {
    char* t = dupChecked(value, "Тип КПП");
    delete[] gearbox;
    gearbox = t;
}

int Car::getTypeId() const { return TYPE_CAR; }
const char* Car::getTypeName() const { return "Автомобиль"; }

void Car::print(std::ostream& out) const {
    out << "Автомобиль | Марка: " << brand << " | Модель: " << model
        << " | Объём: " << engineVolume << " л | Цвет: " << color
        << " | КПП: " << gearbox;
}

void Car::input(std::istream& in) {
    char buf[BUF_SIZE];
    readLine(in, "Марка: ", buf, BUF_SIZE);
    setBrand(buf);
    readLine(in, "Модель: ", buf, BUF_SIZE);
    setModel(buf);
    setEngineVolume(readDouble(in, "Объём двигателя (л): ", 0, 100));
    readLine(in, "Цвет: ", buf, BUF_SIZE);
    setColor(buf);
    readLine(in, "Тип КПП: ", buf, BUF_SIZE);
    setGearbox(buf);
}

void Car::edit() {
    std::cout << "Что изменить?\n 1 - Марка\n 2 - Модель\n 3 - Объём двигателя\n"
                 " 4 - Цвет\n 5 - Тип КПП\n 0 - Отмена\n";
    int c = readInt(std::cin, "Выбор: ", 0, 5);
    if (c == 0 || editCommon(c)) return;
    char buf[BUF_SIZE];
    switch (c) {
    case 3:
        setEngineVolume(readDouble(std::cin, "Новый объём (л): ", 0, 100));
        break;
    case 4:
        readLine(std::cin, "Новый цвет: ", buf, BUF_SIZE);
        setColor(buf);
        break;
    case 5:
        readLine(std::cin, "Новый тип КПП: ", buf, BUF_SIZE);
        setGearbox(buf);
        break;
    }
}

void Car::save(std::ostream& out) const {
    saveBase(out);
    out << engineVolume << '\n' << color << '\n' << gearbox << '\n';
}

void Car::load(std::istream& in) {
    char buf[BUF_SIZE];
    loadBase(in);
    setEngineVolume(readDouble(in, nullptr, 0, 100));
    readLine(in, nullptr, buf, BUF_SIZE);
    setColor(buf);
    readLine(in, nullptr, buf, BUF_SIZE);
    setGearbox(buf);
}

Base* Car::clone() const {
    return new Car(*this);
}
