//
// Created by dianxin on 2020/12/18.
//
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "keyword_factory.h"

int add( int i, int j ) { return i+j; }

template<typename ...Targs>
bool is_syntax_right(Targs&&... args) {
    return SyntaxHandler::create_keyword(std::forward<Targs>(args)...).back()->is_drawable;
}

BOOST_AUTO_TEST_CASE( my_test )
{
    SyntaxHandler::handle_one_line("Title a");
    SyntaxHandler::handle_one_line("A ->B:aaa");
    SyntaxHandler::handle_one_line("A .>B: aaa");
    SyntaxHandler::print_result();
    // seven ways to detect and report the same error:
    BOOST_CHECK( is_syntax_right("Title", "Title aa"));
    BOOST_CHECK( is_syntax_right("Title", "Title aa111 2"));
    BOOST_CHECK( !is_syntax_right("Title", "a Title aa111 2"));
    BOOST_CHECK( is_syntax_right("Title", " Title aa111 2"));

    BOOST_CHECK( is_syntax_right("ArrowLine", " ab -> cd:"));
    BOOST_CHECK( is_syntax_right("ArrowLine", " 1 ab -> cd: dd"));
    BOOST_CHECK( !is_syntax_right("ArrowLine", " 1 ab -> cd  dd"));
    BOOST_CHECK( !is_syntax_right("ArrowLine", " 1 ab --> cd  dd"));
    BOOST_CHECK( is_syntax_right("ArrowLine", " 1 ab --> cd  dd:"));

//    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error
//
//    if( add( 2,2 ) != 4 )
//        BOOST_ERROR( "Ouch..." );            // #3 continues on error
//
//    if( add( 2,2 ) != 4 )
//        BOOST_FAIL( "Ouch..." );             // #4 throws on error
//
//    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error
//
//    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
//                         "add(..) result: " << add( 2,2 ) );
//
//    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error
}