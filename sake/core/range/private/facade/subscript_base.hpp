/*******************************************************************************
 * sake/core/range/private/facade/subscript_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SUBSCRIPT_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_SUBSCRIPT_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/debug.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/private/facade/common_base.hpp>
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
struct subscript_base_index
{
    static int const value = boost_ext::is_convertible<
        typename private_::traits< Params >::range_traversal,
        boost::random_access_traversal_tag
    >::value;
};

template< class Derived, class Params >
class subscript_base< Derived, Params, 0 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename subscript_base,
        (( common_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename subscript_base,
        (( common_base_ ))
    )
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename subscript_base,
        (( common_base_ ))
    )

    template< class T >
    explicit subscript_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            subscript_base, T >::value >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class subscript_base< Derived, Params, 1 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
    typedef private_::traits< Params > traits_;
protected:
    using common_base_::derived;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename subscript_base,
        (( common_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename subscript_base,
        (( common_base_ ))
    )

    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, const_reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, size_type );

    reference
    operator[](difference_type const i)
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE_RELATION( i, >=, sake::zero );
#endif // #ifdef SAKE_RANGE_DEBUG
        return operator[](static_cast< size_type >(i));
    }
    const_reference
    operator[](difference_type const i) const
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE_RELATION( i, >=, sake::zero );
#endif // #ifdef SAKE_RANGE_DEBUG
        return operator[](static_cast< size_type >(i));
    }

    reference
    operator[](size_type const i)
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE_RELATION( i, <, derived.size() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(i);
    }
    const_reference
    operator[](size_type const i) const
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE_RELATION( i, <, derived.size() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return derived().at(i);
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename subscript_base,
        (( common_base_ ))
    )

    template< class T >
    explicit subscript_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            subscript_base, T >::value >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SUBSCRIPT_BASE_HPP
