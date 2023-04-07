//
// Created by Leo on 2023/4/6.
//

#ifndef LEO_JSON_PARSER_H
#define LEO_JSON_PARSER_H

#include <string>
#include "json.h"
using namespace std;

namespace leo {
    namespace json {

        class Parser {
        public:
            Parser();
            ~Parser();

            void load(const string &json);

            Json parse();

        private:
            string m_str;
            int m_idx;

            void skip_whitespace();
            char get_next_char();

            //解析json，返回Json对象
            Json parse_null();
            Json parse_bool();
            Json parse_number();
            string parse_string();
            Json parse_array();
            Json parse_object();
            bool in_range(int x, int lower, int upper)
            {
                return (x >= lower && x <= upper);
            }
        };

    } // leo
} // json

#endif //LEO_JSON_PARSER_H
