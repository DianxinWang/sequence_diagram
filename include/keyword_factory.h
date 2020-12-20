//
// Created by dianxin on 2020/12/18.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_KEYWORD_FACTORY_H
#define SEQUENCE_DIAGRAM_DRAWER_KEYWORD_FACTORY_H

#include <map>
#include <memory>
#include <string_view>
#include <functional>
#include <cxxabi.h>
#include <regex>
#include <utility>
#include <set>
#include <algorithm>
#include <iostream>

class Keyword: public std::enable_shared_from_this<Keyword> {
public:;
    virtual void print(){};
    virtual std::vector<Keyword*> get_keywords_ptr() {
        return {this};
    }
    bool is_drawable = false;
    std::string one_line;
};

class InvalidKeyword: public Keyword {
public:
    virtual void print(){
        std::cout << "invalid print" << std::endl;
    };
};

template <typename ...Targs>
class KeywordFactory {
public:
    static KeywordFactory* Instance() {
        if(nullptr == _p_keyword_factory) {
            _p_keyword_factory = new KeywordFactory{};
        }
        return _p_keyword_factory;
    }

    virtual ~KeywordFactory()=default;

    bool register_constructor(std::string_view type_name, std::function<std::vector<Keyword*>(Targs&&...)> constructor) {
        if(nullptr == constructor) {
            return false;
        }
        return _map_constructor.insert(std::pair{type_name, constructor}).second;
    }

    std::vector<Keyword*> create(std::string_view type_name, Targs&&... args) {
        for (auto& [regex, func]: _map_constructor) {
            if(std::regex_match(type_name.data(), std::regex{regex})) {
                return func(std::forward<Targs>(args)...);
            }
        }
        return {new InvalidKeyword()};
    }
private:
    KeywordFactory()=default;
    static KeywordFactory<Targs...>* _p_keyword_factory;
    std::map<std::string, std::function<std::vector<Keyword*>(Targs...)>> _map_constructor;
};

template<typename ...Targs>
KeywordFactory<Targs...>* KeywordFactory<Targs...>::_p_keyword_factory = nullptr;

template<typename T, typename ...Targs>
class DynamicCreator {
public:
    struct Register {
        Register() {
            KeywordFactory<Targs...>::Instance()->register_constructor(T::syntax, _create_object);
        }
        constexpr void do_nothing() const {};
    };
    DynamicCreator() {
        _register.do_nothing();
    }
private:
    static auto _create_object(Targs&&... args) {
        auto key_word = new T(std::forward<Targs>(args)...);
        return key_word->get_keywords_ptr();
    }
    static Register _register;
};

template<typename T, typename ...Targs>
typename DynamicCreator<T, Targs...>::Register DynamicCreator<T, Targs...>::_register{};


class SyntaxHandler {
public:
    template<typename ...Targs>
    static std::vector<Keyword*> create_keyword(std::string_view type_name, Targs&&... args) {
        return KeywordFactory<std::string_view>::Instance()->create(type_name, std::forward<Targs>(args)...);
    }
    static void handle_one_line(std::string_view one_line) {
        auto result = create_keyword(one_line.data(), one_line.data());
        keywords.insert(keywords.end(), result.begin(), result.end());
    }
    // Temporary use
    static void print_result() {
        for(auto keyword: keywords) {
            keyword->print();
        }
    }
    inline static std::vector<Keyword*> keywords = {};
};




#endif //SEQUENCE_DIAGRAM_DRAWER_KEYWORD_FACTORY_H
