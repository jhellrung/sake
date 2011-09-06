/*******************************************************************************
 * boost_ext/type_traits/propagate_cv.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::propagate_cv< From, To >
 *
 * propagate_cv< From, To > -> To
 * propagate_cv< From const, To > -> To const
 * propagate_cv< From volatile, To > -> To volatile
 * propagate_cv< From const volatile, To > -> To const volatile
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CV_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CV_HPP

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_cv.hpp>
#include <boost/type_traits/add_volatile.hpp>

namespace sake
{

namespace boost_ext
{

template< class From, class To > struct propagate_cv { typedef To type; };
template< class From, class To > struct propagate_cv< From const, To > : boost::add_const< To > { };
template< class From, class To > struct propagate_cv< From volatile, To > : boost::add_volatile< To > { };
template< class From, class To > struct propagate_cv< From const volatile, To > : boost::add_cv< To > { };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_CV_HPP
