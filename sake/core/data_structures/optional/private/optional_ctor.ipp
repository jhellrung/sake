/*******************************************************************************
 * sake/core/data_structures/optional/private/optional_ctor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_lvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/cast/implicit.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct enable_optional_ctor
        : boost::disable_if_c< boost_ext::mpl::or2<
              boost_ext::is_base_of_sans_qualifiers< optional, U >,
              boost::mpl::not_< is_compatible_optional<U> >
          >::value >
    { };
public:

#ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_optional_ctor<U>::type* = 0)
        : m_initialized(x.initialized())
    { initialize_from_optional(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    struct optional_ctor_rv_sink
    {
        optional_ctor_rv_sink(optional& this_) : m_this(this_) { }
        typedef void result_type;
        template< class U >
        void operator()(SAKE_RV_REF( U ) x) const
        {
            m_this.m_initialized = x.initialized();
            m_this.initialize_from_optional(x);
        }
    private:
        optional& m_this;
    public:

        template< class U >
        struct apply
            : boost::mpl::not_< boost_ext::mpl::or2<
                  boost_ext::is_base_of_sans_qualifiers< optional, U >,
                  boost::mpl::not_< is_compatible_optional<U> >
              > >
        { };

        typedef sake::rv_sink<
            optional_ctor_rv_sink, void, optional_ctor_rv_sink
        > type;
    };
    friend struct optional_ctor_rv_sink;

    template< class U >
    struct enable_optional_ctor_cref
        : boost::disable_if_c< boost_ext::mpl::or3<
              boost_ext::is_base_of_sans_qualifiers< optional, U >,
              boost::mpl::not_< is_compatible_optional< U const & > >,
              sake::is_movable<U>
          >::value >
    { };
public:

    // lvalues + explicit movable rvalues
    template< class U >
    optional(U& x,
        typename enable_optional_ctor<
            typename boost_ext::remove_rvalue_reference< U& >::type >::type* = 0)
        : m_initialized(x.initialized())
    { initialize_from_optional(x); }

    // movable implicit rvalues
    optional(typename optional_ctor_rv_sink::type x)
    { x(optional_ctor_rv_sink(*this)); }

    // const lvalues + non-movable rvalues
    template< class U >
    optional(U const & x,
        typename enable_optional_ctor_cref<U>::type* = 0)
        : m_initialized(other.initialized())
    { initialize_from_optional(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #ifndef SAKE_OPTIONAL_REFERENCE

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U&& x,
        typename enable_optional_ctor<U>::type* = 0)
        : mp(x.initialized() ?
             sake::address_of(sake::implicit_cast< T& >(x.get())) :
             0)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    optional(U& x,
        typename enable_optional_ctor<
            typename boost_ext::add_lvalue_reference< U& >::type >::type* = 0)
        : mp(x.initialized() ?
             sake::address_of(sake::implicit_cast< T& >(x.get())) :
             0)
    { }

    template< class U >
    optional(U const & x,
        typename enable_optional_ctor< U const & >::type* = 0)
        : mp(x.initialized() ?
             sake::address_of(sake::implicit_cast< T const & >(x.get())) :
             0)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
