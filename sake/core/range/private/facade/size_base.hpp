/*******************************************************************************
 * sake/core/range/private/facade/size_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SIZE_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_SIZE_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/private/facade/subscript_base.hpp>
#include <sake/core/range/private/facade/traits.hpp>
#include <sake/core/utility/private/msc_adl_friend_workaround.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct size_base_index
{ static int const value = private_::traits< Params >::size_enable::value; };

template< class Derived, class Params >
class size_base< Derived, Params, 0 >
    : public private_::subscript_base< Derived, Params >
{
    typedef private_::subscript_base< Derived, Params > subscript_base_;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename size_base,
        (( subscript_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename size_base,
        (( subscript_base_ ))
    )
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename size_base,
        (( subscript_base_ ))
    )

    template< class T >
    explicit size_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            size_base, T >::value >::type* = 0)
        : subscript_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class size_base< Derived, Params, 1 >
    : public private_::subscript_base< Derived, Params >
{
    typedef private_::subscript_base< Derived, Params > subscript_base_;
    typedef private_::traits< Params > traits_;
protected:
    using subscript_base_::derived;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename size_base,
        (( subscript_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename size_base,
        (( subscript_base_ ))
    )

    SAKE_USING_TYPEDEF( typename traits_, size_type );

    size_type size() const
    { return sake::range::core_access::size(derived()); }
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    inline friend size_type
    range_calculate_size(sake::msc_adl_friend_workaround< Derived const > x)
    { return x.value.size(); }
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, size_type >::type
    range_calculate_size(Derived_ const & this_)
    { return this_.size(); }
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename size_base,
        (( subscript_base_ ))
    )

    template< class T >
    explicit size_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            size_base, T >::value >::type* = 0)
        : subscript_base_(sake::forward<T>(x))
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SIZE_BASE_HPP
