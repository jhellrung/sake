/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/volatile.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_VOLATILE( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_VOLATILE( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_VOLATILE( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_VOLATILE( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_VOLATILE( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_VOLATILE( X )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_VOLATILE_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_VOLATILE_HPP

#include <boost/preprocessor/control/expr_iif.hpp>

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/remove_prefix.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE_volatile ( )
#define volatile_SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_VOLATILE( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_VOLATILE( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_VOLATILE( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_SUFFIX_VOLATILE( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_SUFFIX_PREFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_VOLATILE )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_VOLATILE( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_VOLATILE( X ), volatile )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_VOLATILE( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_VOLATILE( X ), volatile )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_VOLATILE_HPP
