/*******************************************************************************
 * sake/boost_ext/mpl/compose.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::compose<F,G>
 *
 * This composes two Boost.MPL metafunctions.
 *
 * If
 *     typedef boost_ext::mpl::compose<F,G> h;
 * then h is equivalent to
 *
 * struct h
 * {
 *     template< class T0 = unspecified, ... >
 *     struct apply
 *         : boost::mpl::apply<
 *               F,
 *               typename boost::mpl::apply< G, T0, ... >::type
 *           >
 *     { };
 * };
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_MPL_COMPOSE_HPP
#define SAKE_BOOST_EXT_MPL_COMPOSE_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class F, class G >
struct compose;

namespace compose_private
{

struct void_;

template<
    class F, class G,
    BOOST_PP_ENUM_BINARY_PARAMS(
        BOOST_MPL_LIMIT_METAFUNCTION_ARITY,
        class T, = void_ BOOST_PP_INTERCEPT
    )
>
struct apply;

#define BOOST_PP_FILENAME_1       <sake/boost_ext/mpl/compose.hpp>
#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_MPL_LIMIT_METAFUNCTION_ARITY )
#include BOOST_PP_ITERATE()

} // namespace compose_private

template< class F, class G >
struct compose
{
    template<
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_MPL_LIMIT_METAFUNCTION_ARITY,
            class T, = compose_private::void_ BOOST_PP_INTERCEPT
        )
    >
    struct apply
        : compose_private::apply<
              F, G,
              BOOST_PP_ENUM_PARAMS( BOOST_MPL_LIMIT_METAFUNCTION_ARITY, T )
          >
    { };
};

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_COMPOSE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define M BOOST_PP_SUB( BOOST_MPL_LIMIT_METAFUNCTION_ARITY, N )

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define comma_T0N       BOOST_PP_ENUM_TRAILING_PARAMS( N, T )

template< class F, class G comma_class_T0N >
#if M
struct apply< F, G comma_T0N, BOOST_PP_ENUM_PARAMS( M, void_ BOOST_PP_INTERCEPT ) >
#else // #if M
struct apply
#endif // #if M
    : boost::mpl::apply1<
          F,
          typename BOOST_PP_CAT( boost::mpl::apply, N ) < G comma_T0N >::type
      >
{ };

#undef comma_class_T0N
#undef comma_T0N

#undef M

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
