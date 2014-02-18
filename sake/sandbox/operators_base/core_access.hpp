/*******************************************************************************
 * sake/core/operators_base/core_access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_OPERATORS_BASE_CORE_ACCESS_HPP
#define SAKE_CORE_OPERATORS_BASE_CORE_ACCESS_HPP

#include <sake/core/move/forward.hpp>
#include <sake/core/operators_base/fwd.hpp>

namespace sake
{

namespace operators_base
{

class core_access
{
    template< class, class, class > friend class sake::operators_base::plus;

    template< class Derived, class Result, class T0, class T1 >
    static Result
    plus(SAKE_FWD2_REF( T0 ) x0, SAKE_FWD2_REF( T1 ) x1)
    {
        return Derived::derived_plus2(
            sake::forward< T0 >(x0), sake::forward< T1 >(x1));
    }

    template< class Derived, class Result, class T1, class T0 >
    static Result
    rplus(SAKE_RWD2_REF( T1 ) x1, SAKE_FWD2_REF( T0 ) x0)
    {
        return Derived::derived_rplus2(
            sake::forward< T0 >(x0), sake::forward< T1 >(x1));
    }
};

} // namespace operators_base

} // namespace sake

#endif // #ifndef SAKE_CORE_OPERATORS_BASE_CORE_ACCESS_HPP
