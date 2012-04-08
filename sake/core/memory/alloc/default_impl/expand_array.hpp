/*******************************************************************************
 * sake/core/memory/alloc/default_impl/expand_array.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
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

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/check_align.hpp>
#include <sake/core/memory/alloc/expand_array_tag.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_expand_array.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/range/concepts.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/nullptr.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace expand_array_private
{

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    alloc::expand_array_tag::type const tag,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count);

template< class A, class TaggedCounts >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type,
    bool
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts);

} // namespace expand_array_private

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
expand_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    alloc::expand_array_tag::type const tag,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count)
{
    SAKE_ASSERT((alloc::check_align(obj_align, obj_size)));
    SAKE_ASSERT((p_array));
    SAKE_ASSERT_RELATION( tag, !=, alloc::expand_array_tag::alloc );
    SAKE_ASSERT_RELATION( count, <=, limit_count );
    SAKE_ASSERT_RELATION_ANY(
        (( limit_count, <=, preferred_count ))
        (( preferred_count, ==, sake::zero ))
    );
    // return a.expand_array(obj_size, obj_align, p_array, count, tag, limit_count, preferred_count);
    // return a.expand_array(obj_size, obj_align, p_array, tag, limit_count, preferred_count);
    // return sake::tuple< void_pointer, size_type >(nullptr, sake::zero.as< size_type >());
    return expand_array_private::dispatch(a, obj_size, obj_align, p_array, count, tag, limit_count, preferred_count);
}

template< class A, class TaggedCounts >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type,
    bool
>
expand_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    TaggedCounts const & tagged_counts)
{
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
    typedef alloc::expand_array_tag::type tag_type;
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
    return expand_array_private::dispatch(a, obj_size, obj_align, p_array, count, tagged_counts);
}

namespace expand_array_private
{

typedef alloc::expand_array_tag::type tag_type;

template<
    int, class A,
    class Z = typename alloc::traits<A>::size_type,
    class VP = typename alloc::traits<A>::void_pointer,
    class R = sake::tuple<VP,Z>
>
struct dispatch_result;

template< class A, class Z, class VP, class R >
struct dispatch_result<2,A,Z,VP,R>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_expand_array<
              A&, R ( std::size_t, std::size_t, VP, Z, tag_type, Z, Z )
          >::value,
          R
      >
{ };

template< class A >
inline dispatch_result<2,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    alloc::expand_array_tag::type const tag,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    sake::dispatch_priority_tag<2>)
{ return a.expand_array(obj_size, obj_align, p_array, count, tag, limit_count, preferred_count); }

template< class A, class Z, class VP, class R >
struct dispatch_result<1,A,Z,VP,R>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_expand_array<
              A&, R ( std::size_t, std::size_t, VP, tag_type, Z, Z )
          >::value,
          R
      >
{ };

template< class A >
inline dispatch_result<1,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const /*count*/,
    alloc::expand_array_tag::type const tag,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    sake::dispatch_priority_tag<1>)
{ return a.expand_array(obj_size, obj_align, p_array, tag, limit_count, preferred_count); }

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& /*a*/,
    std::size_t const obj_size, std::size_t const /*obj_align*/,
    typename alloc::traits<A>::void_pointer const /*p_array*/,
    typename alloc::traits<A>::size_type const /*count*/,
    alloc::expand_array_tag::type const /*tag*/,
    typename alloc::traits<A>::size_type const /*limit_count*/,
    typename alloc::traits<A>::size_type const /*preferred_count*/)
{
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
    typedef sake::tuple< void_pointer, size_type > result_type;
    return result_type(nullptr, sake::zero.as< size_type >());
}

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_array,
    typename alloc::traits<A>::size_type const count,
    alloc::expand_array_tag::type const tag,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count)
{
    expand_array_private::dispatch(a, obj_size, obj_align, p_array, count, tag, limit_count, preferred_count,
        sake::dispatch_priority_tag<2>());
}

} // namespace expand_array_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_EXPAND_ARRAY_HPP
