//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
#define SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
#include <vector>
#include "keyword_factory.h"
#include "key_components/title.h"
#include "key_components/arrow_line.h"
#include "key_components/block.h"
#include "drawing_board.h"
#include "view_components/rectangle.h"
#include "view_components/straight_line.h"
#include <numeric>

class Composer {

public:
    static inline Title* title{nullptr};
    static inline std::vector<ArrowLine*> arrow_lines{};
    static inline std::vector<Block*> blocks{};

    static void draw(geo::DrawingBoard& drawing_board) {
        constexpr double BLOCK_WIDTH = 10;
        constexpr double ARROW_LINE_HEIGHT = 6;
        auto block_length = arrow_lines.size() * ARROW_LINE_HEIGHT;
        // draw title first
        typedef geo::DrawingBoard::P P;
        double title_x = BLOCK_WIDTH * (blocks.size() - 1) / 2;
        double title_y = block_length + 5;

        drawing_board.draw_title<geo::Rectangle>(title_x, title_y, title->title_name);

        // draw block
        for(int i = 0; i < blocks.size(); i++) {
            auto block_x = i * BLOCK_WIDTH;
            auto content = blocks[i]->block_name;
            drawing_board.draw_block_with_text<geo::Rectangle, geo::StraightLine>(block_x, block_length, content);
        }

        // draw arrows
        auto find_block = [](auto block) {
            return std::find_if(blocks.begin(), blocks.end(), [block](auto other_block){return *block==*other_block;}) - blocks.begin();
        };
        for(int i = 0; i < arrow_lines.size(); i++) {
            auto arrow_line = arrow_lines[i];
            auto source = find_block(arrow_line->source);
            auto dest = find_block(arrow_line->destination);
            auto block1_x = source * BLOCK_WIDTH;
            auto block2_x = dest * BLOCK_WIDTH;
            auto block_y = block_length - i*ARROW_LINE_HEIGHT;
            drawing_board.draw_arrow_line<geo::StraightLine>(block1_x, block2_x, block_y);
        }
    }

    static void add_keywords(const std::vector<Keyword*>& key_words) {
        auto title_handler = [](Keyword* keyword_ptr){
            title = dynamic_cast<Title*>(keyword_ptr);
        };
        auto arrow_line_handler = [](Keyword* keyword_ptr){
            auto arrow_line = dynamic_cast<ArrowLine*>(keyword_ptr);
            arrow_lines.push_back(arrow_line);
        };
        auto block_handler = [](Keyword* keyword_ptr){
            auto block = dynamic_cast<Block*>(keyword_ptr);
            auto iter = std::find_if(blocks.begin(), blocks.end(), [block](auto other_block){return *block==*other_block;});
            if(iter == blocks.end()) {
                blocks.push_back(block);
            }
        };
        for(auto keyword: key_words) {
            if(typeid(*keyword) == typeid(Title)) {
                title_handler(keyword);
            } else if(typeid(*keyword) == typeid(ArrowLine)) {
                arrow_line_handler(keyword);
            } else if(typeid(*keyword) == typeid(Block)) {
                block_handler(keyword);
            }
        }
    }
};

#endif //SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
