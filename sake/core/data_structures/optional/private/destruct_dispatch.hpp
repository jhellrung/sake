/*******************************************************************************
 * sake/core/data_structures/optional/private/destruct_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_DESTRUCT_DISPATCH_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_DESTRUCT_DISPATCH_HPP

#include <boost/type_traits/has_trivial_destructor.hpp>

namespace sake
{

namespace optional_adl
{

namespace private_
{

template<
    class T,
    bool = boost::has_trivial_destructor<T>::value
>
struct destruct_dispatch;

template< class T >
struct destruct_dispatch< T, false >
{
    template< class This >
    static void apply(This& this_)
    { static_cast< T* >(static_cast< void* >(this_.m_storage._))->~T(); }
    template< class This >
    static void apply_reset(This& this_)
    { apply(this_); this_.m_initialized = false; }
    template< class This >
    static void apply_if(This& this_)
    { if(this_.m_initialized) apply(this_); }
    template< class This >
    static void apply_if_reset(This& this_)
    { if(this_.m_initialized) { apply(this_); this_.m_initialized = false; } }
};

template< class T >
struct destruct_dispatch< T, true >
{
    template< class This >
    static void apply(This& /*this_*/)
    { }
    template< class This >
    static void apply_reset(This& this_)
    { this_.m_initialized = false; }
    template< class This >
    static void apply_if(This& /*this_*/)
    { }
    template< class This >
    static void apply_if_reset(This& this_)
    { this_.m_initialized = false; }
};

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_DESTRUCT_DISPATCH_HPP
