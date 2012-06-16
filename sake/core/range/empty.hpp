/*******************************************************************************
 * sake/core/range/empty.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::empty(R const & r) -> bool
 * struct range::functional::empty
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_EMPTY_HPP
#define SAKE_CORE_RANGE_EMPTY_HPP

#include <sake/core/config.hpp>
#include <sake/core/range/traits.hpp>

namespace sake
{

namespace range
{

namespace functional
{

struct empty
{
    typedef bool result_type;

    template< class R >
    bool operator()(R const & r) const
    { return sake::range_traits<R>::empty(r); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace empty_adl_barrier
{ sake::range::functional::empty const empty = { }; }
using namespace empty_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::empty const empty = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_EMPTY_HPP
