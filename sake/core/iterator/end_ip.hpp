/*******************************************************************************
 * sake/core/iterator/end_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::end_ip(I& i) -> void
 * struct iterator::functional::end_ip
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_END_IP_HPP
#define SAKE_CORE_ITERATOR_END_IP_HPP

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

namespace functional
{

struct end_ip
{
    typedef void result_type;

    template< class I >
    void operator()(I& i) const
    { sake::iterator_traits<I>::at_ip(i, sake::_end); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace end_ip_adl_barrier
{ sake::iterator::functional::end_ip const end_ip = { }; }
using namespace end_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::end_ip const end_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_END_IP_HPP
