/*******************************************************************************
 * sake/core/data_structures/private/assign_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PRIVATE_ASSIGN_ENABLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_PRIVATE_ASSIGN_ENABLE_HPP

#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>

namespace sake
{

namespace data_structures_private
{

template< class T, class U >
struct assign_enable
    : sake::has_operator_assign<
          typename boost_ext::add_reference<T>::type, void ( U ) >
{ };

} // namespace data_structures_private

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PRIVATE_ASSIGN_ENABLE_HPP
