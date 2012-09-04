/*******************************************************************************
 * sake/core/math/clamp_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * clamp_ip(L const & lower, T& x, U const & upper) -> void
 * clamp_ip(L const & lower, T& x, U const & upper, Less less) -> void
 *
 * Note: One can use sake::unbounded for either lower or upper to only clamp in
 * one direction.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_CLAMP_IP_HPP
#define SAKE_CORE_MATH_CLAMP_IP_HPP

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/math/unbounded_tag.hpp>
#include <sake/core/utility/assert.hpp>

namespace sake
{

namespace functional
{

struct clamp_ip
{
    typedef void result_type;

    template< class L, class T, class U >
    void operator()(SAKE_FWD_REF( L ) lower, T& x, SAKE_FWD_REF( U ) upper) const
    {
        SAKE_ASSERT(!(upper < lower));
        if(upper < x)
            x = sake::forward<U>(upper);
        else if(x < lower)
            x = sake::forward<L>(lower);
    }

    template< class L, class T, class U, class Less >
    void operator()(SAKE_FWD_REF( L ) lower, T& x, SAKE_FWD_REF( U ) upper, Less less) const
    {
        SAKE_ASSERT(!less(upper, lower));
        if(less(upper, x))
            x = sake::forward<U>(upper);
        else if(less(x, lower))
            x = sake::forward<L>(lower);
    }

    template< class L, class T >
    void operator()(SAKE_FWD_REF( L ) lower, T& x, sake::unbounded_tag) const
    {
        if(x < lower)
            x = sake::forward<L>(lower);
    }

    template< class L, class T, class Less >
    void operator()(SAKE_FWD_REF( L ) lower, T& x, sake::unbounded_tag, Less less) const
    {
        if(less(x, lower))
            x = sake::forward<L>(lower);
    }

    template< class T, class U >
    void operator()(sake::unbounded_tag, T& x, SAKE_FWD_REF( U ) upper) const
    {
        if(upper < x)
            x = sake::forward<U>(upper);
    }

    template< class T, class U, class Less >
    void operator()(sake::unbounded_tag, T& x, SAKE_FWD_REF( U ) upper, Less less) const
    {
        if(less(upper, x))
            x = sake::forward<U>(upper);
    }

    template< class T >
    void operator()(sake::unbounded_tag, T& /*x*/, sake::unbounded_tag) const
    { }

    template< class T, class Less >
    void operator()(sake::unbounded_tag, T& /*x*/, sake::unbounded_tag, Less const & /*less*/) const
    { }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace clamp_ip_adl_barrier
{ sake::functional::clamp_ip const clamp_ip = { }; }
using namespace clamp_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::clamp_ip const clamp_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_CLAMP_IP_HPP
