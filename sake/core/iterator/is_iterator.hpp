/*******************************************************************************
 * sake/core/iterator/is_iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_iterator<
 *     I,
 *     Reference = void,
 *     ReferencePred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_IS_ITERATOR_HPP

#include <boost/mpl/always.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_operator_star.hpp>
#include <sake/core/introspection/is_incrementable.hpp>

namespace sake
{

template<
    class I,
    class Reference = void,
    class ReferencePred = boost::mpl::always< boost::true_type >
>
struct is_iterator
    : boost_ext::mpl::and2<
          sake::has_operator_star< I const &, Reference, ReferencePred >,
          sake::is_incrementable<I>
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_ITERATOR_HPP
