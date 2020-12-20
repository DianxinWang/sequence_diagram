//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H
#define SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H

#include <keyword_factory.h>
#include <key_components/block.h>

class ArrowLine: public Keyword, public DynamicCreator<ArrowLine, std::string_view> {
public:
    class Line {
    public:
        enum class LineType {
            Dahsed,
            Dotted,
            Full
        } line_type = LineType::Full;
        explicit Line(std::string_view one_line):one_line(one_line) {
            if(one_line == "-->") {
                line_type = LineType::Dahsed;
            } else if(one_line == ".>"){
                line_type = LineType::Dotted;
            } else {
                line_type = LineType::Full;
            }
        }
        std::string one_line{};

        friend std::ostream& operator<<(std::ostream& out, const Line& line_rhs) {
            if(line_rhs.line_type == LineType::Dahsed) {
                return out << "-->";
            } else if(line_rhs.line_type == LineType::Full) {
                return out << "->";
            } else if(line_rhs.line_type == LineType::Dotted) {
                return out << ".>";
            }
            return out;
        }
    };
    explicit ArrowLine(std::string_view total_line) {
        std::smatch result;
        one_line = total_line;
        is_drawable = std::regex_match(one_line, result, std::regex{syntax});
        if(is_drawable) {
            source = new Block(result.str(1));
            line = Line(result.str(2));
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
        return out << *arrow_line.source << arrow_line.line << *arrow_line.destination << ": " << arrow_line.note << std::endl;
    }
    Block* source=nullptr;
    Block* destination=nullptr;
    Line line{""};
    std::string note{};
    const inline static std::string syntax{R"(^\s*(.+?)\s*(-?[-.]>)\s*(.+?)\s*:\s*(.*?)\s*$)"};
};


#endif //SEQUENCE_DIAGRAM_DRAWER_ARROW_LINE_H
