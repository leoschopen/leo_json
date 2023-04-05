//
// Created by Leo on 2023/4/5.
//

#include "json.h"

using namespace leo::json;


Json::Json() : m_type(json_null) {}

Json::Json(bool b) : m_type(json_bool) {
    m_value.m_bool = b;
}

Json::Json(int i) : m_type(json_int) {
    m_value.m_int = i;
}

Json::Json(double d) : m_type(json_double) {
    m_value.m_double = d;
}

Json::Json(const string &s) : m_type(json_string) {
    m_value.m_string = new string(s);
}

Json::Json(const char *s) : m_type(json_string) {
    m_value.m_string = new string(s);
}

Json::Json(Type type) : m_type(type) {
    switch (m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            m_value.m_string = new string("");
            break;
        case json_array:
            m_value.m_array = new vector<Json>();
            break;
        case json_object:
            m_value.m_object = new map<string, Json>();
            break;
        default:
            break;
    }
}

Json::Json(const Json &other) : m_type(other.m_type) {
    switch (m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = new string(*other.m_value.m_string);//深拷贝
            break;
        case json_array:
            m_value.m_array = new vector<Json>(*other.m_value.m_array);
            break;
        case json_object:
            m_value.m_object = new map<string, Json>(*other.m_value.m_object);
            break;
        default:
            break;
    }
}

Json::operator bool() const {
    if (m_type != json_bool) {
        throw new logic_error("type error, not bool");
    }
    return m_value.m_bool;
}

Json::operator int() const {
    if (m_type != json_int) {
        throw new logic_error("type error, not int");
    }
    return m_value.m_int;
}

Json::operator double() const {
    if (m_type != json_double) {
        throw new logic_error("type error, not double");
    }
    return m_value.m_double;
}

Json::operator string() const {
    if (m_type != json_string) {
        throw new logic_error("type error, not string");
    }
    return *m_value.m_string;
}

//这里实现的功能是直接使用下标访问数组，如果数组不存在，那么就会创建一个数组
Json &Json::operator[](int index) {
    if (m_type != json_array) {
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    if (index < 0) {
        throw new out_of_range("index out of range");
    }
    int size = (m_value.m_array)->size();
    //如果index大于size，那么就要扩容，等于size的时候，就是在最后添加一个元素
    if (index >= size) {
        for (int i = size; i <= size; ++i) {
            //调用Json的默认构造函数，这样就会导致m_type为json_null
            (m_value.m_array)->push_back(Json());
        }
    }
    return (m_value.m_array)->at(index);
}

void Json::append(const Json &json) {
    if (m_type != json_array) {
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    (m_value.m_array)->push_back(json);
}

