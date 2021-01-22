//
// Created by dianxin on 2021/1/2.
//

#ifndef SEQUENCE_DIAGRAM_DRAWER_CANDY_H
#define SEQUENCE_DIAGRAM_DRAWER_CANDY_H

#include <memory>
#include <utility>
#include <iostream>
#include <algorithm>
#include <iterator>
namespace tools {

struct sequence_tag {};
struct pointer_tag {};

template< class X >
X category( ... );

template< class S >
auto category( const S& s ) -> decltype( std::begin(s), std::end(s), sequence_tag() );

template< class Ptr >
auto category( const Ptr& p ) -> decltype( *p, p==nullptr, pointer_tag() );

template< class T > struct Category {
    using type = decltype( category<T>(std::declval<T>()) );
};

template< class R, class ... X > struct Category< R(&)(X...) > {
    using type = R(&)(X...);
};

template< class T >
using Cat = typename Category<T>::type;

template< class... > struct Functor;

template< class F, class FX, class Fun=Functor< Cat<FX> > >
auto fmap( F&& f, FX&& fx )
-> decltype( Fun::fmap( std::declval<F>(), std::declval<FX>() ) )
{
    return Fun::fmap( std::forward<F>(f), std::forward<FX>(fx) );
}

template< class F, class G >
struct Composition {
    F f;
    G g;

    template< class X >
    auto operator () ( X&& x ) -> decltype( f(g(std::declval<X>())) ) {
        return f(g(std::forward<X>(x)));
    }
};

// General case: composition
template< class Function > struct Functor<Function> {
    template< class F, class G, class C = Composition<F,G> >
    static C fmap( F f, G g ) {
        C( std::move(f), std::move(g) );
    }
};

template<> struct Functor< sequence_tag > {
    template< class F, template<class...>class S, class X,
            class R = typename std::result_of_t<F(X)> >
    static S<R> fmap( F&& f, const S<X>& s ) {
        S<R> r;
        r.reserve( s.size() );
        std::transform( std::begin(s), std::end(s),
                        std::back_inserter(r),
                        std::forward<F>(f) );
        return r;
    }
};

template<> struct Functor< pointer_tag > {
    template< class F, template<class...>class Ptr, class X,
            class R = typename std::result_of_t<F(X)> >
    static Ptr<R> fmap( F&& f, const Ptr<X>& p )
    {
        return p != nullptr
               ? Ptr<R>( new R( std::forward<F>(f)(*p) ) )
               : nullptr;
    }
};

auto sum = [](auto values, auto init_value) {
    return std::accumulate(begin(values), end(values), init_value);
};

auto partial_sum = [](auto input){
    decltype(input) res{};
    std::partial_sum(begin(input), end(input), std::back_inserter(res));
    return res;
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

}

#endif //SEQUENCE_DIAGRAM_DRAWER_CANDY_H
