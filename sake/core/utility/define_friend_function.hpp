/*******************************************************************************
 * sake/core/utility/define_friend_function.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_FRIEND_FUNCTION_HPP
#define SAKE_CORE_UTILITY_DEFINE_FRIEND_FUNCTION_HPP

#include <cstddef>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#define SAKE_DEFINE_FRIEND_SWAP( T ) \
    template< class _SakeFriendThis > \
    inline friend typename ::boost::enable_if_c< \
        ::boost::is_same< _SakeFriendThis, T >::value >::type \
    swap(_SakeFriendThis& x, _SakeFriendThis& y) \
    { x.swap(y); }

#define SAKE_DEFINE_FRIEND_HASH_VALUE( T ) \
    template< class _SakeFriendThis > \
    inline friend typename ::boost::enable_if_c< \
        ::boost::is_same< _SakeFriendThis, T >::value, ::std::size_t >::type \
    hash_value(_SakeFriendThis const & x) \
    { return x.hash_value(); }

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_FRIEND_FUNCTION_HPP
