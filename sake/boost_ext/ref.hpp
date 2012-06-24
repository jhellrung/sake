/*******************************************************************************
 * sake/boost_ext/ref.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * Include this header rather than <boost/ref.hpp> to enable
 * boost::reference_wrapper<T>'s to be viewed as ranges (if T is itself a range)
 * and to be viewed as Boost.Fusion sequences (if T is itself a Boost.Fusion
 * sequence).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_REF_HPP
#define SAKE_BOOST_EXT_REF_HPP

#include <boost/range/const_iterator.hpp>
#include <boost/range/mutable_iterator.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/fusion/adapted/reference_wrapper.hpp>
#include <sake/core/range/begin.hpp>
#include <sake/core/range/end.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/ref/fwd.hpp>

namespace boost
{

/*******************************************************************************
 * Allow a boost::reference_wrapper wrapping a range to be viewed as a range
 * itself.
 ******************************************************************************/

template< class T >
struct range_mutable_iterator< reference_wrapper<T> >
    : sake::range_iterator<T>
{ };

template< class T >
struct range_const_iterator< reference_wrapper<T> >
    : sake::range_iterator<T>
{ };

template< class T >
inline typename sake::range_iterator<T>::type
range_begin(boost::reference_wrapper<T> x)
{ return sake::range::begin(x.get()); }

template< class T >
inline typename sake::range_iterator<T>::type
range_end(boost::reference_wrapper<T> x)
{ return sake::range::end(x.get()); }

template< class T >
inline typename sake::range_iterator<T>::type
begin(boost::reference_wrapper<T> x)
{ return sake::range::begin(x.get()); }

template< class T >
inline typename sake::range_iterator<T>::type
end(boost::reference_wrapper<T> x)
{ return sake::range::end(x.get()); }

} // namespace boost

namespace sake {
namespace extension {

template< class T >
struct unwrap_reference< boost::reference_wrapper<T> >
{ typedef T type; };

template< class T >
struct is_reference_wrapped_parameter< boost::reference_wrapper<T> >
    : boost::true_type
{ };

} // namespace extension
} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_REF_HPP
