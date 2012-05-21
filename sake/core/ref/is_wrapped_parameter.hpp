/*******************************************************************************
 * sake/core/ref/is_wrapped_parameter.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_wrapped_parameter<T>
 * struct extension::is_wrapped_parameter< T, Enable = void >
 * struct default_impl::is_wrapped_parameter<T>
 ******************************************************************************/

#ifndef SAKE_CORE_REF_IS_WRAPPED_PARAMETER_HPP
#define SAKE_CORE_REF_IS_WRAPPED_PARAMETER_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/core/ref/fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct is_wrapped_parameter<T>
 ******************************************************************************/

template< class T >
struct is_wrapped_parameter
    : extension::is_wrapped_parameter<
          typename boost::remove_cv<T>::type
      >
{ };

/*******************************************************************************
 * struct extension::is_wrapped_parameter< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct is_wrapped_parameter
    : default_impl::is_wrapped_parameter<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::is_wrapped_parameter<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct is_wrapped_parameter
    : boost::false_type
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_IS_WRAPPED_PARAMETER_HPP
