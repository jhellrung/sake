/*******************************************************************************
 * sake/core/data_structures/optional/private/swap_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_SWAP_DISPATCH_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_SWAP_DISPATCH_HPP

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/swap.hpp>

namespace sake
{

namespace optional_adl
{

namespace private_
{

template< class T >
class swap_dispatch_bool
{
    typedef typename boost_ext::add_reference_add_const<
        typename boost_ext::add_rvalue_reference<T>::type
    >::type param_type;
public:
    static bool const value =
        sake::has_operator_assign< T&, void ( param_type ) >::value;
};

template<
    class T,
    bool = swap_dispatch_bool<T>::value
>
struct swap_dispatch;

template< class T >
struct swap_dispatch< T, false >
{
    template< class This >
    static void apply(This& x, This& y)
    {
        SAKE_ASSERT_ALL( (( x.initialized() )) (( y.initialized() )) );
        T temp(sake::move(x.get()));
        x.reset(sake::move(y.get()));
        y.reset(sake::move(temp));
    }
};

template< class T >
struct swap_dispatch< T, true >
{
    template< class This >
    static void apply(This& x, This& y)
    {
        SAKE_ASSERT_ALL( (( x.initialized() )) (( y.initialized() )) );
        sake::swap(x.get(), y.get());
    }
};

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_SWAP_DISPATCH_HPP
