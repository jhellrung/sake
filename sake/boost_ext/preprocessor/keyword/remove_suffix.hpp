/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/remove_suffix.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX( X, nullary_when_cat_with_prefix_keyword )
 *
 * This removes a single preprocessor token if it is a particular keyword via
 * "back concatenation", which may be necessary when the token could start with
 * a symbol ("+", "-", etc.) causing "front concatenation" to fail.  The keyword
 * is identified by the nullary_when_cat_with_prefix_keyword parameter, which
 * must expand to "( )" (without quotes) when BOOST_PP_CAT'ed with the keyword
 * as a prefix.
 *
 * Example:
 *
 * #define xyz_XYZ ( )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( xyz, _XYZ ) // (empty)
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( abc, _XYZ ) // abc
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( +xyz, _XYZ ) // +xyz
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( +abc, _XYZ ) // +abc
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_SUFFIX_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_SUFFIX_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/preprocessor/keyword/has_suffix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX( X, nullary_when_cat_with_prefix_keyword ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_, \
        SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, nullary_when_cat_with_prefix_keyword ) \
    ) ( X, nullary_when_cat_with_prefix_keyword )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_0( X, nullary_when_cat_with_prefix_keyword ) \
    X
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_1( X, nullary_when_cat_with_prefix_keyword )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_REMOVE_SUFFIX_HPP
