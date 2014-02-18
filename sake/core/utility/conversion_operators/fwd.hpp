/*******************************************************************************
 * sake/core/utility/conversion_operators/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_FWD_HPP
#define SAKE_CORE_UTILITY_CONVERSION_OPERATORS_FWD_HPP

namespace sake
{

namespace conversion_operators
{

namespace private_
{

struct empty { };

} // namespace private_

template<
  class Derived,
  class ConversionTypes,
  class ChainedBase = private_::empty
>
class base;

namespace private_
{

template< class Derived, class I, class E, class ChainedBase >
class iterate;

} // namespace private_

} // namespace conversion_operators

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_FWD_HPP
