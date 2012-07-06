/*******************************************************************************
 * sake/core/iterator/facade_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_FACADE_FWD_HPP
#define SAKE_CORE_ITERATOR_FACADE_FWD_HPP

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

template< class Derived, class Params >
class facade;

#define forward_declare( x, y ) \
namespace private_ \
{ template< class D0, class P0, class D1, class P1 > struct x ## _enabler; } \
template< class D0, class P0, class D1, class P1 > \
inline typename private_::x ## _enabler< D0, P0, D1, P1 >::type \
y(sake::iterator::facade_adl::facade< D0, P0 > const & i0, \
  sake::iterator::facade_adl::facade< D1, P1 > const & i1);
forward_declare( equal, operator== )
forward_declare( compare, operator< )
forward_declare( cmp, cmp )
forward_declare( difference, operator- )
#undef forward_declare

namespace private_
{

template< class Derived, class Params > struct common_base;

#define forward_declare( x ) \
template< class Params > \
struct x ## _base_index; \
template< \
    class Derived, class Params, \
    int = x ## _base_index< Params >::value \
> \
class x ## _base \
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;
forward_declare( traversal )
forward_declare( begin_introversal )
forward_declare( end_introversal )
#undef forward_declare

} // namespace private_

} // namespace facade_adl

using sake::iterator::facade_adl::facade;

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_FACADE_FWD_HPP
