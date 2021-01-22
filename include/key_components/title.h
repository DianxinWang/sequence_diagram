//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_TITLE_H
#define SEQUENCE_DIAGRAM_DRAWER_TITLE_H

#include <keyword_factory.h>

class Title: public Keyword, public DynamicCreator<Title, const std::string&> {
public:
    explicit Title(const std::string& total_line) {
        std::smatch result;
        is_drawable = std::regex_match(total_line, result, std::regex{syntax});
        if(is_drawable) {
            title_name = result.str(1);
        }
    }

    std::vector<Keyword *> get_keywords_ptr() override {
        if(is_drawable) {
            return {this};
        }
        return {};
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
