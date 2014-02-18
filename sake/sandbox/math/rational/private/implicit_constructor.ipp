/*******************************************************************************
 * sake/core/math/private/rational/implicit_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_cast.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_RATIONAL_INCLUDE_HEADERS

#ifdef SAKE_RATIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct implicit_constructor_enable
        : boost_ext::mpl::and2<
              boost_ext::not_is_base_of_sans_qualifiers< rational, U >,
              typename base_::template common_enable<U>
          >
    { };
    template< class U >
    struct implicit_constructor_enabler
        : boost::enable_if_c< implicit_constructor_enable<U>::value >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    rational(U&& x,
        typename implicit_constructor_enabler<U>::type* = 0)
        : base_(sake::forward<U>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<
        T, boost::mpl::quote1< implicit_constructor_enable >
    > implicit_constructor_rv_sink_traits;
    typedef typename implicit_constructor_rv_sink_traits::template
        default_< sake::functional::construct<T> >
        implicit_constructor_rv_sink_default_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    rational(U& x,
        typename implicit_constructor_rv_sink_traits::template
            ref_enabler<U>::type* = 0)
        : base_(x,
              private_::implicit_constructor_tag())
    { }
    // T rvalues
    rational(typename implicit_constructor_rv_sink_traits::primary_type x)
        : base_(sake::move(x.value),
              private_::implicit_constructor_tag())
    { }
    // movable implicit rvalues
    rational(implicit_constructor_rv_sink_default_type x)
        : base_(SAKE_RV_CAST(x()),
              private_::implicit_constructor_tag())
    { }
    // const lvalues + non-movable rvalues
    template< class U >
    rational(U const & x,
        typename implicit_constructor_rv_sink_traits::template
            cref_enabler<U>::type* = 0)
        : base_(x,
              private_::implicit_constructor_tag())
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_RATIONAL_DEFINE_MEMBERS
