/*******************************************************************************
 * sake/core/range/back.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::back(R&& r) -> sake::range_forward_reference<R>::type
 * range::cback(R const & r) -> sake::range_reference< R const >::type
 * struct range::functional::back
 * struct range::functional::cback
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BACK_HPP
#define SAKE_CORE_RANGE_BACK_HPP

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
struct back
    : sake::range_forward_reference<R>
{ };

template< class R >
struct cback
    : sake::range_reference<
          typename boost_ext::add_const_remove_reference<R>::type >
{ };

} // namespace result_of

namespace functional
{

struct back
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::back, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::back<R>::type
    operator()(R&& r) const
    {
        return sake::range_forward_traits<R>::at(
            sake::forward<R>(r), sake::_end);
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::back< R& >::type
    operator()(R& r) const
    { return sake::range_forward_traits< R& >::at(r, sake::_end); }

    template< class R >
    typename sake::range::result_of::back< R const & >::type
    operator()(R const & r) const
    { return sake::range_forward_traits< R const & >::at(r, sake::_end); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cback
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cback, 1 )

    template< class R >
    typename sake::range::result_of::cback< R const & >::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::at(r, sake::_end); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace back_adl_barrier {
sake::range::functional::back const back = { };
sake::range::functional::cback const cback = { }; }
using namespace back_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::back const back = { };
sake::range::functional::cback const cback = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BACK_HPP
