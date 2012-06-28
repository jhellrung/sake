/*******************************************************************************
 * sake/core/expr_traits/typeof_index.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_TYPEOF_INDEX( expression, candidate_types )
 *
 * Evaluates (at compile-time) to the index within the Boost.MPL sequence
 * candidate_types corresponding to the type of the given expression.
 * If the type of the given expression is not among the types given by
 * candidate_types, the result evaluates to the size of candidate_types.
 *
 * Note: expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_TYPEOF_INDEX_HPP
#define SAKE_CORE_EXPR_TRAITS_TYPEOF_INDEX_HPP

#include <cstddef>

#include <sake/boost_ext/mpl/find_index.hpp>

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/sizeof_t.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_EXPR_TYPEOF_INDEX( expression, candidate_types ) \
    (sizeof( ::sake::expr_typeof_index_private::helper< candidate_types \
        >::apply( SAKE_EXPR_TYPE_TAG_OF( expression ) ) ) - 1)

namespace sake
{

namespace expr_typeof_index_private
{

template< class Sequence >
struct helper
{
    template< class T >
    struct result
    {
        static std::size_t const value =
            boost_ext::mpl::find_index< Sequence, T >::value;
        typedef sake::sizeof_t< 1 + value > type;
    };

    template< class T >
    static typename result<T>::type
    apply(sake::type_tag<T>);
};

} // namespace expr_typeof_index_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_TYPEOF_INDEX_HPP
