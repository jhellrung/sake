/*******************************************************************************
 * sake/core/range/algorithm/for_each.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::algorithm::for_each(R&& r, F f) -> void
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP
#define SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP

#include <boost/config.hpp>
#include <boost/concept/assert.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/algorithm/private/for_each.hpp>
#include <sake/core/range/apply/for_each.hpp>
#include <sake/core/range/apply/fwd.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>
#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

namespace result_of
{

template< class R, class F >
class for_each
{
    typedef typename boost_ext::remove_reference<R>::type range_type;
    typedef typename boost_ext::remove_qualifiers<F>::type function_type;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< range_type >));
    typedef typename sake::range_forward_reference<R>::type reference;
    BOOST_CONCEPT_ASSERT((sake::concepts::Function<
        function_type, void ( reference ) >));
public:
    typedef void type;
};

} // namespace result_of

namespace functional
{

struct for_each
{
    typedef void result_type;

    // We use
    //     typename sake::range::algorithm::result_of::for_each<R,F>::type
    // for early static and concept assertions.

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename sake::range::algorithm::result_of::for_each<R,F>::type
    operator()(R&& r, F const & f) const
    { for_each_private::impl(sake::forward<R>(r), f); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    template< class F >
    struct rv_sink_visitor
    { typedef sake::range::apply::functional::for_each<
        typename sake::call_traits<F>::param_type > type; };
    template< class F >
    struct rv_sink_default
    { typedef rv_sink_traits_::default_<
        typename rv_sink_visitor<F>::type > type; };
public:

    // lvalues + movable explicit rvalues
    template< class R, class F >
    typename sake::range::algorithm::result_of::for_each< R&, F >::type
    operator()(R& r, F const & f) const
    { for_each_private::impl(r,f); }

    // movable implicit rvalues
    template< class F >
    void
    operator()(typename rv_sink_default<F>::type r, F const & f) const
    { r(typename rv_sink_visitor<F>::type(f)); }

    // const lvalues + non-movable rvalues
    template< class R, class F >
    typename rv_sink_traits_::cref_lazy_enabler<
        R,sake::range::algorithm::result_of::for_each< R const &, F > >::type
    operator()(R const & r, F const & f) const
    { for_each_private::impl(r,f); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace for_each_adl_barrier
{ sake::range::algorithm::functional::for_each const for_each = { }; }
using namespace for_each_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::algorithm::functional::for_each const for_each = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_FOR_EACH_HPP
