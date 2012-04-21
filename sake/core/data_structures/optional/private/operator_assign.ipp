/*******************************************************************************
 * sake/core/data_structures/optional/private/operator_assign.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/optional/private/operator_assign_dispatch.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/get.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct enable_cond_operator_assign
        : boost_ext::mpl::and2<
              boost::mpl::not_<
                  boost_ext::is_base_of_sans_qualifiers< optional, U > >,
              boost_ext::mpl::or3<
                  enable_cond_for_value<U>,
                  enable_cond_for_optional<U>,
                  enable_cond_for_emplacer<U>
              >
          >
    { };
    template< class U >
    struct enable_operator_assign
        : boost::enable_if_c< enable_cond_operator_assign<U>::value >
    { };
public:

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    typename enable_for_value< SAKE_FWD2_PARAM( U ), optional& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        private_::operator_assign_dispatch< T, SAKE_FWD2_PARAM( U ) >::
            apply(*this, sake::forward<U>(x));
        return *this;
    }

    template< class U >
    typename enable_for_optional< SAKE_FWD2_PARAM( U ), optional& >::type
    operator_assign_dispatch(SAKE_FWD2_REF( U ) x)
    {
        if(x.initialized())
            return operator_assign_dispatch(sake::get(sake::forward<U>(x)));
        reset();
        return *this;
    }

    template< class Emplacer >
    typename enable_for_emplacer< Emplacer, optional& >::type
    operator_assign_dispatch(Emplacer e)
    {
        reset(e);
        return *this;
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename enable_operator_assign<U>::type
    operator=(U&& x)
    { return operator_assign_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct operator_assign_rv_sink
    {
        template< class U >
        struct apply
            : boost_ext::mpl::and2<
                  enable_cond_operator_assign<U>,
                  boost::mpl::not_< boost::is_same< U, nocv_type > >
              >
        { };
        typedef sake::rv_sink<
            sake::rv_sink_visitors::operator_assign< optional >,
            optional&,
            operator_assign_rv_sink
        > type;
    };

    template< class U >
    struct enable_operator_assign_cref
        : boost::disable_if_c< boost_ext::mpl::or3<
              boost::mpl::not_< enable_cond_operator_assign< U const & > >,
              boost_ext::is_same_sans_qualifiers< U, rv_param_type >,
              sake::is_movable<U>
          >::value, optional& >
    { };
public:

    // lvalues + explicit movable rvalues
    template< class U >
    typename enable_operator_assign<
        typename boost_ext::remove_rvalue_reference< U& >::type
    >::type
    operator=(U& x)
    { return operator_assign_dispatch(x); }

    // T rvalues
    optional& operator=(rv_param_type x)
    { return operator_assign_dispatch(x); }

    // movable implicit rvalues
    optional& operator=(typename operator_assign_rv_sink::type x)
    { return x(sake::rv_sink_visitors::make_operator_assign(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename enable_operator_assign_cref<U>::type
    operator=(U const & x)
    { return operator_assign_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename enable_operator_assign<U>::type
    operator=(U&& x)
    { mp = get_ptr_dispatch(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename enable_operator_assign<
        typename boost_ext::remove_rvalue_reference< U& >::type
    >::type
    operator=(U& x)
    { mp = get_ptr_dispatch(SAKE_AS_LVALUE(x)); return *this; }

    template< class U >
    typename enable_operator_assign<
        typename boost_ext::remove_rvalue_reference< U const & >::type
    >::type
    operator=(U const & x)
    { mp = get_ptr_dispatch(x); return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
