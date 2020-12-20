//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H
#define SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H

#include <iostream>
#include <fstream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


namespace geo {
template<class point_type>
class StraightLine {
public:
    point_type source {};
    point_type destination {};
    std::string style{"opacity:0.4;fill:none;stroke:rgb(212,0,0);stroke-width:5"};

    [[nodiscard]] auto get() const {
        boost::geometry::model::linestring<point_type> c;
        c.push_back(source);
        c.push_back(destination);
        return c;
    }

    StraightLine(const point_type &source, const point_type &destination) : source(source), destination(destination) {}
};
}
#endif //SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H
