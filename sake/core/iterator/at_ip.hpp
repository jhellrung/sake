/*******************************************************************************
 * sake/core/iterator/at_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::at_ip(I& i, T const & x) -> void
 * struct iterator::functional::at_ip
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_AT_IP_HPP
#define SAKE_CORE_ITERATOR_AT_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/private/at_static_assert_cond.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

namespace functional
{

struct at_ip
{
    typedef void result_type;

    template< class I, class T >
    void operator()(I& i, T const & x) const
    {
        BOOST_STATIC_ASSERT((
            sake::iterator::private_::at_static_assert_cond< I&, T >::value));
        sake::iterator_traits<I>::at_ip(i,x);
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace at_ip_adl_barrier
{ sake::iterator::functional::at_ip const at_ip = { }; }
using namespace at_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::at_ip const at_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_AT_IP_HPP
