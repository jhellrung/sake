/*******************************************************************************
 * sake/core/data_structures/optional/private/initialize_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    struct enable_cond_initialize_constructor
        : boost_ext::mpl::or2<
              enable_cond_for_value<U>,
              enable_cond_for_emplacer<U>
          >
    { };
    template< class U >
    struct enable_initialize_constructor
        : boost::enable_if_c< enable_cond_initialize_constructor<U>::value >
    { };

    template< class U >
    void initialize_constructor_impl(SAKE_FWD2_REF( U ) x)
    {
        if(m_initialized)
            sake::emplacer_construct< nocv_type >(sake::forward<U>(x), m_storage._);
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x, bool const initialize,
        typename enable_initialize_constructor<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct initialize_constructor_rv_sink_visitor
    {
        explicit initialize_constructor_rv_sink_visitor(optional& this_)
            : m_this(this_)
        { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            BOOST_STATIC_ASSERT((enable_cond_for_value<U>::value));
            new(m_storage._) nocv_type(x);
        }
    private:
        optional& m_this;
    };
    friend struct initialize_constructor_rv_sink_visitor;
    typedef sake::rv_sink_traits1<
        nocv_type,
        boost::mpl::quote1< enable_cond_initialize_constructor >
    > initialize_constructor_rv_sink_traits;
    typedef typename initialize_constructor_rv_sink_traits::template
        default_< initialize_constructor_rv_sink_visitor >
        initialize_constructor_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x, bool const initialize,
        typename initialize_constructor_rv_sink_traits::template
            enable_ref<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(x); }

    // T rvalues
    optional(
        typename initialize_constructor_rv_sink_traits::primary_type x,
        bool const initialize)
        : m_initialized(initialize)
    { initialize_constructor_impl(x.move()); }

    // movable implicit rvalues
    optional(
        initialize_constructor_rv_sink_default_type x,
        bool const initialize)
        : m_initialized(initialize)
    { if(m_initialized) x(initialize_constructor_rv_sink_visitor(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    optional(U const & x, bool const initialize,
        typename initialize_constructor_rv_sink_traits::template
            enable_cref<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(x); }

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
