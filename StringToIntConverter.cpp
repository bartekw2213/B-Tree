#include "StringToIntConvereter.h"

void StringToIntConverter::AddInt(const char newInt) {
    if(length == capacity)
        Resize();

    string[length++] = newInt;
    string[length] = '\0';
}

int StringToIntConverter::GetInt() {
    length = 0;
    return atoi(string);
}

void StringToIntConverter::Resize() {
    char* temp = new char[capacity*2];
    for(int i = 0; i <= length; i++)
        temp[i] = string[i];
    delete[] string;
    string = temp;
    capacity *= 2;
}