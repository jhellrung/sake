/*******************************************************************************
 * sake/core/emplacer/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_FWD_HPP
#define SAKE_CORE_EMPLACER_FWD_HPP

#ifndef SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_EMPLACER_PERFECT_MAX_ARITY 4
#endif // #ifndef SAKE_EMPLACER_PERFECT_MAX_ARITY

#ifndef SAKE_EMPLACER_REF_MAX_ARITY
#define SAKE_EMPLACER_REF_MAX_ARITY 6
#endif // #ifndef SAKE_EMPLACER_REF_MAX_ARITY

#ifndef SAKE_EMPLACER_CREF_MAX_ARITY
#define SAKE_EMPLACER_CREF_MAX_ARITY 8
#endif // #ifndef SAKE_EMPLACER_CREF_MAX_ARITY

#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_REF_MAX_ARITY
#undef SAKE_EMPLACER_MAX_ARITY
#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_REF_MAX_ARITY
#endif // #if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_REF_MAX_ARITY
#if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_CREF_MAX_ARITY
#undef SAKE_EMPLACER_MAX_ARITY
#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_CREF_MAX_ARITY
#endif // #if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_CREF_MAX_ARITY

namespace sake
{

template< class Signature >
class emplacer;

class emplacer_access;

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_FWD_HPP
