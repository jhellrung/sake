/*******************************************************************************
 * core/iterator/iterator_facade.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_facade<
 *     Derived,
 *     Value,
 *     Traversal,
 *     Introversal,
 *     Reference,
 *     Difference,
 *     ChainedBase
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
#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/core/cursor/introversal.hpp>
#include <sake/core/iterator/detail_facade/category.hpp>
#include <sake/core/iterator/detail_facade/operator_arrow_dispatch.hpp>
#include <sake/core/iterator/detail_facade/operator_bracket_dispatch.hpp>
#include <sake/core/iterator/detail_facade/operator_prototype.hpp>
#include <sake/core/iterator/detail_facade/postincrement_dispatch.hpp>
#include <sake/core/iterator/iterator_core_access.hpp>
#include <sake/core/math/compare.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/autogen_default_ctor.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>
#include <sake/core/utility/void_t.hpp>

namespace sake
{

// This macro is necessary to ensure the cursor member functions can be found via introspection.
#define SAKE_ITERATOR_ENABLE_CURSOR( Base ) \
    using Base ::at_begin; \
    using Base ::at_end; \
    using Base ::to_begin; \
    using Base ::to_end; \
    using Base ::begin; \
    using Base ::end; \
    using Base ::begin_distance; \
    using Base ::end_distance; \
    using Base ::begin_end_distance;

#define SAKE_ITERATOR_ASSERT_TRAVERSAL( Traversal ) \
    BOOST_MPL_ASSERT((::sake::boost_ext::is_convertible< iterator_traversal, Traversal >))
#define SAKE_ITERATOR_ASSERT_INTROVERSAL( Introversal ) \
    BOOST_MPL_ASSERT((::sake::boost_ext::is_convertible< cursor_introversal, Introversal >))

/*******************************************************************************
 * struct iterator_facade<
 *     Derived,
 *     Value,
 *     Traversal,
 *     Introversal,
 *     Reference,
 *     Difference,
 *     ChainedBase
 * >
 ******************************************************************************/

template<
    class Derived,
    class Value,
    class Traversal,
    class Introversal = null_introversal_tag,
    class Reference   = Value&,
    class Difference  = std::ptrdiff_t,
    class ChainedBase = void_t
>
struct iterator_facade
    : ChainedBase
{
    typedef Derived derived_type;
    derived_type& derived() { return *static_cast< derived_type* >(this); }
    const derived_type& derived() const { return *static_cast< const derived_type* >(this); }

    typedef typename boost::remove_const< Value >::type value_type;
    typedef Reference reference;
    typedef Difference difference_type;
    typedef typename detail_iterator_facade::category<
        Traversal, Value
    >::type iterator_category;
    typedef Traversal iterator_traversal;
    typedef Introversal cursor_introversal;
private:
    typedef typename call_traits< difference_type >::param_type difference_param_type;
public:

    reference operator*() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        return iterator_core_access::dereference(derived());
    }
private:
    typedef detail_iterator_facade::operator_arrow_dispatch< Reference > operator_arrow_dispatch_;
public:
    typedef typename operator_arrow_dispatch_::result_type pointer;
    pointer operator->() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        return operator_arrow_dispatch_::apply(operator*());
    }

    // Incrementable
    derived_type& operator++()
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        iterator_core_access::increment(derived());
        return derived();
    }
private:
    typedef detail_iterator_facade::postincrement_dispatch<
        Value, Reference, Traversal, Derived
    > postincrement_dispatch_;
public:
    typename postincrement_dispatch_::result_type
    operator++(int)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::incrementable_traversal_tag );
        typename postincrement_dispatch_::result_type result(derived());
        operator++();
        return result;
    }

    // Bidirectional
    derived_type& operator--()
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        iterator_core_access::decrement(derived());
        return derived();
    }
    derived_type operator--(int)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        derived_type result(derived());
        operator--();
        return result;
    }

    // RandomAccess
private:
    typedef detail_iterator_facade::operator_bracket_dispatch<
        Value, Reference, Derived
    > operator_bracket_dispatch_;
public:
    typename operator_bracket_dispatch_::result_type
    operator[](difference_param_type n) const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        return operator_bracket_dispatch_::apply(derived() + n);
    }
    derived_type& operator+=(difference_param_type n)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        iterator_core_access::advance(derived(), n);
        return derived();
    }
    derived_type& operator-=(difference_param_type n)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        return operator+=(-n);
    }
    //derived_type operator+(difference_param_type n)
    //{
    //    derived_type result(derived());
    //    return result += n;
    //}
    derived_type operator-(difference_param_type n)
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::random_access_traversal_tag );
        derived_type result(derived());
        return result -= n;
    }

    // BeginDetect
    bool at_begin() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( begin_detect_introversal_tag );
        return iterator_core_access::at_begin(derived());
    }
    // BeginAccess
    derived_type& to_begin()
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( begin_access_introversal_tag );
        iterator_core_access::to_begin(derived());
        return derived();
    }
    derived_type begin() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( begin_access_introversal_tag );
        return iterator_core_access::begin(derived());
    }

    // EndDetect
    bool at_end() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( end_detect_introversal_tag );
        return iterator_core_access::at_end(derived());
    }
    // EndAccess
    derived_type& to_end()
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( end_access_introversal_tag );
        iterator_core_access::to_end(derived());
        return derived();
    }
    derived_type end() const
    {
        SAKE_ITERATOR_ASSERT_INTROVERSAL( end_access_introversal_tag );
        return iterator_core_access::end(derived());
    }

    // Bidirectional + BeginDetect
    difference_type begin_distance() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        SAKE_ITERATOR_ASSERT_INTROVERSAL( begin_detect_introversal_tag );
        return iterator_core_access::begin_distance(derived());
    }
    // Forward + EndDetect
    difference_type end_distance() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::forward_traversal_tag );
        SAKE_ITERATOR_ASSERT_INTROVERSAL( end_detect_introversal_tag );
        return iterator_core_access::end_distance(derived());
    }
    // Bidirectional + BeginDetect + EndDetect
    difference_type begin_end_distance() const
    {
        SAKE_ITERATOR_ASSERT_TRAVERSAL( boost::bidirectional_traversal_tag );
        SAKE_ITERATOR_ASSERT_INTROVERSAL( begin_detect_introversal_tag );
        SAKE_ITERATOR_ASSERT_INTROVERSAL( end_detect_introversal_tag );
        return iterator_core_access::begin_end_distance(derived());
    }

protected:
    typedef ChainedBase chained_base_type;

    SAKE_AUTOGEN_DEFAULT_CTOR( iterator_facade )

    template< class ChainedBaseConstruct >
    explicit iterator_facade(const ChainedBaseConstruct& chained_base_construct,
        typename boost::enable_if< boost::mpl::and_<
            boost::mpl::not_< boost::is_base_of< iterator_facade, ChainedBaseConstruct > >,
            boost::mpl::or_<
                boost_ext::is_convertible< const ChainedBaseConstruct&, ChainedBase >,
                is_maybe_typed_emplacer< ChainedBaseConstruct >
            >
        > >::type* = 0)
        : ChainedBase(emplacer_construct< ChainedBase >(chained_base_construct))
    { }

    template<class,class,class,class,class,class,class> friend struct iterator_facade;
    template< class D, class V, class T, class I, class R, class F, class C >
    iterator_facade(const iterator_facade<D,V,T,I,R,F,C>& other,
        typename boost::enable_if< boost_ext::is_convertible<
            typename iterator_facade<D,V,T,I,R,F,C>::chained_base_type const &,
            ChainedBase
        > >::type* = 0)
        : ChainedBase(static_cast< typename iterator_facade<D,V,T,I,R,F,C>::chained_base_type const & >(other))
    { }

    friend class iterator_core_access;

    template< class T >
    sake::sign_t compare_impl(const T& other) const
    { return adl::sign(derived() - other); }
    template< class T >
    bool equal_to_impl(const T& other) const
    { return adl::compare(derived(), other) == sake::zero; }
    template< class T >
    bool less_impl(const T& other) const
    { return adl::compare(derived(), other) < sake::zero; }
    template< class T >
    bool less_equal_impl(const T& other) const
    { return adl::compare(derived(), other) <= sake::zero; }

    derived_type begin_impl() const
    {
        derived_type result(derived());
        return result.to_begin();
    }
    derived_type end_impl() const
    {
        derived_type result(derived());
        return result.to_end();
    }

    difference_type begin_distance_impl() const
    { return derived() - begin(); }
    difference_type end_distance_impl() const
    { return end() - derived(); }
    difference_type begin_end_distance_impl() const
    { return end() - begin(); }
};

/*******************************************************************************
 * iterator_facade comparison operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, == )
{ return iterator_core_access::equal_to(it0.derived(), it1.derived()); }
SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, != )
{ return !(it0.derived() == it1.derived()); }
SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, <  )
{ return iterator_core_access::less(it0.derived(), it1.derived()); }
SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, >  )
{ return !(it0.derived() <= it1.derived()); }
SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, <= )
{ return iterator_core_access::less_equal(it0.derived(), it1.derived()); }
SAKE_ITERATOR_FACADE_operator_prototype( inline, boost::mpl::always< bool >, >= )
{ return !(it0.derived() < it1.derived()); }

template<
    class D0, class V0, class T0, class I0, class R0, class F0, class C0,
    class D1, class V1, class T1, class I1, class R1, class F1, class C1
>
inline typename detail_iterator_facade::lazy_enable_if_is_interoperable< D0, D1, sake::sign_t >::type
compare(
    const ::sake::iterator_facade< D0, V0, T0, I0, R0, F0, C0 >& it0,
    const ::sake::iterator_facade< D1, V1, T1, I1, R1, F1, C1 >& it1)
{ return iterator_core_access::compare(it0.derived(), it1.derived()); }

/*******************************************************************************
 * iterator_facade arithmetic operators
 ******************************************************************************/

SAKE_ITERATOR_FACADE_operator_prototype(
    inline,
    boost::mpl::quote2< detail_iterator_facade::common_difference_type >,
    - )
{ return iterator_core_access::difference(it0.derived(), it1.derived()); }

template< class D, class V, class T, class I, class R, class F, class C >
inline typename boost::enable_if<
    boost_ext::is_convertible<
        typename iterator_facade<D,V,T,I,R,F,C>::iterator_traversal,
        boost::random_access_traversal_tag
    >,
    D
>::type
operator+(
    const iterator_facade<D,V,T,I,R,F,C>& it,
    typename call_traits< typename D::difference_type >::param_type n)
{
    D result(it.derived());
    return result += n;
}

template< class D, class V, class T, class I, class R, class F, class C >
inline typename boost::enable_if<
    boost_ext::is_convertible<
        typename iterator_facade<D,V,T,I,R,F,C>::iterator_traversal,
        boost::random_access_traversal_tag
    >,
    D
>::type
operator+(
    typename call_traits< typename D::difference_type >::param_type n,
    const iterator_facade<D,V,T,I,R,F,C>& it)
{
    D result(it.derived());
    return result += n;
}

} // namespace sake

#endif // SAKE_CORE_ITERATOR_ITERATOR_FACADE_HPP
