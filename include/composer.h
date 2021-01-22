//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
#define SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
#include <vector>
#include <numeric>
#include <algorithm>
#include <variant>
#include "keyword_factory.h"
#include "key_components/title.h"
#include "key_components/arrow_line.h"
#include "key_components/block.h"
#include "key_components/note.h"
#include "drawing_board.h"
#include "view_components/rectangle.h"
#include "view_components/straight_line.h"
#include "view_components/straight_line_with_marker.h"
#include "tools/candy.h"

class Composer {
public:
    using Actor = std::variant<ArrowLine*, Note*>;
    static inline Title* title{nullptr};
    static inline std::vector<Actor> actors{};
    static inline std::vector<Block*> blocks{};

    struct Layout {
        static constexpr double MINIMUM_BLOCK_GAP = 10.;
        static constexpr double BOTTOM_ACTOR_MARGIN = 6.;
        static constexpr double TOP_TITLE_MARGIN = 6.;
        static constexpr double ARROW_LINE_HEIGHT = 6.;
        static constexpr double NOTE_HEIGHT = 7.;
        static constexpr double WORD_WIDTH = 1.;
        double title_x, title_y;
        double block_height;
        std::vector<double> block_x;
        std::vector<double> actor_y;
    } static inline layout{};

    static void draw(geo::DrawingBoard& drawing_board) {
        // draw title first
        drawing_board.draw_title<geo::Rectangle>(layout.title_x, layout.title_y, title->title_name);

        // draw block
        for(int i = 0; i < blocks.size(); i++) {
            drawing_board.draw_block_with_text<geo::Rectangle, geo::StraightLine>(layout.block_x[i], layout.block_height, blocks[i]->block_name);
        }

        // draw actors
        for(int i = 0; i < actors.size(); i++) {
            std::visit(tools::overloaded{
                [&](const ArrowLine* arrow_line){
                    auto source = find_block_index(arrow_line->source);
                    auto dest = find_block_index(arrow_line->destination);
                    drawing_board.draw_arrow_line<geo::StraightLine>(layout.block_x[source], layout.block_x[dest], layout.actor_y[i]);
                    },
                [&](const Note* note){
                    auto target = find_block_index(note->target);
                    drawing_board.draw_note<geo::Rectangle>(layout.block_x[target], layout.actor_y[i], note->content);
                    }
                }, actors[i]);
        }
    }

    /*
     * update layout and keep a reference of the key_words
     */
    static void add_keywords(const std::vector<Keyword*>& key_words) {
        std::vector<double> block_gap{0.};
        std::vector<double> actor_height{};
        auto title_handler = [&](Keyword* keyword_ptr){
            title = dynamic_cast<Title*>(keyword_ptr);
        };
        auto block_handler = [&](Keyword* keyword_ptr){
            auto block = dynamic_cast<Block*>(keyword_ptr);
            if(!has_block(block)) {
                blocks.push_back(block);
                block_gap.push_back(Layout::MINIMUM_BLOCK_GAP);
            }
        };
        auto arrow_line_handler = [&](Keyword* keyword_ptr){
            auto arrow_line = dynamic_cast<ArrowLine*>(keyword_ptr);
            actors.emplace_back(arrow_line);
            // update layout
            actor_height.push_back(Layout::ARROW_LINE_HEIGHT);
            auto src = find_block_index(arrow_line->source);
            auto dest = find_block_index(arrow_line->destination);
            double required_block_gap = arrow_line->note.size() * Layout::WORD_WIDTH;
            double current_block_gap = std::accumulate(begin(block_gap) + 1 + src, begin(block_gap) + 1 + dest, 0.);
            block_gap[dest] += std::max({0., required_block_gap - current_block_gap});
        };
        auto note_handler = [&](Keyword* keyword_ptr){
            auto note = dynamic_cast<Note*>(keyword_ptr);
            actors.emplace_back(note);
            actor_height.push_back(Layout::NOTE_HEIGHT);
            double left = 0., right = 0.;
            if(note->placement == "note over") {
                left = right = note->content.size() * Layout::WORD_WIDTH / 2.;
            } else if(note->placement == "note left of") {
                left = note->content.size() * Layout::WORD_WIDTH;
            } else if(note->placement == "note right of") {
                right = note->content.size() * Layout::WORD_WIDTH;
            }
            auto target = find_block_index(note->target);
            auto& left_gap_width = block_gap[target];
            auto& right_gap_width = block_gap[target + 1];
            left_gap_width = std::max({left_gap_width, left});
            right_gap_width = std::max({right_gap_width, right});
        };

        for(auto keyword: key_words) {
            if     (typeid(*keyword) == typeid(Title))     { title_handler(keyword); }
            else if(typeid(*keyword) == typeid(ArrowLine)) { arrow_line_handler(keyword); }
            else if(typeid(*keyword) == typeid(Block))     { block_handler(keyword); }
            else if(typeid(*keyword) == typeid(Note))      { note_handler(keyword); }
        }

        layout.block_x = tools::partial_sum(block_gap);
        layout.block_height = tools::sum(actor_height, Layout::BOTTOM_ACTOR_MARGIN);
        layout.title_x = tools::sum(block_gap, 0.) / 2.;
        layout.title_y = layout.block_height + Layout::TOP_TITLE_MARGIN;
        layout.actor_y = tools::fmap([block_height=layout.block_height](auto y){ return block_height-y; }, tools::partial_sum(actor_height));
    }

    static size_t find_block_index(Block* block) {
        return std::find_if(blocks.begin(), blocks.end(), [block](auto other_block){return *block==*other_block;}) - blocks.begin();
    };

    static bool has_block(Block* block) {
        if(find_block_index(block) != blocks.size()) {
            return true;
        }
        return false;
    }
};

#endif //SEQUENCE_DIAGRAM_DRAWER_COMPOSER_H
