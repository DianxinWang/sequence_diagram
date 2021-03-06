//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_BLOCK_H
#define SEQUENCE_DIAGRAM_DRAWER_BLOCK_H

#include <keyword_factory.h>
#include <iostream>
class Block: public Keyword {
public:
    explicit Block(std::string_view total_line) {
        block_name = total_line;
    }
    std::string block_name;
    bool operator==(const Block& rhs) const {
        return block_name == rhs.block_name;
    }
    std::vector<Keyword*> get_keywords_ptr() override {
        return {this};
    }

    void print() override {
        std::cout << *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const Block& block) {
        return out << "Block: " << block.block_name << std::endl;
    }

};

#endif //SEQUENCE_DIAGRAM_DRAWER_BLOCK_H
