/*******************************************************************************
 * sake/boost_ext/array.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * This file #include's the Boost.Array header, plus it #include's the
 * Boost.Fusion extension for Boost.Array and specializes the
 * sake::range_static_size metafunction.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_ARRAY_HPP
#define SAKE_BOOST_EXT_ARRAY_HPP

#include <cstddef>

#include <boost/array.hpp>
#include <boost/fusion/adapted/array.hpp>
#include <boost/fusion/mpl.hpp>

//#include <sake/core/range/static_size_fwd.hpp>

namespace sake
{

namespace extension
{

//template< class T, std::size_t N >
//struct range_static_size< boost::array<T,N> >
//{
//    static std::size_t const value = N;
//    typedef range_static_size type;
//};

} // namespace extension

} // namespace boost

#endif // #ifndef SAKE_BOOST_EXT_ARRAY_HPP
