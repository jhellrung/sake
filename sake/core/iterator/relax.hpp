/*******************************************************************************
 * sake/core/iterator/relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::relax(I const & i, Introversal = null_introversal_tag())
 *     -> sake::iterator_relax< I, Introversal >::type
 * struct iterator::functional::relax
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_RELAX_HPP
#define SAKE_CORE_ITERATOR_RELAX_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I, class Introversal = sake::null_introversal_tag >
class relax
{
    typedef typename boost_ext::remove_qualifiers<I>::type iterator;
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, sake::null_introversal_tag >::value));
public:
    typedef typename sake::iterator_relax< iterator, introversal >::type type;
};

} // namespace result_of

namespace functional
{

struct relax
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::relax, (1,2) )

    template< class I, class Introversal >
    typename sake::iterator::result_of::relax< I, Introversal >::type
    operator()(I const & i, Introversal = sake::null_introversal_tag()) const
    { return i; }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace relax_adl_barrier
{ sake::iterator::functional::relax const relax = { }; }
using namespace relax_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::relax const relax = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_RELAX_HPP
