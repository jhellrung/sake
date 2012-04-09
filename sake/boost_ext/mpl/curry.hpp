/*******************************************************************************
 * sake/boost_ext/mpl/curry.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::curry[n]<F>
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_MPL_CURRY_HPP
#define SAKE_BOOST_EXT_MPL_CURRY_HPP

#include <boost/mpl/apply.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class F >
struct curry0
    : boost::mpl::apply0<F>
{ };

template< class F >
struct curry1
{
    template< class T0 >
    struct apply
        : boost::mpl::apply1< F, T0 >
    { };
};

template< class F >
struct curry2
{
    template< class T0 >
    struct apply
    {
        struct _
        {
            template< class T1 >
            struct apply
                : boost::mpl::apply2< F, T0, T1 >
            { };
        };
        typedef _ type;
    };
};

#if BOOST_MPL_LIMIT_METAFUNCTION_ARITY >= 3
#define BOOST_PP_FILENAME_1 <sake/boost_ext/mpl/curry.hpp>
#define BOOST_PP_ITERATION_LIMITS ( 3, BOOST_MPL_LIMIT_METAFUNCTION_ARITY )
#include BOOST_PP_ITERATE()
#endif // #if BOOST_MPL_LIMIT_METAFUNCTION_ARITY >= 3

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_CURRY_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< class F >
struct BOOST_PP_CAT( curry, N )
{
    template< class T0 >
    struct apply
    {
        struct _
        {
            template< BOOST_PP_ENUM_SHIFTED_PARAMS( N, class T ) >
            struct apply
                : boost::mpl::BOOST_PP_CAT( apply, N )< F, BOOST_PP_ENUM_PARAMS( N, T ) >
            { };
        };
        typedef boost_ext::mpl::BOOST_PP_CAT( curry, BOOST_PP_DEC( N ) )<_> type;
    };
};

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
