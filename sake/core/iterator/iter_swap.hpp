/*******************************************************************************
 * sake/core/iterator/iter_swap.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iter_swap(I0 const & i0, I1 const & i1) -> void
 * struct functional::iter_swap
 *
 * Swaps *i0 and *i1.
 *
 * sake::iter_swap(I0, I1) is implemented in terms of
 * - iter_swap(I0, I1) (unqualified, hence subject to ADL), if available; else
 * - iter_swap(I1, I0) (unqualified, hence subject to ADL), if available; else
 * - sake::swap(*i0, *i1).
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ITER_SWAP_HPP
#define SAKE_CORE_ITERATOR_ITER_SWAP_HPP

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/swap.hpp>

namespace sake
{

namespace iter_swap_private
{

template< class I0, class I1 >
inline void
impl(I0 const & i0, I1 const & i1);

} // namespace iter_swap_private

/*******************************************************************************
 * iter_swap(I0 const & i0, I1 const & i1) -> void
 * struct functional::iter_swap
 ******************************************************************************/

namespace functional
{

struct iter_swap
{
    typedef void result_type;

    template< class I0, class I1 >
    void operator()(I0 const & i0, I1 const & i1) const
    { iter_swap_private::impl(i0, i1); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace iter_swap_adl_barrier
{ sake::functional::iter_swap const iter_swap = { }; }
using namespace iter_swap_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::iter_swap const iter_swap = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_iter_swap_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME iter_swap
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I0, class I1 >
inline void
adl(I0 const & i0, I1 const & i1)
{ iter_swap(i0, i1); }

} // namespace sake_iter_swap_private

namespace sake
{

namespace iter_swap_private
{

template< class I0, class I1 >
inline void
dispatch(I0 const & i0, I1 const & i1, sake::int_tag<2>)
{ ::sake_iter_swap_private::adl(i0, i1); }

template< class I0, class I1 >
inline void
dispatch(I0 const & i0, I1 const & i1, sake::int_tag<1>)
{ ::sake_iter_swap_private::adl(i1, i0); }

template< class I0, class I1 >
inline void
dispatch(I0 const & i0, I1 const & i1, sake::int_tag<0>)
{ sake::swap(*i0, *i1); }

template< class I0, class I1 >
inline void
impl(I0 const & i0, I1 const & i1)
{
    typedef typename boost_ext::mpl::
         if_<
        ::sake_iter_swap_private::is_callable< void ( I0 const &, I1 const & ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_iter_swap_private::is_callable< void ( I1 const &, I0 const & ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    iter_swap_private::dispatch(i0, i1, int_tag_());
}

} // namespace iter_swap_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ITER_SWAP_HPP
