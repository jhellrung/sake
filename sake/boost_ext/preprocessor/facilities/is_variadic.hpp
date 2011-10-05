/*******************************************************************************
 * sake/boost_ext/preprocessor/facilities/is_variadic.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_IS_VARIADIC( x )
 *
 * Expands to 1 iff x is variadic, i.e., a comma-separated list of tokens
 * wrapped in parentheses; otherwise, expands to 0.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_IS_VARIADIC_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_IS_VARIADIC_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_MACROS

#define SAKE_BOOST_EXT_PP_IS_VARIADIC( x ) \
    SAKE_BOOST_EXT_PP_IS_VARIADIC_impl0( ( SAKE_BOOST_EXT_PP_IS_VARIADIC_detect x ) )
#define SAKE_BOOST_EXT_PP_IS_VARIADIC_detect( ... ) ~, ~
#define SAKE_BOOST_EXT_PP_IS_VARIADIC_impl0( x ) \
    SAKE_BOOST_EXT_PP_IS_VARIADIC_impl1 x
#define SAKE_BOOST_EXT_PP_IS_VARIADIC_impl1( ... ) \
    SAKE_BOOST_EXT_PP_IS_VARIADIC_impl2( ( __VA_ARGS__, 1, 0 ) )
#define SAKE_BOOST_EXT_PP_IS_VARIADIC_impl2( x ) \
    SAKE_BOOST_EXT_PP_IS_VARIADIC_impl3 x \
#define SAKE_BOOST_EXT_PP_IS_VARIADIC_impl3( A, B, N, ... ) N

#endif // #ifndef BOOST_NO_VARIADIC_MACROS

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_IS_VARIADIC_HPP
