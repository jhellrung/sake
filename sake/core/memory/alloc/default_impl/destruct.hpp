/*******************************************************************************
 * sake/core/memory/alloc/default_impl/destruct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * alloc::default_impl::destruct(A& a,
 *     P const p_obj)
 *     -> void
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DESTRUCT_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DESTRUCT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/memory/alloc/is_callable_mem_fun_destroy.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_destruct.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/pointee.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace destruct_private
{

template< class A, class P >
inline void
impl(A& a,
    P const p_obj);

} // namespace destruct_private

template< class A, class P >
inline void
destruct(A& a,
    P const p_obj)
{
    typedef sake::alloc::traits<A> traits_;
    typedef typename sake::pointee<P>::type value_type;
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< value_type >::value));
    SAKE_USING_TYPEDEF( typename traits_::template value_traits< value_type >, pointer );
    BOOST_STATIC_ASSERT((boost::is_same< P, pointer >::value));
    SAKE_ASSERT((p_obj));
    // a.destruct(p_obj);
    // a.destroy(p_obj);
    // rebind<A,T>::type(a).destroy(p_obj);
    // p_obj->~T();
    destruct_private::impl(a, p_obj);
}

namespace destruct_private
{

template< class A, class P >
inline void
dispatch(A& a,
    P const p_obj,
    sake::int_tag<3>)
{ a.destruct(p_obj); }

template< class A, class P >
inline void
dispatch(A& a,
    P const p_obj,
    sake::int_tag<2>)
{ a.destroy(p_obj); }

template< class A, class P >
inline void
dispatch(A& a,
    P const p_obj,
    sake::int_tag<1>)
{
    typedef typename sake::pointee<P>::type value_type;
    typedef typename sake::alloc::rebind< A, value_type >::type rebind_type;
    BOOST_STATIC_ASSERT((boost::is_same<
        rebind_type,
        typename sake::alloc::rebind< rebind_type, value_type >::type
    >));
    rebind_type a_(a);
    a_.destroy(p_obj);
}

template< class A, class P >
inline void
dispatch(A& /*a*/,
    P const p_obj,
    sake::int_tag<0>)
{
    typedef typename sake::pointee<P>::type value_type;
    p_obj->~value_type();
}

template< class A, class P >
inline void
impl(A& a,
    P const p_obj)
{
    typedef typename sake::pointee<P>::type value_type;
    typedef typename sake::alloc::rebind< A, value_type >::type rebind_type;
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_destruct<
            A&, void ( P ) >,
        sake::int_tag<3>
    >::type::template
    else_if <
        boost_ext::mpl::and2<
            boost::is_same< A, rebind_type >,
            sake::alloc::is_callable_mem_fun_destroy<
                A&, void ( P ) >
        >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_destroy<
            typename boost_ext::add_reference< rebind_type >::type,
            void ( P )
        >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    destruct_private::dispatch(
        a, p_obj,
        int_tag_()
    );
}

} // namespace destruct_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DESTRUCT_HPP
