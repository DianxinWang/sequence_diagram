//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H
#define SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H

#include <keyword_factory.h>
#include <key_components/block.h>

class ArrowLine: public Keyword, public DynamicCreator<ArrowLine, const std::string&> {
public:
    explicit ArrowLine(const std::string& total_line) {
        std::smatch result;
        is_drawable = std::regex_match(total_line, result, std::regex{syntax});
        if(is_drawable) {
            source = new Block(result.str(1));
            line = result.str(2);
            destination = new Block(result.str(3));
            note = result.str(4);
        }
    }

    std::vector<Keyword *> get_keywords_ptr() override {
        if(is_drawable) {
            return {source, destination, this};
        }
        return {};
    }

    void print() override {
        std::cout << *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const ArrowLine& arrow_line) {
        return out << arrow_line.source->block_name << arrow_line.line << arrow_line.destination->block_name << ": " << arrow_line.note << std::endl;
    }
    Block* source=nullptr;
    Block* destination=nullptr;
    std::string line;
    std::string note{};
    const inline static std::string syntax{R"(^\s*(.+?)\s*(-->|->|.>)\s*(.+?)\s*:\s*(.*?)\s*$)"};
};


#endif //SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H
