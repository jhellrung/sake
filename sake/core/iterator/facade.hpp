/*******************************************************************************
 * sake/core/iterator/facade.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator_facade< Derived, Params >
 *
 * Accepted keywords:
 * - iterator_keyword::value
 * - iterator_keyword::reference
 * - iterator_keyword::difference
 * - iterator_keyword::traversal
 * - iterator_keyword::introversal
 * - iterator_keyword::operator_relational_enable
 * - iterator_keyword::operator_minus_enable
 * - iterator_keyword::chained_base
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_FACADE_HPP
#define SAKE_CORE_ITERATOR_FACADE_HPP

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/category.hpp>
#include <sake/core/iterator/private/facade/explicit_constructor_enable.hpp>
#include <sake/core/iterator/private/facade/operator_arrow_dispatch.hpp>
#include <sake/core/iterator/private/facade/operator_equality_enable.hpp>
#include <sake/core/iterator/private/facade/operator_minus_enable.hpp>
#include <sake/core/iterator/private/facade/operator_relational_enable.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/iterator/private/facade/traversal_base.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_facade_adl
{

template< class Derived, class Params >
class iterator_facade
    : public private_::traversal_base< Derived, Params >
{
#if 0
    typedef private_::traversal_base< Derived, Params > traversal_base_;
protected:
    typedef Derived derived_type;
public:
    using traversal_base_::derived;

private:
    typedef private_::traits< Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_value_type );
public:
    /***************************************************************************
     * typedef ... value_type;
     * typedef ... reference;
     * typedef ... difference_type;
     * typedef ... iterator_traversal;
     * typedef ... cursor_introversal;
     **************************************************************************/
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, iterator_traversal );
    SAKE_USING_TYPEDEF( typename traits_, cursor_introversal );

    typedef typename sake::iterator_private::category<
        iterator_traversal, facade_value_type
    >::type iterator_category;

    /***************************************************************************
     * Dereferenceable
     * operator*() const -> reference
     * typedef ... pointer
     * operator->() const -> pointer
     **************************************************************************/
    reference
    operator*() const
    { return sake::iterator_core_access::operator_star(derived()); }
private:
    typedef private_::operator_arrow_dispatch<
        reference > operator_arrow_dispatch_;
public:
    typedef typename operator_arrow_dispatch_::type pointer;
    pointer
    operator->() const
    { return operator_arrow_dispatch_::apply(operator*()); }

    /***************************************************************************
     * Incrementable
     * operator++() -> Derived&
     * operator++(int) -> ...
     *
     * Bidirectional
     * operator--() -> Derived&
     * operator--(int) -> Derived
     *
     * RandomAccess
     * operator[](difference_type n) const -> ...
     * operator+=(difference_type n) -> Derived&
     * operator-=(difference_type n) -> Derived&
     * operator+(difference_type n) -> Derived
     * friend operator+(difference_type n, Derived const & this_) -> Derived
     * operator-(difference_type n) -> Derived
     *
     * BeginDetect
     * at_begin() const -> bool
     *
     * BeginAccess
     * begin() -> Derived
     *
     * EndDetect
     * at_end() const -> bool
     *
     * EndAccess
     * end() -> Derived
     **************************************************************************/
 
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename iterator_facade,
        (( traversal_base_ ))
    )

private:
    template< class T >
    struct explicit_constructor_enabler
        : private_::explicit_constructor_enabler< Derived, Params, T >
    { };
protected:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    explicit iterator_facade(T&& x,
        typename explicit_constructor_enabler<T>::type* = 0)
        : traversal_base_(sake::forward<T>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    explicit iterator_facade(T& x,
        typename explicit_constructor_enabler< T& >::type* = 0)
        : traversal_base_(x)
    { }

    template< class T >
    explicit iterator_facade(T const & x,
        typename explicit_constructor_enabler< T const & >::type* = 0)
        : traversal_base_(x)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class V >
    explicit iterator_facade(sake::emplacer< V ( ) >)
    { }

    friend class sake::iterator_core_access;

    template< class Other >
    bool
    operator_equal_impl(Other const & other) const
    { return sake::iterator_core_access::cmp(derived(), other) == sake::zero; }
    template< class Other >
    bool
    operator_less_impl(Other const & other) const
    { return sake::iterator_core_access::cmp(derived(), other) < sake::zero; }
    template< class Other >
    bool
    operator_less_equal_impl(Other const & other) const
    { return sake::iterator_core_access::cmp(derived(), other) <= sake::zero; }
    template< class Other >
    sake::sign_t
    cmp_impl(Other const & other) const
    { return sake::sign(derived() - other); }

    difference_type
    operator_minus_begin_impl() const
    { return derived() - derived().begin(); }
    difference_type
    operator_minus_end_impl() const
    { return derived() - derived().end(); }
#endif
};

/*******************************************************************************
 * iterator_facade relational operators
 ******************************************************************************/

#define function_prototype( x, y ) \
template< class D0, class P0, class D1, class P1 > \
inline typename private_::x ## _enabler< D0, P0, D1, P1 >::type \
y(sake::iterator_facade_adl::iterator_facade< D0, P0 > const & i0, \
  sake::iterator_facade_adl::iterator_facade< D1, P1 > const & i1)

function_prototype( operator_equality, operator== )
{ return sake::iterator_core_access::operator_equal(i0.derived(), i1.derived()); }
function_prototype( operator_equality, operator!= )
{ return !(i0.derived() == i1.derived()); }

function_prototype( operator_relational, operator< )
{ return sake::iterator_core_access::operator_less(i0.derived(), i1.derived()); }
function_prototype( operator_relational, operator> )
{ return  (i1.derived() < i0.derived()); }
function_prototype( operator_relational, operator<= )
{ return !(i1.derived() < i0.derived()); }
function_prototype( operator_relational, operator>= )
{ return !(i0.derived() < i1.derived()); }

function_prototype( cmp, cmp )
{ return sake::iterator_core_access::cmp(i0.derived(), i1.derived()); }

/*******************************************************************************
 * iterator_facade arithmetic operators
 ******************************************************************************/

function_prototype( operator_minus, operator- )
{ return sake::iterator_core_access::operator_minus(i0.derived(), i1.derived()); }

#undef function_prototype
#define function_prototype( x ) \
template< class D, class P > \
inline typename private_::operator_minus_ ## x ## _enabler<D,P>::type \
operator-(sake::iterator_facade<D,P> const & i, sake::x ## _tag)

function_prototype( begin )
{ return sake::iterator_core_access::operator_minus_begin(i.derived()); }
function_prototype( end )
{ return sake::iterator_core_access::operator_minus_end(i.derived()); }

#undef function_prototype
#define function_prototype( x ) \
template< class D, class P > \
inline typename private_::operator_minus_ ## x ## _enabler<D,P>::type \
operator-(sake::x ## _tag, sake::iterator_facade<D,P> const & i)

function_prototype( begin )
{ return -(i.derived() - sake::begin_tag()); }
function_prototype( end )
{ return -(i.derived() - sake::end_tag()); }

#undef function_prototype

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_FACADE_HPP
