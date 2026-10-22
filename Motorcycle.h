#pragma once
#include "Base.h"

// Мотоцикл: марка, модель (из Base), объём и мощность двигателя, местность
class Motorcycle : public Base {
private:
    double engineVolume;   // литры
    double power;          // л.с.
    char* terrain;         // для какой местности предназначен
public:
    Motorcycle();
    Motorcycle(const char* brandValue, const char* modelValue, double volume,
               double powerValue, const char* terrainValue);
    Motorcycle(const Motorcycle& other);
    ~Motorcycle() override;
    Motorcycle& operator=(const Motorcycle& other);

    double getEngineVolume() const;
    double getPower() const;
    const char* getTerrain() const;
    void setEngineVolume(double value);
    void setPower(double value);
    void setTerrain(const char* value);

    int getTypeId() const override;
    const char* getTypeName() const override;
    void print(std::ostream& out) const override;
    void input(std::istream& in) override;
    void edit() override;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    Base* clone() const override;
};
