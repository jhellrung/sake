/*******************************************************************************
 * sake/core/data_structures/tuple/private/sequence_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include <sake/boost_ext/fusion/sequence/default_rv_sink.hpp>

#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_TUPLE_INCLUDE_HEADERS

#ifdef SAKE_TUPLE_DEFINE_MEMBERS

#ifndef N
#error
#endif // #ifndef N

private:
    template< class Sequence >
    struct sequence_constructor_enabler
        : sake::tuple_private::sequence_constructor_enabler< tuple, Sequence >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    tuple(Sequence&& s,
        typename sequence_constructor_enabler< Sequence >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, sake::forward< Sequence >(s) )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class Sequence >
    struct sequence_constructor_enable
        : sake::tuple_private::sequence_constructor_enable< tuple, Sequence >
    { };
    typedef sake::rv_sink_traits0<
        boost::mpl::quote1< sequence_constructor_enable >
    > sequence_constructor_rv_sink_traits;
    typedef boost_ext::fusion::default_rv_sink0<
        nocv_types,
        boost::mpl::quote1< sequence_constructor_enable >
    > sequence_constructor_rv_sink_default_type;
public:
    // lvalues + explicit movable rvalues
    template< class Sequence >
    tuple(Sequence& s,
        typename sequence_constructor_rv_sink_traits::template
            ref_enabler< Sequence >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, s )
    { }
    // implicit movable rvalues
    tuple(sequence_constructor_rv_sink_default_type s)
        : BOOST_PP_ENUM( N, _n_data_at_n, s )
    { }
    // const lvalues + non-movable rvalues
    template< class Sequence >
    tuple(Sequence const & s,
        typename sequence_constructor_rv_sink_traits::template
            cref_enabler< Sequence >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, s )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS
