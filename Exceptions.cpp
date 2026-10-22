#include "Exceptions.h"
#include <cstdio>

GarageException::GarageException(const char* text) {
    std::snprintf(message, sizeof(message), "%s", text ? text : "Неизвестная ошибка");
}

const char* GarageException::what() const noexcept {
    return message;
}

IndexException::IndexException(int index, int count) : GarageException("") {
    std::snprintf(message, sizeof(message),
        "Объекта с номером %d не существует (в гараже объектов: %d)", index + 1, count);
}

EmptyGarageException::EmptyGarageException()
    : GarageException("Гараж пуст, операция невозможна") {}

FileException::FileException(const char* filename, const char* reason) : GarageException("") {
    std::snprintf(message, sizeof(message), "Ошибка файла '%s': %s", filename, reason);
}

FormatException::FormatException(const char* text) : GarageException(text) {}

InvalidValueException::InvalidValueException(const char* text) : GarageException(text) {}
