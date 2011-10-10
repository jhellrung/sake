/*******************************************************************************
 * sake/core/keyword/satisfies.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_SATISFIES_HPP
#define SAKE_CORE_KEYWORD_SATISFIES_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/keyword/tagged_value.hpp>

namespace sake
{

namespace keyword
{

template< class A, class P >
struct satisfies;

namespace satisfies_private
{

template< class Tag, class Pred = void >
struct tag_counter
{
    template< class State, class >
    struct apply
    { typedef State type; };
    template< class State, class T >
    struct apply< State, keyword::tagged_value< Tag, T > >
    {
        static bool const b = boost::mpl::apply1<
            Pred, typename boost_ext::remove_rvalue_reference<T>::type
        >::type::value;
        typedef boost::mpl::int_< (b ? 1 + State::value : 2) > type;
    };
    template< class T >
    struct apply< boost::mpl::int_<2>, keyword::tagged_value< Tag, T > >
    { typedef boost::mpl::int_<2> type; };
};

template< class Tag >
struct tag_counter< Tag, void >
{
    template< class State, class >
    struct apply
    { typedef State type; };
    template< class State, class T >
    struct apply< State, keyword::tagged_value< Tag, T > >
    { typedef boost::mpl::int_< 1 + State::value > type; };
    template< class T >
    struct apply< boost::mpl::int_<2>, keyword::tagged_value< Tag, T > >
    { typedef boost::mpl::int_<2> type; };
};

template< class A >
struct pred
{
    template< class Tag >
    struct apply
        : apply< keyword::optional< Tag > >
    { };

    template< class Tag, class Pred >
    struct apply< keyword::optional< Tag, Pred > >
    {
        static bool const value = boost::mpl::fold<
            A,
            boost::mpl::int_<0>,
            tag_counter< Tag, typename boost::mpl::lambda< Pred >::type >
        >::type::value <= 1;
        typedef apply type;
    };

    template< class Tag, class Pred >
    struct apply< keyword::required< Tag, Pred > >
    {
        static bool const value = boost::mpl::fold<
            A,
            boost::mpl::int_<0>,
            tag_counter< Tag, typename boost::mpl::lambda< Pred >::type >
        >::type::value == 1;
        typedef apply type;
    };

    template< class Tag, class Pred >
    struct apply< keyword::optional< keyword::deduced< Tag >, Pred > >
        : apply< keyword::optional< Tag, Pred > >
    { };
    template< class Tag, class Pred >
    struct apply< keyword::required< keyword::deduced< Tag >, Pred > >
        : apply< keyword::required< Tag, Pred > >
    { };
};

} // namespace satisfies_private

template< class A, class P >
struct satisfies
    : boost_ext::mpl::all< P, satisfies_private::pred<A> >
{ };

template< class P >
struct satisfies< void, P >
    : boost::false_type
{ };

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_SATISFIES_HPP
