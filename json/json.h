//
// Created by Leo on 2023/4/5.
//

#ifndef JSON_JSON_H
#define JSON_JSON_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;
namespace leo{
    namespace json{
        class Json {
        public:
            enum Type {
                json_null = 0,
                json_bool,
                json_int,
                json_double,
                json_string,
                json_array,
                json_object
            };
            Json();
            Json(bool b);
            Json(int i);
            Json(double d);
            Json(const string &s);
            Json(const char *s);
            Json(Type type);
            Json(const Json &other);//拷贝构造函数
        private:
            union Value {
                bool m_bool;
                int m_int;
                double m_double;
                string *m_string;
                vector<Json> *m_array;
                map<string, Json> *m_object;
            };
            Type m_type;
            Value m_value;
        };
    }
};


#endif //JSON_JSON_H
