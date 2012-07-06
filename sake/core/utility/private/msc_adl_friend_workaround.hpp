/*******************************************************************************
 * sake/core/utility/private/msc_adl_friend_workaround.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * MSVC9 sometimes gets confused (ambiguous overloads) when ADL finds a friend
 * function template.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_MSC_ADL_FRIEND_WORKAROUND_HPP
#define SAKE_CORE_UTILITY_PRIVATE_MSC_ADL_FRIEND_WORKAROUND_HPP

#include <sake/core/config.hpp>

#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace sake
{

template< class T >
struct msc_adl_friend_workaround
{
    T& value;
    template< class U >
    msc_adl_friend_workaround(U& value_,
        typename boost::enable_if_c<
            boost::is_same<U,T>::value >::type* = 0)
        : value(value_)
    { }
    template< class U >
    msc_adl_friend_workaround(U const & value_,
        typename boost::enable_if_c<
            boost::is_same< U const, T >::value >::type* = 0)
        : value(value_)
    { }
};

} // namespace sake

#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_MSC_ADL_FRIEND_WORKAROUND_HPP
