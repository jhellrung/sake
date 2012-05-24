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

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/category.hpp>
#include <sake/core/iterator/private/facade/function_prototype.hpp>
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

    template< class T >
    explicit iterator_facade(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< iterator_facade, T >::value
        >::type* = 0)
        : traversal_base_(sake::forward<T>(x))
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
};

/*******************************************************************************
 * iterator_facade relational operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_function_prototype( inline, operator_equality_enabler, operator== )
{ return sake::iterator_core_access::operator_equal(i0.derived(), i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_equality_enabler, operator!= )
{ return !(i0.derived() == i1.derived()); }

SAKE_ITERATOR_FACADE_function_prototype( inline, operator_relational_enabler, operator< )
{ return sake::iterator_core_access::operator_less(i0.derived(), i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_relational_enabler, operator> )
{ return  (i1.derived() < i0.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_relational_enabler, operator<= )
{ return !(i1.derived() < i0.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_relational_enabler, operator>= )
{ return !(i0.derived() < i1.derived()); }

SAKE_ITERATOR_FACADE_function_prototype( inline, cmp_enabler, cmp )
{ return sake::iterator_core_access::cmp(i0.derived(), i1.derived()); }

/*******************************************************************************
 * iterator_facade arithmetic operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_function_prototype( inline, operator_minus_enabler, operator- )
{ return sake::iterator_core_access::operator_minus(i0.derived(), i1.derived()); }

template< class D, class P >
inline typename private_::operator_minus_begin_enabler<D,P>::type
operator-(sake::iterator_facade<D,P> const & i, sake::begin_tag)
{ return sake::iterator_core_access::operator_minus_begin(i.derived()); }

template< class D, class P >
inline typename private_::operator_minus_begin_enabler<D,P>::type
operator-(sake::begin_tag, sake::iterator_facade<D,P> const & i)
{ return -(i.derived() - sake::begin_tag()); }

template< class D, class P >
inline typename private_::operator_minus_end_enabler<D,P>::type
operator-(sake::iterator_facade<D,P> const & i, sake::end_tag)
{ return sake::iterator_core_access::operator_minus_end(i.derived()); }

template< class D, class P >
inline typename private_::operator_minus_end_enabler<D,P>::type
operator-(sake::end_tag, sake::iterator_facade<D,P> const & i)
{ return -(i.derived() - sake::end_tag()); }

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_FACADE_HPP
