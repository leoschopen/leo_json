//
// Created by Leo on 2023/4/5.
//
#include "json/json.h"

using namespace std;
using namespace leo::json;

int main() {
    Json arr;
    arr[0] = true;
    arr[1] = 123;

    arr.append(1.23);
    arr.append("hello");

    bool b = arr[0];
    int i = arr[1];
    double d = arr[2];
    const string &s = arr[3];
    return 0;
}