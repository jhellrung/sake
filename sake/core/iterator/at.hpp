/*******************************************************************************
 * sake/core/iterator/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::at(
 *     I const & i, T const & x,
 *     Introterminal = null_introterminal_tag())
 *     -> sake::iterator_relax< I, Introterminal >::type
 * struct iterator::functional::at
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_AT_HPP
#define SAKE_CORE_ITERATOR_AT_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/at_static_assert_cond.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I, class T, class Introterminal = sake::null_introterminal_tag >
class at
{
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((
        sake::iterator::private_::at_static_assert_cond<I,T>::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::iterator_relax<
        typename boost_ext::remove_qualifiers<I>::type,
        introterminal
    >::type type;
};

} // namespace result_of

namespace functional
{

struct at
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::at, (2,3) )

    template< class I, class T >
    typename result< at ( I, T ) >::type
    operator()(I const & i, T const & x) const
    { return sake::iterator_traits<I>::at(i,x); }

    template< class I, class T, class Introterminal >
    typename result< at ( I, T, Introterminal ) >::type
    operator()(I const & i, T const & x, Introterminal) const
    { return sake::iterator_traits<I>::at(i, x, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace at_adl_barrier
{ sake::iterator::functional::at const at = { }; }
using namespace at_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::at const at = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_AT_HPP
