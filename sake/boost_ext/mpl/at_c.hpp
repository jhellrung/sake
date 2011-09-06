/*******************************************************************************
 * boost_ext/mpl/at_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::mpl::at_c< Sequence, N >
 * boost_ext::mpl::at_c< Sequence, N, Default >
 *
 * boost_ext::mpl::at_c is identical to boost::mpl::at_c, except one can
 * optionally specify a 3rd argument to return in the event that N is not a
 * valid index into Sequence.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_AT_C_HPP
#define SAKE_BOOST_EXT_MPL_AT_C_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

namespace detail_at_c
{

struct dummy;

} // namespace detail_at_c

template< class Sequence, long N, class Default = detail_at_c::dummy >
struct at_c;

namespace detail_at_c
{

template<
    class Sequence, long N, class Default,
    bool = (N >= 0) && (N < boost::mpl::size< Sequence >::value)
>
struct at_c_dispatch;

template< class Sequence, long N, class Default >
struct at_c_dispatch< Sequence, N, Default, true >
    : boost::mpl::at_c< Sequence, N >
{ };

template< class Sequence, long N, class Default >
struct at_c_dispatch< Sequence, N, Default, false >
{ typedef Default type; };

template< class Sequence, long N >
struct at_c_dispatch< Sequence, N, dummy, false >
{ };

} // namespace detail_at_c

template< class Sequence, long N, class Default >
struct at_c
    : detail_at_c::at_c_dispatch< Sequence, N, Default >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_AT_C_HPP
