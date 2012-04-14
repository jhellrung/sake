/*******************************************************************************
 * sake/core/memory/alloc/default_impl/destruct.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
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
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/memory/alloc/is_callable_mem_fun_destroy.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_destruct.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
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
dispatch(A& a,
    P const p_obj);

} // namespace destruct_private

template< class A, class P >
inline void
destruct(A& a,
    P const p_obj)
{
    typedef typename sake::pointee<P>::type value_type;
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< value_type >::value));
    SAKE_USING_TYPEDEF( typename alloc::traits<A>::template value_traits< value_type >, pointer );
    BOOST_STATIC_ASSERT((boost::is_same< P, pointer >::value));
    SAKE_ASSERT((p_obj));
    // a.destruct(p_obj);
    // rebind<A,T>::type(a).destroy(p_obj);
    // p_obj->~T();
    destruct_private::dispatch<A,P>::apply(a, p_obj);
}

namespace destruct_private
{

template< class A, class P >
inline typename boost::enable_if_c<
    alloc::is_callable_mem_fun_destruct<
        A&, void ( P )
    >::value
>::type
dispatch(A& a,
    P const p_obj,
    sake::dispatch_priority_tag<3>)
{ a.destruct(p_obj); }

template<
    int, class A, class P,
    class T = typename sake::pointee<P>::type,
    class B = typename alloc::rebind<A,T>::type
>
struct dispatch_result;

template< class A, class P, class T, class B >
struct dispatch_result<2,A,P,T,B>
    : boost::enable_if_c< boost_ext::mpl::and2<
          boost::is_same<A,B>,
          alloc::is_callable_mem_fun_destroy<
              A&, void ( P )
          >
      >::value >
{ };

template< class A, class P >
inline typename dispatch_result<2,A,P>::type
dispatch(A& a,
    P const p_obj,
    sake::dispatch_priority_tag<2>)
{ a.destroy(p_obj); }

template< class A, class P, class T, class B >
struct dispatch_result<1,A,P,T,B>
    : boost::enable_if_c<
        alloc::is_callable_mem_fun_destroy<
            typename boost_ext::add_reference<B>::type,
            void ( P )
        >::value
    >
{ };

template< class A, class P >
inline typename dispatch_result<1,A,P>::type
dispatch(A& a,
    P const p_obj,
    sake::dispatch_priority_tag<1>)
{
    typedef typename sake::pointee<P>::type value_type;
    typedef typename alloc::rebind< A, value_type >::type rebind_type;
    BOOST_STATIC_ASSERT((boost::is_same<
        rebind_type, typename alloc::rebind< rebind_type, value_type >::type
    >));
    rebind_type a_(a);
    a_.destroy(p_obj);
}

template< class A, class P >
inline void
dispatch(A& /*a*/,
    P const p_obj,
    sake::dispatch_priority_tag<0>)
{
    typedef typename sake::pointee<P>::type value_type;
    p_obj->~value_type();
}

template< class A, class P >
inline void
dispatch(A& a,
    P const p_obj)
{
    destruct_private::dispatch(a, p_obj,
        sake::dispatch_priority_tag<3>());
}

} // namespace destruct_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_DESTRUCT_HPP
