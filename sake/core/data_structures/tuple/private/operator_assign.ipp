/*******************************************************************************
 * sake/core/data_structures/tuple/private/operator_assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/tuple/private/operator_assign_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_TUPLE_INCLUDE_HEADERS

#ifdef SAKE_TUPLE_DEFINE_MEMBERS

#ifndef N
#error
#endif // #ifndef N

private:
    template< class Sequence >
    tuple& operator_assign_impl(SAKE_FWD2_REF( Sequence ) s)
    {
        BOOST_PP_REPEAT( N, _n_assign_at_c_n_forward_Sequence_s, ~ )
        return *this;
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class Sequence >
    struct operator_assign_enabler
        : sake::tuple_private::operator_assign_enabler< tuple, Sequence >
    { };
public:
    template< class Sequence >
    typename operator_assign_enabler< Sequence >::type
    operator=(Sequence&& s)
    { return operator_assign_impl(sake::forward< Sequence >(s)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits0<
        typename boost_ext::mpl::curry_quote2<
            sake::tuple_private::operator_assign_enable
        >::apply< tuple >::type
    > operator_assign_rv_sink_traits;
    typedef typename operator_assign_rv_sink_traits::template
        default_< sake::rv_sink_visitors::operator_assign< tuple > >
        operator_assign_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class Sequence >
    typename operator_assign_rv_sink_traits::template
        ref_enabler< Sequence, tuple& >::type
    operator=(Sequence& s)
    { return operator_assign_impl(s); }

    // implicit movable rvalues
    tuple& operator=(operator_assign_rv_sink_default_type s)
    { return s(sake::rv_sink_visitors::construct::operator_assign(*this)); }

    // const lvalues + non-movable rvalues
    template< class Sequence >
    typename operator_assign_rv_sink_traits::template
        cref_enabler< Sequence, tuple& >::type
    operator=(Sequence const & s)
    { return operator_assign_impl(s); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS
