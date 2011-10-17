/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/explicit.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_IS_EXPLICIT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_EXPLICIT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_EXPLICIT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_EXPLICIT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_EXPLICIT( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_EXPLICIT_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_EXPLICIT_HPP

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/is.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_IS_EXPLICIT_explicit
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_EXPLICIT_explicit ( )
#define explicit_SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_EXPLICIT ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_IS_EXPLICIT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_IS( X, SAKE_BOOST_EXT_PP_KEYWORD_IS_EXPLICIT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_EXPLICIT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_EXPLICIT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_EXPLICIT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_EXPLICIT )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_EXPLICIT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_EXPLICIT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_EXPLICIT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_EXPLICIT )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_EXPLICIT_HPP
