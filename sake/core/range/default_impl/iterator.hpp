/*******************************************************************************
 * sake/core/range/default_impl/iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_HPP

#include <boost/range/mutable_iterator.hpp>
#include <boost/range/const_iterator.hpp>

#include <sake/core/introspection/has_type.hpp>

namespace sake
{

namespace range
{

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_iterator, iterator )
SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_const_iterator, const_iterator )

namespace iterator_private
{

template< class R, bool = has_type_iterator<R>::value >
struct dispatch;

template< class R >
struct dispatch< R, false >
    : boost::range_mutable_iterator<R>
{ };

template< class R >
struct dispatch< R, true >
{ typedef typename R::iterator type; };

template< class R, bool = has_type_const_iterator<R>::value >
struct dispatch_const;

template< class R >
struct dispatch_const< R, false >
    : boost::range_const_iterator<R>
{ };

template< class R >
struct dispatch_const< R, true >
{ typedef typename R::const_iterator type; };

} // namespace iterator_private

template< class R >
struct iterator
    : iterator_private::dispatch<R>
{ };

template< class R >
struct iterator< R const >
    : iterator_private::dispatch_const<R>
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_HPP
