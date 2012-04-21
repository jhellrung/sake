/*******************************************************************************
 * sake/core/data_structures/optional/private/reset.ipp
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

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/get.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

#ifndef SAKE_OPTIONAL_REFERENCE

private:
    template< class U >
    typename enable_for_value< SAKE_FWD2_PARAM( U ) >::type
    reset_dispatch(SAKE_FWD2_REF( U ) x)
    {
        reset();
        new(m_storage._) nocv_type(sake::forward<U>(x));
        m_initialized = true;
    }

    template< class U >
    typename enable_for_optional< SAKE_FWD2_PARAM( U ) >::type
    reset_dispatch(SAKE_FWD2_REF( U ) x)
    {
        reset();
        if(x.initialized()) {
            new(m_storage._) nocv_type(sake::get(boost_ext::forward<U>(x)));
            m_initialized = true;
        }
    }

    template< class Emplacer >
    typename enable_for_emplacer< Emplacer >::type
    reset_dispatch(Emplacer e)
    {
        reset();
        sake::emplacer_construct< nocv_type >(e, m_storage._);
        m_initialized = true;
    }
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    void reset(U&& x)
    { reset_dispatch(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct reset_rv_sink
    {
        explicit reset_rv_sink(optional& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        { m_this.reset(x); }
    private:
        optional& m_this;
    public:

        template< class U >
        struct apply
            : boost::mpl::not_< boost_ext::mpl::or2<
                  boost::is_same< U, optional >,
                  boost::is_same< U, nocv_type >
              > >
        { };

        typedef sake::rv_sink<
            reset_rv_sink, void, reset_rv_sink
        > type;
    };

    template< class U >
    struct enable_reset_cref
        : boost::disable_if_c< boost_ext::mpl::or3<
              boost::is_same< U, optional >,
              boost_ext::is_same_sans_qualifiers< U, rv_param_type >,
              sake::is_movable<U>
          >::value >
    { };
public:

    // lvalues + explicit movable rvalues
    template< class U >
    void reset(U& x)
    { reset_dispatch(x); }

    // this rvalues
    void reset(this_rvalue_param_type x)
    { reset_dispatch(x); }

    // T rvalues
    void reset(rv_param_type x)
    { reset_dispatch(x); }

    // movable implicit rvalues
    void reset(typename reset_rv_sink::type x)
    { x(reset_rv_sink(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename enable_reset_cref<U>::type
    reset(U const & x)
    { reset_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    void reset(U&& x)
    { mp = get_ptr_dispatch(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    void reset(U& x)
    { mp = get_ptr_dispatch(SAKE_AS_LVALUE(x)); }

    template< class U >
    void reset(U const & x)
    { mp = get_ptr_dispatch(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
