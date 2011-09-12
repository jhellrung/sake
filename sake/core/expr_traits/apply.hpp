/*******************************************************************************
 * core/expr_traits/apply.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_EXPR_APPLY( metafunction, expression )
 *
 * This evaluates (at compile-time) to true iff the type of the given expression
 * satisfies the given Boost.MPL metafunction class or lambda expression.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_APPLY_HPP
#define SAKE_CORE_EXPR_TRAITS_APPLY_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/if.hpp>

#include <sake/core/utility/yes_no_type.hpp>

#define SAKE_EXPR_APPLY( metafunction, expression ) \
    ( \
        sizeof( ::sake::yes_type ) \
     == sizeof( ::sake::expr_apply_private::helper< metafunction >( expression, expression ) ) \
    )

namespace sake
{

namespace expr_apply_private
{

template< class F, class T >
struct result
{
// MSVC9 chokes if you use boost::mpl::apply1<...>::type::value as a boolean template parameter.
// Hence, we have to be very explicit about what we want to do :/
    static const bool b = boost::mpl::apply1<F,T>::type::value;
    typedef typename boost::mpl::if_c< b, sake::yes_type, sake::no_type >::type type;
};

template< class F, class T, class U >
typename result< F, T& >::type
helper(T&, U&);

template< class F, class T, class U >
typename result< F, T const & >::type
helper(T const &, U&);

template< class F, class T >
typename result< F, T >::type
helper(T const &, ...);

} // namespace expr_apply_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_APPLY_HPP