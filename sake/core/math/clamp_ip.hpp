/*******************************************************************************
 * sake/core/math/clamp_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * clamp_ip(L const & lower, T& x, U const & upper) -> T&
 * clamp_ip(L const & lower, T& x, U const & upper, Less less) -> T&
 *
 * Note: One can use sake::unbounded for either lower or upper to only clamp in
 * one direction.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_CLAMP_IP_HPP
#define SAKE_CORE_MATH_CLAMP_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/math/unbounded_tag.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class L, class T, class U, class Less = void >
struct clamp_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<T>::value));
    typedef T type;
};

} // namespace result_of

namespace functional
{

struct clamp_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::clamp_ip, (3,4) )

    template< class L, class T, class U >
    T& operator()(SAKE_FWD_REF( L ) lower, T& x, SAKE_FWD_REF( U ) upper) const
    {
        SAKE_ASSERT(!(upper < lower));
        return !(upper < x) ? !(x < lower) ? x : x = sake::forward<L>(lower) : x = sake::forward<U>(upper);
    }

    template< class L, class T, class U, class Less >
    T& operator()(SAKE_FWD_REF( L ) lower, T& x, SAKE_FWD_REF( U ) upper, Less less) const
    {
        SAKE_ASSERT(!less(upper, lower));
        return !less(upper, x) ? !less(x, lower) ? x : x = sake::forward<L>(lower) : x = sake::forward<U>(upper);
    }

    template< class L, class T >
    T& operator()(SAKE_FWD_REF( L ) lower, T& x, sake::unbounded_tag) const
    { return !(x < lower) ? x : x = sake::forward<L>(lower); }

    template< class L, class T, class Less >
    T& operator()(SAKE_FWD_REF( L ) lower, T& x, sake::unbounded_tag, Less less) const
    { return !less(x, lower) ? x : x = sake::forward<L>(lower); }

    template< class T, class U >
    T& operator()(sake::unbounded_tag, T& x, SAKE_FWD_REF( U ) upper) const
    { return !(upper < x) ? x : x = sake::forward<U>(upper); }

    template< class T, class U, class Less >
    T& operator()(sake::unbounded_tag, T& x, SAKE_FWD_REF( U ) upper, Less less) const
    { return !less(upper, x) ? x : x = sake::forward<U>(upper); }

    template< class T >
    T& operator()(sake::unbounded_tag, T& x, sake::unbounded_tag) const
    { return x; }

    template< class T, class Less >
    T& operator()(sake::unbounded_tag, T& x, sake::unbounded_tag, Less const & /*less*/) const
    { return x; }
};

} // namespace functional

functional::clamp_ip const clamp_ip = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_CLAMP_IP_HPP
