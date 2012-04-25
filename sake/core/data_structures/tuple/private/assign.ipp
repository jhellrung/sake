/*******************************************************************************
 * sake/core/data_structures/tuple/private/assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/vector.hpp>

#include <sake/core/data_structures/private/assign_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_TUPLE_INCLUDE_HEADERS

#ifdef SAKE_TUPLE_DEFINE_MEMBERS

#ifndef N
#error
#endif // #ifndef N

private:
    template< class_U0N >
    struct assign_enable
        : boost::mpl::equal<
              values_type,
#ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost_ext::mpl::vector< U0N >,
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost::mpl::BOOST_PP_CAT( vector, N )< U0N >,
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
              boost::mpl::quote2<
                  sake::data_structures_private::assign_enable >
          >
    { };
    template< class_U0N >
    struct assign_enabler
        : boost::enable_if_c< assign_enable< U0N >::value >
    { };

    template< class_U0N >
    void assign_impl( BOOST_PP_ENUM( N, fwd2_ref_Un_xn, ~ ) )
    { BOOST_PP_REPEAT( N, _n_assign_forward_Un_xn, ~ ) }
public:

#if !defined( BOOST_NO_RVALUE_REFERENCES ) || N > 2

    template< class_U0N >
    typename assign_enabler< U0N >::type
    assign( BOOST_PP_ENUM( N, fwd_ref_Un_xn, ~ ) )
    { assign_impl(BOOST_PP_ENUM( N, forward_Un_xn, ~ )); }

#else // #if !defined( BOOST_NO_RVALUE_REFERENCES ) || N > 2

private:
    struct assign0_rv_sink_visitor
    {
        assign0_rv_sink_visitor(tuple& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x0) const
        { m_this._0 = x0; }
    private:
        tuple& m_this;
    };
    typedef sake::rv_sink_traits1<
        T0,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::assign_enable
        >::apply< T0 >::type
    > assign0_rv_sink_traits;
    typedef typename assign0_rv_sink_traits::template
        default_< assign0_rv_sink_visitor > assign0_rv_sink_default_type;
public:

#if N == 1

    // lvalues + explicit movable rvalues
    template< class U0 >
    typename assign0_rv_sink_traits::template ref_enabler< U0 >::type
    assign(U0& x0)
    { assign_impl(x0); }

    // T0 rvalues
    void assign(typename assign0_rv_sink_traits::primary_type x0)
    { assign_impl(x0.move()); }

    // implicit movable rvalues
    void assign(assign0_rv_sink_default_type x0)
    { x0(assign0_rv_sink_visitor(*this)); }

    // const lvalues + non-movable rvalues
    template< class U0 >
    typename assign0_rv_sink_traits::template cref_enabler< U0 >::type
    assign(U0& x0)
    { assign_impl(x0); }

#else // #if N == 1

private:
    struct assign1_rv_sink_visitor
    {
        assign1_rv_sink_visitor(tuple& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x1) const
        { m_this._1 = x1; }
    private:
        tuple& m_this;
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
    { assign_impl(x0, x1.move()); }

    template< class U0 >
    typename assign0_rv_sink_traits::template ref_enabler< U0 >::type
    assign(
        U0& x0,
        assign1_rv_sink_default_type x1)
    {
        _0 = x0;
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
    { assign_impl(x0.move(), x1); }

    void
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        typename assign1_rv_sink_traits::primary_type x1)
    { assign_impl(x0.move(), x1.move()); }

    void
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        assign1_rv_sink_default_type x1)
    {
        _0 = x0.move();
        x1(assign1_rv_sink_visitor(*this));
    }

    template< class U1 >
    typename assign1_rv_sink_traits::template cref_enabler< U1 >::type
    assign(
        typename assign0_rv_sink_traits::primary_type x0,
        U1 const & x1)
    { assign_impl(x0.move(), x1); }

    template< class U1 >
    typename assign1_rv_sink_traits::template ref_enabler< U1 >::type
    assign(
        assign0_rv_sink_default_type x0,
        U1& x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        _1 = x1;
    }

    void
    assign(
        assign0_rv_sink_default_type x0,
        typename assign1_rv_sink_traits::primary_type x1)
    {
        x0(assign0_rv_sink_visitor(*this));
        _1 = x1.move();
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
        _1 = x1;
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
    { assign_impl(x0, x1.move()); }

    template< class U0 >
    typename assign0_rv_sink_traits::template cref_enabler< U0 >::type
    assign(
        U0 const & x0,
        assign1_rv_sink_default_type x1)
    {
        _0 = x0;
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

#endif // #if N == 1

#endif // #if !defined( BOOST_NO_RVALUE_REFERENCES ) || N > 2

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS
