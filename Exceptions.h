#pragma once
#include <exception>

// Базовое исключение программы. Хранит поясняющий текст.
class GarageException : public std::exception {
protected:
    char message[300];
public:
    explicit GarageException(const char* text);
    const char* what() const noexcept override;
};

// Выход за границы контейнера
class IndexException : public GarageException {
public:
    IndexException(int index, int count);
};

// Операция над пустым гаражом
class EmptyGarageException : public GarageException {
public:
    EmptyGarageException();
};

// Ошибки работы с файлом
class FileException : public GarageException {
public:
    FileException(const char* filename, const char* reason);
};

// Неверный формат вводимых или читаемых данных
class FormatException : public GarageException {
public:
    explicit FormatException(const char* text);
};

// Недопустимое значение поля
class InvalidValueException : public GarageException {
public:
    explicit InvalidValueException(const char* text);
};
