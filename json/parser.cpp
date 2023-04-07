//
// Created by Leo on 2023/4/6.
//

#include "parser.h"
#include <stdexcept>
#include <cstdlib>

using namespace leo::json;

Parser::Parser() : m_str(""), m_idx(0) {}

Parser::~Parser() {}

void Parser::load(const string &str) {
    m_str = str;
    m_idx = 0;
}


void Parser::skip_whitespace() {
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t' || m_str[m_idx] == '\r') {
        m_idx++;
    }
}

//跳过空白字符，返回当前字符，然后指针后移
char Parser::get_next_char() {
    skip_whitespace();
    //判断是否到了字符串末尾
    if (m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++];
}


//解析json字符串
Json Parser::parse() {
    char c = get_next_char();
    switch (c) {
        case 'n':
            --m_idx;
            return parse_null();
        case 't':
        case 'f':
            --m_idx;
            return parse_bool();
        case '"':
            return Json(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            --m_idx;
            return parse_number();
        default:
            break;
    }
    throw new logic_error("unexpected character in parse json");
}


Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw logic_error("parse null error");
}


Json Parser::parse_bool() {
    if (m_str.compare(m_idx, 4, "true") == 0) {
        m_idx += 4;
        return Json(true);
    } else if (m_str.compare(m_idx, 5, "false") == 0) {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number() {
    int pos = m_idx;
    if (m_str[pos] == '-') {
        pos++;
    }
    if (m_str[pos] < '0' || m_str[pos] > '9') {
        throw new logic_error("unexpected number character");
    }
    while (m_str[pos] >= '0' && m_str[pos] <= '9') {
        pos++;
    }
    if (m_str[pos] != '.') {
        long long i = stoll(m_str.substr(m_idx, pos - m_idx));
        m_idx = pos;
        return Json(i);
    }
    pos++;
    if (m_str[pos] < '0' || m_str[pos] > '9') {
        throw new logic_error("unexpected number character");
    }
    while (m_str[pos] >= '0' && m_str[pos] <= '9') {
        pos++;
    }
    double d = stod(m_str.substr(m_idx, pos - m_idx));
    m_idx = pos;
    return Json(d);
}

// Json Parser::parse_number() {
//     size_t pos = m_idx;
//
//     if (m_str[m_idx] == '-')
//         m_idx++;
//
//     // integer part
//     if (m_str[m_idx] == '0') {
//         m_idx++;
//     } else if (in_range(m_str[m_idx], '1', '9')) {
//         m_idx++;
//         while (in_range(m_str[m_idx], '0', '9')) {
//             m_idx++;
//         }
//     } else {
//         throw std::logic_error("invalid character in number");
//     }
//
//     if (m_str[m_idx] != '.') {
//         return Json(std::atoi(m_str.c_str() + pos));
//     }
//
//     // decimal part
//     m_idx++;
//     if (!in_range(m_str[m_idx], '0', '9')) {
//         throw std::logic_error("at least one digit required in fractional part");
//     }
//     while (in_range(m_str[m_idx], '0', '9')) {
//         m_idx++;
//     }
//     return Json(std::atof(m_str.c_str() + pos));
// }

string Parser::parse_string() {
    string out;
    while (true) {
        char c = m_str[m_idx++];//这里不用get_next_char()是因为数组中的元素可以有空格
        if (c == '"')
            break;
        if (c == '\\') {
            c = m_str[m_idx++];
            switch (c) {
                case '\n':
                    out += '\n';
                    break;
                case '\t':
                    out += '\t';
                    break;
                case '\r':
                    out += '\r';
                    break;
                case '\b':
                    out += '\b';
                    break;
                case '\f':
                    out += '\f';
                    break;
                case '"':
                    out += "\\\"";//我希望输出的字符串是\"而不是"
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case 'u':
                    out += "\\u";
                    for (int i = 0; i < 4; i++) {
                        out += m_str[m_idx++];
                    }
                    break;
                default:
                    break;
            }
        } else {
            out += c;
        }
    }
    return out;
}

Json Parser::parse_array() {
    //创建一个空数组，类型为json_array
    Json arr(Json::json_array);
    //获取下一个字符
    char ch = get_next_char();
    //如果是]，说明是空数组，直接返回
    if (ch == ']') {
        return arr;
    }
    //如果不是]，说明数组中至少有一个元素，将m_idx回退一位，因为get_next_char()会将m_idx++
    --m_idx;
    while (true) {
        //将解析到的元素添加到数组中
        arr.append(parse());
        ch = get_next_char();
        if (ch == ']') {
            break;
        }
        if (ch != ',') {
            throw new logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object() {
    Json obj(Json::json_object);
    char ch = get_next_char();
    if (ch == '}') {
        return obj;
    }
    --m_idx;
    while (true) {
        ch = get_next_char();
        if (ch != '"') {
            throw new logic_error("expected '\"' in object");
        }
        string key = parse_string();
        //相当于这个key是一个动态的
        ch = get_next_char();
        if (ch != ':') {
            throw new logic_error("expected ':' in object");
        }
        obj[key] = parse();//下标指向逗号或者}
        ch = get_next_char();
        if (ch == '}') {
            break;
        }
        if (ch != ',') {
            throw new logic_error("expected ',' in object");
        }
    }
    return obj;
}


