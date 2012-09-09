/*******************************************************************************
 * sake/core/range/algorithm/private/for_each.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_PRIVATE_FOR_EACH_HPP
#define SAKE_CORE_RANGE_ALGORITHM_PRIVATE_FOR_EACH_HPP

#include <cstddef>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/multidim_traits.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/range/algorithm/for_each.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/range/apply/for_each.hpp>
#include <sake/core/range/apply/fwd.hpp>
#include <sake/core/range/forward_multidim_traits.hpp>
#include <sake/core/range/forward_multidim_traits_fwd.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

namespace for_each_private
{

template< class R, class F >
inline void
dispatch(SAKE_FWD2_REF( R ) r, F const & f, sake::int_tag<3>)
{
    typedef sake::range_forward_multidim_traits< SAKE_FWD2_PARAM( R ) > traits_;
    sake::range::algorithm::functional::for_each()(
        traits_::outer(sake::forward<R>(r)),
        sake::range::apply::functional::for_each<
            typename sake::call_traits<F>::param_type >(f)
    );
}

template< class R, class F >
inline void
dispatch(SAKE_FWD2_REF( R ) r, F const & f, sake::int_tag<2>)
{
    typedef sake::range_forward_traits< SAKE_FWD2_PARAM( R ) > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator );
    typedef sake::iterator_multidim_traits< iterator > iterator_traits_;
    SAKE_USING_TYPEDEF( typename iterator_traits_, outer_iterator );
    typedef sake::range_forward_traits<
        typename sake::iterator_reference< outer_iterator >::type
    > inner_traits_;
    iterator b = traits_::begin(sake::forward<R>(r));
    iterator e = traits_::end(sake::forward<R>(r));
    outer_iterator bi = iterator_traits_::outer(b);
    outer_iterator ei = iterator_traits_::outer(e);
    if(bi == ei) {
        sake::range::algorithm::functional::for_each()(
            inner_traits_::sub(
                *bi,
                iterator_traits_::inner(b),
                iterator_traits_::inner(e)
            ),
            f
        );
        return;
    }
    sake::range::algorithm::functional::for_each()(
        inner_traits_::sub(*bi, iterator_traits_::inner(b), sake::_end), f);
    while(++bi != ei)
        sake::range::algorithm::functional::for_each<>()(*bi, f);
    sake::range::algorithm::functional::for_each()(
        inner_traits_::sub(*bi, sake::_begin, iterator_traits_::inner(e)), f);
}

template< std::size_t N, class I, class F >
inline void
dispatch1_impl(I i, F f)
{
    for(std::size_t j = 0; j != N; ++j, ++i)
        f(*i);
}

template< class R, class F >
inline void
dispatch(SAKE_FWD2_REF( R ) r, F const & f, sake::int_tag<1>)
{
    static std::size_t const n = sake::range_static_size<
        typename boost_ext::remove_qualifiers<R>::type >::value;
    typedef sake::range_forward_traits< SAKE_FWD2_PARAM( R ) > traits_;
    for_each_private::dispatch1_impl<n>(traits_::begin(sake::forward<R>(r)), f);
}

template< class I, class F >
inline void
dispatch0_impl(I i, F f)
{
    for(; i != sake::_end; ++i)
        f(*i);
}

template< class R, class F >
inline void
dispatch(SAKE_FWD2_REF( R ) r, F const & f, sake::int_tag<0>)
{
    typedef sake::range_forward_traits< SAKE_FWD2_PARAM( R ) > traits_;
    for_each_private::dispatch0_impl(
        traits_::begin(
            sake::forward<R>(r),
            sake::end_detect_introterminal_tag()
        ),
        f
    );
}

template< class R, class F >
inline void
impl(SAKE_FWD2_REF( R ) r, F const & f)
{
    typedef typename sake::range_forward_iterator<
        SAKE_FWD2_PARAM( R ) >::type iterator;
    typedef typename boost_ext::mpl::
         if_< sake::range_forward_multidim_enable< SAKE_FWD2_PARAM( R ) >,
              sake::int_tag<3> >::type::template
    else_if < sake::iterator_multidim_enable< iterator >,
              sake::int_tag<2> >::type::template
    else_if < sake::range_has_static_size<
                  typename boost_ext::remove_qualifiers<R>::type >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    for_each_private::dispatch(sake::forward<R>(r), f, int_tag_());
}

} // namespace for_each_private

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_PRIVATE_FOR_EACH_HPP
