/*******************************************************************************
 * sake/core/utility/convertible_from_any.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct convertible_from_any
 *
 * This struct is only used in metaprogramming function declarations to deduce
 * the presence or absence of function or operator overloads.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERTIBLE_FROM_ANY_HPP
#define SAKE_CORE_UTILITY_CONVERTIBLE_FROM_ANY_HPP

namespace sake
{

struct convertible_from_any
{
    convertible_from_any(...);
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERTIBLE_FROM_ANY_HPP
