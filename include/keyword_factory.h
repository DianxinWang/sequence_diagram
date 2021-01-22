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
    virtual void print()=0;
    virtual std::vector<Keyword*> get_keywords_ptr()=0;
    bool is_drawable = false;
};

class InvalidKeyword: public Keyword {
public:
    std::vector<Keyword *> get_keywords_ptr() override {
        return {this};
    }
    void print() override{
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

    void register_constructor(std::function<std::vector<Keyword*>(Targs&&...)> constructor) {
        if(nullptr != constructor) {
            _create_funcs.push_back(constructor);
        }
    }

    std::vector<Keyword*> create(const std::string& type_name, Targs&&... args) {
        for (auto& func: _create_funcs) {
            auto keys = func(std::forward<Targs>(args)...);
            if(!keys.empty()) {
                return keys;
            }
        }
        return {new InvalidKeyword()};
    }
private:
    KeywordFactory()=default;
    static KeywordFactory<Targs...>* _p_keyword_factory;
    std::vector<std::function<std::vector<Keyword*>(Targs...)>> _create_funcs;
};

template<typename ...Targs>
KeywordFactory<Targs...>* KeywordFactory<Targs...>::_p_keyword_factory = nullptr;

template<typename T, typename ...Targs>
class DynamicCreator {
public:
    struct Register {
        Register() {
            KeywordFactory<Targs...>::Instance()->register_constructor(_create_object);
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
    static std::vector<Keyword*> create_keyword(const std::string& type_name, Targs&&... args) {
        return KeywordFactory<const std::string&>::Instance()->create(type_name, std::forward<Targs>(args)...);
    }
    static void handle_one_line(const std::string& one_line) {
        auto result = create_keyword(one_line, one_line);
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
