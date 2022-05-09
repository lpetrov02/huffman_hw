#pragma once

#include <string>
#include <iostream>

class MyException {
protected:
    const char* message;
public:
    MyException(const char* msg) {
        message = msg;
    }

    void get() {
        std::cerr << message << "\n";
    }
};


class FileOpenException: public MyException {
private:
    const char* file_name;
public:
    FileOpenException(const char* f) : MyException("Cannot open file") {
        file_name = f;
    }
    void get() {
        std::cerr << message << ": " << file_name << "\n";
    }
};


class MemoryException: public MyException {
private:
    const char* object;
public:
    MemoryException(const char* obj) : MyException("Cannot create an object") {
        object = obj;
    }
    void get() {
        std::cerr << message << ": " << object << "\n";
    }
};


class FileIOException: public MyException {
private:
    const char* action;
public:
    FileIOException(const char* act) : MyException("error") {
        action = act;
    }
    void get() {
        std::cerr << action << " " << message << "\n";
    }
};

class ArgnumException: public MyException {
private:
    int expected_;
    int got_;
public:
    ArgnumException(int expected, int got) : MyException("Invalid number of arguments: ") {
        expected_ = expected;
        got_ = got;
    }
    void get() {
        std::cerr << message << "expected " << expected_ << ", got " << got_ << " actually\n";
    }
};

class InvalidArgException: public MyException {
private:
    char* name_;
public:
    InvalidArgException(char* name) : MyException("Invalid key: ") {
        name_ = name;
    }
    void get() {
        std::cerr << message << name_ << "\n";
    }
};
