#include "Utils.h"
#include "Exceptions.h"
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>

char* dupStr(const char* s) {
    size_t n = std::strlen(s);
    char* r = new char[n + 1];
    std::memcpy(r, s, n + 1);
    return r;
}

char* dupChecked(const char* s, const char* fieldName) {
    if (s == nullptr || s[0] == '\0') {
        char msg[200];
        std::snprintf(msg, sizeof(msg), "Поле '%s' не может быть пустым", fieldName);
        throw InvalidValueException(msg);
    }
    return dupStr(s);
}

void readLine(std::istream& in, const char* prompt, char* buf, int size) {
    if (prompt) std::cout << prompt;
    in.getline(buf, size);
    if (in.fail()) {
        if (in.eof()) throw FormatException("Неожиданный конец ввода");
        in.clear();
        in.ignore(10000, '\n');
        throw FormatException("Слишком длинная строка");
    }
    size_t len = std::strlen(buf);
    if (len > 0 && buf[len - 1] == '\r') buf[len - 1] = '\0';
    if (buf[0] == '\0') throw FormatException("Пустое значение недопустимо");
}

int readInt(std::istream& in, const char* prompt, int minV, int maxV) {
    char buf[64];
    readLine(in, prompt, buf, sizeof(buf));
    char* end = nullptr;
    errno = 0;
    long v = std::strtol(buf, &end, 10);
    if (end == buf || *end != '\0' || errno == ERANGE)
        throw FormatException("Ожидалось целое число");
    if (v < minV || v > maxV) {
        char msg[120];
        std::snprintf(msg, sizeof(msg), "Значение должно быть в диапазоне от %d до %d", minV, maxV);
        throw InvalidValueException(msg);
    }
    return static_cast<int>(v);
}

double readDouble(std::istream& in, const char* prompt, double minV, double maxV) {
    char buf[64];
    readLine(in, prompt, buf, sizeof(buf));
    char* end = nullptr;
    errno = 0;
    double v = std::strtod(buf, &end);
    if (end == buf || *end != '\0' || errno == ERANGE)
        throw FormatException("Ожидалось число (дробная часть через точку)");
    if (v < minV || v > maxV) {
        char msg[120];
        std::snprintf(msg, sizeof(msg), "Значение должно быть в диапазоне от %g до %g", minV, maxV);
        throw InvalidValueException(msg);
    }
    return v;
}
