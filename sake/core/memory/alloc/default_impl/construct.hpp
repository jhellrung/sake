/*******************************************************************************
 * sake/core/memory/alloc/default_impl/construct.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * alloc::default_impl::construct(A& a,
 *     alloc::traits<A>::void_pointer const p_obj,
 *     Emplacer const & e)
 *     -> void
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_CONSTRUCT_HPP
#define SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_CONSTRUCT_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/memory/alloc/is_callable_mem_fun_construct.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/emplacer/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace alloc
{

namespace default_impl
{

namespace construct_private
{

template< class A, class Emplacer >
inline void
dispatch(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e);

} // namespace construct_private

template< class A, class Emplacer >
inline void
construct(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e)
{
    BOOST_STATIC_ASSERT((sake::is_emplacer<
        Emplacer, boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
    >::value));
    SAKE_ASSERT((p_obj));
    // a.construct(p_obj, e);
    // rebind<A,T>::type(a).construct(p_obj, e.construct());
    // e.construct(p_obj);
    construct_private::impl< A, Emplacer >::apply(a, p_obj, e);
}

namespace construct_private
{

template< class A, class Emplacer >
inline typename boost::enable_if_c<
    alloc::is_callable_mem_fun_construct<
        A&, void ( typename alloc::traits<A>::void_pointer, Emplacer )
    >::value
>::type
dispatch(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::dispatch_priority_tag<3>)
{ a.construct(p_obj, e); }

template<
    int, class A, class Emplacer,
    class T = typename Emplacer::value_type,
    class B = typename alloc::rebind<A,T>::type,
    class P = typename alloc::traits<A>::template value_traits<T>::pointer
>
struct dispatch_result;

template< class A, class Emplacer, class T, class B, class P >
struct dispatch_result<2,A,Emplacer,T,B,P>
    : boost::enable_if_c< boost_ext::mpl::and2<
          boost::is_same<A,B>,
          alloc::is_callable_mem_fun_construct<
              A&, void ( P, T )
          >
      >::value >
{ };

template< class A, class Emplacer >
inline dispatch_result< 2, A, Emplacer >::type
dispatch(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::dispatch_priority_tag<2>)
{
    SAKE_USING_TYPEDEF( typename Emplacer, value_type );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>::template value_traits< value_type >, pointer );
    a.construct(static_cast< pointer >(p_obj), e.construct());
}

template< class A, class Emplacer, class T, class B, class P >
struct dispatch_result<1,A,Emplacer,T,B,P>
    : boost::enable_if_c<
          alloc::is_callable_mem_fun_construct<
              typename boost_ext::add_reference<B>::type,
              void ( P, T )
          >::value
      >
{ };

template< class A, class Emplacer >
inline dispatch_result< 1, A, Emplacer >::type
dispatch(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::dispatch_priority_tag<1>)
{
    SAKE_USING_TYPEDEF( typename Emplacer, value_type );
    SAKE_USING_TYPEDEF( typename alloc::traits<A>::template value_traits< value_type >, pointer );
    typedef typename alloc::rebind< A, value_type >::type rebind_type;
    BOOST_STATIC_ASSERT((boost::is_same<
        rebind_type, typename alloc::rebind< rebind_type, value_type >::type
    >));
    rebind_type a_(a);
    a_.construct(static_cast< pointer >(p_obj), e.construct());
}

template< class A, class Emplacer >
inline void
dispatch(A& /*a*/,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::dispatch_priority_tag<0>)
{ e.construct(static_cast< void* >(p_obj)); }

template< class A, class Emplacer >
inline void
dispatch(A& a,
    typename alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e)
{
    construct_private::dispatch(a, p_obj, e,
        sake::dispatch_priority_tag<3>());
}

} // namespace construct_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_CONSTRUCT_HPP
