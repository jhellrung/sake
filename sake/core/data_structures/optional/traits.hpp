/*******************************************************************************
 * sake/core/data_structures/optional/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_optional< T, ValuePred = default_tag >
 * struct is_optional_sans_qualifiers< T, ValuePred = default_tag >
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_TRAITS_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_TRAITS_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/utility/default_tag.hpp>

namespace sake
{

template< class T, class ValuePred = sake::default_tag >
struct is_optional
    : boost::false_type
{ };

template< class T >
struct is_optional< sake::optional<T>, sake::default_tag >
    : boost::true_type
{ };

template< class T, class ValuePred >
struct is_optional< sake::optional<T>, ValuePred >
{
    static bool const value = boost::mpl::apply1< ValuePred, T >::type::value;
    typedef is_optional type;
};

template< class T, class ValuePred = sake::default_tag >
struct is_optional_sans_qualifiers
    : sake::is_optional<
          typename boost_ext::remove_qualifiers<T>::type,
          ValuePred
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_TRAITS_HPP
