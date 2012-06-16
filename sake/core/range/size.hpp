/*******************************************************************************
 * sake/core/range/size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::size(R const & r) -> range_size<R>::type
 * struct range::functional::size
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_SIZE_HPP
#define SAKE_CORE_RANGE_SIZE_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R >
struct size
    : sake::range_size< typename boost_ext::remove_qualifiers<R>::type >
{ };

} // namespace result_of

namespace functional
{

struct size
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::size, 1 )

    template< class R >
    typename sake::range::result_of::size<R>::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::size(r); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace size_adl_barrier
{ sake::range::functional::size const size = { }; }
using namespace size_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::size const size = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_SIZE_HPP
