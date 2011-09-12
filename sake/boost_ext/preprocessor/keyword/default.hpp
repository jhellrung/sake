/*******************************************************************************
 * boost_ext/preprocessor/keyword/default.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_DEFAULT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_DEFAULT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_DEFAULT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_DEFAULT( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_DEFAULT_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_DEFAULT_HPP

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT_default ( )
#define default_SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_DEFAULT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_DEFAULT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_DEFAULT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_DEFAULT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_DEFAULT )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_DEFAULT_HPP
