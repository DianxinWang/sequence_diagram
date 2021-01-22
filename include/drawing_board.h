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
    template<class Geometry, class ...Targs>
    void draw(const std::string& style, Targs&&... args) {
        auto geometry = Geometry(std::forward<Targs>(args)...);
        mapper->add_geometry(geometry.get_geometry(), style);
    }

public:
    using P = boost::geometry::model::d2::point_xy<double>;
    enum class Align {
        left, right, over
    };
    class SvgMapper: private boost::geometry::svg_mapper<P> {
    public:
        std::ofstream stream;
        std::vector<std::function<void(SvgMapper*)>> map_functions;

        SvgMapper(const std::string& output, double width, double height,
                  const std::string &widthHeight= R"(width="100%" height="100%")") : stream(output), svg_mapper(
                stream, width, height, widthHeight) {
            add_text(0, 0, "");
            map_functions.emplace_back([](SvgMapper* _mapper){
                _mapper->stream <<
                                "<defs>\n"
                                "    <marker id=\"markerCircle\" markerWidth=\"8\" markerHeight=\"8\" refX=\"5\" refY=\"5\">\n"
                                "        <circle cx=\"5\" cy=\"5\" r=\"3\" style=\"stroke: none; fill:#000000;\"/>\n"
                                "    </marker>\n"
                                "\n"
                                "    <marker id=\"markerArrow\" markerWidth=\"13\" markerHeight=\"13\" refX=\"2\" refY=\"6\"\n"
                                "           orient=\"auto\">\n"
                                "        <path d=\"M2,2 L2,11 L10,6 L2,2\" style=\"fill: #000000;\" />\n"
                                "    </marker>\n"
                                "</defs>";
            });
        }

        ~SvgMapper() override {
            for (const auto& func: map_functions) {
                func(this);
            }
            stream << "</svg>"; // This is because stream will be closed before svg_mapper
        }

        template <typename Geometry>
        void add_geometry(const Geometry& geometry, const std::string& style) {
            add(geometry);
            map_functions.emplace_back([geometry, style](SvgMapper* _mapper){
                _mapper->map(geometry, style);
            });
        }

        void add_text(double x, double y, const std::string& text) {
            map_functions.emplace_back([text, x, y](SvgMapper* _mapper){
                auto style = "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2";
                _mapper->text(P{x, y}, text, style);});
        }
    };
    std::unique_ptr<SvgMapper> mapper;
    DrawingBoard()  {
        mapper = std::make_unique<SvgMapper>("result.svg", 400, 400);
    }

    ~DrawingBoard()  {
        mapper = nullptr;
    }


    template <template <class> class  RectangleType, template <class> class  Line>
    void draw_block(double x, double height) {
        auto bottom_rect = RectangleType(P{x-1, -1}, P{x + 1, 1});
        auto top_rect = RectangleType(P{x-1, height-1}, P{x + 1, height + 1});
        auto style = std::string{"stroke:#000000;stroke-width:2;fill:#fff"};
        draw<RectangleType<P>>(style, P{x-1, -1}, P{x + 1, 1});
        draw<RectangleType<P>>(style, P{x-1, height-1}, P{x + 1, height + 1});
        draw_line<RectangleType, Line>(bottom_rect, top_rect);
    }

    template <template <class> class  Rectangle, template <class> class  Line>
    void draw_block_with_text(double x, double height, const std::string& content) {
        draw_block<Rectangle, Line>(x, height);
        mapper->add_text(x, height, content);
    }

    template <template <class> class  Line>
    void draw_line(double x1, double x2, double y) {
        std::string style="opacity:0.4;fill:none;stroke:rgb(212,0,0);stroke-width:5";
        draw<Line<P>>(style, P{x1, y}, P{x2, y});
    }

    template <template <class> class  Line>
    void draw_arrow_line(double x1, double x2, double y) {
        std::string style="stroke:#000000;stroke-width:2;fill:none;marker-end:url(#markerArrow);";
        draw<Line<P>>(style, P{x1, y}, P{x2, y});
    }

    template <template <class> class  Geometry, template <class> class Line>
    void draw_line(const Geometry<P>& g1, const Geometry<P>& g2) {
        auto whole_line = Line(g1.get_center(), g2.get_center());
        auto src = g1.intersect_point(whole_line);
        auto dest = g2.intersect_point(whole_line);
        auto style = std::string{"fill:none;stroke:#000000;stroke-width:2"};
        assert(src.size() >= 1 && dest.size() >= 1);
        draw<Line<P>>(style, src[0], dest[0]);
    }

    template <template <class> class  Rectangle>
    void draw_title(double x, double y, const std::string& content) {
        auto style = std::string{"fill-opacity:0.0;fill:rgb(0,0,0);stroke:rgb(0,0,0);stroke-width:0"};
        draw<Rectangle<P>>(style, P{x, y}, P{x + 3, y + 3});
        mapper->add_text(x, y, content);
    }

    template <template <class> class  Rectangle>
    void draw_note(double x, double y, const std::string& content) {
        auto style = std::string{"fill-opacity:0.3;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:2"};
        draw<Rectangle<P>>(style, P{x, y}, P{x + 3, y + 3});
        mapper->add_text(x, y, content);
    }
};
}

#endif //SEQUENCE_DIAGRAM_DRAWER_DRAWING_BOARD_H
