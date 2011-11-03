/*******************************************************************************
 * sake/core/math/zero_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ZERO_FWD_HPP
#define SAKE_CORE_MATH_ZERO_FWD_HPP

namespace sake
{

struct zero_t
{
    template< class T >
    static T
    as();
};

zero_t const zero = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ZERO_FWD_HPP
