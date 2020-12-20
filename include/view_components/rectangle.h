//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
#define SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
#include <iostream>
#include <fstream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


namespace geo {
template<class point_type>
class Rectangle {
public:
    point_type left_up_corner {};
    point_type right_bottom_corner {};
    std::string style{"fill-opacity:0.3;fill:rgb(51,51,153);stroke:rgb(51,51,153);stroke-width:2"};

    [[nodiscard]] auto get() const {
        boost::geometry::model::polygon<point_type> b;
        auto p1 = left_up_corner;
        auto p2 = point_type{left_up_corner.x(), right_bottom_corner.y()};
        auto p3 = right_bottom_corner;
        auto p4 = point_type{right_bottom_corner.x() , left_up_corner.y()};
        b.outer().push_back(p1);
        b.outer().push_back(p2);
        b.outer().push_back(p3);
        b.outer().push_back(p4);
        return b;
    }

    Rectangle(const point_type &leftUpCorner, const point_type &rightBottomCorner) : left_up_corner(leftUpCorner),
                                                                                     right_bottom_corner(
                                                                                             rightBottomCorner) {}
};
}
#endif //SEQUENCE_DIAGRAM_DRAWER_RECTANGLE_H
