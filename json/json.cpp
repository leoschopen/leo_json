//
// Created by Leo on 2023/4/5.
//

#include "json.h"
#include "parser.h"
#include <sstream>

using namespace leo::json;


Json::Json() : m_type(json_null) {}

Json::Json(bool b) : m_type(json_bool) {
    m_value.m_bool = b;
}

Json::Json(int i) : m_type(json_int) {
    m_value.m_int = i;
}

Json::Json(long long int i) {
    m_type = json_long;
    m_value.m_long = i;
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
        case json_long:
            m_value.m_long = 0;
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
            m_value.m_object = new unordered_map<string, Json>();
            break;
        default:
            break;
    }
}

Json::Json(const Json &other) {
    copy(other);
}

Json::Json(Json &&other) noexcept {
    m_type = other.m_type;
    switch (m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_long:
            m_value.m_long = other.m_value.m_long;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = other.m_value.m_string;
            break;
        case json_array:
            m_value.m_array = other.m_value.m_array;
            break;
        case json_object:
            m_value.m_object = other.m_value.m_object;
            break;
        default:
            break;
    }
    other.m_type = json_null;
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

Json::operator long long int() const {
    if (m_type != json_long) {
        throw new logic_error("type error, not long");
    }
    return m_value.m_long;
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
        for (int i = size; i <= index; ++i) {
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

string Json::str() const {
    stringstream ss;
    switch (m_type) {
        case json_null:
            ss << "null";
            break;
        case json_bool:
            if (m_value.m_bool)
                ss << "true";
            else
                ss << "false";
            break;
        case json_int:
            ss << to_string(m_value.m_int);
            break;
        case json_long:
            ss << to_string(m_value.m_long);
            break;
        case json_double:
            ss << to_string(m_value.m_double);
            break;
        case json_string:
            //q:为什么用'\"'而不是'"'?
            //a:因为'\"'是一个字符，而"是两个字符，所以要用'\"'
            ss << '\"' << *m_value.m_string << '\"';
            break;
        case json_array: {
            string s = "[";
            for (int i = 0; i < (m_value.m_array)->size(); ++i) {
                //q:这里调用的str()方法是谁的？
                //a:是Json的str()方法
                s += (m_value.m_array)->at(i).str();
                //q:这里需要加上条件i!=0吗？
                //a:不需要，因为如果size==0，那么就是空数组，所以不需要加逗号
                if (i != (m_value.m_array)->size() - 1) {
                    s += ",";
                }
            }
            s += "]";
            ss << s;
            break;
        }
        case json_object: {
            string s = "{";
            int i = 0;
            for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++it) {
                s += "\"" + it->first + "\":" + it->second.str();
                if (i != (m_value.m_object)->size() - 1) {
                    s += ",";
                }
                ++i;
            }
            s += "}";
            ss << s;
            break;
        }
        default:
            ss << "";
            break;
    }
    //q:这里的str()是谁的?
    //a:是stringstream的str()方法
    return ss.str();
}

Json &Json::operator[](const string &key) {
    if (m_type != json_object) {
        m_type = json_object;
        m_value.m_object = new unordered_map<string, Json>();
    }
    return (m_value.m_object)->operator[](key);
}

Json &Json::operator[](const char *key) {
    string str(key);
    return (*this)[str];
}

void Json::operator=(const Json &other) {
    copy(other);
}

//解决重复代码问题吗，实现深拷贝
//拷贝构造函数和赋值运算符一个是用于初始化，一个是用于赋值
void Json::copy(const Json &other) {
    m_type = other.m_type;
    switch (m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_long:
            m_value.m_long = other.m_value.m_long;
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
            m_value.m_object = new unordered_map<string, Json>(*other.m_value.m_object);
            break;
        default:
            break;
    }
}

bool Json::operator==(const Json &other) {
    if (m_type != other.m_type) {
        return false;
    }
    switch (m_type) {
        case json_null:
            return true;
        case json_bool:
            return m_value.m_bool == other.m_value.m_bool;
        case json_int:
            return m_value.m_int == other.m_value.m_int;
        case json_double:
            return m_value.m_double == other.m_value.m_double;
        case json_string:
            return *m_value.m_string == *other.m_value.m_string;
        case json_array: {
            if (m_value.m_array->size() != other.m_value.m_array->size()) {
                return false;
            }
            for (int i = 0; i < m_value.m_array->size(); ++i) {
                if ((*m_value.m_array)[i] != (*other.m_value.m_array)[i]) {
                    return false;
                }
            }
            return true;
        }
        case json_object: {
            if (m_value.m_object->size() != other.m_value.m_object->size()) {
                return false;
            }
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
                if (other.m_value.m_object->find(it->first) == other.m_value.m_object->end()) {
                    return false;
                }
                if (it->second != (*other.m_value.m_object)[it->first]) {
                    return false;
                }
            }
            return true;
        }
        default:
            return false;
    }
}

bool Json::operator!=(const Json &other) {
    return !((*this) == other);
}

bool Json::asBool() const {
    if (m_type != json_bool) {
        throw new logic_error("type error, not bool");
    }
    return m_value.m_bool;
}

int Json::asInt() const {
    if (m_type != json_int) {
        throw new logic_error("type error, not int");
    }
    return m_value.m_int;
}

double Json::asDouble() const {
    if (m_type != json_double) {
        throw new logic_error("type error, not double");
    }
    return m_value.m_double;
}

string Json::asString() const {
    if (m_type != json_string) {
        throw new logic_error("type error, not string");
    }
    return *(m_value.m_string);
}

bool Json::has(int index) {
    if (m_type != json_array) {
        return false;
    }
    int size = (m_value.m_array)->size();
    return index >= 0 && index < size;
}

bool Json::has(const string &key) {
    if (m_type != json_object) {
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

bool Json::has(const char *key) {
    string name(key);
    return has(name);
}

void Json::remove(int index) {
    if (m_type != json_array) {
        return;
    }
    int size = m_value.m_array->size();
    if (index < 0 || index >= size) {
        return;
    }
    (m_value.m_array)->erase(m_value.m_array->begin() + index);
}

void Json::remove(const string &key) {
    auto it = m_value.m_object->find(key);
    if (it != m_value.m_object->end()) {
        m_value.m_object->erase(it);
    }
    return;
}

void Json::remove(const char *key) {
    string name(key);
    remove(name);
}

void Json::parse(const string &str) {
    Parser p;
    p.load(str);
    *this = p.parse();
}









// void Json::clear() {
//     switch (m_type) {
//         case json_null:
//             break;
//         case json_bool:
//             m_value.m_bool = false;
//             break;
//         case json_int:
//             m_value.m_int = 0;
//             break;
//         case json_double:
//             m_value.m_double = 0.0;
//             break;
//         case json_string:
//             delete m_value.m_string;
//             break;
//         case json_array:
//             for(auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it){
//                 it->clear();
//             }
//             delete m_value.m_array;
//             break;
//         case json_object:
//             for(auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it){
//                 it->second.clear();
//             }
//             delete m_value.m_object;
//             break;
//         default:
//             break;
//     }
//     m_type = json_null;
// }

