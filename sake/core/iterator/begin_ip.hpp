/*******************************************************************************
 * sake/core/iterator/begin_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::begin_ip(I& i) -> I&
 * struct iterator::functional::begin_ip
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_BEGIN_IP_HPP
#define SAKE_CORE_ITERATOR_BEGIN_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I >
struct begin_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<I>::value));
    typedef I type;
};

} // namespace result_of

namespace functional
{

struct begin_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::begin_ip, 1 )

    template< class I >
    typename sake::iterator::result_of::begin_ip< I& >::type
    operator()(I& i) const
    {
        sake::iterator_traits<I>::iter_at_ip(i, sake::_begin);
        return i;
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace begin_ip_adl_barrier
{ sake::iterator::functional::begin_ip const begin_ip = { }; }
using namespace begin_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::begin_ip const begin_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_BEGIN_IP_HPP
