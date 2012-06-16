/*******************************************************************************
 * sake/core/range/begin.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::begin(R&& r, Introversal = null_introversal_tag())
 *     -> range_forward_iterator< R, Introversal >::type
 * range::cbegin(R const & r, Introversal = null_introversal_tag())
 *     -> range_iterator< R const, Introversal >::type
 * struct range::functional::begin
 * struct range::functional::cbegin
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BEGIN_HPP
#define SAKE_CORE_RANGE_BEGIN_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R, class Introversal = sake::null_introversal_tag >
class begin
{
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, sake::null_introversal_tag >::value));
public:
    typedef typename sake::range_forward_iterator< R, introversal >::type type;
};

template< class R, class Introversal = sake::null_introversal_tag >
class cbegin
{
    typedef typename boost_ext::add_const_remove_reference<R>::type range_;
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, sake::null_introversal_tag >::value));
public:
    typedef typename sake::range_iterator< range_, introversal >::type type;
};

} // namespace result_of

namespace functional
{

struct begin
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::begin, (1,2) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class Introversal >
    typename sake::range::result_of::begin< R, Introversal >::type
    operator()(R&& r, Introversal = sake::null_introversal_tag()) const
    {
        return sake::range_forward_traits<R>::iter_at(
            sake::forward<R>(r), sake::_begin, Introversal());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class Introversal >
    typename sake::range::result_of::begin< R&, Introversal >::type
    operator()(R& r, Introversal = sake::null_introversal_tag()) const
    {
        return sake::range_forward_traits< R& >::iter_at(
            r, sake::_begin, Introversal());
    }

    template< class R, class Introversal >
    typename sake::range::result_of::begin< R const &, Introversal >::type
    operator()(R const & r, Introversal = sake::null_introversal_tag()) const
    {
        return sake::range_forward_traits< R const & >::iter_at(
            r, sake::_begin, Introversal());
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cbegin
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cbegin, (1,2) )

    template< class R, class Introversal >
    typename sake::range::result_of::cbegin< R const &, Introversal >::type
    operator()(R const & r, Introversal = sake::null_introversal_tag()) const
    {
        return sake::range_traits< R const >::iter_at(
            r, sake::_begin, Introversal());
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace begin_adl_barrier
{
    sake::range::functional::begin const begin = { };
    sake::range::functional::cbegin const cbegin = { };
}
using namespace begin_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::begin const begin = { };
sake::range::functional::cbegin const cbegin = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BEGIN_HPP
