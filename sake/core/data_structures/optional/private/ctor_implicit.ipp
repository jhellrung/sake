/*******************************************************************************
 * sake/core/data_structures/optional/private/ctor_implicit.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/get.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct enable_cond_ctor_implicit
        : boost_ext::mpl::and2<
              boost::mpl::not_<
                  boost_ext::is_base_of_sans_qualifiers< optional, U > >,
              boost_ext::mpl::or2<
                  enable_cond_for_value<U>,
                  enable_cond_for_optional<U>
              >
          >
    { };
    template< class U >
    struct enable_ctor_implicit
        : boost::enable_if_c< enable_cond_ctor_implicit<U>::value >
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
    ctor_implicit_dispatch(SAKE_FWD2_REF( U ) x)
    { new(m_storage._) nocv_type(sake::forward<U>(x)); }
    template< class U >
    typename enable_for_optional< SAKE_FWD2_PARAM( U ) >::type
    ctor_implicit_dispatch(SAKE_FWD2_REF( U ) x)
    {
        if(m_initialized)
            ctor_implicit_dispatch(sake::get(sake::forward<U>(x)));
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_ctor_implicit<U>::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(sake::forward<U>(x)))
    { ctor_implicit_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct ctor_implicit_rv_sink
    {
        explicit ctor_implicit_rv_sink(optional& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            m_this.m_initialized = initialize_m_initialized_dispatch(x);
            m_this.ctor_implicit_dispatch(x);
        }
    private:
        optional& m_this;
    public:

        template< class U >
        struct apply
            : boost_ext::mpl::and2<
                  enable_cond_ctor_implicit<U>,
                  boost::mpl::not_< boost::is_same< U, nocv_type > >
              >
        { };

        typedef sake::rv_sink<
            ctor_implicit_rv_sink, void, ctor_implicit_rv_sink
        > type;

    };
    friend struct ctor_implicit_rv_sink;

    template< class U >
    struct enable_ctor_implicit_cref
        : boost::disable_if_c< boost_ext::mpl::or3<
              boost::mpl::not_< enable_cond_ctor_implicit< U const & > >,
              boost_ext::is_same_sans_qualifiers< U, rv_param_type >,
              sake::is_movable<U>
          >::value >
    { };
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x,
        typename enable_ctor_implicit<
            typename boost_ext::remove_rvalue_reference< U& >::type
        >::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(x))
    { ctor_implicit_dispatch(x); }

    // T rvalues
    optional(rv_param_type x)
        : m_initialized(initialize_m_initialized_dispatch(x))
    { ctor_implicit_dispatch(x); }

    // movable implicit rvalues
    optional(typename ctor_implicit_rv_sink::type x)
    { x(ctor_implicit_rv_sink(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    optional(U const & x,
        typename enable_ctor_implicit_cref<U>::type* = 0)
        : m_initialized(initialize_m_initialized_dispatch(x))
    { ctor_implicit_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_ctor_implicit<U>::type* = 0)
        : mp(get_ptr_dispatch(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U& x,
        typename enable_ctor_implicit<
            typename boost_ext::remove_rvalue_reference< U& >::type
        >::type* = 0)
        : mp(get_ptr_dispatch(SAKE_AS_LVALUE(x)))
    { }

    template< class U >
    optional(U const & x,
        typename enable_ctor_implicit< U const & >::type* = 0)
        : mp(get_ptr_dispatch(x))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
