/*******************************************************************************
 * sake/boost_ext/preprocessor/keyword/dependent.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_DEPENDENT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_DEPENDENT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_DEPENDENT( X )
 * #define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_DEPENDENT( X )
 *
 * These PP keyword macros operate on the "class" and "typename" C++ keywords.
 * *_HAS_*_DEPENDENT( X ) evaluates to 1 iff the PP token sequence X is begins
 * with a "class" or "typename" keyword (and to 0 otherwise).  *_GET_*_DEPENDENT
 * evaluates to "typename" (without quotes) iff the PP token sequence X begins
 * with a "class" or "typename" keyword.
 *
 * This allows one to support an interface to easily distinguish between
 * dependent and non-dependent types.  A type prefixed by "typename" is, by
 * definition, dependent.  Any dependent types not otherwise prefixed by
 * "typename" should be prefixed with "class" (which should then be stripped off
 * in the implementation).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_DEPENDENT_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_DEPENDENT_HPP

#include <boost/preprocessor/control/expr_iif.hpp>

#include <sake/boost_ext/preprocessor/keyword/has_prefix.hpp>
#include <sake/boost_ext/preprocessor/keyword/is.hpp>

#define SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT_class    ( )
#define SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT_typename ( )
#define    class_SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT ( )
#define typename_SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT ( )

#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_DEPENDENT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX( X, SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT_ )
#define SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_DEPENDENT( X ) \
    SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX( X, _SAKE_BOOST_EXT_PP_KEYWORD_DEPENDENT )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_DEPENDENT( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_DEPENDENT( X ), typename )
#define SAKE_BOOST_EXT_PP_KEYWORD_GET_SUFFIX_DEPENDENT( X ) \
    BOOST_PP_EXPR_IIF( SAKE_BOOST_EXT_PP_KEYWORD_HAS_SUFFIX_DEPENDENT( X ), typename )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_KEYWORD_VOID_HPP
