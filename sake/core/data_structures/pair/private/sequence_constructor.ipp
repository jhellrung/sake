/*******************************************************************************
 * sake/core/data_structures/pair/private/sequence_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_PAIR_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/fusion/sequence/default_rv_sink.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>

#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>

#endif // #ifdef SAKE_PAIR_INCLUDE_HEADERS

#ifdef SAKE_PAIR_DEFINE_MEMBERS

private:
    template< class Sequence >
    struct sequence_constructor_enabler
        : sake::tuple_private::sequence_constructor_enabler< pair, Sequence >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    pair(Sequence&& s,
        typename sequence_constructor_enabler< Sequence >::type* = 0)
        : first(boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
          second(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class Sequence >
    struct sequence_constructor_enable
        : sake::tuple_private::sequence_constructor_enable< pair, Sequence >
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
    pair(Sequence& s,
        typename sequence_constructor_rv_sink_traits::template
            ref_enabler< Sequence >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }
    // implicit movable rvalues
    pair(sequence_constructor_rv_sink_default_type s)
        : first (s[boost::integral_constant< std::size_t, 0 >()]),
          second(s[boost::integral_constant< std::size_t, 1 >()])
    { }
    // const lvalues + non-movable rvalues
    template< class Sequence >
    pair(Sequence const & s,
        typename sequence_constructor_rv_sink_traits::template
            cref_enabler< Sequence >::type* = 0)
        : first(boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_PAIR_DEFINE_MEMBERS
