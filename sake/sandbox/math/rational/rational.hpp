/*******************************************************************************
 * sake/core/math/rational/rational.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct rational<T>
 *
 * This is similar to Boost.Rational.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_RATIONAL_RATIONAL_HPP
#define SAKE_CORE_MATH_RATIONAL_RATIONAL_HPP

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/math/rational/fwd.hpp>
#include <sake/core/math/rational/private/base.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/value_constructor.hpp>

#define SAKE_RATIONAL_INCLUDE_HEADERS
#include <sake/core/math/rational/private/implicit_constructor.ipp>
#include <sake/core/math/rational/private/operator_assign.ipp>
#include <sake/core/math/rational/private/assign.ipp>
#undef SAKE_RATIONAL_INCLUDE_HEADERS

namespace sake
{

namespace rational_adl
{

/*******************************************************************************
 * struct rational<T>
 ******************************************************************************/

template< class T >
class rational
    : public private_::base<T>
{
    typedef private_::base<T> base_;
public:
    typedef T integer_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename rational,
        (( base_ ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename rational,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( base_ ))
    )

    // template< class U0, class U1 >
    // rational(U0&& x0, U1&& x1,
    //     typename value_constructor_enabler< U0, U1 >::type* = 0);
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME rational
#define SAKE_VALUE_CONSTRUCTOR_TYPES      ( T ) ( T )
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    base_
#define SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

#if 0 // for exposition purposes only
    template< class U >
    rational(U&& x,
        typename implicit_constructor_enabler<U>::type* = 0);

    template< class U >
    typename operator_assign_enabler<U>::type // -> rational&
    operator=(U&& x);

    template< class U0, class U1 >
    typename assign_enabler< U0, U1 >::type // -> void
    assign(U0&& x0, U1&& x1);
#endif
#define SAKE_RATIONAL_DEFINE_MEMBERS
#include <sake/core/math/rational/private/implicit_constructor.ipp>
#include <sake/core/math/rational/private/operator_assign.ipp>
#include <sake/core/math/rational/private/assign.ipp>
#undef SAKE_RATIONAL_DEFINE_MEMBERS

    template< class Signature >
    explicit rational(sake::emplacer< Signature > e,
        typename base_::template
            emplacer_enable< sake::emplacer< Signature > >::type* = 0);

    T &
    numer()
    { return m_numer; }
    T &
    denom()
    { return m_denom; }

    T const &
    numer() const
    { return m_numer; }
    T const &
    denom() const
    { return m_denom; }

    rational &
    operator+=(rational const & other);
    rational &
    operator-=(rational const & other);
    rational &
    operator*=(rational const & other);
    rational &
    operator/=(rational const & other);

    inline friend rational
    rcp(rational const & this_)
    { return rational(this_.m_denom, this_.m_numer); }
    inline friend rational
    rcp(this_rvalue_param_type this_)
    { return rational(sake::move(this_.m_denom), sake::move(this_.m_numer)); }

private:
    using base_::m_numer;
    using base_::m_denom;
};

/*******************************************************************************
 * rational<T> member definitions
 ******************************************************************************/

template< class T >
template< class Signature >
inline
rational<T>::
rational(sake::emplacer< Signature > e,
    typename base_::template
        emplacer_enable< sake::emplacer< Signature > >::type*)
    : base_(e)
{ }

template< class T >
inline T &
rational<T>::
numer()
{ return m_numer; }

template< class T >
inline T &
rational<T>::
denom()
{ return m_denom; }

template< class T >
T const &
rational<T>::
numer() const
{ return m_numer; }

template< class T >
T const &
rational<T>::
denom() const
{ return m_denom; }

rational&
operator+=(rational const & other)
{
    T const denom_gcd = sake::gcd(m_denom, other.m_denom);
    m_numer = m_numer * (other.m_denom / denom_gcd)
            + other.m_numer * (m_denom / denom_gcd);
    m_denom *= other.m_denom / denom_gcd;
    reduce();
    return *this;
}

rational&
operator-=(rational const & other)
{
    T const denom_gcd = sake::gcd(m_denom, other.m_denom);
    m_numer = m_numer * (other.m_denom / denom_gcd)
            - other.m_numer * (m_denom / denom_gcd);
    m_denom *= other.m_denom / denom_gcd;
    reduce();
    return *this;
}

} // namespace rational_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_RATIONAL_RATIONAL_HPP
