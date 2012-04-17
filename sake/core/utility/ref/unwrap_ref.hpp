/*******************************************************************************
 * sake/core/utility/ref/unwrap_ref.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * unwrap_ref(T& x) -> unwrap_reference<T>::type&
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REF_UNWRAP_REF_HPP
#define SAKE_CORE_UTILITY_REF_UNWRAP_REF_HPP

#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/utility/ref/unwrap_reference.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

template< class T >
inline typename sake::unwrap_reference<T>::type &
unwrap_ref(T& x)
{ return x; }

template< class T >
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
// Otherwise, MSVC9 complains about a const U -> U& conversion :/
inline typename boost_ext::add_reference<
    typename sake::unwrap_reference< T const >::type
>::type
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
inline typename sake::unwrap_reference< T const >::type &
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
unwrap_ref(T const & x)
{ return x; }

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REF_UNWRAP_REF_HPP
