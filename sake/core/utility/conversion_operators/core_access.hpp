/*******************************************************************************
 * sake/core/utility/conversion_operators/core_acces.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_CORE_ACCESS_HPP
#define SAKE_CORE_UTILITY_CONVERSION_OPERATORS_CORE_ACCESS_HPP

#include <sake/core/utility/conversion_operators/fwd.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace conversion_operators
{

class core_access
{
  template< class, class, class, class >
  friend class sake::conversion_operators::private_::iterate;

  template< class Derived, class T >
  static T convert(Derived& this_, sake::type_tag<T>)
  { return this_.derived_convert(sake::type_tag<T>()); }
};

} // namespace conversion_operators

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_CORE_ACCESS_HPP
