/*******************************************************************************
 * sake/core/memory/alloc/default_impl/allocate_array.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * allocate_array(A& a,
 *     std::size_t const obj_size, std::size_t const obj_align,
 *     alloc::traits<A>::size_type const limit_count,
 *     alloc::traits<A>::size_type const preferred_count,
 *     alloc::traits<A>::const_void_pointer const p_hint)
 *     -> sake::tuple<
 *            alloc::traits<A>::void_pointer,
 *            alloc::traits<A>::size_type
 *        >
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_ARRAY_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_ARRAY_HPP

#include <cstddef>

#include <new>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/aligned_type.hpp>
#include <sake/boost_ext/type_traits/max_align.hpp>

#include <sake/core/data_structures/tuple.hpp>
#include <sake/core/math/intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate_array.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace allocate_array_private
{

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const p_hint);

} // namespace allocate_array_private

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
allocate_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const p_hint)
{
    SAKE_ASSERT((alloc::check_align(obj_align, obj_size)));
    SAKE_ASSERT_RELATION_ANY(
        (( limit_count, <=, preferred_count ))
        (( preferred_count, ==, sake::zero ))
    );
    // return a.allocate_array(obj_size, obj_align, limit_count, preferred_count, p_hint);
    // return a.allocate_array(obj_size, obj_align, limit_count, preferred_count);
    // return rebind<A,T>::type(a).allocate([limit|preferred]_count * obj_size / sizeof( T ), p_hint);
    // return rebind<A,T>::type(a).allocate([limit|preferred]_count * obj_size / sizeof( T ));
    // return sake::tuple< void_pointer, size_type >(nullptr, sake::zero.as< size_type >());
    return allocate_array_private::dispatch(a, obj_size, obj_align, limit_count, preferred_count, p_hint);
}

namespace allocate_array_private
{

template<
    int, class A,
    class Z = typename alloc::traits<A>::size_type,
    class VP = typename alloc::traits<A>::void_pointer,
    class CVP = typename alloc::traits<A>::const_void_pointer,
    class R = sake::tuple<VP,Z>
>
struct dispatch_result;

template< class A, class Z, class VP, class CVP, class R >
struct dispatch_result<2,A,Z,VP,CVP,R>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_allocate_array<
              A&, R ( std::size_t, std::size_t, Z, Z, CVP )
          >::value,
          R
      >
{ };

template< class A >
inline typename dispatch_result<2,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const p_hint,
    sake::dispatch_priority_tag<2>)
{ return a.allocate_array(obj_size, obj_align, limit_count, preferred_count, p_hint); }

template< class A, class Z, class VP, class CVP, class R >
struct dispatch_result<1,A,Z,VP,CVP,R>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_allocate_array<
              A&, R ( std::size_t, std::size_t, Z, Z )
          >::value,
          R
      >
{ };

template< class A >
inline typename dispatch_result<1,A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
    sake::dispatch_priority_tag<1>)
{ return a.allocate_array(obj_size, obj_align, limit_count, preferred_count); }

template< class A, class Z, class VP, class CVP, class R >
struct dispatch_result<2,A,Z,VP,CVP,R>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_allocate_array<
              A&, R ( std::size_t, std::size_t, Z, CVP )
          >::value,
          R
      >
{ };

template<
    std::size_t Align,
    class T = typename boost_ext::aligned_type< Align >::type
>
struct dispatch_obj_align;

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const p_hint,
    sake::dispatch_priority_tag<0>)
{
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>, const_void_pointer );
    typedef sake::tuple< void_pointer, size_type > result_type;
    if(obj_align == 0)
        obj_align = boost_ext::max_align;
    else if(obj_align > 128)
        return nullptr;
    typedef result_type (*fun_ptr_type)
        ( A&, std::size_t const, size_type const, size_type const, const_void_pointer );
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
    return (*fun_ptrs[sake::intlog2(obj_align)])
        (a, obj_size, limit_count, preferred_count, p_hint);
}

template< class A >
inline sake::tuple<
    typename alloc::traits<A>::void_pointer,
    typename alloc::traits<A>::size_type
>
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename alloc::traits<A>::size_type const limit_count,
    typename alloc::traits<A>::size_type const preferred_count,
    typename alloc::traits<A>::const_void_pointer const p_hint)
{
    return allocate_array_private::dispatch(a, obj_size, obj_align, limit_count, preferred_count, p_hint,
        sake::dispatch_priority_tag<4>());
}

template< std::size_t Align, class T >
struct dispatch_obj_align
{
    template< class A >
    static typename boost::disable_if_c<
        boost::is_same< A, typename alloc::rebind<A,T>::type >::value,
        sake::tuple<
            typename alloc::traits<A>::void_pointer,
            typename alloc::traits<A>::size_type
        >
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::size_type const limit_count,
        typename alloc::traits<A>::size_type const preferred_count,
        typename alloc::traits<A>::const_void_pointer const p_hint)
    {
        typedef typename alloc::rebind<A,T>::type rebind_type;
        BOOST_STATIC_ASSERT((boost::is_same<
            rebind_type, typename alloc::rebind< rebind_type, T >::type
        >::value));
        rebind_type a_(a);
        return apply(a_, obj_size, limit_count, preferred_count, p_hint);
    };

    template< class A >
    static typename boost::enable_if_c<
        boost::is_same< A, typename alloc::rebind<A,T>::type >::value,
        sake::tuple<
            typename alloc::traits<A>::void_pointer,
            typename alloc::traits<A>::size_type
        >
    >::type
    apply(A& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::size_type const limit_count,
        typename alloc::traits<A>::size_type const preferred_count,
        typename alloc::traits<A>::const_void_pointer const p_hint)
    {
        SAKE_ASSERT_RELATION( sizeof( T ) % obj_size, ==, sake::zero );
        return apply(a, obj_size, limit_count, preferred_count, p_hint,
            sake::dispatch_priority_tag<2>());
    }

    template<
        int, class A,
        class Z = typename alloc::traits<A>::size_type,
        class VP = typename alloc::traits<A>::void_pointer,
        class CVP = typename alloc::traits<A>::const_void_pointer,
        class R = sake::tuple< VP, Z >
    >
    struct result;

    template< class A, class Z, class VP, class CVP, class R >
    struct result<2,A,Z,VP,CVP,R>
        : boost::enable_if_c<
              alloc::is_callable_mem_fun_allocate<
                  A&, VP ( Z, CVP )
              >::value,
              R
          >
    { };

    template< class A >
    static typename result<2,A>::type
    apply(rebind_type& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::size_type const limit_count,
        typename alloc::traits<A>::size_type const preferred_count,
        typename alloc::traits<A>::const_void_pointer const p_hint,
        sake::dispatch_priority_tag<2>)
    {
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
        typedef sake::tuple< void_pointer, size_type > result_type;
        if(preferred_count != sake::zero) {
            try {
                void_pointer const p = a.allocate(preferred_count * obj_size / sizeof( T ), p_hint);
                return result_type(p, preferred_count);
            }
            catch(std::bad_alloc&)
            { }
        }
        try {
            void_pointer const p = a.allocate(limit_count * obj_size / sizeof( T ), p_hint);
            return result_type(p, limit_count);
        }
        catch(std::bad_alloc&)
        { }
        return result_type(nullptr, sake::zero.as< size_type >());
    }

    template< class A, class Z, class VP, class CVP, class R >
    struct result<1,A,Z,VP,CVP,R>
        : boost::enable_if_c<
              alloc::is_callable_mem_fun_allocate<
                  A&, VP ( Z )
              >::value,
              R
          >
    { };

    template< class A >
    static typename result<1,A>::type
    apply(rebind_type& a,
        std::size_t const obj_size,
        typename alloc::traits<A>::size_type const limit_count,
        typename alloc::traits<A>::size_type const preferred_count,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::dispatch_priority_tag<1>)
    {
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
        typedef sake::tuple< void_pointer, size_type > result_type;
        if(preferred_count != sake::zero) {
            try {
                void_pointer const p = a.allocate(preferred_count * obj_size / sizeof( T ));
                return result_type(p, preferred_count);
            }
            catch(std::bad_alloc&)
            { }
        }
        try {
            void_pointer const p = a.allocate(limit_count * obj_size / sizeof( T ));
            return result_type(p, limit_count);
        }
        catch(std::bad_alloc&)
        { }
        return result_type(nullptr, sake::zero.as< size_type >());
    }

    template< class A >
    static sake::tuple<
        typename alloc::traits<A>::void_pointer,
        typename alloc::traits<A>::size_type
    >
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::size_type const /*limit_count*/,
        typename alloc::traits<A>::size_type const /*preferred_count*/,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::dispatch_priority_tag<0>)
    {
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
        typedef sake::tuple< void_pointer, size_type > result_type;
        return result_type(nullptr, sake::zero.as< size_type >());
    }
};

template< std::size_t Align >
struct dispatch_obj_align< Align, void >
{
    template< class A >
    static sake::tuple<
        typename alloc::traits<A>::void_pointer,
        typename alloc::traits<A>::size_type
    >
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename alloc::traits<A>::size_type const /*limit_count*/,
        typename alloc::traits<A>::size_type const /*preferred_count*/,
        typename alloc::traits<A>::const_void_pointer const /*p_hint*/)
    {
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, size_type );
        SAKE_USING_TYPEDEF( typename alloc::traits<A>, void_pointer );
        typedef sake::tuple< void_pointer, size_type > result_type;
        return result_type(nullptr, sake::zero.as< size_type >());
    }
};

} // namespace allocate_array_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_ARRAY_HPP
