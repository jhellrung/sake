/*******************************************************************************
 * sake/core/expr_traits/is_lvalue.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_IS_LVALUE( Expr )
 *
 * Evaluates (at compile-time) to true iff the given expression is an lvalue.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_IS_LVALUE_HPP
#define SAKE_CORE_EXPR_TRAITS_IS_LVALUE_HPP

#include <sake/core/expr_traits/is_rvalue.hpp>

#define SAKE_EXPR_IS_LVALUE( expression ) \
    ( ! SAKE_EXPR_IS_RVALUE( expression ) )

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_LVALUE_HPP
