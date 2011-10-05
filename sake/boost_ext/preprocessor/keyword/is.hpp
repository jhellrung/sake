/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/is.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_IS( X, empty_when_cat_with_suffix_keyword )
 *
 * This expands to either 0 or 1, indicating whether a preprocessor token
 * is a particular keyword.  The keyword is identified by the
 * empty_when_cat_with_suffix_keyword parameter, which must expand to nothing
 * when BOOST_PP_CAT'ed with the keyword as a suffix.
 *
 * Example:
 *
 * #define XYZ_xyz
 * SAKE_BOOST_EXT_PP_KEYWORD_IS( xyz, XYZ_ ) // 1
 * SAKE_BOOST_EXT_PP_KEYWORD_IS( abc, XYZ_ ) // 0
 * SAKE_BOOST_EXT_PP_KEYWORD_IS( xyz abc, XYZ_ ) // 0
 * SAKE_BOOST_EXT_PP_KEYWORD_IS( abc xyz, XYZ_ ) // 0
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_IS_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_IS_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_IS( X, empty_when_cat_with_suffix_keyword ) \
    BOOST_PP_IS_EMPTY( BOOST_PP_CAT( empty_when_cat_with_suffix_keyword, X ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_IS_HPP
