/*******************************************************************************
 * sake/core/expr_traits/best_conversion_index.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_BEST_CONVERSION_INDEX( expression, candidates_type )
 *
 * Evaluates (at compile-time) to the index within the Boost.MPL sequence
 * candidates_type corresponding to the type with the best conversion from the
 * type of the given expression.
 * If the type of the given expression is not convertible to any of the types
 * given by candidates_type, or if the best conversion is ambiguous, the result
 * evaluates to the size of candidates_type.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_BEST_CONVERSION_INDEX_HPP
#define SAKE_CORE_EXPR_TRAITS_BEST_CONVERSION_INDEX_HPP

#include <cstddef>

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>

#include <sake/core/utility/sizeof_t.hpp>

#define SAKE_EXPR_BEST_CONVERSION_INDEX( expression, candidates_type ) \
    ( sizeof( ::sake::expr_best_conversion_index_private::overloads< candidates_type >::apply( expression ) ) - 1 )

namespace sake
{

namespace expr_best_conversion_index_private
{

template< class I, class E, std::size_t K >
struct overloads_iterate
    : overloads_iterate< typename boost::mpl::next<I>::type, E, K+1 >
{
    using overloads_iterate< typename boost::mpl::next<I>::type, E, K+1 >::apply;
    static sake::sizeof_t< 1+K > apply(typename boost::mpl::deref<I>::type);
};

template< class E, std::size_t K >
struct overloads_iterate< E, E, K >
{ static sake::sizeof_t< 1+K > apply(...); };

template< class Sequence >
struct overloads
    : overloads_iterate<
          typename boost::mpl::begin< Sequence >::type,
          typename boost::mpl::end< Sequence >::type,
          0
      >
{ };

} // namespace expr_best_conversion_index_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_BEST_CONVERSION_INDEX_HPP
