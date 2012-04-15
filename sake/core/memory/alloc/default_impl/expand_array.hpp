/*******************************************************************************
 * sake/core/memory/alloc/default_impl/expand_array.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * alloc::default_impl::expand_array(A& a,
 *     std::size_t const obj_size, std::size_t const obj_align,
 *     alloc::traits<A>::void_pointer const p_array,
 *     alloc::traits<A>::size_type const count,
 *     alloc::expand_array_tag::type const tag,
 *     alloc::traits<A>::size_type const limit_count,
 *     alloc::traits<A>::size_type const preferred_count)
 *     -> sake::tuple<
 *            alloc::traits<A>::void_pointer,
 *            alloc::traits<A>::size_type
 *        >
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_EXPAND_ARRAY_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_EXPAND_ARRAY_HPP

#include <cstddef>

#include <boost/concept/assert.hpp>
#include <boost/range/reference.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/check_align.hpp>
#include <sake/core/memory/alloc/expand_array_tag.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_expand_array.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/range/concepts.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/nullptr.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace result_of
{

template< class A >
class expand_array
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
public:
    typedef sake::tuple< void_pointer, size_type > type2;
    typedef sake::tuple< void_pointer, size_type, bool > type3;
};

} // namespace result_of

namespace expand_array_private
{

template< class A >
inline typename sake::alloc::default_impl::expand_array<A>::type2
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    sake::alloc::expand_array_tag::type const tag,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count);

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::expand_array<A>::type3
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts);

} // namespace expand_array_private

template< class A >
inline typename sake::alloc::default_impl::expand_array<A>::type2
expand_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    sake::alloc::expand_array_tag::type const tag,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count)
{
    SAKE_ASSERT((sake::alloc::check_align(obj_align, obj_size)));
    SAKE_ASSERT((p_array));
    SAKE_ASSERT_RELATION( tag, !=, sake::alloc::expand_array_tag::alloc );
    SAKE_ASSERT_RELATION( count, <=, limit_count );
    SAKE_ASSERT_RELATION_ANY(
        (( limit_count, <=, preferred_count ))
        (( preferred_count, ==, sake::zero ))
    );
    // return a.expand_array(obj_size, obj_align, p_array, count, tag, limit_count, preferred_count);
    // return a.expand_array(obj_size, obj_align, p_array, tag, limit_count, preferred_count);
    // return result_type(nullptr, sake::zero.as< size_type >());
    return expand_array_private::impl(
        a, obj_size, obj_align, p_array, count, tag, limit_count, preferred_count);
}

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::expand_array<A>::type3
expand_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts)
{
    SAKE_USING_TYPEDEF( typename sake::alloc::traits<A>, size_type );
    SAKE_USING_TYPEDEF( typename sake::alloc::traits<A>, void_pointer );
    typedef sake::alloc::expand_array_tag::type tag_type;
    BOOST_CONCEPT_ASSERT((sake::concepts::ForwardReadableRange< TaggedCounts const >));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename boost::range_reference< TaggedCounts const >::type,
        sake::tuple< tag_type, size_type, size_type >
    >::value));
    SAKE_ASSERT((alloc::check_align(obj_align, obj_size)));
    SAKE_ASSERT((p_array));
    // return a.expand_array(obj_size, obj_align, p_array, count, tagged_counts);
    // return a.expand_array(obj_size, obj_align, p_array, tagged_counts);
    // ...
    return expand_array_private::impl(
        a, obj_size, obj_align, p_array, count, tagged_counts);
}

namespace expand_array_private
{

typedef sake::alloc::expand_array_tag::type tag_type;

template< class A >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type2
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    sake::alloc::expand_array_tag::type const tag,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    sake::int_tag<2>)
{ return a.expand_array(obj_size, obj_align, p_array, count, tag, limit_count, preferred_count); }

template< class A >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type2
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const /*count*/,
    sake::alloc::expand_array_tag::type const tag,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    sake::int_tag<1>)
{ return a.expand_array(obj_size, obj_align, p_array, tag, limit_count, preferred_count); }

template< class A >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type2
dispatch(A& /*a*/,
    std::size_t const obj_size, std::size_t const /*obj_align*/,
    typename sake::alloc::traits<A>::void_pointer const /*p_array*/,
    typename sake::alloc::traits<A>::size_type const /*count*/,
    sake::alloc::expand_array_tag::type const /*tag*/,
    typename sake::alloc::traits<A>::size_type const /*limit_count*/,
    typename sake::alloc::traits<A>::size_type const /*preferred_count*/,
    sake::int_tag<0>)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    typedef typename sake::alloc::default_impl::result_of::expand_array<A>::type2 result_type;
    return result_type(nullptr, sake::zero.as< size_type >());
}

template< class A >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type2
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    sake::alloc::expand_array_tag::type const tag,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
    typedef typename sake::alloc::default_impl::result_of::expand_array<A>::type2 result_type;
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_expand_array<
            A&, result_type
            ( std::size_t, std::size_t,
              void_pointer, size_type,
              tag_type, size_type, size_type )
        >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_expand_array<
            A&, result_type
            ( std::size_t, std::size_t,
              void_pointer,
              tag_type, size_type, size_type )
        >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    expand_array_private::dispatch(
        a, obj_size, obj_align, p_array, count, tag, limit_count, preferred_count,
        int_tag_()
    );
}

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type3
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts,
    sake::int_tag<2>)
{ return a.expand_array(obj_size, obj_align, p_array, count, tagged_counts); }

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type3
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const /*count*/,
    TaggedCounts const & tagged_counts,
    sake::int_tag<1>)
{ return a.expand_array(obj_size, obj_align, p_array, tagged_counts); }

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type3
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts,
    sake::int_tag<0>)
{
    // TODO: Iterate through tagged_counts, trying each in turn...
    std::abort();
}

template< class A, class TaggedCounts >
inline typename sake::alloc::default_impl::result_of::expand_array<A>::type3
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::void_pointer const p_array,
    typename sake::alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
    typedef typename sake::alloc::default_impl::result_of::expand_array<A>::type3 result_type;
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_expand_array<
            A&, result_type
            ( std::size_t, std::size_t,
              void_pointer, size_type,
              TaggedCounts const & )
        >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_expand_array<
            A&, result_type
            ( std::size_t, std::size_t,
              void_pointer,
              TaggedCounts const & )
        >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    expand_array_private::dispatch(
        a, obj_size, obj_align, p_array, count, tagged_counts,
        int_tag_()
    );
}

} // namespace expand_array_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_EXPAND_ARRAY_HPP
