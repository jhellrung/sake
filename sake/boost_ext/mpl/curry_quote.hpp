/*******************************************************************************
 * sake/boost_ext/mpl/curry_quote.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::curry_quote[n]<F>
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_MPL_CURRY_QUOTE_HPP
#define SAKE_BOOST_EXT_MPL_CURRY_QUOTE_HPP

#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <sake/boost_ext/mpl/curry.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< template< class _0 > class F >
struct curry_quote1
    : boost::mpl::quote1<F>
{ };

#if BOOST_MPL_LIMIT_METAFUNCTION_ARITY >= 2
#define BOOST_PP_FILENAME_1 <sake/boost_ext/mpl/curry_quote.hpp>
#define BOOST_PP_ITERATION_LIMITS ( 2, BOOST_MPL_LIMIT_METAFUNCTION_ARITY )
#include BOOST_PP_ITERATE()
#endif // #if BOOST_MPL_LIMIT_METAFUNCTION_ARITY >= 2

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_CURRY_QUOTE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template< template< BOOST_PP_ENUM_PARAMS( N, _ ) > class F >
struct BOOST_PP_CAT( curry_quote, N )
    : boost_ext::mpl::BOOST_PP_CAT( curry, N )< boost::mpl::BOOST_PP_CAT( quote, N )<F> >
{ };

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
