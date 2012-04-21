/*******************************************************************************
 * sake/core/data_structures/optional/private/ctor_initialize.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    void ctor_initialize_impl(SAKE_FWD2_REF( U ) x)
    {
        BOOST_STATIC_ASSERT((
            enable_cond_for_value< SAKE_FWD2_PARAM( U ) >::value
         || enable_cond_for_emplacer< SAKE_FWD2_PARAM( U ) >::value
        ));
        if(m_initialized)
            sake::emplacer_construct< nocv_type >(sake::forward<U>(x), m_storage._);
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x, bool const initialize)
        : m_initialized(initialize)
    { ctor_initialize_impl(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct explicit_ctor_rv_sink
    {
        explicit explicit_ctor_rv_sink(optional& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            BOOST_STATIC_ASSERT((enable_cond_for_value<U>::value));
            new(m_storage._) nocv_type(x);
        }
    private:
        optional& m_this;
    public:

        template< class U >
        struct apply
            : boost::mpl::not_< boost::is_same< U, nocv_type > >
        { };

        typedef sake::rv_sink<
            explicit_ctor_rv_sink, void, explicit_ctor_rv_sink
        > type;

    };
    friend struct explicit_ctor_rv_sink;

    template< class U >
    struct enable_ctor_initialize_cref
        : boost::disable_if_c< boost_ext::mpl::or2<
              boost_ext::is_same_sans_qualifiers< U, rv_param_type >,
              sake::is_movable<U>
          >::value >
    { };
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x, bool const initialize)
        : m_initialized(initialize)
    { ctor_initialize_impl(x); }

    // T rvalues
    optional(rv_param_type x, bool const initialize)
        : m_initialized(initialize)
    { ctor_initialize_impl(x); }

    // movable implicit rvalues
    optional(typename explicit_ctor_rv_sink::type x, bool const initialize)
        : m_initialized(initialize)
    { if(m_initialized) x(explicit_ctor_rv_sink(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    optional(U const & x, bool const initialize,
        typename enable_ctor_initialize_cref<U>::type* = 0)
        : m_initialized(initialize)
    { ctor_initialize_impl(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

    optional(T& x, bool const initialize)
        : mp(initialize ? get_ptr_dispatch(x) : 0)
    { }

    template< class Signature >
    optional(sake::emplacer< Signature > e, bool const initialize)
        : mp(initialize ? get_ptr_dispatch(e) : 0)
    { }

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
