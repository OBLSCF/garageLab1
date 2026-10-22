#include "Base.h"
#include "Utils.h"

Base::Base() : brand(dupStr("Не указано")), model(dupStr("Не указано")) {
    std::cout << "[Base] Конструктор по умолчанию\n";
}

Base::Base(const char* brandValue, const char* modelValue) : brand(nullptr), model(nullptr) {
    try {
        setBrand(brandValue);
        setModel(modelValue);
    } catch (...) {
        delete[] brand;
        delete[] model;
        throw;
    }
    std::cout << "[Base] Конструктор с параметрами\n";
}

Base::Base(const Base& other) : brand(dupStr(other.brand)), model(dupStr(other.model)) {
    std::cout << "[Base] Конструктор копирования\n";
}

Base::~Base() {
    std::cout << "[Base] Деструктор\n";
    delete[] brand;
    delete[] model;
}

Base& Base::operator=(const Base& other) {
    if (this != &other) {
        char* b = dupStr(other.brand);
        char* m = dupStr(other.model);
        delete[] brand;
        delete[] model;
        brand = b;
        model = m;
    }
    return *this;
}

const char* Base::getBrand() const { return brand; }
const char* Base::getModel() const { return model; }

void Base::setBrand(const char* value) {
    char* t = dupChecked(value, "Марка");
    delete[] brand;
    brand = t;
}

void Base::setModel(const char* value) {
    char* t = dupChecked(value, "Модель");
    delete[] model;
    model = t;
}

void Base::saveBase(std::ostream& out) const {
    out << brand << '\n' << model << '\n';
}

void Base::loadBase(std::istream& in) {
    char buf[BUF_SIZE];
    readLine(in, nullptr, buf, BUF_SIZE);
    setBrand(buf);
    readLine(in, nullptr, buf, BUF_SIZE);
    setModel(buf);
}

bool Base::editCommon(int choice) {
    char buf[BUF_SIZE];
    if (choice == 1) {
        readLine(std::cin, "Новая марка: ", buf, BUF_SIZE);
        setBrand(buf);
        return true;
    }
    if (choice == 2) {
        readLine(std::cin, "Новая модель: ", buf, BUF_SIZE);
        setModel(buf);
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const Base& obj) {
    obj.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, Base& obj) {
    obj.input(in);
    return in;
}
