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

#include <sake/core/emplacer/construct.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/rv.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    struct initialize_constructor_enable
        : boost_ext::mpl::or2< value_enable<U>, emplacer_enable<U> >
    { };
    template< class U >
    struct initialize_constructor_enabler
        : boost::enable_if_c< initialize_constructor_enable<U>::value >
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
        typename initialize_constructor_enabler<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    class initialize_constructor_rv_sink_visitor
    {
        optional& m_this;
        explicit initialize_constructor_rv_sink_visitor(optional& this_)
            : m_this(this_)
        { }
        friend struct optional;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            BOOST_STATIC_ASSERT((value_enable<U>::value));
            new(m_this.m_storage._) nocv_type(x);
        }
    };
    friend class initialize_constructor_rv_sink_visitor;
    typedef sake::rv_sink_traits1<
        nocv_type, boost::mpl::quote1< initialize_constructor_enable >
    > initialize_constructor_rv_sink_traits;
    typedef typename initialize_constructor_rv_sink_traits::template
        default_< initialize_constructor_rv_sink_visitor >
        initialize_constructor_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x, bool const initialize,
        typename initialize_constructor_rv_sink_traits::template
            ref_enabler<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(x); }

    // T rvalues
    optional(
        typename initialize_constructor_rv_sink_traits::primary_type x,
        bool const initialize)
        : m_initialized(initialize)
    { initialize_constructor_impl(sake::move(x.value)); }

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
            cref_enabler<U>::type* = 0)
        : m_initialized(initialize)
    { initialize_constructor_impl(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

    optional(T& x, bool const initialize)
        : m_p(initialize ? get_ptr_dispatch(x) : 0)
    { }

    template< class Signature >
    optional(sake::emplacer< Signature > e, bool const initialize)
        : m_p(initialize ? get_ptr_dispatch(e) : 0)
    { }

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
