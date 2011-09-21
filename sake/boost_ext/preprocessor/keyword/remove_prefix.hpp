/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/remove_prefix.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, nullary_when_cat_with_suffix_keyword )
 *
 * This removes a keyword from the front of a sequence of preprocessor tokens.
 * The keyword is identified by the nullary_when_cat_with_suffix_keyword
 * parameter, which must expand to "( )" (without quotes) when BOOST_PP_CAT'ed
 * with the keyword as a suffix.
 *
 * Example:
 *
 * #define XYZ_xyz ( )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( xyz, XYZ_ ) // (empty)
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( abc, XYZ_ ) // abc
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( xyz abc, XYZ_ ) // abc
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( abc xyz, XYZ_ ) // abc xyz
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_PREFIX_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_PREFIX_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, nullary_when_cat_with_suffix_keyword ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_, \
        SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, nullary_when_cat_with_suffix_keyword ) \
    ) ( X, nullary_when_cat_with_suffix_keyword )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_0( X, nullary_when_cat_with_suffix_keyword ) \
    X
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_1( X, nullary_when_cat_with_suffix_keyword ) \
    BOOST_PP_EXPAND( BOOST_PP_EMPTY BOOST_PP_CAT( nullary_when_cat_with_suffix_keyword, X ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_PREFIX_HPP
