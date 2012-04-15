/*******************************************************************************
 * sake/core/memory/alloc/default_impl/deallocate_object.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * alloc::default_impl::deallocate_object(A& a,
 *     std::size_t const obj_size, std::size_t const obj_align,
 *     alloc::traits<A>::void_pointer const p_obj)
 *     -> void
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DEALLOCATE_OBJECT_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DEALLOCATE_OBJECT_HPP

#include <cstddef>
#include <cstdlib>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/aligned_type.hpp>
#include <sake/boost_ext/type_traits/max_align.hpp>

#include <sake/core/math/intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/check_align.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_deallocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_deallocate_object.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace deallocate_object_private
{

template< class A >
inline void
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj);

} // namespace deallocate_object_private

template< class A >
inline void
deallocate_object(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj)
{
    SAKE_ASSERT((alloc::check_align(obj_align, obj_size)));
    if(p_obj == nullptr)
        return;
    // a.deallocate_object(obj_size, obj_align, p_obj);
    // a.deallocate_object(p_obj);
    // rebind<A,T>::type(a).deallocate(p_obj, obj_size / sizeof( T ));
    // rebind<A,T>::type(a).deallocate(p_obj);
    deallocate_object_private::impl(a, obj_size, obj_align, p_obj);
}

namespace deallocate_object_private
{

template< class A >
inline void
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj,
    sake::int_tag<2>)
{ a.deallocate_object(obj_size, obj_align, p_obj); }

template< class A >
inline void
dispatch(A& a,
    std::size_t const /*obj_size*/, std::size_t const /*obj_align*/,
    typename alloc::traits<A>::void_pointer const p_obj,
    sake::int_tag<1>)
{ a.deallocate_object(p_obj); }

template< class A >
inline void
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj,
    sake::int_tag<0>);

template< class A >
inline void
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_deallocate_object<
            A&, void ( std::size_t, std::size_t, void_pointer ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_deallocate_object<
            A&, void ( void_pointer ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return deallocate_object_private::dispatch(
        a, obj_size, obj_align, p_obj,
        int_tag_()
    );
}

template<
    std::size_t Align,
    class Aligned = typename boost_ext::aligned_type< Align >::type
>
struct dispatch_obj_align;

template< class A >
template< class A >
inline void
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::void_pointer const p_obj,
    sake::int_tag<1>)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
    SAKE_ASSERT_RELATION( obj_align, <=, 128 );
    if(obj_align == 0)
        obj_align = boost_ext::max_align;
    typedef void (*fun_ptr_type)( A&, std::size_t, void_pointer );
    static fun_ptr_type const fun_ptrs[8] = {
        &dispatch_obj_align<  1>::apply,
        &dispatch_obj_align<  2>::apply,
        &dispatch_obj_align<  4>::apply,
        &dispatch_obj_align<  8>::apply,
        &dispatch_obj_align< 16>::apply,
        &dispatch_obj_align< 32>::apply,
        &dispatch_obj_align< 64>::apply,
        &dispatch_obj_align<128>::apply
    };
    (*fun_ptrs[sake::intlog2(obj_align)])(a, obj_size, p_obj);
}

template< std::size_t Align, class Aligned >
struct dispatch_obj_align
{
    template< class A >
    static typename boost::disable_if_c<
        boost::is_same< A, typename alloc::rebind< A, Aligned >::type >::value
    >::type
    apply(A& a,
        std::size_t obj_size,
        typename alloc::traits<A>::void_pointer p_obj)
    {
        typedef typename alloc::rebind< A, Aligned >::type rebind_type;
        BOOST_STATIC_ASSERT((boost::is_same<
            rebind_type,
            typename alloc::rebind< rebind_type, Aligned >::type
        >::value));
        rebind_type a_(a);
        apply(a_, obj_size, p_obj);
    };

    template< class A >
    static typename boost::enable_if_c<
        boost::is_same< A, typename alloc::rebind< A, Aligned >::type >::value
    >::type
    apply(A& a,
        std::size_t obj_size,
        typename alloc::traits<A>::void_pointer p_obj)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_::template value_traits< Aligned >, pointer );
        typedef typename boost_ext::mpl::
             if_<
            sake::alloc::is_callable_mem_fun_deallocate<
                A&, void ( pointer, std::size_t ) >,
            sake::int_tag<2>
        >::type::template
        else_if <
            sake::alloc::is_callable_mem_fun_deallocate<
                A&, void ( pointer ) >,
            sake::int_tag<1>
        >::type::template
        else_   <
            sake::int_tag<0>
        >::type int_tag_;
        SAKE_ASSERT_RELATION( obj_size % sizeof( Aligned ), ==, sake::zero );
        return apply(
            a, obj_size, p_hint,
            int_tag_()
        );
    }

    template< class A >
    static void
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::void_pointer const p_obj,
        sake::int_tag<2>)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_::template value_traits< Aligned >, pointer );
        a.deallocate(static_cast< pointer >(p_obj), obj_size / sizeof( Aligned ));
    }

    template< class A >
    static void
    apply(A& a,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::void_pointer const p_obj,
        sake::int_tag<1>)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_::template value_traits< Aligned >, pointer );
        a.deallocate(static_cast< pointer >(p_obj));
    }

    template< class A >
    static void
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::void_pointer const /*p_obj*/,
        sake::int_tag<0>)
    { std::abort(); }
};

template< std::size_t Align >
struct dispatch_obj_align< Align, void >
{
    template< class A>
    static void
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::void_pointer const /*p_obj*/)
    { std::abort(); }
};

} // namespace deallocate_object_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DEALLOCATE_OBJECT_HPP
