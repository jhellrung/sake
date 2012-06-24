/*******************************************************************************
 * sake/core/range/private/facade/iter_at_helper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_ITER_AT_HELPER_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_ITER_AT_HELPER_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Derived >
struct iterator_of
{ typedef typename Derived::iterator type; };

template< class Derived >
struct iterator_of< Derived const >
{ typedef typename Derived::const_iterator type; };

template< class Result, class CDerived, class T >
inline typename boost::enable_if_c<
    boost_ext::is_convertible<
        typename private_::iterator_of< CDerived >::type, Result >::value,
    Result
>::type
iter_at_helper(CDerived& this_, T const & x)
{ return this_.iter_at(x); }

template< class Result, class CDerived, class T >
inline typename boost::disable_if_c<
    boost_ext::is_convertible<
        typename private_::iterator_of< CDerived >::type, Result >::value,
    Result
>::type
iter_at_helper(CDerived& this_, T const & x)
{ return Result(this_, x); }

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_ITER_AT_HELPER_HPP
