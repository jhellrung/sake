/*******************************************************************************
 * sake/boost_ext/mpl/at_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::at_c< Sequence, N >
 * struct boost_ext::mpl::at_c< Sequence, N, Default >
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

namespace at_c_private
{

struct dummy;

} // namespace at_c_private

template< class Sequence, long N, class Default = at_c_private::dummy >
struct at_c;

namespace at_c_private
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

} // namespace at_c_private

template< class Sequence, long N, class Default >
struct at_c
    : at_c_private::at_c_dispatch< Sequence, N, Default >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_AT_C_HPP
