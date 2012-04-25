/*******************************************************************************
 * sake/core/data_structures/pair/private/sequence_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_PAIR_INCLUDE_HEADERS

#include <boost/config.hpp>

#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/forward.hpp>

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

    template< class Sequence >
    pair(Sequence& s,
        typename sequence_constructor_enabler<
            typename boost_ext::remove_rvalue_reference< Sequence& >::type
        >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

    template< class Sequence >
    pair(Sequence const & s,
        typename sequence_constructor_enabler< Sequence const & >::type* = 0)
        : first(boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_PAIR_DEFINE_MEMBERS
