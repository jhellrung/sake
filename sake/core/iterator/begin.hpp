/*******************************************************************************
 * sake/core/iterator/begin.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::begin(I const & i, Introterminal = null_introterminal_tag())
 *     -> sake::iterator_relax< I, Introterminal >::type
 * struct iterator::functional::begin
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_BEGIN_HPP
#define SAKE_CORE_ITERATOR_BEGIN_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I, class Introterminal = sake::null_introterminal_tag >
class begin
{
    typedef typename boost_ext::remove_qualifiers<I>::type iterator;
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::iterator_relax< iterator, introterminal >::type type;
};

} // namespace result_of

namespace functional
{

struct begin
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::begin, (1,2) )

    template< class I >
    typename result< begin ( I ) >::type
    operator()(I const & i) const
    { return sake::iterator_traits<I>::at(i, sake::_begin); }

    template< class I, class Introterminal >
    typename result< begin ( I, Introterminal ) >::type
    operator()(I const & i, Introterminal) const
    { return sake::iterator_traits<I>::at(i, sake::_begin, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace begin_adl_barrier
{ sake::iterator::functional::begin const begin = { }; }
using namespace begin_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::begin const begin = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_BEGIN_HPP
