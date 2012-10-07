/*******************************************************************************
 * sake/core/memory/alloc/traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_TRAITS_HPP
#define SAKE_CORE_MEMORY_ALLOC_TRAITS_HPP

#include <cstddef>

#include <limits>

#include <boost/mpl/eval_if_c.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/difference_type.hpp>
#include <sake/boost_ext/mpl/size_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/introspection/has_type_difference_type.hpp>
#include <sake/core/introspection/has_type_size_type.hpp>
#include <sake/core/memory/alloc/check_align.hpp>
#include <sake/core/memory/alloc/contract_array_tag.hpp>
#include <sake/core/memory/alloc/expand_array_op.hpp>
#include <sake/core/memory/alloc/expand_array_tag.hpp>
#include <sake/core/memory/alloc/default_impl/allocate_array.hpp>
#include <sake/core/memory/alloc/default_impl/allocate_object.hpp>
#include <sake/core/memory/alloc/default_impl/construct.hpp>
#include <sake/core/memory/alloc/default_impl/contract_array.hpp>
#include <sake/core/memory/alloc/default_impl/deallocate_array.hpp>
#include <sake/core/memory/alloc/default_impl/deallocate_object.hpp>
#include <sake/core/memory/alloc/default_impl/destruct.hpp>
#include <sake/core/memory/alloc/default_impl/expand_array.hpp>
#include <sake/core/memory/alloc/default_impl/value_traits.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/pointee.hpp>
#include <sake/core/utility/swap.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

template< class A >
struct traits
    : extension::traits<A>
{ };

namespace extension
{

template< class A, class Enable /*= void*/ >
struct traits
    : sake::alloc::default_impl::traits<A>
{ };

} // namespace extension

namespace default_impl
{

template< class A >
struct traits
{
    typedef A allocator_type;

    template< class T >
    struct value_traits
        : sake::alloc::default_impl::value_traits<A,T>
    { };

    SAKE_USING_TYPEDEF( typename value_traits< void >, pointer );
    SAKE_USING_TYPEDEF( typename value_traits< void >, const_pointer );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        void_pointer, const_void_pointer >::value));

    typedef typename boost::mpl::eval_if_c<
        sake::has_type_size_type<A>::value,
        boost_ext::mpl::size_type<A>,
        boost::mpl::identity< std::size_t >
    >::type size_type;
    typedef typename boost::mpl::eval_if_c<
        sake::has_type_difference_type<A>::value,
        boost_ext::mpl::difference_type<A>,
        boost::mpl::identity< std::ptrdiff_t >
    >::type difference_type;

    template< class A_ >
    static SAKE_FWD2_REF( A_ )
    on_container_construct(SAKE_FWD2_REF( A_ ) a)
    { return sake::forward<A_>(a); }

    template< class A_ >
    static void
    on_container_assign(A& dest, SAKE_FWD2_REF( A_ ) srce)
    { dest = sake::forward<A_>(srce); }

    static void
    on_container_swap(A& a0, A& a1)
    { sake::swap(a0,a1); }

    static void_pointer
    allocate_object(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        const_void_pointer const p_hint)
    {
        return sake::alloc::default_impl::allocate_object(a,
            obj_size, obj_align,
            p_hint
        );
    }

    static void
    deallocate_object(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        void_pointer const p_obj)
    {
        sake::alloc::default_impl::deallocate_object(a,
            obj_size, obj_align,
            p_obj
        );
    }

    static sake::tuple< void_pointer, size_type >
    allocate_array(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        size_type const limit_count, size_type const preferred_count,
        const_void_pointer const p_hint)
    {
        return sake::alloc::default_impl::allocate_array(a,
            obj_size, obj_align,
            limit_count, preferred_count,
            p_hint
        );
    }

    static sake::tuple< void_pointer, size_type >
    expand_array(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        void_pointer const p_array, size_type const count,
        alloc::expand_array_tag::type const tag,
        size_type const limit_count, size_type const preferred_count)
    {
        sake::alloc::default_impl::expand_array(a,
            obj_size, obj_align,
            p_array, count, tag,
            limit_count, preferred_count
        );
    }

    template< class TaggedCounts >
    static sake::tuple< void_pointer, size_type, bool >
    expand_array(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        void_pointer const p_array, size_type const count,
        TaggedCounts const & tagged_counts)
    {
        sake::alloc::default_impl::expand_array(a,
            obj_size, obj_align,
            p_array, count,
            tagged_counts
        );
    }

    static void
    deallocate_array(A& a,
        std::size_t const obj_size, std::size_t const obj_align,
        void_pointer const p_array, size_type const count)
    {
        sake::alloc::default_impl::deallocate_array(a,
            obj_size, obj_align,
            p_array, count
        );
    }

    template< class Signature >
    static void
    construct(A& a,
        void_pointer const p_obj,
        sake::emplacer< Signature > e)
    { sake::alloc::default_impl::construct(a, p_obj, e); }

    template< class P >
    static void
    destruct(A& a,
        P const p_obj)
    { sake::alloc::default_impl::destruct(a, p_obj); }
};

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_TRAITS_HPP
