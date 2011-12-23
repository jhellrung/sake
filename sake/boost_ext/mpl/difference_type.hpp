/*******************************************************************************
 * sake/boost_ext/mpl/difference_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::difference_type<T>
 *
 * This defines a trivial metafunction evaluating to T::difference_type.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_DIFFERENCE_TYPE_HPP
#define SAKE_BOOST_EXT_MPL_DIFFERENCE_TYPE_HPP

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class T >
struct difference_type
{ typedef typename T::difference_type type; };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_DIFFERENCE_TYPE_HPP
