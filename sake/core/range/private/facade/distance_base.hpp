/*******************************************************************************
 * sake/core/range/private/facade/distance_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/private/facade/size_base.hpp>
#include <sake/core/range/private/facade/traits.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct distance_base_index
{ static int const value = private_::traits< Params >::distance_enable::value; };

template< class Derived, class Params >
class distance_base< Derived, Params, 0 >
    : public private_::size_base< Derived, Params >
{
    typedef private_::size_base< Derived, Params > size_base_;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename distance_base,
        (( size_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename distance_base,
        (( size_base_ ))
    )
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename distance_base,
        (( size_base_ ))
    )

    template< class T >
    explicit distance_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            distance_base, T >::value >::type* = 0)
        : size_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class distance_base< Derived, Params, 1 >
    : public private_::size_base< Derived, Params >
{
    typedef private_::size_base< Derived, Params > size_base_;
    typedef private_::traits< Params > traits_;
protected:
    using size_base_::derived;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename distance_base,
        (( size_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename distance_base,
        (( size_base_ ))
    )

    SAKE_USING_TYPEDEF( typename traits_, difference_type );

    difference_type distance() const
    { return sake::range::core_access::distance(derived()); }

    inline friend
    difference_type range_distance(Derived const & this_)
    { return this_.distance(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename distance_base,
        (( size_base_ ))
    )

    template< class T >
    explicit distance_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            distance_base, T >::value >::type* = 0)
        : size_base_(sake::forward<T>(x))
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_BASE_HPP
