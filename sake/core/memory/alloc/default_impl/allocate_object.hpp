/*******************************************************************************
 * sake/core/memory/alloc/default_impl/allocate_object_impl.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * alloc::default_impl::allocate_object(A& a,
 *     std::size_t const obj_size, std::size_t const obj_align,
 *     alloc::traits<A>::const_void_pointer const p_hint)
 *     -> alloc::traits<A>::void_pointer
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_OBJECT_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_OBJECT_HPP

#include <cstddef>

#include <new>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/aligned_type.hpp>
#include <sake/boost_ext/type_traits/max_align.hpp>

#include <sake/core/math/intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/check_align.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate_object.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace allocate_object_private
{

template< class A >
inline typename alloc::traits<A>::void_pointer
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const p_hint);

} // namespace allocate_object_private

template< class A >
inline typename alloc::traits<A>::void_pointer
allocate_object(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const p_hint)
{
    SAKE_ASSERT((alloc::check_align(obj_align, obj_size)));
    // return a.allocate_object(obj_size, obj_align, p_hint);
    // return a.allocate_object(obj_size, obj_align);
    // return rebind<A,T>::type(a).allocate(obj_size / sizeof( T ), p_hint);
    // return rebind<A,T>::type(a).allocate(obj_size / sizeof( T ));
    // return nullptr;
    return allocate_object_private::dispatch(a, obj_size, obj_align, p_hint);
}

namespace allocate_object_private
{

template<
    int, class A,
    class VP = typename alloc::traits<A>::void_pointer,
    class CVP = typename alloc::traits<A>::const_void_pointer
>
struct dispatch_result;

template< class A, class VP, class CVP >
struct dispatch_result<2,A,VP,CVP>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_allocate_object<
              A&, VP ( std::size_t, std::size_t, CVP )
          >::value,
          VP
      >
{ };

template< class A >
inline typename dispatch_result<2,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const p_hint,
    sake::dispatch_priority_tag<2>)
{ return a.allocate_object(obj_size, obj_align, p_hint); }

template< class A, class VP, class CVP >
struct dispatch_result<1,A,VP,CVP>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_allocate_object<
              A&, VP ( std::size_t, std::size_t )
          >::value,
          VP
      >
{ };

template< class A >
inline typename dispatch_result<1,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
    sake::dispatch_priority_tag<1>)
{ return a.allocate_object(obj_size, obj_align); }

template<
    std::size_t Align,
    class T = typename boost_ext::aligned_type< Align >::type
>
struct dispatch_obj_align;

template< class A >
inline typename alloc::traits<A>::void_pointer
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const p_hint,
    sake::dispatch_priority_tag<0>)
{
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, const_void_pointer );
    if(obj_align == 0)
        obj_align = boost_ext::max_align;
    else if(obj_align > 128)
        return nullptr;
    typedef void_pointer (*fun_ptr_type)( A&, std::size_t const, const_void_pointer const );
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
    return (*fun_ptrs[sake::intlog2(obj_align)])(a, obj_size, p_hint);
}

template< class A >
inline typename alloc::traits<A>::void_pointer
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::const_void_pointer const p_hint)
{
    return allocate_object_private::dispatch(a, obj_size, obj_align, p_hint,
        sake::dispatch_priority_tag<2>());
}

template< std::size_t Align, class T >
struct dispatch_obj_align
{
    template< class A >
    static typename boost::disable_if_c<
        boost::is_same< A, typename alloc::rebind<A,T>::type >::value,
        typename alloc::traits<A>::void_pointer
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::const_void_pointer const p_hint)
    {
        typedef typename alloc::rebind<A,T>::type rebind_type;
        BOOST_STATIC_ASSERT((boost::is_same<
            rebind_type, typename alloc::rebind< rebind_type, T >::type
        >::value));
        rebind_type a_(a);
        return apply(a_, obj_size, p_hint);
    };

    template< class A >
    static typename boost::enable_if_c<
        boost::is_same< A, typename alloc::rebind<A,T>::type >::value,
        typename alloc::traits<A>::void_pointer
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::const_void_pointer const p_hint)
    {
        SAKE_ASSERT_RELATION( obj_size % sizeof( T ), ==, sake::zero );
        return apply(a, obj_size, p_hint,
            sake::dispatch_priority_tag<2>());
    }

    template< class A >
    static typename boost::enable_if_c<
        alloc::is_callable_mem_fun_allocate<
            A&,
            typename alloc::traits<A>::void_pointer (
                std::size_t,
                typename alloc::traits<A>::const_void_pointer
            )
        >::value,
        typename alloc::traits<A>::void_pointer
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::const_void_pointer const p_hint,
        sake::dispatch_priority_tag<2>)
    {
        try { return a.allocate(obj_size / sizeof( T ), p_hint); }
        catch(std::bad_alloc&) { return nullptr; }
    }

    template< class A >
    static typename boost::enable_if_c<
        alloc::is_callable_mem_fun_allocate<
            A&, typename alloc::traits<A>::void_pointer ( std::size_t )
        >::value,
        typename alloc::traits<A>::void_pointer
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::dispatch_priority_tag<1>)
    {
        try { return a.allocate(obj_size / sizeof( T )); }
        catch(std::bad_alloc&) { return nullptr; }
    }

    template< class A >
    static typename alloc::traits<A>::void_pointer
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::dispatch_priority_tag<0>)
    { return nullptr; }
};

template< std::size_t Align >
struct dispatch_obj_align< Align, void >
{
    template< class A >
    static typename alloc::traits<A>::void_pointer
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/)
    { return nullptr; }
};

} // namespace allocate_object_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_OBJECT_HPP
