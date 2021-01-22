//
// Created by dianxin on 2020/12/19.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H
#define SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H

#include <iostream>
#include <fstream>
#include <boost/geometry.hpp>

namespace geo {
template<class PointType>
class StraightLine {
public:
    PointType src {};
    PointType dest {};
    std::string style{"opacity:0.4;fill:none;stroke:rgb(212,0,0);stroke-width:5"};

    [[nodiscard]] std::string get_style() const {
        return style;
    }

    [[nodiscard]] auto get_geometry() const {
        boost::geometry::model::linestring<PointType> c;
        c.push_back(src);
        c.push_back(dest);
        return c;
    }


    template<class GeometryType>
    StraightLine(const GeometryType &source, const GeometryType &destination) {
        auto whole_line = StraightLine(source.get_center(), destination.get_center());
        src = source.intersect_point(whole_line);
        dest = destination.intersect_point(whole_line);
    }

    StraightLine(const PointType &source, const PointType &destination): src(source), dest(destination) {}

    template<class Geometry>
    PointType intersect_point(const Geometry& geometry) const {
        return {};
    }

    std::vector<PointType> intersect_point(const StraightLine& line) const {
        std::vector<PointType> output;
        boost::geometry::intersection(this->get_geometry(), line.get_geometry(), output);
        return output;
    }

    PointType get_center() const {
        return boost::geometry::return_centroid(get_geometry());
    }
};
}
#endif //SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_H
