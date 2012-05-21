/*******************************************************************************
 * sake/core/memory/alloc/default_impl/construct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
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

#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_void.hpp>

#include <sake/core/emplacer/traits.hpp>
#include <sake/core/memory/alloc/is_callable_mem_fun_construct.hpp>
#include <sake/core/memory/alloc/rebind.hpp>
#include <sake/core/memory/alloc/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/int_tag.hpp>
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
impl(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e);

} // namespace construct_private

template< class A, class Emplacer >
inline void
construct(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e)
{
    BOOST_STATIC_ASSERT((sake::is_emplacer<
        Emplacer, boost::mpl::quote1< boost_ext::not_is_void >
    >::value));
    SAKE_ASSERT((p_obj));
    // a.construct(p_obj, e);
    // a.construct(p_obj, e.construct());
    // rebind<A,T>::type(a).construct(p_obj, e.construct());
    // e.construct(p_obj);
    construct_private::impl< A, Emplacer >::apply(a, p_obj, e);
}

namespace construct_private
{

template< class A, class Emplacer >
inline void
dispatch(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::int_tag<3>)
{ a.construct(p_obj, e); }

template< class A, class Emplacer >
inline void
dispatch(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::int_tag<2>)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename Emplacer, value_type );
    SAKE_USING_TYPEDEF( typename traits_::template value_traits< value_type >, pointer );
    a.construct(static_cast< pointer >(p_obj), e.construct());
}

template< class A, class Emplacer >
inline void
dispatch(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::int_tag<1>)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename Emplacer, value_type );
    SAKE_USING_TYPEDEF( typename traits_::template value_traits< value_type >, pointer );
    typedef typename sake::alloc::rebind< A, value_type >::type rebind_type;
    BOOST_STATIC_ASSERT((boost::is_same<
        rebind_type,
        typename sake::alloc::rebind< rebind_type, value_type >::type
    >));
    rebind_type a_(a);
    a_.construct(static_cast< pointer >(p_obj), e.construct());
}

template< class A, class Emplacer >
inline void
dispatch(A& /*a*/,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e,
    sake::int_tag<0>)
{ e.construct(static_cast< void* >(p_obj)); }

template< class A, class Emplacer >
inline void
impl(A& a,
    typename sake::alloc::traits<A>::void_pointer const p_obj,
    Emplacer const & e)
{
    typedef sake::alloc::traits<A> traits_;
    SAKE_USING_TYPEDEF( typename traits_, void_pointer );
    SAKE_USING_TYPEDEF( typename Emplacer, value_type );
    SAKE_USING_TYPEDEF( typename traits_::template value_traits< value_type >, pointer );
    typedef typename sake::alloc::rebind< A, value_type >::type rebind_type;
    typedef typename boost_ext::mpl::
         if_<
        sake::alloc::is_callable_mem_fun_construct<
            A&, void ( void_pointer, Emplacer ) >,
        sake::int_tag<3>
    >::type::template
    else_if <
        boost_ext::mpl::and2<
            boost::is_same< A, rebind_type >,
            sake::alloc::is_callable_mem_fun_construct<
                A&, void ( pointer, value_type ) >
        >,
        sake::int_tag<2>
    >::type::template
    else_if <
        sake::alloc::is_callable_mem_fun_construct<
            typename boost_ext::add_reference< rebind_type >::type,
            void ( pointer, value_type )
        >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    construct_private::dispatch(
        a, p_obj, e,
        int_tag_()
    );
}

} // namespace construct_private

} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_DEFAULT_IMPL_CONSTRUCT_HPP
