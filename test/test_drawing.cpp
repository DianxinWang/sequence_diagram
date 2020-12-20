//
// Created by dianxin on 2020/12/19.
//
#include "view_components/rectangle.h"
#include "view_components/straight_line.h"
#include "drawing_board.h"

int main()
{

    auto drawing_board = geo::DrawingBoard();
    typedef geo::DrawingBoard::point_type P;
    drawing_board.draw<geo::StraightLine>(P{1,2}, P{3,4});
    drawing_board.draw<geo::Rectangle>(P{5,5}, P{11,11});
    return 0;
}