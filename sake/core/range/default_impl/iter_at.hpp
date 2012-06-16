/*******************************************************************************
 * sake/core/range/default_impl/iter_at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITER_AT_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_ITER_AT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake_range_iter_at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_iter_at
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 3, 3 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R, class T, class Introversal >
inline typename ::sake::range_traits<R>::template
    iterator_with< Introversal >::type
adl(R& r, T const & x, Introversal)
{ return range_iter_at(r, x, Introversal()); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_begin
#define SAKE_INTROSPECTION_FUNCTION_NAME range_begin
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R, class Introversal >
inline typename ::sake::range_traits<R>::template
    iterator_with< Introversal >::type
adl_begin(R& r, Introversal)
{ return range_begin(r, Introversal()); }

template< class R >
inline typename ::sake::range_traits<R>::iterator
adl_begin(R& r)
{ return range_begin(r); }

template< class R >
inline typename ::sake::range_traits<R>::iterator
std_adl_begin(R& r)
{ return begin(r); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_end
#define SAKE_INTROSPECTION_FUNCTION_NAME range_end
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R, class Introversal >
inline typename ::sake::range_traits<R>::template
    iterator_with< Introversal >::type
adl_end(R& r, Introversal)
{ return range_end(r, Introversal()); }

template< class R >
inline typename ::sake::range_traits<R>::iterator
adl_end(R& r)
{ return range_end(r); }

template< class R >
inline typename ::sake::range_traits<R>::iterator
std_adl_end(R& r)
{ return end(r); }

} // namespace sake_range_iter_at_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace iter_at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME iter_at
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R, class T, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& /*r*/, T const & x, Introversal, sake::int_tag<3>)
{ return x; }

template< class R, class T, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& r, T const & x, Introversal, sake::int_tag<2>)
{ return r.iter_at(x, Introversal()); }

template< class R, class T, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& r, T const & x, Introversal, sake::int_tag<1>)
{ return ::sake_range_iter_at_private::adl(r, x, Introversal()); }

template< class R, class T, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& r, T const & x, Introversal, sake::int_tag<0>)
{
    return typename sake::range_traits<R>::template
        iterator_with< Introversal >::type(r,x);
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_begin
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME begin
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<5>)
{ return r.begin(Introversal()); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<4>)
{ return ::sake_range_iter_at_private::adl_begin(r, Introversal()); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<3>)
{ return r.begin(); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<2>)
{ return ::sake_range_iter_at_private::adl_begin(r); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<1>)
{ return ::sake_range_iter_at_private::std_adl_begin(r); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_begin(R& r, Introversal, sake::int_tag<0>)
{
    return typename sake::range_traits<R>::template
        iterator_with< Introversal >::type(r, sake::_begin);
}

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& r, sake::begin_tag, Introversal, sake::int_tag<0>)
{
    typedef typename sake::range_traits<R>::template
        iterator_with< Introversal >::type result_type;
    typedef typename boost_ext::mpl::
         if_< iter_at_private::is_callable_mem_fun_begin<
                  R&, result_type ( Introversal ) >,
              sake::int_tag<5> >::type::template
    else_if < ::sake_range_iter_at_private::is_callable_begin<
                  result_type ( R&, Introversal ) >,
              sake::int_tag<4> >::type::template
    else_if < iter_at_private::is_callable_mem_fun_begin<
                  R&, result_type ( ) >,
              sake::int_tag<3> >::type::template
    else_if < ::sake_range_iter_at_private::is_callable_begin<
                  result_type ( R& ) >,
              sake::int_tag<2> >::type::template
    else_if < boost_ext::is_convertible<
                  typename sake::range_traits<R>::iterator, result_type >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_begin(r, Introversal(), int_tag_());
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<5>)
{ return r.end(Introversal()); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<4>)
{ return ::sake_range_iter_at_private::adl_end(r, Introversal()); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<3>)
{ return r.end(); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<2>)
{ return ::sake_range_iter_at_private::adl_end(r); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<1>)
{ return ::sake_range_iter_at_private::std_adl_end(r); }

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch_end(R& r, Introversal, sake::int_tag<0>)
{
    return typename sake::range_traits<R>::template
        iterator_with< Introversal >::type(r, sake::_end);
}

template< class R, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
dispatch(R& r, sake::end_tag, Introversal, sake::int_tag<0>)
{
    typedef typename sake::range_traits<R>::template
        iterator_with< Introversal >::type result_type;
    typedef typename boost_ext::mpl::
         if_< iter_at_private::is_callable_mem_fun_end<
                  R&, result_type ( Introversal ) >,
              sake::int_tag<5> >::type::template
    else_if < ::sake_range_iter_at_private::is_callable_end<
                  result_type ( R&, Introversal ) >,
              sake::int_tag<4> >::type::template
    else_if < iter_at_private::is_callable_mem_fun_end<
                  R&, result_type ( ) >,
              sake::int_tag<3> >::type::template
    else_if < ::sake_range_iter_at_private::is_callable_end<
                  result_type ( R& ) >,
              sake::int_tag<2> >::type::template
    else_if < boost_ext::is_convertible<
                  typename sake::range_traits<R>::iterator, result_type >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_end(r, Introversal(), int_tag_());
}

} // namespace iter_at_private

template< class R, class T, class Introversal >
inline typename sake::range_traits<R>::template
    iterator_with< Introversal >::type
iter_at(R& r, T const & x, Introversal)
{
    typedef sake::range_traits<R> traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator );
    BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
        boost::is_same< T, sake::begin_tag >,
        boost::is_same< T, sake::end_tag >,
        boost_ext::is_convertible< T, iterator >
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Introversal, sake::null_introversal_tag >::value));
    typedef typename traits_::template
        iterator_with< Introversal >::type result_type;
    typedef typename boost_ext::mpl::
         if_< boost_ext::is_convertible< T, result_type >,
              sake::int_tag<3> >::type::template
    else_if < iter_at_private::is_callable_mem_fun<
                  R&, result_type ( T, Introversal ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_iter_at_private::is_callable<
                  result_type ( R&, T, Introversal ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return iter_at_private::dispatch(r, x, Introversal(), int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITER_AT_HPP
