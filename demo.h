#ifndef FORCA_DEMO_H
#define FORCA_DEMO_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "readFile.h"
#include <optional>
#include <stdexcept>

enum Enum {
    ALPHA, BETA
};

template<Enum E>
class Foo {
public:
    virtual void execute();
};

template<>
void Foo<ALPHA>::execute() {
    cout << "execute ALPHA" << endl;
}

template<>
void Foo<BETA>::execute() {
    cout << "execute BETA" << endl;
}
//template<Foo F> class ABC{
//public:
//    virtual void apply();
//};
//
//template<Foo F>
//void ABC<F>::apply() {
//}

enum MyEnum : int {
    A, B, C, D
};

template<typename _t>
class MyTemplate {
public:
    void print(int value);
};

template<>
class MyTemplate<MyEnum> {
public:
    void print(int value) {
        cout << "MyEnum specialisation:  " << value<< endl;
    }
};

template<>
class MyTemplate<int> {
public:
    void print(int value) {
        cout << "int specialisation: " << value << endl;
    }
};

template<typename _t>
void print(_t param) {
    MyTemplate<_t> m;
    m.print('c');
}

void abac(){
    print(MyEnum::A);
}
#endif //FORCA_DEMO_H
