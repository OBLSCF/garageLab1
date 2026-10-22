#pragma once
#include <iostream>

// Идентификаторы типов (записываются в файл)
enum VehicleType { TYPE_CAR = 1, TYPE_MOTORCYCLE = 2, TYPE_BUS = 3 };

// Абстрактный базовый класс "Транспортное средство"
class Base {
protected:
    char* brand;   // марка
    char* model;   // модель

    void saveBase(std::ostream& out) const;
    void loadBase(std::istream& in);
    bool editCommon(int choice);   // редактирование марки/модели (пункты 1 и 2)
public:
    Base();
    Base(const char* brandValue, const char* modelValue);
    Base(const Base& other);
    virtual ~Base();
    Base& operator=(const Base& other);

    const char* getBrand() const;
    const char* getModel() const;
    void setBrand(const char* value);
    void setModel(const char* value);

    virtual int getTypeId() const = 0;
    virtual const char* getTypeName() const = 0;
    virtual void print(std::ostream& out) const = 0;   // вывод на экран
    virtual void input(std::istream& in) = 0;          // ввод с клавиатуры
    virtual void edit() = 0;                           // изменение полей
    virtual void save(std::ostream& out) const = 0;    // сохранение в файл
    virtual void load(std::istream& in) = 0;           // загрузка из файла
    virtual Base* clone() const = 0;                   // полная копия объекта
};

std::ostream& operator<<(std::ostream& out, const Base& obj);
std::istream& operator>>(std::istream& in, Base& obj);
