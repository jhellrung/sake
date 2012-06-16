/*******************************************************************************
 * sake/core/iterator/is_cursor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_cursor<
 *     C,
 *     Introversal = sake::null_introversal_tag,
 *     Reference = void,
 *     ReferencePred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_CURSOR_HPP
#define SAKE_CORE_ITERATOR_IS_CURSOR_HPP

#include <boost/mpl/always.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/cursor/introversal.hpp>
#include <sake/core/iterator/is_iterator.hpp>

namespace sake
{

template<
    class C,
    class Introversal = sake::null_introversal_tag,
    class Reference = void,
    class ReferencePred = boost::mpl::always< boost::true_type >
>
struct is_cursor
    : boost_ext::mpl::and2<
          sake::is_iterator< C, Reference, ReferencePred >,
          boost_ext::is_convertible<
              typename sake::cursor_introversal<C>::type, Traversal >
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_CURSOR_HPP
