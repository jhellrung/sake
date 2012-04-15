/*******************************************************************************
 * sake/core/memory/alloc/default_impl/allocate_array.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
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

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/aligned_type.hpp>
#include <sake/boost_ext/type_traits/max_align.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_allocate_array.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/nullptr.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace result_of
{

template< class A >
class allocate_array
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
public:
    typedef sake::tuple< void_pointer, size_type > type;
};

} // namespace result_of

namespace allocate_array_private
{

template< class A >
inline typename sake::alloc::default_impl::allocate_array<A>::type
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint);

} // namespace allocate_array_private

template< class A >
inline typename sake::alloc::default_impl::allocate_array<A>::type
allocate_array(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint)
{
    SAKE_ASSERT((sake::alloc::check_align(obj_align, obj_size)));
    SAKE_ASSERT_RELATION_ANY(
        (( limit_count, <=, preferred_count ))
        (( preferred_count, ==, sake::zero ))
    );
    // return a.allocate_array(obj_size, obj_align, limit_count, preferred_count, p_hint);
    // return a.allocate_array(obj_size, obj_align, limit_count, preferred_count);
    // return a.allocate_array(obj_size, obj_align, limit_count, p_hint);
    // return a.allocate_array(obj_size, obj_align, limit_count);
    // return rebind<A,T>::type(a).allocate([limit|preferred]_count * obj_size / sizeof( T ), p_hint);
    // return rebind<A,T>::type(a).allocate([limit|preferred]_count * obj_size / sizeof( T ));
    // return result_type(nullptr, sake::zero.as< size_type >());
    return allocate_array_private::impl(
        a, obj_size, obj_align, limit_count, preferred_count, p_hint);
}

namespace allocate_array_private
{

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint,
    sake::int_tag<4>)
{ return a.allocate_array(obj_size, obj_align, limit_count, preferred_count, p_hint); }

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const /*p_hint*/,
    sake::int_tag<3>)
{ return a.allocate_array(obj_size, obj_align, limit_count, preferred_count); }

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const /*preferred_count*/,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint,
    sake::int_tag<2>)
{ return a.allocate_array(obj_size, obj_align, limit_count, p_hint); }

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const /*preferred_count*/,
    typename sake::alloc::traits<A>::const_void_pointer const /*p_hint*/,
    sake::int_tag<1>)
{ return a.allocate_array(obj_size, obj_align, limit_count); }

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint,
    sake::int_tag<0>);

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
impl(A& a,
    std::size_t const obj_size, std::size_t const obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, const_void_pointer );
    typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_allocate_array<
            A&, result_type
            ( std::size_t, std::size_t, size_type, size_type, const_void_pointer )
        >,
        sake::int_tag<4>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_allocate_array<
            A&, result_type
            ( std::size_t, std::size_t, size_type, size_type )
        >,
        sake::int_tag<3>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_allocate_array<
            A&, result_type
            ( std::size_t, std::size_t, size_type, const_void_pointer )
        >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_allocate_array<
            A&, result_type
            ( std::size_t, std::size_t, size_type )
        >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return allocate_array_private::dispatch(
        a, obj_size, obj_align, limit_count, preferred_count, p_hint,
        int_tag_()
    );
}

template<
    std::size_t Align,
    class Aligned = typename boost_ext::aligned_type< Align >::type
>
struct dispatch_obj_align;

template< class A >
inline typename sake::alloc::default_impl::result_of::allocate_array<A>::type
dispatch(A& a,
    std::size_t const obj_size, std::size_t obj_align,
    typename sake::alloc::traits<A>::size_type const limit_count,
    typename sake::alloc::traits<A>::size_type const preferred_count,
    typename sake::alloc::traits<A>::const_void_pointer const p_hint,
    sake::int_tag<0>)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, const_void_pointer );
    typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
    if(obj_align == 0)
        obj_align = boost_ext::max_align;
    else if(obj_align > 128)
        return nullptr;
    typedef result_type (*fun_ptr_type)( A&,
        std::size_t,
        size_type, size_type,
        const_void_pointer );
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
    return (*fun_ptrs[sake::intlog2(obj_align)])(
        a, obj_size, limit_count, preferred_count, p_hint);
}

template< std::size_t Align, class Aligned >
struct dispatch_obj_align
{
    template< class A >
    static typename boost::lazy_disable_if_c<
        boost::is_same< A, typename sake::alloc::rebind< A, Aligned >::type >::value,
        sake::alloc::default_impl::result_of::allocate_array<A>
    >::type
    apply(A& a,
        std::size_t obj_size,
        typename sake::alloc::traits<A>::size_type limit_count,
        typename sake::alloc::traits<A>::size_type preferred_count,
        typename sake::alloc::traits<A>::const_void_pointer p_hint)
    {
        typedef typename sake::alloc::rebind< A, Aligned >::type rebind_type;
        BOOST_STATIC_ASSERT((boost::is_same<
            rebind_type,
            typename sake::alloc::rebind< rebind_type, Aligned >::type
        >::value));
        rebind_type a_(a);
        return apply(a_, obj_size, limit_count, preferred_count, p_hint);
    };

    template< class A >
    static typename boost::lazy_enable_if_c<
        boost::is_same< A, typename sake::alloc::rebind< A, Aligned >::type >::value,
        sake::alloc::default_impl::result_of::allocate_array<A>
    >::type
    apply(A& a,
        std::size_t obj_size,
        typename sake::alloc::traits<A>::size_type limit_count,
        typename sake::alloc::traits<A>::size_type preferred_count,
        typename sake::alloc::traits<A>::const_void_pointer p_hint)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_, size_type );
        SAKE_USING_TYPEDEF( typename traits_, void_pointer );
        SAKE_USING_TYPEDEF( typename traits_, const_void_pointer );
        typedef typename boost_ext::mpl::
             if_<
            sake::alloc::is_callable_mem_fun_allocate<
                A&, void_pointer ( size_type, const_void_pointer ) >,
            sake::int_tag<2>
        >::type::template
        else_if <
            sake::alloc::is_callable_mem_fun_allocate<
                A&, void_pointer ( size_type ) >,
            sake::int_tag<1>
        >::type::template
        else_   <
            sake::int_tag<0>
        >::type int_tag_;
        SAKE_ASSERT_RELATION( sizeof( Aligned ) % obj_size, ==, sake::zero );
        return apply(
            a, obj_size, limit_count, preferred_count, p_hint,
            int_tag_()
        );
    }

    template< class A >
    static typename sake::alloc::default_impl::result_of::allocate_array<A>::type
    apply(A& a,
        std::size_t const obj_size,
        typename sake::alloc::traits<A>::size_type const limit_count,
        typename sake::alloc::traits<A>::size_type const preferred_count,
        typename sake::alloc::traits<A>::const_void_pointer const p_hint,
        sake::int_tag<2>)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_, size_type );
        SAKE_USING_TYPEDEF( typename traits_, void_pointer );
        typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
        if(preferred_count != sake::zero) {
            try {
                void_pointer const p = a.allocate(
                    preferred_count * obj_size / sizeof( Aligned ), p_hint);
                return result_type(p, preferred_count);
            }
            catch(std::bad_alloc&)
            { }
        }
        try {
            void_pointer const p = a.allocate(
                limit_count * obj_size / sizeof( Aligned ), p_hint);
            return result_type(p, limit_count);
        }
        catch(std::bad_alloc&)
        { }
        return result_type(nullptr, sake::zero.as< size_type >());
    }

    template< class A >
    static typename sake::alloc::default_impl::result_of::allocate_array<A>::type
    apply(A& a,
        std::size_t const obj_size,
        typename sake::alloc::traits<A>::size_type const limit_count,
        typename sake::alloc::traits<A>::size_type const preferred_count,
        typename sake::alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::int_tag<1>)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_, size_type );
        SAKE_USING_TYPEDEF( typename traits_, void_pointer );
        typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
        if(preferred_count != sake::zero) {
            try {
                void_pointer const p = a.allocate(
                    preferred_count * obj_size / sizeof( Aligned ));
                return result_type(p, preferred_count);
            }
            catch(std::bad_alloc&)
            { }
        }
        try {
            void_pointer const p = a.allocate(
                limit_count * obj_size / sizeof( Aligned ));
            return result_type(p, limit_count);
        }
        catch(std::bad_alloc&)
        { }
        return result_type(nullptr, sake::zero.as< size_type >());
    }

    template< class A >
    static typename sake::alloc::default_impl::result_of::allocate_array<A>::type
    apply(A& /*a*/,
        std::size_t const /*obj_size*/,
        typename sake::alloc::traits<A>::size_type const /*limit_count*/,
        typename sake::alloc::traits<A>::size_type const /*preferred_count*/,
        typename sake::alloc::traits<A>::const_void_pointer const /*p_hint*/,
        sake::int_tag<0>)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_, size_type );
        typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
        return result_type(nullptr, sake::zero.as< size_type >());
    }
};

template< std::size_t Align >
struct dispatch_obj_align< Align, void >
{
    template< class A >
    static typename sake::alloc::default_impl::result_of::allocate_array<A>::type
    apply(A& /*a*/,
        std::size_t /*obj_size*/,
        typename sake::alloc::traits<A>::size_type /*limit_count*/,
        typename sake::alloc::traits<A>::size_type /*preferred_count*/,
        typename sake::alloc::traits<A>::const_void_pointer /*p_hint*/)
    {
        typedef sake::alloc::traits<A> traits_;
        SAKE_USING_TYPEDEF( typename traits_, size_type );
        typedef typename sake::alloc::default_impl::result_of::allocate_array<A>::type result_type;
        return result_type(nullptr, sake::zero.as< size_type >());
    }
};

} // namespace allocate_array_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_ALLOCATE_ARRAY_HPP
