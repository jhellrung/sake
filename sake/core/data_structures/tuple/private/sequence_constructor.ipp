/*******************************************************************************
 * sake/core/data_structures/tuple/private/sequence_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/forward.hpp>

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

    template< class Sequence >
    tuple(Sequence& s,
        typename sequence_constructor_enabler<
            typename boost_ext::remove_rvalue_reference< Sequence& >::type
        >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, sake::forward< Sequence >(s) )
    { }

    template< class Sequence >
    tuple(Sequence const & s,
        typename sequence_constructor_enabler< Sequence const & >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, sake::forward< Sequence >(s) )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS
