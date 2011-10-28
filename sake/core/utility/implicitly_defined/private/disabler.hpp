/*******************************************************************************
 * sake/core/utility/private/implicitly_defined/disabler.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_DISABLER_HPP
#define SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_DISABLER_HPP

namespace sake
{

namespace implicitly_defined_private
{

template< class T >
struct disabler
{ };

} // namespace implicitly_defined_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_DISABLER_HPP
