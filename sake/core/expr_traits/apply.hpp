/*******************************************************************************
 * sake/core/expr_traits/apply.hpp
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

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/sizeof_t.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_EXPR_APPLY( metafunction, expression ) \
    (SAKE_SIZEOF_TRUE_TAG == sizeof( ::sake::expr_apply_private::helper< \
        metafunction >::apply( SAKE_EXPR_TYPE_TAG_OF( expression )) ))

namespace sake
{

namespace expr_apply_private
{

template< class F >
struct helper
{
    template< class T >
    struct result
    {
        // MSVC9 workaround.
        static bool const value = boost::mpl::apply1<F,T>::type::value;
        typedef sake::sizeof_t< 1 + value > type;
    };

    template< class T >
    static typename result<T>::type
    apply(sake::type_tag<T>);
};

} // namespace expr_apply_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_APPLY_HPP
