/*******************************************************************************
 * sake/core/data_structures/optional/private/reset.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/core/emplacer/construct.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/get.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    typename value_enabler< SAKE_FWD2_PARAM( U ) >::type
    reset_dispatch(SAKE_FWD2_REF( U ) x)
    {
        reset();
        new(m_storage._) nocv_type(sake::forward<U>(x));
        m_initialized = true;
    }

    template< class U >
    typename optional_enabler< SAKE_FWD2_PARAM( U ) >::type
    reset_dispatch(SAKE_FWD2_REF( U ) x)
    {
        reset();
        if(x.initialized()) {
            new(m_storage._) nocv_type(sake::get(sake::forward<U>(x)));
            m_initialized = true;
        }
    }

    template< class Emplacer >
    typename emplacer_enabler< Emplacer >::type
    reset_dispatch(Emplacer e)
    {
        reset();
        sake::emplacer_construct< nocv_type >(e, m_storage._);
        m_initialized = true;
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename common_enabler<U>::type
    reset(U&& x)
    { reset_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    class reset_rv_sink_visitor
    {
        optional& m_this;
        explicit reset_rv_sink_visitor(optional& this_) : m_this(this_) { }
        friend struct optional;
    public:
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        { m_this.reset(x); }
    };
    typedef sake::rv_sink_traits<
        boost::mpl::vector2< optional, nocv_type >,
        boost::mpl::quote1< common_enable >
    > reset_rv_sink_traits;
    typedef typename boost::mpl::at_c<
        typename reset_rv_sink_traits::primary_types, 0
    >::type reset_rv_sink_primary0_type;
    typedef typename boost::mpl::at_c<
        typename reset_rv_sink_traits::primary_types, 1
    >::type reset_rv_sink_primary1_type;
    typedef typename reset_rv_sink_traits::template
        default_< reset_rv_sink_visitor > reset_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U >
    typename reset_rv_sink_traits::template ref_enabler<U>::type
    reset(U& x)
    { reset_dispatch(x); }

    // this rvalues
    void reset(reset_rv_sink_primary0_type x)
    { reset_dispatch(sake::move(x.value)); }

    // T rvalues
    void reset(reset_rv_sink_primary1_type x)
    { reset_dispatch(sake::move(x.value)); }

    // movable implicit rvalues
    void reset(reset_rv_sink_default_type x)
    { x(reset_rv_sink_visitor(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename reset_rv_sink_traits::template cref_enabler<U>::type
    reset(U const & x)
    { reset_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename common_enabler<U>::type
    reset(U&& x)
    { m_p = get_ptr_dispatch(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename common_enabler< U& >::type
    reset(U& x)
    { m_p = get_ptr_dispatch(SAKE_AS_LVALUE(x)); }

    template< class U >
    typename common_enabler< U const & >::type
    reset(U const & x)
    { m_p = get_ptr_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
