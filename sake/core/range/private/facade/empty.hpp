/*******************************************************************************
 * sake/core/range/private/facade/empty.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_EMPTY_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_EMPTY_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/core/range/static_size.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Derived >
inline typename boost::enable_if_c<
    sake::range_has_static_size< Derived >::value, bool >::type
empty(Derived const & /*this_*/)
{ return sake::range_static_size< Derived >::value == 0; }

template< class Derived >
inline typename boost::disable_if_c<
    sake::range_has_static_size< Derived >::value, bool >::type
empty(Derived const & this_)
{ return this_.begin() == this_.end(); }

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_EMPTY_HPP