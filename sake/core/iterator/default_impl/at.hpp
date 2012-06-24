/*******************************************************************************
 * sake/core/iterator/default_impl/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_HPP
#define SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/is_interoperable.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake_iterator_at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_at
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 3 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I, class T, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl3(I const & i, T const & x, Introversal)
{ return iterator_at(i, x, Introversal()); }

template< class I, class T, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl2(I const & i, T const & x, Introversal)
{ return iterator_at(i,x); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_begin
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_begin
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl_begin2(I const & i, Introversal)
{ return iterator_begin(i, Introversal()); }

template< class I, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl_begin1(I const & i, Introversal)
{ return iterator_begin(i); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_end
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_end
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl_end2(I const & i, Introversal)
{ return iterator_end(i, Introversal()); }

template< class I, class Introversal >
inline typename ::sake::iterator_relax< I, Introversal >::type
adl_end1(I const & i, Introversal)
{ return iterator_end(i); }

} // namespace sake_iterator_at_private

namespace sake
{

namespace iterator
{

namespace default_impl
{

namespace at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME at
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & /*i*/, T const & x, Introversal, sake::int_tag<5>)
{ return x; }

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, T const & x, Introversal, sake::int_tag<4>)
{ return i.at(x, Introversal()); }

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, T const & x, Introversal, sake::int_tag<3>)
{ return ::sake_iterator_at_private::adl3(i, x, Introversal()); }

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, T const & x, Introversal, sake::int_tag<2>)
{ return i.at(x); }

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, T const & x, Introversal, sake::int_tag<1>)
{ return ::sake_iterator_at_private::adl2(i, x, Introversal()); }

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I i, T const & x, Introversal, sake::int_tag<0>)
{ return sake::iterator_traits<I>::at_ip(i,x); }

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_begin
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME begin
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_begin(I const & i, Introversal, sake::int_tag<4>)
{ return i.begin(Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_begin(I const & i, Introversal, sake::int_tag<3>)
{ return ::sake_iterator_at_private::adl_begin2(i, Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_begin(I const & i, Introversal, sake::int_tag<2>)
{ return i.begin(); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_begin(I const & i, Introversal, sake::int_tag<1>)
{ return ::sake_iterator_at_private::adl_begin1(i, Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_begin(I i, Introversal, sake::int_tag<0>)
{ return sake::iterator_traits<I>::at_ip(i, sake::_begin); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, sake::begin_tag, Introversal, sake::int_tag<0>)
{
    typedef typename sake::iterator_relax< I, Introversal >::type result_type;
    typedef typename boost_ext::mpl::
         if_< at_private::is_callable_mem_fun_begin<
                  I const &, result_type ( Introversal ) >,
              sake::int_tag<4> >::type::template
    else_if < ::sake_iterator_at_private::is_callable_begin<
                  result_type ( I const &, Introversal ) >,
              sake::int_tag<3> >::type::template
    else_if < at_private::is_callable_mem_fun_begin<
                  I const &, result_type ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_private::is_callable_begin<
                  result_type ( I const & ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return at_private::dispatch_begin(i, Introversal(), int_tag_());
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_end(I const & i, Introversal, sake::int_tag<4>)
{ return i.end(Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_end(I const & i, Introversal, sake::int_tag<3>)
{ return ::sake_iterator_at_private::adl_end2(i, Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_end(I const & i, Introversal, sake::int_tag<2>)
{ return i.end(); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_end(I const & i, Introversal, sake::int_tag<1>)
{ return ::sake_iterator_at_private::adl_end1(i, Introversal()); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch_end(I i, Introversal, sake::int_tag<0>)
{ return sake::iterator_traits<I>::at_ip(i, sake::_end); }

template< class I, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
dispatch(I const & i, sake::end_tag, Introversal, sake::int_tag<0>)
{
    typedef typename sake::iterator_relax< I, Introversal >::type result_type;
    typedef typename boost_ext::mpl::
         if_< at_private::is_callable_mem_fun_end<
                  I const &, result_type ( Introversal ) >,
              sake::int_tag<4> >::type::template
    else_if < ::sake_iterator_at_private::is_callable_end<
                  result_type ( I const &, Introversal ) >,
              sake::int_tag<3> >::type::template
    else_if < at_private::is_callable_mem_fun_end<
                  I const &, result_type ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_private::is_callable_end<
                  result_type ( I const & ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return at_private::dispatch_end(i, Introversal(), int_tag_());
}

} // namespace at_private

template< class I, class T, class Introversal >
inline typename sake::iterator_relax< I, Introversal >::type
at(I const & i, T const & x, Introversal)
{
    typedef typename sake::iterator_introversal<I>::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
        boost_ext::mpl::and2<
            boost::is_same< T, sake::begin_tag >,
            boost_ext::is_convertible<
                introversal, sake::begin_access_introversal_tag >
        >,
        boost_ext::mpl::and2<
            boost::is_same< T, sake::end_tag >,
            boost_ext::is_convertible<
                introversal, sake::end_access_introversal_tag >
        >,
        sake::iterator::private_::is_interoperable<I,T>
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Introversal, sake::null_introversal_tag >::value));
    typedef typename sake::iterator_relax< I, Introversal >::type result_type;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible< I, result_type >::value));
    typedef typename boost_ext::mpl::
         if_< boost_ext::is_convertible< T, result_type >,
              sake::int_tag<5> >::type::template
    else_if < at_private::is_callable_mem_fun<
                  I const &, result_type ( T, Introversal ) >,
              sake::int_tag<4> >::type::template
    else_if < ::sake_iterator_at_private::is_callable<
                  result_type ( I const &, T, Introversal ) >,
              sake::int_tag<3> >::type::template
    else_if < at_private::is_callable_mem_fun<
                  I const &, result_type ( T ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_private::is_callable<
                  result_type ( I const &, T ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return at_private::dispatch(i, x, Introversal(), int_tag_());
}

} // namespace default_impl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_HPP
