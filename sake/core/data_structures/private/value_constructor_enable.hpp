/*******************************************************************************
 * sake/core/data_structures/private/value_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PRIVATE_VALUE_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_PRIVATE_VALUE_CONSTRUCTOR_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/emplacer/traits.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

namespace data_structures_private
{

template< class T, class U >
struct value_constructor_enable
    : boost_ext::mpl::or2<
          sake::is_convertible_wnrbt<U,T>,
          sake::is_emplacer_sans_qualifiers_with_value<U,T>
      >
{ };

template< class T, class U >
struct value_constructor_enable< T const, U >
    : value_constructor_enable<T,U>
{ };

template< class T, class U >
struct value_constructor_enable< T volatile, U >
    : value_constructor_enable<T,U>
{ };

template< class T, class U >
struct value_constructor_enable< T const volatile, U >
    : value_constructor_enable<T,U>
{ };

template< class T, class U >
struct value_constructor_enabler
    : boost::enable_if_c< value_constructor_enable<T,U>::value >
{ };

} // namespace data_structures_private

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PRIVATE_VALUE_CONSTRUCTOR_ENABLE_HPP
