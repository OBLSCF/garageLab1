#pragma once
#include <iostream>
#include "Base.h"

// Контейнер "Гараж": динамический массив указателей на Base.
// Владеет добавленными объектами и удаляет их в деструкторе.
class Keeper {
private:
    Base** items;
    int count;
    int capacity;

    void grow();
    void swap(Keeper& other);
public:
    Keeper();
    explicit Keeper(int initialCapacity);
    Keeper(const Keeper& other);
    ~Keeper();
    Keeper& operator=(const Keeper& other);

    int getCount() const;
    int getCapacity() const;
    int countByType(int typeId) const;

    void add(Base* obj);          // Keeper становится владельцем obj
    void remove(int index);       // индекс с нуля
    void clear();

    Base& operator[](int index);
    const Base& operator[](int index) const;
    Keeper& operator+=(Base* obj);

    void showAll(std::ostream& out) const;
    void showByType(int typeId, std::ostream& out) const;

    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);
};

std::ostream& operator<<(std::ostream& out, const Keeper& keeper);
