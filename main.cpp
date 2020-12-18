#include <iostream>
#include "keyword_factory.h"



int main() {
    auto b = SyntaxHandler::create_keyword("Title", std::string(""));
    std::cout << std::get<1>(b->check_syntax()) << std::endl;
    auto one_line = std::string("Title 123");
    auto a = SyntaxHandler::create_keyword("Title", one_line);
    std::cout << std::get<1>(a->check_syntax()) << std::endl;
    return 0;
}
