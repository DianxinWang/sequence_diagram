//
// Created by dianxin on 2020/12/27.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_NOTE_H
#define SEQUENCE_DIAGRAM_DRAWER_NOTE_H

#include "keyword_factory.h"
#include "block.h"

class Note: public Keyword, public DynamicCreator<Note, const std::string&> {
public:
    explicit Note(const std::string& total_line) {
        std::smatch result;
        is_drawable = std::regex_match(total_line, result, std::regex{syntax});
        if(is_drawable) {
            placement = result.str(1);
            target = new Block(result.str(2));
            content = result.str(3);
        }
    }

    std::vector<Keyword *> get_keywords_ptr() override {
        if(is_drawable) {
            return {target, this};
        }
        return {};
    }

    void print() override {
        std::cout << *this;
    }
    Block* target=nullptr;
    std::string placement;
    std::string content;
    const inline static std::string syntax{R"(^\s*\b(note over|note left of|note right of)\b\s*(.*?)\s*:\s*(.+?)\s*$)"};
    friend std::ostream& operator<<(std::ostream& out, const Note& note) {
        return out << note.placement << " " << note.target->block_name << ":" << note.content << std::endl;
    }
};


#endif //SEQUENCE_DIAGRAM_DRAWER_NOTE_H
