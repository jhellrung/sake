/*******************************************************************************
 * sake/core/iterator/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::at(
 *     I const & i, T const & x,
 *     Introversal = null_introversal_tag())
 *     -> sake::iterator_relax< I, Introversal >::type
 * struct iterator::functional::at
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_AT_HPP
#define SAKE_CORE_ITERATOR_AT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/is_interoperable.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I, class T, class Introversal = sake::null_introversal_tag >
class at
{
    typedef typename boost_ext::remove_qualifiers<I>::type iterator;
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal_;
    SAKE_USING_TYPEDEF( typename sake::iterator_traits< iterator >, introversal );
    BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
        boost_ext::mpl::and2<
            boost::is_same< noqual_type, sake::begin_tag >,
            boost_ext::is_convertible<
                introversal, sake::begin_access_introversal_tag >
        >,
        boost_ext::mpl::and2<
            boost::is_same< noqual_type, sake::end_tag >,
            boost_ext::is_convertible<
                introversal, sake::end_access_introversal_tag >
        >,
        sake::iterator::private_::is_interoperable< iterator, noqual_type >
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal_, sake::null_introversal_tag >::value));
public:
    typedef typename sake::iterator_relax< iterator, introversal_ >::type type;
};

} // namespace result_of

namespace functional
{

struct at
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::at, (2,3) )

    template< class I, class T >
    typename sake::iterator::result_of::at< I const &, T const & >::type
    operator()(I const & i, T const & x) const
    { return sake::iterator_traits<I>::at(i, x, sake::null_introversal_tag()); }

    template< class I, class T, class Introversal >
    typename sake::iterator::result_of::at<
        I const &, T const &, Introversal >::type
    operator()(I const & i, T const & x, Introversal) const
    { return sake::iterator_traits<I>::at(i, x, Introversal()); }
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
