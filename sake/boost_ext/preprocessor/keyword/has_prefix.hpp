/*******************************************************************************
 * boost_ext/preprocessor/keyword/has_prefix.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, nullary_when_cat_with_suffix_keyword )
 *
 * This expands to either 0 or 1, indicating whether a sequence of preprocessor
 * tokens begins with a particular keyword.  The keyword is identified by the
 * nullary_when_cat_with_suffix_keyword parameter, which must expand to "( )"
 * (without quotes) when BOOST_PP_CAT'ed with the keyword as a suffix.
 *
 * Example:
 *
 * #define XYZ_xyz ( )
 * SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( xyz, XYZ_ ) // 1
 * SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( abc, XYZ_ ) // 0
 * SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( xyz abc, XYZ_ ) // 1
 * SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( abc xyz, XYZ_ ) // 0
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_PREFIX_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_PREFIX_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_nullary.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, nullary_when_cat_with_suffix_keyword ) \
    BOOST_PP_IS_NULLARY( BOOST_PP_CAT( nullary_when_cat_with_suffix_keyword, X ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_HAS_PREFIX_HPP
