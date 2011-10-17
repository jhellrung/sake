/*******************************************************************************
 * sake/core/iterator/is_iterator.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_iterator< T, Reference = void, ReferencePred = void >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_IS_ITERATOR_HPP

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_operator_star.hpp>
#include <sake/core/introspection/is_incrementable.hpp>

namespace sake
{

template< class T, class Reference = void, class ReferencePred = void >
struct is_iterator
    : boost_ext::mpl::and2<
          sake::has_operator_star< T const &, Reference, ReferencePred >,
          sake::is_incrementable<T>
      >
{ };

template< class T, class Reference >
struct is_iterator< T, Reference, void >
    : boost_ext::mpl::and2<
          sake::has_operator_star< T const &, Reference >,
          sake::is_incrementable<T>
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_ITERATOR_HPP
