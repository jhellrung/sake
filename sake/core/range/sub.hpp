/*******************************************************************************
 * sake/core/range/sub.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::sub(R&& r, Begin b, End e)
 *     -> range_forward_subrange< R, Begin, End >::type
 * range::csub(R const & r, Begin b, End e)
 *     -> range_subrange< R const, Begin, End >::type
 * struct range::functional::sub
 * struct range::functional::csub
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_SUB_HPP
#define SAKE_CORE_RANGE_SUB_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R, class Begin, class End >
struct sub
    : sake::range_forward_subrange<
          R,
          typename boost_ext::remove_qualifiers< Begin >::type,
          typename boost_ext::remove_qualifiers< End >::type
      >
{ };

template< class R, class T, class Introterminal >
class csub
    : sake::range_subrange<
          typename boost_ext::add_const_remove_reference<R>::type,
          typename boost_ext::remove_qualifiers< Begin >::type,
          typename boost_ext::remove_qualifiers< End >::type
      >
{ };

} // namespace result_of

namespace functional
{

struct sub
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::sub, 3 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class Begin, class End >
    typename result< sub ( R, Begin, End ) >::type
    operator()(R&& r, Begin const & b, End const & e) const
    { return sake::range_forward_traits<R>::sub(sake::forward<R>(r), b, e); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class Begin, class End >
    typename result< sub ( R&, Begin, End ) >::type
    operator()(R& r, Begin const & b, End const & e) const
    { return sake::range_forward_traits< R& >::sub(r,b,e); }

    template< class R, class Begin, class End >
    typename result< sub ( R const &, Begin, End ) >::type
    operator()(R const & r, Begin const & b, End const & e) const
    { return sake::range_forward_traits< R const & >::sub(r,b,e); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct csub
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::csub, 3 )

    template< class R, class Begin, class End >
    typename result< csub ( R, Begin, End ) >::type
    operator()(R const & r, Begin const & b, End const & e) const
    { return sake::range_traits< R const >::sub(r,b,e); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sub_adl_barrier {
sake::range::functional::sub const sub = { };
sake::range::functional::csub const csub = { }; }
using namespace sub_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::sub const sub = { };
sake::range::functional::csub const csub = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_SUB_HPP
