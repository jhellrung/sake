/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/const.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CONST( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CONST( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CONST( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_CONST( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CONST( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_CONST( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CONST_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CONST_HPP

#include <boost/preprocessor/control/expr_iif.hpp>

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_CONST_const ( )
#define const_SAKE_BOOST_EXT_PP_KEYWORD_CONST ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CONST( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_CONST_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CONST( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_CONST )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CONST( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_CONST_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_CONST( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_CONST )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CONST( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CONST( X ), const )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_CONST( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CONST( X ), const )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CONST_HPP
