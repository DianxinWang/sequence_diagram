//
// Created by dianxin on 2020/12/21.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_WITH_MARKER_H
#define SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_WITH_MARKER_H

#include "view_components/straight_line.h"
#include <boost/geometry.hpp>

namespace geo {

template<class PointType>
class StraightLineWithArrow {
public:
    PointType src {};
    PointType dest {};
    std::string style{"stroke:#6666ff;stroke-width:1px;fill:none;marker-start:url(#markerCircle);marker-end:url(#markerArrow);"};
    StraightLineWithArrow(const PointType &source, const PointType &destination): src(source), dest(destination) {}
    [[nodiscard]] std::string get_style() const {
        return style;
    }

    [[nodiscard]] auto get_geometry() const {
        boost::geometry::model::linestring<PointType> c;
        c.push_back(src);
        c.push_back(dest);
        return c;
    }
};
}

#endif //SEQUENCE_DIAGRAM_DRAWER_STRAIGHT_LINE_WITH_MARKER_H
