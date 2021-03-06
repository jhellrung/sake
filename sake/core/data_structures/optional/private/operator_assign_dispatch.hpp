/*******************************************************************************
 * sake/core/data_structures/optional/private/operator_assign_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_OPERATOR_ASSIGN_DISPATCH_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_OPERATOR_ASSIGN_DISPATCH_HPP

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/has_operator_assign.hpp>

namespace sake
{

namespace optional_adl
{

namespace private_
{

template<
    class T, class U,
    bool = sake::has_operator_assign< T&, void ( U ) >::value
>
struct operator_assign_dispatch
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;

template< class T, class U >
struct operator_assign_dispatch< T, U, false >
{
    typedef typename boost_ext::add_rvalue_reference<U>::type fwd_type;
    template< class This >
    static void apply(This& this_, fwd_type x)
    { this_.reset(static_cast< fwd_type >(x)); }
};

template< class T, class U >
struct operator_assign_dispatch< T, U, true >
{
    typedef typename boost_ext::add_rvalue_reference<U>::type fwd_type;
    template< class This >
    static void apply(This& this_, fwd_type x)
    {
        if(this_.m_initialized)
            this_.get() = static_cast< fwd_type >(x);
        else {
            new(this_.m_storage._) T(static_cast< fwd_type >(x));
            this_.m_initialized = true;
        }
    }
};

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_OPERATOR_ASSIGN_DISPATCH_HPP
