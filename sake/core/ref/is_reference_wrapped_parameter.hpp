/*******************************************************************************
 * sake/core/ref/is_reference_wrapped_parameter.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_reference_wrapped_parameter<T>
 ******************************************************************************/

#ifndef SAKE_CORE_REF_IS_REFERENCE_WRAPPED_PARAMETER_HPP
#define SAKE_CORE_REF_IS_REFERENCE_WRAPPED_PARAMETER_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/core/utility/extension.hpp>

namespace sake
{

namespace no_ext
{

template< class T >
struct is_reference_wrapped_parameter
    : boost::false_type
{ };

} // namespace no_ext

SAKE_EXTENSION_UNARY_CLASS( is_reference_wrapped_parameter )

template< class T >
struct is_reference_wrapped_parameter
    : ext::is_reference_wrapped_parameter<
          typename boost::remove_cv<T>::type
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_IS_REFERENCE_WRAPPED_PARAMETER_HPP
