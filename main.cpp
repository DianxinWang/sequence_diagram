
#include "keyword_factory.h"
#include "composer.h"

int main()
{
    SyntaxHandler::handle_one_line("Title A");
    SyntaxHandler::handle_one_line("A ->B:aaa");
    SyntaxHandler::handle_one_line("A .>B: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    SyntaxHandler::handle_one_line("A .>C: aaa");
    SyntaxHandler::handle_one_line("note over B: sdfdkfjsdf");
    Composer::add_keywords(SyntaxHandler::keywords);
    auto drawing_board = geo::DrawingBoard();
    Composer::draw(drawing_board);
    return 0;
}