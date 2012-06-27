/*******************************************************************************
 * sake/core/range/facade_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FACADE_FWD_HPP
#define SAKE_CORE_RANGE_FACADE_FWD_HPP

#include <sake/core/config.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

template< class Derived, class Params >
class facade;

namespace private_
{

#define forward_declare( x ) \
template< class Params > \
struct x ## _base_index; \
template< \
    class Derived, class Params, \
    int = x ## _base_index< Params >::value \
> \
class x ## _base \
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;
forward_declare( front_back )
forward_declare( distance )
forward_declare( size )
forward_declare( subscript )
#undef forward_declare

template< class Params >
struct chained_base_index;
template< class Params, int = chained_base_index< Params >::value >
struct chained_base;

} // namespace private_

} // namespace facade_adl

using sake::range::facade_adl::facade;

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FACADE_FWD_HPP
