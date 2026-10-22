#pragma once
#include "Base.h"

// Автобус: марка, модель (из Base), сидячие места, всего мест, конечный пункт
class Bus : public Base {
private:
    int seatingPlaces;   // сидячих мест
    int totalPlaces;     // всего пассажирских мест
    char* destination;   // конечный пункт
public:
    Bus();
    Bus(const char* brandValue, const char* modelValue, int seats, int total,
        const char* destinationValue);
    Bus(const Bus& other);
    ~Bus() override;
    Bus& operator=(const Bus& other);

    int getSeatingPlaces() const;
    int getTotalPlaces() const;
    const char* getDestination() const;
    void setSeatingPlaces(int value);
    void setTotalPlaces(int value);
    void setCapacity(int seats, int total);   // установка обоих значений сразу
    void setDestination(const char* value);

    int getTypeId() const override;
    const char* getTypeName() const override;
    void print(std::ostream& out) const override;
    void input(std::istream& in) override;
    void edit() override;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    Base* clone() const override;
};
