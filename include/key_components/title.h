//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_TITLE_H
#define SEQUENCE_DIAGRAM_DRAWER_TITLE_H

#include <keyword_factory.h>

class Title: public Keyword, public DynamicCreator<Title, std::string_view> {
public:
    explicit Title(std::string_view total_line) {
        std::smatch result;
        one_line = total_line;
        is_drawable = std::regex_match(one_line, result, std::regex{syntax});
        if(is_drawable) {
            title_name = result.str(1);
        }
    }


    void print() override {
        std::cout << *this;
    }
    std::string title_name;
    const inline static std::string syntax{R"(^\s*\bTitle\b\s+(.+?)\s*$)"};
    friend std::ostream& operator<<(std::ostream& out, const Title& line_rhs) {
        return out << "Title: " << line_rhs.title_name << std::endl;
    }
};


#endif //SEQUENCE_DIAGRAM_DRAWER_TITLE_H
