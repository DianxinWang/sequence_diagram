//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_DRAWING_BOARD_H
#define SEQUENCE_DIAGRAM_DRAWER_DRAWING_BOARD_H

#include <iostream>
#include <fstream>
#include <memory>

#include <boost/geometry.hpp>


namespace geo {
class DrawingBoard {
public:
    using P = boost::geometry::model::d2::point_xy<double>;
    using mapper_type = boost::geometry::svg_mapper<P>;

    DrawingBoard()  {
        svg = std::make_unique<std::ofstream>("result.svg");
        mapper = std::make_unique<boost::geometry::svg_mapper<P>>(*svg, 400, 400);
    }

    void text(double x, double y, const std::string& text) {
        auto style = "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2";
        mapper->text(P{x, y}, text, style);
    }

    template<template <class> class Geometry, class ...Targs>
    void draw(Targs&&... args) {
        auto geometry = Geometry<P>(std::forward<Targs>(args)...);
        mapper->add(geometry.get());
        map_functions.push_back([geometry](mapper_type& _mapper){_mapper.map(geometry.get(), geometry.style);});
    }

    template <template <class> class  Rectangle, template <class> class  Line>
    void draw_block(double x, double height) {
        draw<Rectangle>(P{x, 0}, P{x + 3, 3});
        draw<Rectangle>(P{x, height}, P{x + 3, height + 3});
        draw<Line>(P{x, 0}, P{x, height});
    }

    template <template <class> class  Rectangle, template <class> class  Line>
    void draw_block_with_text(double x, double height, std::string content) {
        draw_block<Rectangle, Line>(x, height);
        text(x, height, content);
    }

    template <template <class> class  Line>
    void draw_arrow_line(double x1, double x2, double y) {
        draw<Line>(P{x1, y}, P{x2, y});
    }

    template <template <class> class  Rectangle>
    void draw_title(double x, double y, std::string content) {
        draw<Rectangle>(P{x, y}, P{x + 3, y + 3});
//        text(x, y, content);
    }
    ~DrawingBoard()  {
        for (const auto& func: map_functions) {
            func(*mapper);
        }
        mapper = nullptr;
        svg = nullptr;
    }
    std::unique_ptr<mapper_type> mapper;
    std::vector<std::function<void(mapper_type&)>> map_functions;
    std::unique_ptr<std::ofstream> svg;
};
}

#endif //SEQUENCE_DIAGRAM_DRAWER_DRAWING_BOARD_H
