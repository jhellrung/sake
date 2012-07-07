/*******************************************************************************
 * sake/core/iterator/end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::end(I const & i, Introterminal = null_introterminal_tag())
 *     -> sake::iterator_relax< I, Introterminal >::type
 * struct iterator::functional::end
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_END_HPP
#define SAKE_CORE_ITERATOR_END_HPP

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
class end
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

struct end
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::end, (1,2) )

    template< class I >
    typename result< end ( I ) >::type
    operator()(I const & i) const
    { return sake::iterator_traits<I>::at(i, sake::_end); }

    template< class I, class Introterminal >
    typename result< end ( I, Introterminal ) >::type
    operator()(I const & i, Introterminal) const
    { return sake::iterator_traits<I>::at(i, sake::_end, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace end_adl_barrier
{ sake::iterator::functional::end const end = { }; }
using namespace end_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::end const end = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_END_HPP
