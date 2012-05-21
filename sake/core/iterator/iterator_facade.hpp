/*******************************************************************************
 * sake/core/iterator/iterator_facade.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_facade<
 *     Derived,
 *     Value, Reference, Difference,
 *     Traversal, Introversal,
 *     ChainedBase = sake::void_
 * >
 *
 * SAKE_ITERATOR_ENABLE_CURSOR( Base )
 *
 * boost::iterator_facade has a couple quirky behaviors, including how it
 * determines iterator_category when using proxy references and its broken
 * operator-> proxy.  sake::iterator_facade fixes these.  In addition, it
 * provides a cursor interface.
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ITERATOR_FACADE_HPP
#define SAKE_CORE_ITERATOR_ITERATOR_FACADE_HPP

#include <cstddef>

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/introversal.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/traits.hpp>
#include <sake/core/iterator/private/category.hpp>
#include <sake/core/iterator/private/facade/common_difference_type.hpp>
#include <sake/core/iterator/private/facade/function_prototype.hpp>
#include <sake/core/iterator/private/facade/operator_arrow_dispatch.hpp>
#include <sake/core/iterator/private/facade/operator_bracket_dispatch.hpp>
#include <sake/core/iterator/private/facade/operator_post_increment_dispatch.hpp>
#include <sake/core/iterator/iterator_core_access.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/memberwise/default_constructor.hpp>
#include <sake/core/utility/void.hpp>

namespace sake
{

// This macro is necessary to ensure the cursor member functions can be found via introspection.
#define SAKE_ITERATOR_ENABLE_CURSOR( Base ) \
    using Base::at_begin; \
    using Base::at_end; \
    using Base::to_begin; \
    using Base::to_end; \
    using Base::begin; \
    using Base::end; \
    using Base::distance_from_begin; \
    using Base::distance_to_end; \
    using Base::distance_between_begin_end;

#define SAKE_ITERATOR_ASSERT_TRAVERSAL( Traversal ) \
    BOOST_STATIC_ASSERT((::sake::boost_ext::is_convertible< iterator_traversal, Traversal >::value))
#define SAKE_ITERATOR_ASSERT_INTROVERSAL( Introversal ) \
    BOOST_STATIC_ASSERT((::sake::boost_ext::is_convertible< cursor_introversal, Introversal >::value))

/*******************************************************************************
 * struct iterator_facade<
 *     Derived,
 *     Value, Reference, Difference,
 *     Traversal, Introversal,
 *     ChainedBase
 * >
 ******************************************************************************/

template<
    class Derived,
    class Value, class Reference, class Difference,
    class Traversal, class Introversal,
    class ChainedBase = sake::void_
>
struct iterator_facade
    : ChainedBase
{
    typedef Derived derived_type;
    derived_type& derived() { return *static_cast< derived_type* >(this); }
    derived_type const & derived() const { return *static_cast< derived_type const * >(this); }

    typedef typename boost::remove_const< Value >::type value_type;
    typedef Reference reference;
    typedef Difference difference_type;
    typedef typename iterator_private::category< Traversal, Value >::type iterator_category;
    typedef Traversal iterator_traversal;
    typedef Introversal cursor_introversal;

    /***************************************************************************
     * Dereferenceable
     * operator*() const -> reference
     * operator->() const -> pointer
     **************************************************************************/
    reference
    operator*() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        return iterator_core_access::operator_star(derived());
    }
private:
    typedef iterator_facade_private::operator_arrow_dispatch<
        Reference
    > operator_arrow_dispatch_;
public:
    typedef typename operator_arrow_dispatch_::result_type pointer;
    pointer
    operator->() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        return operator_arrow_dispatch_::apply(operator*());
    }

    /***************************************************************************
     * Incrementable
     * operator++() -> derived_type&
     * operator++(int) -> ...
     **************************************************************************/
    derived_type&
    operator++()
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        iterator_core_access::operator_pre_increment(derived());
        return derived();
    }
private:
    typedef iterator_facade_private::operator_post_increment_dispatch<
        Value, Reference, Traversal, Derived
    > operator_post_increment_dispatch_;
public:
    typename operator_post_increment_dispatch_::result_type
    operator++(int)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        typename operator_post_increment_dispatch_::result_type result(derived());
        operator++();
        return result;
    }

    /***************************************************************************
     * Bidirectional
     * operator--() -> derived_type&
     * operator--(int) -> derived_type
     **************************************************************************/
    derived_type&
    operator--()
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        iterator_core_access::operator_pre_decrement(derived());
        return derived();
    }
    derived_type
    operator--(int)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        derived_type result(derived());
        operator--();
        return result;
    }

    /***************************************************************************
     * RandomAccess
     * operator[](difference_type const n) const -> ...
     * operator+=(difference_type const n) -> derived_type&
     * operator-=(difference_type const n) -> derived_type&
     * operator+(difference_type const n) -> derived_type
     * operator-(difference_type const n) -> derived_type
     **************************************************************************/
private:
    typedef iterator_facade_private::operator_bracket_dispatch<
        Value, Reference, Derived
    > operator_bracket_dispatch_;
public:
    typename operator_bracket_dispatch_::result_type
    operator[](difference_type const n) const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        return operator_bracket_dispatch_::apply(derived() + n);
    }
    derived_type&
    operator+=(difference_type const n)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        iterator_core_access::operator_plus_equal(derived(), n);
        return derived();
    }
    derived_type&
    operator-=(difference_type const n)
    { return operator+=(-n); }
    derived_type
    operator+(difference_type const n)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        return iterator_core_access::operator_plus(derived(), n);
    }
    derived_type
    operator-(difference_type const n)
    { return operator+(-n); }

    /***************************************************************************
     * BeginDetect
     * at_begin() const -> bool
     **************************************************************************/
    bool
    at_begin() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::begin_detect_introversal_tag );
        return iterator_core_access::at_begin(derived());
    }
    /***************************************************************************
     * BeginAccess
     * to_begin() -> derived_type&
     * begin() -> derived_type
     **************************************************************************/
    derived_type&
    to_begin()
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::begin_access_introversal_tag );
        iterator_core_access::to_begin(derived());
        return derived();
    }
    derived_type
    begin() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::begin_access_introversal_tag );
        return iterator_core_access::begin(derived());
    }

    /***************************************************************************
     * EndDetect
     * at_end() const -> bool
     **************************************************************************/
    bool
    at_end() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::end_detect_introversal_tag );
        return iterator_core_access::at_end(derived());
    }
    /***************************************************************************
     * EndAccess
     * to_end() -> derived_type&
     * end() -> derived_type
     **************************************************************************/
    derived_type&
    to_end()
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::end_access_introversal_tag );
        iterator_core_access::to_end(derived());
        return derived();
    }
    derived_type end() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::end_access_introversal_tag );
        return iterator_core_access::end(derived());
    }

    /***************************************************************************
     * RandomAccess + BeginAccess
     **************************************************************************/
    difference_type
    distance_from_begin() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::begin_detect_introversal_tag );
        return iterator_core_access::distance_from_begin(derived());
    }
    /***************************************************************************
     * RandomAccess + EndAccess
     **************************************************************************/
    difference_type
    distance_to_end() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( sake::end_detect_introversal_tag );
        return iterator_core_access::distance_to_end(derived());
    }
    /***************************************************************************
     * RandomAccess + BeginAccess + EndAccess
     **************************************************************************/
    difference_type
    distance_between_begin_end() const
    { return iterator_core_access::distance_between_begin_end(derived()); }

protected:
    typedef ChainedBase chained_base_type;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR( iterator_facade, BOOST_PP_SEQ_NIL )

    template< class ChainedBaseConstruct >
    explicit iterator_facade(ChainedBaseConstruct const & chained_base_construct,
        typename boost::enable_if_c<
            !boost::is_base_of< iterator_facade, ChainedBaseConstruct >::value
         && (boost_ext::is_convertible< ChainedBaseConstruct const &, ChainedBase >::value
          || sake::is_emplacer< ChainedBaseConstruct >::value)
        >::type* = 0)
        : ChainedBase(sake::emplacer_constructible< ChainedBase >(chained_base_construct))
    { }

    template< class, class, class, class, class, class, class > friend struct iterator_facade;
    template< class D, class V, class R, class F, class T, class I, class B >
    iterator_facade(iterator_facade<D,V,R,F,T,I,B> const & other,
        typename boost::enable_if_c<
            boost_ext::is_convertible<
                typename iterator_facade<D,V,R,F,T,I,B>::chained_base_type const &,
                ChainedBase
            >::value
        >::type* = 0)
        : ChainedBase(static_cast< typename iterator_facade<D,V,R,F,T,I,B>::chained_base_type const & >(other))
    { }

    friend class iterator_core_access;

    template< class T >
    sake::sign_t
    cmp_impl(T const & other) const
    { return sake::sign(derived() - other); }
    template< class T >
    bool
    operator_equal_impl(T const & other) const
    { return sake::cmp(derived(), other) == sake::zero; }
    template< class T >
    bool
    operator_less_impl(T const & other) const
    { return sake::cmp(derived(), other) < sake::zero; }
    template< class T >
    bool
    operator_less_equal_impl(T const & other) const
    { return sake::cmp(derived(), other) <= sake::zero; }

    derived_type
    operator_plus_impl(difference_type const n) const
    { derived_type result(derived()); return result += n; }

    derived_type
    begin_impl() const
    { derived_type result(derived()); return result.to_begin(); }
    derived_type
    end_impl() const
    { derived_type result(derived()); return result.to_end(); }

    difference_type
    distance_from_begin_impl() const
    { return derived() - begin(); }
    difference_type
    distance_to_end_impl() const
    { return end() - derived(); }
    difference_type
    distance_between_begin_end_impl() const
    { return end() - begin(); }
};

/*******************************************************************************
 * iterator_facade relational operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator== )
{ return iterator_core_access::operator_equal(i0.derived(), i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator!= )
{ return !(i0.derived() == i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator< )
{ return iterator_core_access::operator_less(i0.derived(), i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator> )
{ return i1.derived() < i0.derived(); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator<= )
{ return !(i1.derived() < i0.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< bool >, operator>= )
{ return !(i0.derived() < i1.derived()); }
SAKE_ITERATOR_FACADE_function_prototype( inline, boost::mpl::always< sake::sign_t >, cmp )
{ return iterator_core_access::cmp(i0.derived(), i1.derived()); }

/*******************************************************************************
 * iterator_facade arithmetic operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_function_prototype(
    inline, boost::mpl::quote2< iterator_facade_private::common_difference_type >, operator-
) { return iterator_core_access::operator_minus(i0.derived(), i1.derived()); }

template< class Derived, class V, class R, class F, class T, class I, class B >
inline Derived
operator+(
    typename Derived::difference_type const n,
    iterator_facade< Derived, V,R,F,T,I,B > const & i)
{ return i + n; }

} // namespace sake

#endif // SAKE_CORE_ITERATOR_ITERATOR_FACADE_HPP
