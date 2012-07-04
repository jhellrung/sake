/*******************************************************************************
 * sake/core/data_structures/pair/private/assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_PAIR_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/data_structures/private/assign_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_PAIR_INCLUDE_HEADERS

#ifdef SAKE_PAIR_DEFINE_MEMBERS

private:
    template< class U0, class U1 >
    void assign_impl(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
    {
        first = sake::forward< U0 >(x0);
        second = sake::forward< U1 >(x1);
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        sake::data_structures_private::assign_enable< T0, U0 >,
        sake::data_structures_private::assign_enable< T1, U1 >
    >::value >::type
    assign(U0&& x0, U1&& x1)
    { assign_impl(sake::forward< U0 >(x0), sake::forward< U1 >(x1)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    class assign0_rv_sink_visitor
    {
        pair& m_this;
        assign0_rv_sink_visitor(pair& this_) : m_this(this_) { }
        friend struct pair;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x0) const
        { m_this.first = x0; }
    };
    typedef sake::rv_sink_traits1<
        T0,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::assign_enable
        >::apply< T0 >::type
    > assign0_rv_sink_traits;
    typedef typename assign0_rv_sink_traits::template
        default_< assign0_rv_sink_visitor > assign0_rv_sink_default_type;

    class assign1_rv_sink_visitor
    {
        pair& m_this;
        assign1_rv_sink_visitor(pair& this_) : m_this(this_) { }
        friend struct pair;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x1) const
        { m_this.second = x1; }
    };
    typedef sake::rv_sink_traits1<
        T1,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::assign_enable
        >::apply< T1 >::type
    > assign1_rv_sink_traits;
    typedef typename assign1_rv_sink_traits::template
        default_< assign1_rv_sink_visitor > assign1_rv_sink_default_type;
public:

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign0_rv_sink_traits::template ref_enable< U0 >,
        typename assign1_rv_sink_traits::template ref_enable< U1 >
    >::value >::type
    assign(
        U0& x0,
        U1& x1)
    { assign_impl(x0, x1); }

    template< class U0 >
    typename assign0_rv_sink_traits::template ref_enabler< U0 >::type
    assign(
        U0& x0,
        typename assign1_rv_sink_traits::primary_type x1)
    { assign_impl(x0, sake::move(x1.value)); }

    template< class U0 >
    typename assign0_rv_sink_traits::template ref_enabler< U0 >::type
    assign(
        U0& x0,
        assign1_rv_sink_default_type x1)
    {
        first = x0;
        x1(assign1_rv_sink_visitor(*this));
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign0_rv_sink_traits::template ref_enable< U0 >,
        typename assign1_rv_sink_traits::template cref_enable< U1 >
    >::value >::type
    assign(
        U0& x0,
        U1 const & x1)
    { assign_impl(x0, x1); }

    template< class U1 >
    typename assign1_rv_sink_traits::template ref_enabler< U1 >::type
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        U1& x1)
    { assign_impl(sake::move(x0.value), x1); }

    void
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        typename assign1_rv_sink_traits::primary_type x1)
    { assign_impl(sake::move(x0.value), sake::move(x1.value)); }

    void
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        assign1_rv_sink_default_type x1)
    {
        first = sake::move(x0.value);
        x1(assign1_rv_sink_visitor(*this));
    }

    template< class U1 >
    typename assign1_rv_sink_traits::template cref_enabler< U1 >::type
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        U1 const & x1)
    { assign_impl(sake::move(x0.value), x1); }

    template< class U1 >
    typename assign1_rv_sink_traits::template ref_enabler< U1 >::type
    assign(
        assign0_rv_sink_default_type x0,
        U1& x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        second = x1;
    }

    void
    assign(
        assign0_rv_sink_default_type x0,
        typename assign1_rv_sink_traits::primary_type x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        second = sake::move(x1.value);
    }

    void
    assign(
        assign0_rv_sink_default_type x0,
        assign1_rv_sink_default_type x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        x1(assign1_rv_sink_visitor(*this));
    }

    template< class U1 >
    typename assign1_rv_sink_traits::template cref_enabler< U1 >::type
    assign(
        assign0_rv_sink_default_type x0,
        U1 const & x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        second = x1;
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign0_rv_sink_traits::template cref_enable< U0 >,
        typename assign1_rv_sink_traits::template ref_enable< U1 >
    >::value >::type
    assign(
        U0 const & x0,
        U1& x1)
    { assign_impl(x0, x1); }

    template< class U0 >
    typename assign0_rv_sink_traits::template cref_enabler< U0 >::type
    assign(
        U0 const & x0,
        typename assign1_rv_sink_traits::primary_type x1)
    { assign_impl(x0, sake::move(x1.value)); }

    template< class U0 >
    typename assign0_rv_sink_traits::template cref_enabler< U0 >::type
    assign(
        U0 const & x0,
        assign1_rv_sink_default_type x1)
    {
        first = x0;
        x1(assign1_rv_sink_visitor(*this));
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign0_rv_sink_traits::template cref_enable< U0 >,
        typename assign1_rv_sink_traits::template cref_enable< U1 >
    >::value >::type
    assign(
        U0 const & x0,
        U1 const & x1)
    { assign_impl(x0, x1); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_PAIR_DEFINE_MEMBERS
