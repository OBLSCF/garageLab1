#pragma once
#include <iostream>

const int BUF_SIZE = 256;

// Создаёт динамическую копию строки (освобождать через delete[])
char* dupStr(const char* s);

// То же, но бросает InvalidValueException, если строка пустая или nullptr
char* dupChecked(const char* s, const char* fieldName);

// Чтение строки. Если prompt != nullptr, он выводится на экран.
// Для чтения из файла передаётся prompt = nullptr.
void readLine(std::istream& in, const char* prompt, char* buf, int size);
int readInt(std::istream& in, const char* prompt, int minV, int maxV);
double readDouble(std::istream& in, const char* prompt, double minV, double maxV);
