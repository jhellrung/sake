/*******************************************************************************
 * sake/core/utility/compressed_tuple/private/at_c_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_AT_C_DISPATCH_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_AT_C_DISPATCH_HPP

#include <cstddef>

#include <boost/mpl/at.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

namespace sake
{

namespace compressed_tuple_adl
{

namespace private_
{

template<
    std::size_t I, std::size_t J,
    int = (J+0 <= I) + (J+1 <= I) + (J+2 <= I)
>
struct at_c_dispatch;

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,0>
{
    BOOST_STATIC_ASSERT((I < J+0));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::value_types, I
            >::type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<I>(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,1>
{
    BOOST_STATIC_ASSERT((I == J+0));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::value_types, J
            >::type::first_type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<J>().first(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,2>
{
    BOOST_STATIC_ASSERT((I == J+1));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::value_types, J
            >::type::second_type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<J>().second(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,3>
{
    BOOST_STATIC_ASSERT((I > J+1));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::value_types, I-1
            >::type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<I-1>(); }
};

} // namespace private_

} // namespace compressed_tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_AT_C_DISPATCH_HPP
