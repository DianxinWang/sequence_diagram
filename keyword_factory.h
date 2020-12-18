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

class Keyword: public std::enable_shared_from_this<Keyword> {
public:

    virtual std::tuple<bool, std::string> check_syntax()=0;
    virtual void pre_draw(){};
    virtual void draw(){};
    bool is_drawable = false;
};

class InvalidKeyword: public Keyword {
public:
    std::tuple<bool, std::string> check_syntax() override {
        return {false, "invalid keyword"};
    }
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

    bool register_constructor(std::string_view type_name, std::function<Keyword*(Targs&&...)> constructor) {
        if(nullptr == constructor) {
            return false;
        }
        return _map_constructor.insert(std::pair{type_name, constructor}).second;
    }

    Keyword* create(std::string type_name, Targs&&... args) {
        auto iter = _map_constructor.find(type_name);
        if(iter != _map_constructor.end()) {
            return iter->second(std::forward<Targs>(args)...);
        }
        return new InvalidKeyword();
    }
private:
    KeywordFactory()=default;
    static KeywordFactory<Targs...>* _p_keyword_factory;
    std::map<std::string, std::function<Keyword*(Targs...)>> _map_constructor;
};

template<typename ...Targs>
KeywordFactory<Targs...>* KeywordFactory<Targs...>::_p_keyword_factory = nullptr;

template<typename T, typename ...Targs>
class DynamicCreator {
public:
    struct Register {
        Register() {
            std::string type_name;
            char *sz_demangle_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
            if(nullptr != sz_demangle_name) {
                type_name = sz_demangle_name;
                free(sz_demangle_name);
            }
            KeywordFactory<Targs...>::Instance()->register_constructor(type_name, _create_object);
        }
        constexpr void do_nothing() const {};
    };
    DynamicCreator() {
        _register.do_nothing();
    }
private:
    static T* _create_object(Targs&&... args) {
        return new T(std::forward<Targs>(args)...);
    }
    static Register _register;
};

template<typename T, typename ...Targs>
typename DynamicCreator<T, Targs...>::Register DynamicCreator<T, Targs...>::_register{};

class Title;
class ArrowLine;

class Block {
public:
    explicit Block(std::string_view total_line): one_line(total_line) {}

    std::string one_line;
    bool operator<(const Block& rhs) const {
        return one_line < rhs.one_line;
    }
    std::ostream& operator<<(std::ostream& out) const {
        return out << one_line;
    }
};


class SyntaxHandler {
public:
    template<typename ...Targs>
    static Keyword* create_keyword(const std::string& type_name, Targs&&... args) {
        return KeywordFactory<std::string_view>::Instance()->create(type_name, std::forward<Targs>(args)...);
    }
    static void handle_one_line(std::string_view one_line) {

    }
    inline static std::set<Block> block_set = {};
};


class Title: public Keyword, public DynamicCreator<Title, std::string_view> {
public:
    explicit Title(std::string_view total_line): one_line(total_line) {
        is_drawable = std::get<0>(Title::check_syntax());
    }
    std::tuple<bool, std::string> check_syntax() override {
        if(std::regex_match(one_line, syntax)) {
            return {true, "good"};
        }
        return {false, "require title name"};
    }
    std::string one_line;
    const inline static std::regex syntax{R"(^\s*\bTitle\b\s+(.+?)\s*$)"};
    std::ostream& operator<<(std::ostream& out) const {
        return out << one_line;
    }
};

class ArrowLine: public Keyword, public DynamicCreator<ArrowLine, std::string_view> {
public:
    explicit ArrowLine(std::string_view total_line): one_line(total_line) {
        is_drawable = std::get<0>(ArrowLine::check_syntax());
    }
    std::tuple<bool, std::string> check_syntax() override {
        if(std::regex_match(one_line, syntax)) {
            return {true, "good"};
        }
        return {false, "require title name"};
    }

    void pre_draw() override {
        if (is_drawable) {
            return;
        }
        auto [source, destination] = get_source_and_destination();
        SyntaxHandler::block_set.emplace(source);
        SyntaxHandler::block_set.emplace(destination);
    }

    std::tuple<std::string, std::string> get_source_and_destination() {
        std::smatch result;
        if(std::regex_match(one_line, result, syntax)) {
            return {result.str(1), result.str(2)};
        }
        return {"", ""};
    }

    std::string one_line;
    const inline static std::regex syntax{R"(^\s*(.+?)\s*-?->(.+)\s*:\s*(.*?)$)"};
};

void debug_print_all_blocks() {
    for (auto& block:SyntaxHandler::block_set) {
        std::cout << block;
    }
    std::copy(std::begin(SyntaxHandler::block_set), std::end(SyntaxHandler::block_set), std::ostream_iterator<Block>(std::cout, " "));
}

#endif //SEQUENCE_DIAGRAM_DRAWER_KEYWORD_FACTORY_H
