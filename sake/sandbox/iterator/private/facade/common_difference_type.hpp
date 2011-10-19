/*******************************************************************************
 * sake/core/iterator/private/facade/common_difference_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_DIFFERENCE_TYPE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_DIFFERENCE_TYPE_HPP

#include <sake/core/utility/common_type.hpp>

namespace sake
{

namespace iterator_facade_private
{

template< class T, class U >
struct common_difference_type
    : sake::common_type<
          typename T::difference_type,
          typename U::difference_type
      >
{ };

} // namespace iterator_facade_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_DIFFERENCE_TYPE_HPP
