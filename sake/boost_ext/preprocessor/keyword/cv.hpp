/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/cv.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CV( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CV( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CV( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_CV( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_CV( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CV_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CV_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/preprocessor/keyword/const.hpp>
#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/volatile.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_CV_const    ( )
#define SAKE_BOOST_EXT_PP_KEYWORD_CV_volatile ( )
#define    const_SAKE_BOOST_EXT_PP_KEYWORD_CV ( )
#define volatile_SAKE_BOOST_EXT_PP_KEYWORD_CV ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CV( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_CV_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CV( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_CV )
// 2 rounds to remove both "const" and "volatile"
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CV( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( \
    X, \
    SAKE_BOOST_EXT_PP_KEYWORD_CV_ ), \
    SAKE_BOOST_EXT_PP_KEYWORD_CV_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_CV( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_CV )

#define SAKE_BOOST_EXT_PP_KEYWORD_CV_00
#define SAKE_BOOST_EXT_PP_KEYWORD_CV_10 const
#define SAKE_BOOST_EXT_PP_KEYWORD_CV_01 volatile

#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV( X ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_, \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CONST   ( X ), \
                  SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_VOLATILE( X ) ) ) ( X )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_00( X )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_10( X ) \
    const SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_I( \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CONST( X ) )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_01( X ) \
    volatile SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_I( \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_VOLATILE( X ) )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_CV_I( X ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_CV_, \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CONST   ( X ), \
                  SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_VOLATILE( X ) ) )

#define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_CV( X ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_CV_, \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_CONST   ( X ), \
                  SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_VOLATILE( X ) ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_CV_HPP
