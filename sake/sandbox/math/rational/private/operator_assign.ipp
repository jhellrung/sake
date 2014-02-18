/*******************************************************************************
 * sake/core/math/private/rational/operator_assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/math/one.hpp>
#include <sake/core/math/rational/denom.hpp>
#include <sake/core/math/rational/numer.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#ifdef SAKE_RATIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct operator_assign_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< rational, U >,
              typename base_::template common_enable<U>
          >
    { };
    template< class U >
    struct operator_assign_enabler
        : boost::enable_if_c< operator_assign_enable<
              typename boost_ext::remove_rvalue_reference<U>::type
          >::value, rational& >
    { };

    template< class U >
    typename base_::template
        value_enabler< SAKE_FWD2_PARAM( U ), rational& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        m_numer = sake::forward<U>(x);
        m_denom = sake::one.as<T>();
        return *this;
    }

    template< class U >
    typename base_::template
        rational_enabler< SAKE_FWD2_PARAM( U ), rational& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        m_numer = sake::numer(sake::forward<U>(x));
        m_denom = sake::denom(sake::forward<U>(x));
        return *this;
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename operator_assign_enabler<U>::type
    operator=(U&& x)
    { return operator_assign_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<
        T, boost::mpl::quote1< operator_assign_enable >
    > operator_assign_rv_sink_traits;
    typedef typename operator_assign_rv_sink_traits::template
        default_< sake::rv_sink_visitors::operator_assign< rational > >
        operator_assign_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    typename operator_assign_rv_sink_traits::template
        ref_enabler< U, rational& >::type
    operator=(U& x)
    { return operator_assign_dispatch(x); }

    // T rvalues
    rational& operator=(
        typename operator_assign_rv_sink_traits::primary_type x)
    { return operator_assign_dispatch(sake::move(x.value)); }

    // implicit movable rvalues
    rational& operator=(operator_assign_rv_sink_default_type x)
    { return x(sake::rv_sink_visitors::construct::operator_assign(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename operator_assign_rv_sink_traits::template
        cref_enabler< U, rational& >::type
    operator=(U const & x)
    { return operator_assign_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_RATIONAL_DEFINE_MEMBERS
