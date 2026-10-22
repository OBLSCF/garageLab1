#include "Motorcycle.h"
#include "Utils.h"
#include "Exceptions.h"

Motorcycle::Motorcycle()
    : Base(), engineVolume(0), power(0), terrain(dupStr("Не указано")) {
    std::cout << "[Motorcycle] Конструктор по умолчанию\n";
}

Motorcycle::Motorcycle(const char* brandValue, const char* modelValue, double volume,
                       double powerValue, const char* terrainValue)
    : Base(brandValue, modelValue), engineVolume(0), power(0), terrain(nullptr) {
    try {
        setEngineVolume(volume);
        setPower(powerValue);
        setTerrain(terrainValue);
    } catch (...) {
        delete[] terrain;
        throw;
    }
    std::cout << "[Motorcycle] Конструктор с параметрами\n";
}

Motorcycle::Motorcycle(const Motorcycle& other)
    : Base(other), engineVolume(other.engineVolume), power(other.power),
      terrain(dupStr(other.terrain)) {
    std::cout << "[Motorcycle] Конструктор копирования\n";
}

Motorcycle::~Motorcycle() {
    std::cout << "[Motorcycle] Деструктор\n";
    delete[] terrain;
}

Motorcycle& Motorcycle::operator=(const Motorcycle& other) {
    if (this != &other) {
        Base::operator=(other);
        char* t = dupStr(other.terrain);
        delete[] terrain;
        terrain = t;
        engineVolume = other.engineVolume;
        power = other.power;
    }
    return *this;
}

double Motorcycle::getEngineVolume() const { return engineVolume; }
double Motorcycle::getPower() const { return power; }
const char* Motorcycle::getTerrain() const { return terrain; }

void Motorcycle::setEngineVolume(double value) {
    if (value <= 0) throw InvalidValueException("Объём двигателя должен быть больше нуля");
    engineVolume = value;
}

void Motorcycle::setPower(double value) {
    if (value <= 0) throw InvalidValueException("Мощность двигателя должна быть больше нуля");
    power = value;
}

void Motorcycle::setTerrain(const char* value) {
    char* t = dupChecked(value, "Местность");
    delete[] terrain;
    terrain = t;
}

int Motorcycle::getTypeId() const { return TYPE_MOTORCYCLE; }
const char* Motorcycle::getTypeName() const { return "Мотоцикл"; }

void Motorcycle::print(std::ostream& out) const {
    out << "Мотоцикл | Марка: " << brand << " | Модель: " << model
        << " | Объём: " << engineVolume << " л | Мощность: " << power
        << " л.с. | Местность: " << terrain;
}

void Motorcycle::input(std::istream& in) {
    char buf[BUF_SIZE];
    readLine(in, "Марка: ", buf, BUF_SIZE);
    setBrand(buf);
    readLine(in, "Модель: ", buf, BUF_SIZE);
    setModel(buf);
    setEngineVolume(readDouble(in, "Объём двигателя (л): ", 0, 100));
    setPower(readDouble(in, "Мощность двигателя (л.с.): ", 0, 5000));
    readLine(in, "Местность (дорога, бездорожье и т.п.): ", buf, BUF_SIZE);
    setTerrain(buf);
}

void Motorcycle::edit() {
    std::cout << "Что изменить?\n 1 - Марка\n 2 - Модель\n 3 - Объём двигателя\n"
                 " 4 - Мощность двигателя\n 5 - Местность\n 0 - Отмена\n";
    int c = readInt(std::cin, "Выбор: ", 0, 5);
    if (c == 0 || editCommon(c)) return;
    char buf[BUF_SIZE];
    switch (c) {
    case 3:
        setEngineVolume(readDouble(std::cin, "Новый объём (л): ", 0, 100));
        break;
    case 4:
        setPower(readDouble(std::cin, "Новая мощность (л.с.): ", 0, 5000));
        break;
    case 5:
        readLine(std::cin, "Новая местность: ", buf, BUF_SIZE);
        setTerrain(buf);
        break;
    }
}

void Motorcycle::save(std::ostream& out) const {
    saveBase(out);
    out << engineVolume << '\n' << power << '\n' << terrain << '\n';
}

void Motorcycle::load(std::istream& in) {
    char buf[BUF_SIZE];
    loadBase(in);
    setEngineVolume(readDouble(in, nullptr, 0, 100));
    setPower(readDouble(in, nullptr, 0, 5000));
    readLine(in, nullptr, buf, BUF_SIZE);
    setTerrain(buf);
}

Base* Motorcycle::clone() const {
    return new Motorcycle(*this);
}
