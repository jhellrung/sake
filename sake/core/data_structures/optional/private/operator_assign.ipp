/*******************************************************************************
 * sake/core/data_structures/optional/private/operator_assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/optional/private/operator_assign_dispatch.hpp>
#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/get.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct operator_assign_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< optional, U >,
              common_enable<U>
          >
    { };
    template< class U >
    struct operator_assign_enabler
        : boost::enable_if_c< operator_assign_enable<
              typename boost_ext::remove_rvalue_reference<U>::type
          >::value, optional& >
    { };
public:

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    typename value_enabler< SAKE_FWD2_PARAM( U ), optional& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        private_::operator_assign_dispatch< T, SAKE_FWD2_PARAM( U ) >::
            apply(*this, sake::forward<U>(x));
        return *this;
    }

    template< class U >
    typename optional_enabler< SAKE_FWD2_PARAM( U ), optional& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        if(x.initialized())
            return operator_assign_dispatch(sake::get(sake::forward<U>(x)));
        reset();
        return *this;
    }

    template< class Emplacer >
    typename emplacer_enabler< Emplacer, optional& >::type
    operator_assign_dispatch(Emplacer e)
    {
        reset(e);
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
        nocv_type, boost::mpl::quote1< operator_assign_enable >
    > operator_assign_rv_sink_traits;
    typedef typename operator_assign_rv_sink_traits::template
        default_< sake::rv_sink_visitors::operator_assign< optional > >
        operator_assign_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    typename operator_assign_rv_sink_traits::template
        ref_enabler< U, optional& >::type
    operator=(U& x)
    { return operator_assign_dispatch(x); }

    // T rvalues
    optional& operator=(
        typename operator_assign_rv_sink_traits::primary_type x)
    { return operator_assign_dispatch(sake::move(x.value)); }

    // movable implicit rvalues
    optional& operator=(operator_assign_rv_sink_default_type x)
    { return x(sake::rv_sink_visitors::construct::operator_assign(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename operator_assign_rv_sink_traits::template
        cref_enabler< U, optional& >::type
    operator=(U const & x)
    { return operator_assign_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename operator_assign_enabler<U>::type
    operator=(U&& x)
    { m_p = get_ptr_dispatch(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename operator_assign_enabler< U& >::type
    operator=(U& x)
    { m_p = get_ptr_dispatch(SAKE_AS_LVALUE(x)); return *this; }

    template< class U >
    typename operator_assign_enabler< U const & >::type
    operator=(U const & x)
    { m_p = get_ptr_dispatch(x); return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
