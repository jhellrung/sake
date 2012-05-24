/*******************************************************************************
 * sake/core/utility/value_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct value_constructor_enable< T, U = void >
 * struct value_constructor_enabler<T,U>
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_ENABLE_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/emplacer/traits.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

namespace value_constructor_enable_private
{

template< class T, class U >
struct impl
    : boost_ext::mpl::or2<
          sake::is_convertible_wnrbt<U,T>,
          sake::is_emplacer_sans_qualifiers_with_value<U,T>
      >
{ };

template< class T >
struct impl< T, void >
{
    template< class U >
    struct apply
        : value_constructor_enable_private::impl<T,U>
    { };
};

template<>
struct impl< void, void >
{
    template< class U >
    struct apply
        : boost::false_type
    { };
};

} // namespace value_constructor_enable_private

template< class T, class U = void >
struct value_constructor_enable
    : value_constructor_enable_private::impl<
          typename boost::remove_cv<T>::type,
          typename boost::remove_cv<U>::type
      >
{ };

template< class T, class U >
struct value_constructor_enabler
    : boost::enable_if_c< sake::value_constructor_enable<T,U>::value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_ENABLE_HPP
