/*******************************************************************************
 * boost_ext/type_traits/propagate_const.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::propagate_const< From, To >
 *
 * propagate_const< From, To > -> To
 * propagate_const< From const, To > -> To const
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CONST_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CONST_HPP

#include <boost/type_traits/add_const.hpp>

namespace sake
{

namespace boost_ext
{

template< class From, class To > struct propagate_const { typedef To type; };
template< class From, class To > struct propagate_const< From const, To > : boost::add_const< To > { };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CONST_HPP
