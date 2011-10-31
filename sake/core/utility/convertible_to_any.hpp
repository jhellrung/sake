/*******************************************************************************
 * sake/core/utility/convertible_to_any.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct convertible_to_any
 *
 * This struct is only used in metaprogramming applications.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERTIBLE_TO_ANY_HPP
#define SAKE_CORE_UTILITY_CONVERTIBLE_TO_ANY_HPP

namespace sake
{

struct convertible_to_any
{ template< class T > operator T&() const; };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERTIBLE_TO_ANY_HPP
