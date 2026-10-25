#include "Keeper.h"
#include "Car.h"
#include "Motorcycle.h"
#include "Bus.h"
#include "Utils.h"
#include "Exceptions.h"
#include <fstream>
#include <iomanip>
#include <cstring>

static const char* FILE_HEADER = "GARAGE_V1";

static Base* createByType(int typeId) {
    switch (typeId) {
    case TYPE_CAR: return new Car();
    case TYPE_MOTORCYCLE: return new Motorcycle();
    case TYPE_BUS: return new Bus();
    default: throw FormatException("Неизвестный тип объекта в файле");
    }
}

Keeper::Keeper() : items(new Base*[4]), count(0), capacity(4) {
    std::cout << "[Keeper] Конструктор по умолчанию\n";
}

Keeper::Keeper(int initialCapacity) : items(nullptr), count(0), capacity(initialCapacity) {
    if (initialCapacity < 1)
        throw InvalidValueException("Начальная вместимость гаража должна быть не меньше 1");
    items = new Base*[capacity];
    std::cout << "[Keeper] Конструктор с параметрами\n";
}

Keeper::Keeper(const Keeper& other)
    : items(new Base*[other.capacity]), count(0), capacity(other.capacity) {
    try {
        for (int i = 0; i < other.count; ++i) {
            items[i] = other.items[i]->clone();
            ++count;
        }
    } catch (...) {
        for (int i = 0; i < count; ++i) delete items[i];
        delete[] items;
        throw;
    }
    std::cout << "[Keeper] Конструктор копирования\n";
}

Keeper::~Keeper() {
    std::cout << "[Keeper] Деструктор\n";
    for (int i = 0; i < count; ++i) delete items[i];
    delete[] items;
}

void Keeper::swap(Keeper& other) {
    Base** i = items;     items = other.items;       other.items = i;
    int c = count;        count = other.count;       other.count = c;
    int cap = capacity;   capacity = other.capacity; other.capacity = cap;
}

Keeper& Keeper::operator=(const Keeper& other) {
    if (this != &other) {
        Keeper tmp(other);
        swap(tmp);
    }
    return *this;
}

int Keeper::getCount() const { return count; }
int Keeper::getCapacity() const { return capacity; }

int Keeper::countByType(int typeId) const {
    int n = 0;
    for (int i = 0; i < count; ++i)
        if (items[i]->getTypeId() == typeId) ++n;
    return n;
}

void Keeper::grow() {
    int newCap = capacity * 2;
    Base** bigger = new Base*[newCap];
    for (int i = 0; i < count; ++i) bigger[i] = items[i];
    delete[] items;
    items = bigger;
    capacity = newCap;
}

void Keeper::add(Base* obj) {
    if (obj == nullptr) throw InvalidValueException("Нельзя добавить пустой указатель");
    if (count == capacity) grow();
    items[count++] = obj;
}

void Keeper::remove(int index) {
    if (count == 0) throw EmptyGarageException();
    if (index < 0 || index >= count) throw IndexException(index, count);
    delete items[index];
    for (int i = index; i < count - 1; ++i) items[i] = items[i + 1];
    --count;
}

void Keeper::clear() {
    for (int i = 0; i < count; ++i) delete items[i];
    count = 0;
}

Base& Keeper::operator[](int index) {
    if (index < 0 || index >= count) throw IndexException(index, count);
    return *items[index];
}

const Base& Keeper::operator[](int index) const {
    if (index < 0 || index >= count) throw IndexException(index, count);
    return *items[index];
}

Keeper& Keeper::operator+=(Base* obj) {
    add(obj);
    return *this;
}

void Keeper::showAll(std::ostream& out) const {
    if (count == 0) {
        out << "Гараж пуст.\n";
        return;
    }
    for (int i = 0; i < count; ++i)
        out << (i + 1) << ") " << *items[i] << '\n';
}

void Keeper::showByType(int typeId, std::ostream& out) const {
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (items[i]->getTypeId() == typeId) {
            out << (i + 1) << ") " << *items[i] << '\n';
            found = true;
        }
    }
    if (!found) out << "Объектов такого типа нет.\n";
}

void Keeper::saveToFile(const char* filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) throw FileException(filename, "не удалось открыть файл для записи");
    out << std::setprecision(10);
    out << FILE_HEADER << '\n' << count << '\n';
    for (int i = 0; i < count; ++i) {
        out << items[i]->getTypeId() << '\n';
        items[i]->save(out);
    }
    out.flush();
    if (!out) throw FileException(filename, "ошибка при записи данных");
}

void Keeper::loadFromFile(const char* filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw FileException(filename, "файл не найден или недоступен для чтения");

    char buf[BUF_SIZE];
    readLine(in, nullptr, buf, BUF_SIZE);
    if (std::strcmp(buf, FILE_HEADER) != 0)
        throw FileException(filename, "неверный формат файла (нет заголовка гаража)");

    int n = readInt(in, nullptr, 0, 1000000);
    Keeper loaded;   // временный гараж: текущие данные меняем только при полном успехе
    for (int i = 0; i < n; ++i) {
        int typeId = readInt(in, nullptr, 1, 3);
        Base* obj = createByType(typeId);
        try {
            obj->load(in);
            loaded.add(obj);
        } catch (...) {
            delete obj;
            throw;
        }
    }
    swap(loaded);
}

std::ostream& operator<<(std::ostream& out, const Keeper& keeper) {
    keeper.showAll(out);
    return out;
}
