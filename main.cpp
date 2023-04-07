//
// Created by Leo on 2023/4/5.
//
#include "json/json.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <typeinfo>
#include <ctime>
#include <unistd.h>

using namespace std;
using namespace leo::json;

int main() {
    ifstream ifs("../test.json");
    if (!ifs.is_open()) {
        cout << "open file failed" << endl;
        return 0;
    }
    stringstream ss;
    ss << ifs.rdbuf();
    const string &str = ss.str();

    Json v;
    v.parse(str);
    Json label = v["data"];
    Json current = v["data"]["officialVerify"]["officialVerify"];
    label.printType();
    current.printType();
    cout << v.str();


    // const string &str = "-1.23";
    // Json v;
    // v.parse(str);
    // v.printType();
    // return 0;

    // try {
    //     const string &str = R"({"data": {"isLogin": true, "level_info": {"current_level": 1}}})";
    //     // const string &str = R"(["a",1,1.23])";
    //     Json v;
    //     v.parse(str);
    //     bool a = v["data"]["isLogin"];
    //     cout << a;
    // } catch (const std::exception &e) {
    //     std::cerr << "Exception caught: " << e.what() << std::endl;
    // }
    // return 0;
}