/*******************************************************************************
 * sake/core/iterator/facade_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_FACADE_FWD_HPP
#define SAKE_CORE_ITERATOR_FACADE_FWD_HPP

#include <boost/mpl/identity.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/private/facade/function_prototype.hpp>

namespace sake
{

namespace iterator_facade_adl
{

template< class Derived, class Params >
class iterator_facade;

namespace private_
{

template< class D0, class P0, class D1, class P1 > struct operator_equality_enabler;
template< class D0, class P0, class D1, class P1 > struct operator_relational_enabler;
template< class D0, class P0, class D1, class P1 > struct cmp_enabler;
template< class D0, class P0, class D1, class P1 > struct operator_minus_enabler;

template< class D, class P > struct operator_minus_begin_enabler;
template< class D, class P > struct operator_minus_end_enabler;

} // namespace private_

SAKE_ITERATOR_FACADE_function_prototype( inline, operator_equality_enabler, operator== );
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_relational_enabler, operator< );
SAKE_ITERATOR_FACADE_function_prototype( inline, cmp_enabler, cmp );
SAKE_ITERATOR_FACADE_function_prototype( inline, operator_minus_enabler, operator- );

template< class D, class P >
inline typename ::sake::iterator_facade_adl::private_::operator_minus_begin_enabler<D,P>::type
operator-(::sake::iterator_facade_adl::iterator_facade<D,P> const & i, ::sake::begin_tag);
template< class D, class P >
inline typename ::sake::iterator_facade_adl::private_::operator_minus_end_enabler<D,P>::type
operator-(::sake::iterator_facade_adl::iterator_facade<D,P> const & i, ::sake::end_tag);

namespace private_
{

template< class Params >
struct traversal_base_index;
template<
    class Derived, class Params,
    int = traversal_base_index< Params >::value
>
class traversal_base;

template< class Params >
struct begin_introversal_base_index;
template<
    class Derived, class Params,
    int = begin_introversal_base_index< Params >::value
>
class begin_introversal_base;

template< class Params >
struct end_introversal_base_index;
template<
    class Derived, class Params,
    int = end_introversal_base_index< Params >::value
>
class end_introversal_base;

template< class Params >
struct common_base_index;
template<
    class Derived, class Params,
    int = common_base_index< Params >::value
>
class common_base;

} // namespace private_

} // namespace iterator_facade_adl

using iterator_facade_adl::iterator_facade;

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_FACADE_FWD_HPP
