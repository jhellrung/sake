/*******************************************************************************
 * sake/core/iterator/private/common_difference_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_COMMON_DIFFERENCE_TYPE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_COMMON_DIFFERENCE_TYPE_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I0, class I1 >
struct common_difference_type
  : boost_ext::common_type<
      typename I0::difference_type,
      typename I1::difference_type
    >
{ };

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_COMMON_DIFFERENCE_TYPE_HPP
