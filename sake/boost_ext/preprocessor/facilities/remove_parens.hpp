/*******************************************************************************
 * sake/boost_ext/preprocessor/facilities/remove_parens.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_REMOVE_PARENS( x )
 *
 * Expands to x with any outer-level parentheses removed.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_MACROS

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/preprocessor/facilities/is_variadic.hpp>

#define SAKE_BOOST_EXT_PP_REMOVE_PARENS( x ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_REMOVE_PARENS_, \
        SAKE_BOOST_EXT_PP_IS_VARIADIC( x ) \
    ) ( x )
#define SAKE_BOOST_EXT_PP_REMOVE_PARENS_0( x ) x
#define SAKE_BOOST_EXT_PP_REMOVE_PARENS_1( x ) \
    SAKE_BOOST_EXT_PP_REMOVE_PARENS_1_impl x
#define SAKE_BOOST_EXT_PP_REMOVE_PARENS_1_impl( ... ) __VA_ARGS__

#endif // #ifndef BOOST_NO_VARIADIC_MACROS

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_FACILITIES_REMOVE_PARENS_HPP
