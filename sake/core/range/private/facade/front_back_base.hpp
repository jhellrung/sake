/*******************************************************************************
 * sake/core/range/private/facade/front_back_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_FRONT_BACK_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_FRONT_BACK_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/debug.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/private/facade/size_base.hpp>
#include <sake/core/range/private/facade/traits.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct front_back_base_index
{ static int const value = private_::traits< Params >::back_enable::value; };

template< class Derived, class Params >
class front_back_base< Derived, Params, 0 >
    : public private_::size_base< Derived, Params >
{
    typedef private_::size_base< Derived, Params > size_base_;
    typedef private_::traits< Params > traits_;
protected:
    using size_base_::derived;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename front_back_base,
        (( size_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename front_back_base,
        (( size_base_ ))
    )

    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, const_reference );

    reference
    front()
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !derived().empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(sake::_begin);
    }

    const_reference
    front() const
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !derived().empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(sake::_begin);
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename front_back_base,
        (( size_base_ ))
    )

    template< class T >
    explicit front_back_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            front_back_base, T >::value >::type* = 0)
        : size_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class front_back_base< Derived, Params, 1 >
    : public private_::front_back_base< Derived, Params, 0 >
{
    typedef private_::front_back_base< Derived, Params, 0 > front_back_base_;
    typedef private_::traits< Params > traits_;
protected:
    using front_back_base_::derived;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename front_back_base,
        (( front_back_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename front_back_base,
        (( front_back_base_ ))
    )

    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, const_reference );

    reference
    back()
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !derived().empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(sake::_end);
    }

    const_reference
    back() const
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !derived().empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(sake::_end);
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename front_back_base,
        (( front_back_base_ ))
    )

    template< class T >
    explicit front_back_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            front_back_base, T >::value >::type* = 0)
        : front_back_base_(sake::forward<T>(x))
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_FRONT_BACK_BASE_HPP
