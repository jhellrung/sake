/*******************************************************************************
 * sake/core/expr_traits/typeof.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_TYPEOF( expression, candidates_type )
 * #define SAKE_EXPR_TYPEOF_TYPEDEF( expression, candidates_type, type )
 *
 * SAKE_EXPR_TYPEOF expands to the type within the Boost.MPL sequence
 * candidates_type matching the type of the given expression.
 * SAKE_EXPR_TYPEOF_TYPEDEF defines a typedef for the result of SAKE_EXPR_TYPEOF
 * as a workaround for deficient compilers.
 * If the type of the given expression is not among the types given by
 * candidates_type, the result evaluates to void.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_TYPEOF_HPP
#define SAKE_CORE_EXPR_TRAITS_TYPEOF_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/mpl/at_c.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/expr_traits/typeof_index.hpp>

// This is prone to the following error on MSVC9:
//     error C2064: term does not evaluate to a function taking 1 arguments
// The trouble appears to be the use of a sizeof expression as an integral
// template parameter in boost_ext::mpl::at_c.  As a workaround, the TYPEDEF
// macro is provided.
#define SAKE_EXPR_TYPEOF( expression, candidates_type ) \
    ::sake::boost_ext::mpl::at_c< \
        candidates_type, \
        SAKE_EXPR_TYPEOF_INDEX( expression, candidates_type ), \
        void \
    >

#define SAKE_EXPR_TYPEOF_TYPEDEF( expression, candidates_type, type_ ) \
    static int const BOOST_PP_CAT( _sake_expr_typeof_index_for_, type_ ) = \
        SAKE_EXPR_TYPEOF_INDEX( \
            SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( expression ), \
            candidates_type \
        ); \
    typedef SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( expression ) \
        ::sake::boost_ext::mpl::at_c< \
            candidates_type, \
            BOOST_PP_CAT( _sake_expr_typeof_index_for_, type_ ), \
            void \
        >::type type_

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_TYPEOF_HPP
