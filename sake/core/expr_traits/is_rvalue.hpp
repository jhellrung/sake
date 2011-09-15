/*******************************************************************************
 * core/expr_traits/is_rvalue.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_IS_RVALUE( expression )
 *
 * Evaluates (at compile-time) to true iff the given expression is an rvalue.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_IS_RVALUE_HPP
#define SAKE_CORE_EXPR_TRAITS_IS_RVALUE_HPP

#include <boost/static_assert.hpp>

#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_EXPR_IS_RVALUE( expression ) \
    ( sizeof( ::sake::yes_tag ) == sizeof( ::sake::expr_is_rvalue_private::test( expression ) ) )

namespace sake
{

namespace expr_is_rvalue_private
{

template< class T >
sake::no_tag test(T&);
sake::yes_tag test(...);

struct dummy { };
BOOST_STATIC_ASSERT(( !SAKE_EXPR_IS_RVALUE( sake::declval_ref < int >() ) ));
BOOST_STATIC_ASSERT(( !SAKE_EXPR_IS_RVALUE( sake::declval_cref< int >() ) ));
BOOST_STATIC_ASSERT((  SAKE_EXPR_IS_RVALUE( sake::declval     < int >() ) ));
BOOST_STATIC_ASSERT(( !SAKE_EXPR_IS_RVALUE( sake::declval_ref < dummy >() ) ));
BOOST_STATIC_ASSERT(( !SAKE_EXPR_IS_RVALUE( sake::declval_cref< dummy >() ) ));
BOOST_STATIC_ASSERT((  SAKE_EXPR_IS_RVALUE( sake::declval     < dummy >() ) ));

} // namespace expr_is_rvalue_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_RVALUE_HPP
