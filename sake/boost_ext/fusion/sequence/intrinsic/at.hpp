/*******************************************************************************
 * sake/boost_ext/fusion/sequence/intrisic/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::at<N>(Sequence&& s)
 *     -> boost_ext::fusion::result_of::at< Sequence, N >::type
 * boost_ext::fusion::at_c<N>(Sequence&& s)
 *     -> boost_ext::fusion::result_of::at_c< Sequence, N >::type
 *
 * boost_ext::fusion::at is identical to boost::fusion::at, except it propagates
 * rvalue reference qualifiers from Sequence&& onto the result whenever
 * - Sequence&& is an rvalue reference; and
 * - the result of boost::fusion::at is a reference-to-non-const; and
 * - Sequence is not a view (i.e., it owns its non-reference elements); and
 * - the underlying value_at is a not a reference.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_AT_HPP
#define SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_AT_HPP

#include <boost/config.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

namespace at_private
{

template< class Sequence, int N >
struct dispatch
{
private;
    typedef typename boost::fusion::result_of::at_c< Sequence, N >::type at_type;
    typedef typename boost::fusion::result_of::value_at_c< Sequence, N >::type value_at_type;
public:
    typedef typename boost::mpl::if_c<
        boost_ext::mpl::and3<
            boost_ext::is_lvalue_reference_to_nonconst< at_type >,
            boost::mpl::not_< boost::fusion::traits::is_view< Sequence > >,
            boost::mpl::not_< boost_ext::is_reference< value_at_type > >
        >::value,
        typename boost_ext::add_reference<
            typename boost_ext::add_rvalue_reference<
                typename boost_ext::remove_reference< at_type >::type
            >::type
        >::type,
        at_type
    >::type type;
};

template< class Sequence, int N >
struct dispatch< Sequence&, N >
    : boost::fusion::result_of::at_c< Sequence, N >
{ };

} // namespace at_private

template< class Sequence, int N >
struct at_c
    : at_private::dispatch<
          typename boost_ext::remove_rvalue_reference< Sequence >::type,
          N
      >
{ };

template< class Sequence, class N >
struct at
    : at_c< Sequence, N::value >
{ };

} // namespace result_of

#ifndef BOOST_NO_RVALUE_REFERENCES

template< int N, class Sequence >
inline typename boost_ext::fusion::result_of::at_c< Sequence, N >::type
at_c(Sequence&& s)
{
    typedef typename boost_ext::fusion::result_of::at_c< Sequence, N >::type result_type;
    return static_cast< result_type >(boost::fusion::at_c<N>(s));
}

template< class N, class Sequence >
inline typename boost_ext::fusion::result_of::at< Sequence, N >::type
at(Sequence&& s)
{
    typedef typename boost_ext::fusion::result_of::at< Sequence, N >::type result_type;
    return static_cast< result_type >(boost::fusion::at<N>(s));
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< int N, class Sequence >
inline typename boost_ext::fusion::result_of::at_c< Sequence&, N >::type
at_c(Sequence& s)
{
    typedef typename boost_ext::fusion::result_of::at_c< Sequence&, N >::type result_type;
    return static_cast< result_type >(boost::fusion::at_c<N>(SAKE_AS_LVALUE( s )));
}

template< int N, class Sequence >
inline typename boost_ext::fusion::result_of::at_c< Sequence const &, N >::type
at_c(Sequence const & s)
{ boost::fusion::at_c<N>(s); }

template< class N, class Sequence >
inline typename boost_ext::fusion::result_of::at< Sequence&, N >::type
at(Sequence& s)
{
    typedef typename boost_ext::fusion::result_of::at< Sequence&, N >::type result_type;
    return static_cast< result_type >(boost::fusion::at<N>(SAKE_AS_LVALUE( s )));
}

template< class N, class Sequence >
inline typename boost_ext::fusion::result_of::at< Sequence const &, N >::type
at(Sequence const & s)
{ boost::fusion::at<N>(s); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_AT_HPP
