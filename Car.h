#pragma once
#include "Base.h"

// Автомобиль: марка, модель (из Base), объём двигателя, цвет, тип КПП
class Car : public Base {
private:
    double engineVolume;   // литры
    char* color;
    char* gearbox;
public:
    Car();
    Car(const char* brandValue, const char* modelValue, double volume,
        const char* colorValue, const char* gearboxValue);
    Car(const Car& other);
    ~Car() override;
    Car& operator=(const Car& other);

    double getEngineVolume() const;
    const char* getColor() const;
    const char* getGearbox() const;
    void setEngineVolume(double value);
    void setColor(const char* value);
    void setGearbox(const char* value);

    int getTypeId() const override;
    const char* getTypeName() const override;
    void print(std::ostream& out) const override;
    void input(std::istream& in) override;
    void edit() override;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    Base* clone() const override;
};
