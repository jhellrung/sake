/*******************************************************************************
 * sake/core/math/rational/private/assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>

#include <sake/core/data_structures/private/assign_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#ifdef SAKE_RATIONAL_DEFINE_MEMBERS

private:
    template< class U0, class U1 >
    void assign_impl(SAKE_FWD2_REF( U0 ) numer, SAKE_FWD2_REF( U1 ) denom)
    {
        m_numer = sake::forward< U0 >(numer);
        m_denom = sake::forward< U1 >(denom);
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        sake::data_structures_private::assign_enable< T, U0 >,
        sake::data_structures_private::assign_enable< T, U1 >
    >::value >::type
    assign(U0&& numer, U1&& denom)
    { assign_impl(sake::forward< U0 >(numer), sake::forward< U1 >(denom)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    class assign_numer_rv_sink_visitor
    {
        rational& m_this;
        assign_numer_rv_sink_visitor(rational& this_) : m_this(this_) { }
        friend struct rational;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) numer) const
        { m_this.m_numer = numer; }
    };

    class assign_denom_rv_sink_visitor
    {
        rational& m_this;
        assign_denom_rv_sink_visitor(rational& this_) : m_this(this_) { }
        friend struct rational;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) denom) const
        { m_this.m_denom = denom; }
    };

    typedef sake::rv_sink_traits1<
        T,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::assign_enable >::apply<T>::type
    > assign_rv_sink_traits;
    typedef typename assign_rv_sink_traits::template
        default_< assign_numer_rv_sink_visitor >
        assign_numer_rv_sink_default_type;
    typedef typename assign_rv_sink_traits::template
        default_< assign_denom_rv_sink_visitor >
        assign_denom_rv_sink_default_type;
public:

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign_rv_sink_traits::template ref_enable< U0 >,
        typename assign_rv_sink_traits::template ref_enable< U1 >
    >::value >::type
    assign(
        U0& numer,
        U1& denom)
    { assign_impl(numer, denom); }

    template< class U0 >
    typename assign_rv_sink_traits::template ref_enabler< U0 >::type
    assign(
        U0& numer,
        typename assign_rv_sink_traits::primary_type denom)
    { assign_impl(numer, sake::move(denom.value)); }

    template< class U0 >
    typename assign_rv_sink_traits::template ref_enabler< U0 >::type
    assign(
        U0& numer,
        assign_denom_rv_sink_default_type denom)
    {
        m_numer = numer;
        denom(assign_denom_rv_sink_visitor(*this));
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign_rv_sink_traits::template ref_enable< U0 >,
        typename assign_rv_sink_traits::template cref_enable< U1 >
    >::value >::type
    assign(
        U0& numer,
        U1 const & denom)
    { assign_impl(numer, denom); }

    template< class U1 >
    typename assign_rv_sink_traits::template ref_enabler< U1 >::type
    assign(
        typename assign_rv_sink_traits::primary_type numer,
        U1& denom)
    { assign_impl(sake::move(numer.value), denom); }

    void
    assign(
        typename assign_rv_sink_traits::primary_type numer,
        typename assign_rv_sink_traits::primary_type denom)
    { assign_impl(sake::move(numer.value), sake::move(denom.value)); }

    void
    assign(
        typename assign_rv_sink_traits::primary_type numer,
        assign_denom_rv_sink_default_type denom)
    {
        m_numer = sake::move(numer.value);
        denom(assign_denom_rv_sink_visitor(*this));
    }

    template< class U1 >
    typename assign_rv_sink_traits::template cref_enabler< U1 >::type
    assign(
        typename assign_rv_sink_traits::primary_type numer,
        U1 const & denom)
    { assign_impl(sake::move(numer.value), denom); }

    template< class U1 >
    typename assign_rv_sink_traits::template ref_enabler< U1 >::type
    assign(
        assign_numer_rv_sink_default_type numer,
        U1& denom)
    {
        numer(assign_numer_rv_sink_visitor(*this));
        m_denom = denom;
    }

    void
    assign(
        assign_numer_rv_sink_default_type numer,
        typename assign_rv_sink_traits::primary_type denom)
    {
        numer(assign_numer_rv_sink_visitor(*this));
        m_denom = sake::move(denom.value);
    }

    void
    assign(
        assign_numer_rv_sink_default_type numer,
        assign_denom_rv_sink_default_type denom)
    {
        numer(assign_numer_rv_sink_visitor(*this));
        denom(assign_denom_rv_sink_visitor(*this));
    }

    template< class U1 >
    typename assign_rv_sink_traits::template cref_enabler< U1 >::type
    assign(
        assign_numer_rv_sink_default_type numer,
        U1 const & denom)
    {
        numer(assign_numer_rv_sink_visitor(*this));
        m_denom = denom;
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign_rv_sink_traits::template cref_enable< U0 >,
        typename assign_rv_sink_traits::template ref_enable< U1 >
    >::value >::type
    assign(
        U0 const & numer,
        U1& denom)
    { assign_impl(numer, denom); }

    template< class U0 >
    typename assign_rv_sink_traits::template cref_enabler< U0 >::type
    assign(
        U0 const & numer,
        typename assign_rv_sink_traits::primary_type denom)
    { assign_impl(numer, sake::move(denom.value)); }

    template< class U0 >
    typename assign_rv_sink_traits::template cref_enabler< U0 >::type
    assign(
        U0 const & numer,
        assign_denom_rv_sink_default_type denom)
    {
        m_numer = numer;
        denom(assign_denom_rv_sink_visitor(*this));
    }

    template< class U0, class U1 >
    typename boost::enable_if_c< boost_ext::mpl::and2<
        typename assign_rv_sink_traits::template cref_enable< U0 >,
        typename assign_rv_sink_traits::template cref_enable< U1 >
    >::value >::type
    assign(
        U0 const & numer,
        U1 const & denom)
    { assign_impl(numer, denom); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_RATIONAL_DEFINE_MEMBERS
