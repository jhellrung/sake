/*******************************************************************************
 * sake/core/math/private/rational/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_PRIVATE_RATIONAL_BASE_HPP
#define SAKE_CORE_MATH_PRIVATE_RATIONAL_BASE_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/has_qualifier.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/math/rational/denom.hpp>
#include <sake/core/math/rational/fwd.hpp>
#include <sake/core/math/rational/numer.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>

namespace sake
{

namespace rational_adl
{

namespace private_
{

struct implicit_constructor_tag { };

template< class T >
struct base
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));

    typedef T int_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename base,
        (( T )( m_numer ))
        (( T )( m_denom ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T )( m_numer ))
        (( T )( m_denom ))
    )

protected:
    template< class U >
    struct value_enable
        : boost_ext::is_convertible<U,T>
    { };

    template< class U, class Result = void >
    struct value_enabler
        : boost::enable_if_c< value_enable<U>::value, Result >
    { };

    template< class U >
    struct rational_enable
        : boost_ext::mpl::or2<
              boost_ext::is_same_sans_qualifiers< U, sake::rational<T> >,
              boost_ext::mpl::and2<
                  boost::mpl::not_< boost_ext::is_convertible<U,T> >,
                  sake::is_rational_sans_qualifiers<
                      U,
                      boost_ext::is_convertible<
                          boost_ext::propagate_qualifiers< U, boost::mpl::_1 >,
                          T
                      >
                  >
              >
          >
    { };

    template< class U, class Result = void >
    struct rational_enabler
        : boost::enable_if_c< rational_enable<U>::value, Result >
    { };

    template< class U >
    struct common_enable
        : boost_ext::mpl::or2< value_enable<U>, rational_enable<U> >
    { };

    template< class U >
    struct common_enabler
        : boost::enable_if_c< common_enable<
              typename boost_ext::remove_rvalue_reference<U>::type >::value >
    { };

    template< class U >
    base(SAKE_FWD2_REF( U ) x,
        private_::implicit_constructor_tag,
        typename value_enabler< SAKE_FWD2_PARAM( U ) >::type* = 0)
        : m_numer(sake::forward<U>(x)),
          m_denom(sake::one.as<T>())
    { }

    template< class U >
    base(SAKE_FWD2_REF( U ) x,
        private_::implicit_constructor_tag,
        typename rational_enabler< SAKE_FWD2_PARAM( U ) >::type* = 0)
        : m_numer(sake::numer(sake::forward<U>(x))),
          m_denom(sake::denom(sake::forward<U>(x)))
    { }

    template< class Signature >
    explicit base(sake::emplacer< Signature > e)
        : m_numer(sake::emplacer_constructible<T>(e)),
          m_denom(sake::one.as<T>())
    { }

    template< class U0, class U1 >
    base(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : m_numer(sake::emplacer_constructible<T>(sake::forward< U0 >(x0))),
          m_denom(sake::emplacer_constructible<T>(sake::forward< U1 >(x1)))
    { }

    T m_numer;
    T m_denom;
};

} // namespace private_

} // namespace rational_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_PRIVATE_RATIONAL_BASE_HPP
