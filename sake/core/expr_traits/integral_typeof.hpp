/*******************************************************************************
 * sake/core/expr_traits/integral_typeof.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_INTEGRAL_TYPEOF( expression )
 * #define SAKE_EXPR_INTEGRAL_TYPEOF_TYPEDEF( expression, type )
 *
 * SAKE_EXPR_INTEGRAL_TYPEOF expands to the integral type of the given
 * expression.
 * SAKE_EXPR_INTEGRAL_TYPEOF_TYPEDEF defines a typedef for the result of
 * SAKE_EXPR_INTEGRAL_TYPEOF as a workaround for deficient compilers.
 * If the type of the given expression is not a builtin integral type, the
 * result evaluates to void.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_INTEGRAL_TYPEOF_HPP
#define SAKE_CORE_EXPR_TRAITS_INTEGRAL_TYPEOF_HPP

#include <sake/boost_ext/preprocessor/seq/to_mpl_vector.hpp>

#include <sake/core/expr_traits/typeof.hpp>

#define SAKE_EXPR_INTEGRAL_TYPEOF( expression ) \
    SAKE_EXPR_TYPEOF( \
        expression, \
        ::sake::expr_integral_typeof_private::integral_types \
    )

#define SAKE_EXPR_INTEGRAL_TYPEOF_TYPEDEF( expression, type ) \
    SAKE_EXPR_TYPEOF_TYPEDEF( \
        expression, \
        ::sake::expr_integral_typeof_private::integral_types, \
        type \
    )

namespace sake
{

namespace expr_integral_typeof_private
{

typedef SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( SAKE_BOOST_EXT_INTEGRAL_TYPES ) integral_types;

} // namespace expr_integral_typeof_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_INTEGRAL_TYPEOF_HPP
