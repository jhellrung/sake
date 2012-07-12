/*******************************************************************************
 * sake/core/range/algorithm/private/accumulate.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_PRIVATE_ACCUMULATE_HPP
#define SAKE_CORE_RANGE_ALGORITHM_PRIVATE_ACCUMULATE_HPP

#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/algorithm/for_each.hpp>
#include <sake/core/range/algorithm/fwd.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

namespace accumulate_private
{

template< class T, class F, class U >
class for_each_fun
    : sake::base_member<F>
{
    typedef sake::base_member<F> base_member_;
    T& x;
public:

    for_each_fun(T& x_, F const & f)
        : base_member_(f), x(x_)
    { }

    SAKE_MEMBERWISE_COPY_TAGS( (( base_member_ )) (( T& )( x_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( base_member_ )) (( T& )( x_ )) )

    typedef void result_type;

private:
    typedef sake::call_traits<U> call_traits_;
public:
    void operator()(typename call_traits_::fwd_param_type y) const
    {
        x = base_member_::member()(
            sake::move(x), call_traits_::fwd_cast(y));
    }
};

template< class R, class T, class F >
inline T
impl(SAKE_FWD2_REF( R ) r, T x, F const & f)
{
    typedef typename sake::range_forward_reference<
        SAKE_FWD2_PARAM( R ) >::type reference;
    typedef accumulate_private::for_each_fun< T, F, reference > for_each_fun_;
    sake::range::algorithm::functional::for_each()(
        sake::forward<R>(r), for_each_fun_(x,f));
    return sake::move(x);
}

} // namespace accumulate_private

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_PRIVATE_ACCUMULATE_HPP
