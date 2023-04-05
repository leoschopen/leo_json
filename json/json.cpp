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