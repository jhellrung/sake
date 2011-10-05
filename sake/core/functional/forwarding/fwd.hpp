/*******************************************************************************
 * sake/core/functional/forwarding/fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_FWD_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_FWD_HPP

#include <sake/core/functional/forwarding/keyword.hpp>

#ifndef SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY
#define SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY 4
#endif // #ifndef SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY

#ifndef SAKE_FORWARDING_BASE_REF_MAX_ARITY
#define SAKE_FORWARDING_BASE_REF_MAX_ARITY 6
#endif // #ifndef SAKE_FORWARDING_BASE_REF_MAX_ARITY

#ifndef SAKE_FORWARDING_BASE_CREF_MAX_ARITY
#define SAKE_FORWARDING_BASE_CREF_MAX_ARITY 8
#endif // #ifndef SAKE_FORWARDING_BASE_CREF_MAX_ARITY

#define SAKE_FORWARDING_BASE_MAX_ARITY SAKE_FORWARDING_BASE_PERFECT_MAX_ARITY
#if SAKE_FORWARDING_BASE_MAX_ARITY < SAKE_FORWARDING_BASE_REF_MAX_ARITY
#undef SAKE_FORWARDING_BASE_MAX_ARITY
#define SAKE_FORWARDING_BASE_MAX_ARITY SAKE_FORWARDING_BASE_REF_MAX_ARITY
#endif // #if SAKE_FORWARDING_BASE_MAX_ARITY < SAKE_FORWARDING_BASE_REF_MAX_ARITY
#if SAKE_FORWARDING_BASE_MAX_ARITY < SAKE_FORWARDING_BASE_CREF_MAX_ARITY
#undef SAKE_FORWARDING_BASE_MAX_ARITY
#define SAKE_FORWARDING_BASE_MAX_ARITY SAKE_FORWARDING_BASE_CREF_MAX_ARITY
#endif // #if SAKE_FORWARDING_BASE_MAX_ARITY < SAKE_FORWARDING_BASE_CREF_MAX_ARITY

namespace sake
{

namespace forwarding
{

template< class Derived, class Params = keyword::default_params >
class nullary_base;

template< class Derived, class Params = keyword::default_params >
class base;

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_FWD_HPP
