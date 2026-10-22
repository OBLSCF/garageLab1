#include "Bus.h"
#include "Utils.h"
#include "Exceptions.h"

Bus::Bus() : Base(), seatingPlaces(0), totalPlaces(0), destination(dupStr("Не указано")) {
    std::cout << "[Bus] Конструктор по умолчанию\n";
}

Bus::Bus(const char* brandValue, const char* modelValue, int seats, int total,
         const char* destinationValue)
    : Base(brandValue, modelValue), seatingPlaces(0), totalPlaces(0), destination(nullptr) {
    try {
        setCapacity(seats, total);
        setDestination(destinationValue);
    } catch (...) {
        delete[] destination;
        throw;
    }
    std::cout << "[Bus] Конструктор с параметрами\n";
}

Bus::Bus(const Bus& other)
    : Base(other), seatingPlaces(other.seatingPlaces), totalPlaces(other.totalPlaces),
      destination(dupStr(other.destination)) {
    std::cout << "[Bus] Конструктор копирования\n";
}

Bus::~Bus() {
    std::cout << "[Bus] Деструктор\n";
    delete[] destination;
}

Bus& Bus::operator=(const Bus& other) {
    if (this != &other) {
        Base::operator=(other);
        char* d = dupStr(other.destination);
        delete[] destination;
        destination = d;
        seatingPlaces = other.seatingPlaces;
        totalPlaces = other.totalPlaces;
    }
    return *this;
}

int Bus::getSeatingPlaces() const { return seatingPlaces; }
int Bus::getTotalPlaces() const { return totalPlaces; }
const char* Bus::getDestination() const { return destination; }

void Bus::setSeatingPlaces(int value) {
    if (value <= 0) throw InvalidValueException("Количество сидячих мест должно быть положительным");
    if (totalPlaces > 0 && value > totalPlaces)
        throw InvalidValueException("Сидячих мест не может быть больше общего количества мест");
    seatingPlaces = value;
}

void Bus::setTotalPlaces(int value) {
    if (value <= 0) throw InvalidValueException("Общее количество мест должно быть положительным");
    if (seatingPlaces > value)
        throw InvalidValueException("Общее количество мест не может быть меньше сидячих");
    totalPlaces = value;
}

void Bus::setCapacity(int seats, int total) {
    if (seats <= 0 || total <= 0)
        throw InvalidValueException("Количество мест должно быть положительным");
    if (seats > total)
        throw InvalidValueException("Сидячих мест не может быть больше общего количества мест");
    seatingPlaces = seats;
    totalPlaces = total;
}

void Bus::setDestination(const char* value) {
    char* t = dupChecked(value, "Конечный пункт");
    delete[] destination;
    destination = t;
}

int Bus::getTypeId() const { return TYPE_BUS; }
const char* Bus::getTypeName() const { return "Автобус"; }

void Bus::print(std::ostream& out) const {
    out << "Автобус | Марка: " << brand << " | Модель: " << model
        << " | Сидячих мест: " << seatingPlaces << " | Всего мест: " << totalPlaces
        << " | Конечный пункт: " << destination;
}

void Bus::input(std::istream& in) {
    char buf[BUF_SIZE];
    readLine(in, "Марка: ", buf, BUF_SIZE);
    setBrand(buf);
    readLine(in, "Модель: ", buf, BUF_SIZE);
    setModel(buf);
    int seats = readInt(in, "Количество сидячих мест: ", 1, 1000);
    int total = readInt(in, "Общее количество мест: ", 1, 1000);
    setCapacity(seats, total);
    readLine(in, "Конечный пункт: ", buf, BUF_SIZE);
    setDestination(buf);
}

void Bus::edit() {
    std::cout << "Что изменить?\n 1 - Марка\n 2 - Модель\n 3 - Сидячие места\n"
                 " 4 - Общее количество мест\n 5 - Оба значения сразу\n"
                 " 6 - Конечный пункт\n 0 - Отмена\n";
    int c = readInt(std::cin, "Выбор: ", 0, 6);
    if (c == 0 || editCommon(c)) return;
    char buf[BUF_SIZE];
    switch (c) {
    case 3:
        setSeatingPlaces(readInt(std::cin, "Новое количество сидячих мест: ", 1, 1000));
        break;
    case 4:
        setTotalPlaces(readInt(std::cin, "Новое общее количество мест: ", 1, 1000));
        break;
    case 5: {
        int seats = readInt(std::cin, "Сидячих мест: ", 1, 1000);
        int total = readInt(std::cin, "Всего мест: ", 1, 1000);
        setCapacity(seats, total);
        break;
    }
    case 6:
        readLine(std::cin, "Новый конечный пункт: ", buf, BUF_SIZE);
        setDestination(buf);
        break;
    }
}

void Bus::save(std::ostream& out) const {
    saveBase(out);
    out << seatingPlaces << '\n' << totalPlaces << '\n' << destination << '\n';
}

void Bus::load(std::istream& in) {
    char buf[BUF_SIZE];
    loadBase(in);
    int seats = readInt(in, nullptr, 1, 1000);
    int total = readInt(in, nullptr, 1, 1000);
    setCapacity(seats, total);
    readLine(in, nullptr, buf, BUF_SIZE);
    setDestination(buf);
}

Base* Bus::clone() const {
    return new Bus(*this);
}
