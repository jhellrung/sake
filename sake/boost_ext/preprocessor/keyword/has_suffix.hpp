/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/has_suffix.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, nullary_when_cat_with_prefix_keyword )
 *
 * This expands to either 0 or 1, indicating whether a single preprocessor token
 * is a particular keyword via "back concatenation", which may be necessary when
 * the token could start with a symbol ("+", "-", etc.) causing "front
 * concatenation" to fail.  The keyword is identified by the
 * nullary_when_cat_with_suffix_keyword parameter, which must expand to "( )"
 * (without quotes) when BOOST_PP_CAT'ed with the keyword as a suffix.
 *
 * Example:
 *
 * #define xyz_XYZ ( )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( xyz, _XYZ ) // 1
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( abc, _XYZ ) // 0
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( +xyz, _XYZ ) // 0
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( +abc, _XYZ ) // 0
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_SUFFIX_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_SUFFIX_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_nullary.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, nullary_when_cat_with_prefix_keyword ) \
    BOOST_PP_IS_NULLARY( BOOST_PP_CAT( X, nullary_when_cat_with_prefix_keyword ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_SUFFIX_HPP
