/*******************************************************************************
 * sake/core/iterator/private/is_interoperable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_INTEROPERABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_IS_INTEROPERABLE_HPP

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I0, class I1 >
struct is_interoperable
    : boost_ext::mpl::or2<
          boost_ext::is_convertible< I0, I1 >,
          boost_ext::is_convertible< I1, I0 >
      >
{ };

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_INTEROPERABLE_HPP
