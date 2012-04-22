/*******************************************************************************
 * sake/core/data_structures/optional/private/implicit_constructor.ipp
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
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/get.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct enable_cond_implicit_constructor
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< optional, U >,
              boost_ext::mpl::or2<
                  enable_cond_for_value<U>,
                  enable_cond_for_optional<U>
              >
          >
    { };
    template< class U >
    struct enable_implicit_constructor
        : boost::enable_if_c< enable_cond_implicit_constructor<U>::value >
    { };
public:

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    typename enable_for_value< SAKE_FWD2_PARAM( U ), bool >::type
    static initialize_m_initialized_dispatch(SAKE_FWD2_REF( U ) /*x*/)
    { return true; }
    template< class U >
    typename enable_for_optional< SAKE_FWD2_PARAM( U ), bool >::type
    static initialize_m_initialized_dispatch(SAKE_FWD2_REF( U ) x)
    { return x.initialized(); }
    template< class U >
    typename enable_for_value< SAKE_FWD2_PARAM( U ) >::type
    implicit_constructor_dispatch(SAKE_FWD2_REF( U ) x)
    { new(m_storage._) nocv_type(sake::forward<U>(x)); }
    template< class U >
    typename enable_for_optional< SAKE_FWD2_PARAM( U ) >::type
    implicit_constructor_dispatch(SAKE_FWD2_REF( U ) x)
    {
        if(m_initialized)
            implicit_constructor_dispatch(sake::get(sake::forward<U>(x)));
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_implicit_constructor<U>::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(sake::forward<U>(x)))
    { implicit_constructor_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct implicit_constructor_rv_sink_visitor
    {
        explicit implicit_constructor_rv_sink_visitor(optional& this_)
            : m_this(this_)
        { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            m_this.m_initialized = initialize_m_initialized_dispatch(x);
            m_this.implicit_constructor_dispatch(x);
        }
    private:
        optional& m_this;
    };
    friend struct implicit_constructor_rv_sink;
    typedef sake::rv_sink_traits1<
        nocv_type,
        boost::mpl::quote1< enable_cond_implicit_constructor >
    > implicit_constructor_rv_sink_traits;
    typedef typename implicit_constructor_rv_sink_traits::template
        default_< implicit_constructor_rv_sink_visitor >
        implicit_constructor_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x,
        typename implicit_constructor_rv_sink_traits::template
            enable_ref<U>::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(x))
    { implicit_constructor_dispatch(x); }

    // T rvalues
    optional(typename implicit_constructor_rv_sink_traits::primary_type x)
        : m_initialized(initialize_m_initialized_dispatch(x.move()))
    { implicit_constructor_dispatch(x.move()); }

    // movable implicit rvalues
    optional(implicit_constructor_rv_sink_default_type x)
    { x(implicit_constructor_rv_sink(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    optional(U const & x,
        typename implicit_constructor_rv_sink_traits::template
            enable_cref<U>::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(x))
    { implicit_constructor_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_implicit_constructor<U>::type* = 0)
        : mp(get_ptr_dispatch(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U& x,
        typename enable_implicit_constructor<
            typename boost_ext::remove_rvalue_reference< U& >::type
        >::type* = 0)
        : mp(get_ptr_dispatch(SAKE_AS_LVALUE(x)))
    { }

    template< class U >
    optional(U const & x,
        typename enable_implicit_constructor< U const & >::type* = 0)
        : mp(get_ptr_dispatch(x))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
