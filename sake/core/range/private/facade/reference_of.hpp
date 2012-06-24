/*******************************************************************************
 * sake/core/range/private/facade/reference_of.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_REFERENCE_OF_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_REFERENCE_OF_HPP

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Derived >
struct reference_of
{ typedef typename Derived::reference type; };

template< class Derived >
struct reference_of< Derived const >
{ typedef typename Derived::const_reference type; };

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_REFERENCE_OF_HPP
