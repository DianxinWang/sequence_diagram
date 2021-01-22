//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
#define SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
#include <iostream>
#include <fstream>

#include <boost/geometry.hpp>

namespace geo {
template<class PointType>
class Rectangle {
public:
    using RectangleType = boost::geometry::model::polygon<PointType>;
    PointType left_up_corner {};
    PointType right_bottom_corner {};
    std::string style{"fill-opacity:0.3;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:2"};

    [[nodiscard]] std::string get_style() const {
        return style;
    }

    [[nodiscard]] auto get_geometry() const {
        RectangleType b;
        auto p1 = left_up_corner;
        auto p2 = PointType{left_up_corner.x(), right_bottom_corner.y()};
        auto p3 = right_bottom_corner;
        auto p4 = PointType{right_bottom_corner.x() , left_up_corner.y()};
        b.outer().push_back(p1);
        b.outer().push_back(p2);
        b.outer().push_back(p3);
        b.outer().push_back(p4);
        return b;
    }

    Rectangle(const PointType &leftUpCorner, const PointType &rightBottomCorner) : left_up_corner(leftUpCorner),
                                                                                   right_bottom_corner(
                                                                                             rightBottomCorner) {}
    PointType get_center() const {
        return boost::geometry::return_centroid<PointType>(get_geometry());
    }

    template<class LineType, int line_index>
    LineType get_line() const {
        std::array<PointType, 4> points = {left_up_corner,
                                           PointType{left_up_corner.x(), right_bottom_corner.y()},
                                           right_bottom_corner,
                                           PointType{right_bottom_corner.x() , left_up_corner.y()}};
        return LineType{points[line_index%4], points[(line_index+1)%4]};
    }

    template<class LineType>
    std::vector<PointType> intersect_point(const LineType& line) const {
        auto result = line.intersect_point(get_line<LineType, 0>());
        auto tmp = line.intersect_point(get_line<LineType, 1>());
        auto tmp1 = line.intersect_point(get_line<LineType, 2>());
        auto tmp2 = line.intersect_point(get_line<LineType, 3>());
        std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
        std::copy(tmp1.begin(), tmp1.end(), std::back_inserter(result));
        std::copy(tmp2.begin(), tmp2.end(), std::back_inserter(result));
        return result;
    }
};
}
#endif //SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
