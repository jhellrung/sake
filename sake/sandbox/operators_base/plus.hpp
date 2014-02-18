/*******************************************************************************
 * sake/core/operators_base/plus.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_OPERATORS_BASE_PLUS_HPP
#define SAKE_CORE_OPERATORS_BASE_PLUS_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/has_qualifier.hpp>

#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/operators_base/fwd.hpp>
#include <sake/core/operators_base/private/rv_sink.hpp>

namespace sake
{

namespace operators_base
{

template<
    class Derived,
    class Signature,
    class ChainedBase = sake::void_
>
struct plus;

/*******************************************************************************
 ******************************************************************************/

template< class Derived, class Result, class T0, class T1, class ChainedBase >
struct plus< Derived, Result ( T0, T1 ), ChainedBase >
    : ChainedBase
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< Result >::value));
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T1 >::value));

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename plus,
        (( ChainedBase ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename plus,
        ( swap ) ( hash_value ),
        (( ChainedBase ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0_, class T1_ >
    inline friend typename boost::enable_if_c<
        boost_ext::mpl::and2<
            boost_ext::is_same_sans_qualifiers< T0, T0_ >,
            boost_ext::is_same_sans_qualifiers< T1, T1_ >
        >::value,
        Result
    >::type
    operator+(T0_ && x0, T1_ && x1)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(
                sake::forward< T0_ >(x0), sake::forward< T1_ >(x1));
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0_, class T1_ >
    inline friend typename boost::enable_if_c<
        boost_ext::mpl::and2<
            boost_ext::is_same_sans_qualifiers< T0, T0_ >,
            boost_ext::is_same_sans_qualifiers< T1, T1_ >
        >::value,
        Result
    >::type
    operator+(T0_ & x0, T1_ & x1)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(x0, x1);
    }

    template< class T0_ >
    inline friend typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T0, T0_ >::value,
        Result
    >::type
    operator+(T0_ & x0, private_::rv_sink< T1 > x1)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(x0, sake::move(x1.value));
    }

    template< class T1_ >
    inline friend typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T1, T1_ >::value,
        Result
    >::type
    operator+(private_::rv_sink< T0 > x0, T1_ & x1)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(sake::move(x0.value), x1);
    }

    inline friend Result
    operator+(private_::rv_sink< T0 > x0, private_::rv_sink< T1 > x1)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(sake::move(x0.value), sake::move(x1.value));
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename plus,
        (( ChainedBase ))
    )

    template< class U >
    explicit plus(SAKE_FWD2_REF( U ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            plus, U >::value >::type* = 0)
        : ChainedBase(sake::forward<U>(x))
    { }

    friend class sake::operators_base::core_access;

    template< class T0_, class T1_ >
    static Result
    derived_plus2(T0_ & x0, T1_ & x1)
    { return sake::move(T0(x0) += x1); }
    template< class T0_, class T1_ >
    static Result
    derived_plus2(SAKE_RV_REF( T0_ ) x0, T1_ & x1)
    { return sake::move(SAKE_AS_LVALUE( x0 ) += x1); }
    template< class T0_, class T1_ >
    static Result
    derived_plus2(SAKE_RV_REF( T0_ ) x0, SAKE_RV_REF( T1_ ) x1)
    { return sake::move(SAKE_AS_LVALUE( x0 ) += sake::move(x1)); }
    template< class T0_, class T1_ >
    static Result
    derived_plus2(T0_ & x0, SAKE_RV_REF( T1_ ) x1)
    {
        return operators_base::core_access::
            rplus< Derived, Result >(sake::move(x1), x0);
    }

    template< class T1_, class T0_ >
    static Result
    derived_rplus2(SAKE_RV_REF( T1_ ) x1, T0_ & x0)
    { return sake::move(SAKE_AS_LVALUE( x1 ) += x0); }
};

/*******************************************************************************
 ******************************************************************************/

template< class Derived, class Result, class T, class ChainedBase >
struct plus< Derived, Result ( T ), ChainedBase >
    : ChainedBase
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< Result >::value));
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename plus,
        (( ChainedBase ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename plus,
        ( swap ) ( hash_value ),
        (( ChainedBase ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T_ >
    inline friend typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T, T_ >,
        Result
    >::type
    operator+(T_ && x)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(sake::forward< T_ >(x));
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T_ >
    inline friend typename boost::enable_if_c<
        boost_ext::is_same_sans_qualifiers< T, T_ >,
        Result
    >::type
    operator+(T_ & x)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(x);
    }

    inline friend Result
    operator+(private_::rv_sink<T> x)
    {
        return sake::operators_base::core_access::
            plus< Derived, Result >(sake::move(x.value));
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename plus,
        (( ChainedBase ))
    )

    template< class U >
    explicit plus(SAKE_FWD2_REF( U ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            plus, U >::value >::type* = 0)
        : ChainedBase(sake::forward<U>(x))
    { }

    friend class sake::operators_base::core_access;
};

} // namespace operators_base

} // namespace sake

#endif // #ifndef SAKE_CORE_OPERATORS_BASE_PLUS_HPP
