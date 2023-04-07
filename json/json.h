//
// Created by Leo on 2023/4/5.
//

#ifndef JSON_JSON_H
#define JSON_JSON_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;
namespace leo {
    namespace json {
        class Json {
        public:
            enum Type {
                json_null = 0,
                json_bool,
                json_int,
                json_long,
                json_double,
                json_string,
                json_array,
                json_object
            };

            Json();

            Json(bool b);

            Json(int i);

            Json(long long i);

            Json(double d);

            Json(const string &s);

            Json(const char *s);

            Json(Type type);

            //实现移动构造函数
            Json(Json &&other) noexcept;//移动构造函数

            //q:这就相当于重载=运算符了吧
            //a:是的，这里是重载了=运算符，但是这里的=运算符是成员函数，而不是全局函数
            Json(const Json &other);//拷贝构造函数

            operator bool() const;//重载bool类型转换运算符

            operator int() const;

            operator long long() const;

            operator double() const;

            operator string() const;

            Json &operator[](int index);//重载[]运算符，用于访问数组元素
            Json &operator[](const string &key);//重载[]运算符，用于访问对象成员
            Json &operator[](const char *key);//重载[]运算符，用于访问对象成员
            void operator=(const Json &other);//重载=运算符，用于赋值
            bool operator==(const Json &other);//重载==运算符，用于比较
            bool operator!=(const Json &other);//重载!=运算符，用于比较
            void append(const Json &json);

            void copy(const Json &json);
            // void clear();//清空Json对象，如果使用浅拷贝方式，使用这个函数手动释放内存

            string str() const;//将Json对象转换为字符串


            bool isNull() const { return m_type == json_null; }

            bool isBool() const { return m_type == json_bool; }

            bool isInt() const { return m_type == json_int; }

            bool isDouble() const { return m_type == json_double; }

            bool isString() const { return m_type == json_string; }

            bool isArray() const { return m_type == json_array; }

            bool isObject() const { return m_type == json_object; }

            bool asBool() const;

            int asInt() const;

            double asDouble() const;

            string asString() const;

            bool has(int index);

            bool has(const string &key);

            bool has(const char *key);

            void remove(int index);

            void remove(const string &key);

            void remove(const char *key);

            void parse(const string &str);

            //实现迭代器
            typedef vector<Json>::iterator iterator;

            iterator begin() {
                return m_value.m_array->begin();
            }

            iterator end() {
                return m_value.m_array->end();
            }

            //输出m_value的类型
            void printType() {
                switch(m_type){
                    case(0):
                        cout<<"json_null"<<endl;
                        break;
                    case(1):
                        cout<<"json_bool"<<endl;
                        break;
                    case(2):
                        cout<<"json_int"<<endl;
                        break;
                    case(3):
                        cout<<"json_long"<<endl;
                        break;
                    case(4):
                        cout<<"json_double"<<endl;
                        break;
                    case(5):
                        cout<<"json_string"<<endl;
                        break;
                    case(6):
                        cout<<"json_array"<<endl;
                        break;
                    case(7):
                        cout<<"json_object"<<endl;
                        break;
                }
            }


            ~Json() {
                switch (m_type) {
                    case json_string:
                        delete m_value.m_string;
                        break;
                    case json_array:
                        delete m_value.m_array;
                        break;
                    case json_object:
                        delete m_value.m_object;
                        break;
                    default:
                        break;
                }
            }


        private:
            union Value {
                bool m_bool;
                int m_int;
                long long m_long;
                double m_double;
                string *m_string;
                vector<Json> *m_array;
                unordered_map<string, Json> *m_object;
            };
            Type m_type;
            Value m_value;
        };
    }
};


#endif //JSON_JSON_H
