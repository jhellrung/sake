/*******************************************************************************
 * sake/core/range/front.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::front(R&& r) -> sake::range_forward_reference<R>::type
 * range::cfront(R const & r) -> sake::range_reference< R const >::type
 * struct range::functional::front
 * struct range::functional::cfront
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FRONT_HPP
#define SAKE_CORE_RANGE_FRONT_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R >
struct front
    : sake::range_forward_reference<R>
{ };

template< class R >
struct cfront
    : sake::range_reference<
          typename boost_ext::add_const_remove_reference<R>::type >
{ };

} // namespace result_of

namespace functional
{

struct front
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::front, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::front<R>::type
    operator()(R&& r) const
    {
        return sake::range_forward_traits<R>::at(
            sake::forward<R>(r), sake::_begin);
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::front< R& >::type
    operator()(R& r) const
    { return sake::range_forward_traits< R& >::at(r, sake::_begin); }

    template< class R >
    typename sake::range::result_of::front< R const & >::type
    operator()(R const & r) const
    { return sake::range_forward_traits< R const & >::at(r, sake::_begin); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cfront
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cfront, 1 )

    template< class R >
    typename sake::range::result_of::cfront< R const & >::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::at(r, sake::_begin); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace front_adl_barrier
{
    sake::range::functional::front const front = { };
    sake::range::functional::cfront const cfront = { };
}
using namespace front_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::front const front = { };
sake::range::functional::cfront const cfront = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FRONT_HPP
