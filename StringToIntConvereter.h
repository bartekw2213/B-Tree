#pragma once
#include <iostream>

class StringToIntConverter {
    private:
        char* string;
        int capacity;
        int length;
    public:
        StringToIntConverter() : string(new char[20]), capacity(20), length(0) { };
        ~StringToIntConverter() { delete[] string; };
        void AddInt(const char newInt);
        int GetInt(); 
    private:
        void Resize();
};