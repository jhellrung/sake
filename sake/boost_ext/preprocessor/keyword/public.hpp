/*******************************************************************************
 * boost_ext/preprocessor/keyword/public.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_PUBLIC( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_PUBLIC( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_PUBLIC( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_PUBLIC( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_PUBLIC_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_PUBLIC_HPP

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC_public ( )
#define public_SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_PUBLIC( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_PUBLIC( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_PUBLIC( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_PUBLIC( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_PUBLIC )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_PUBLIC_HPP
