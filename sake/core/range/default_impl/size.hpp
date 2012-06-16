/*******************************************************************************
 * sake/core/range/default_impl/size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_SIZE_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_SIZE_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake_range_size_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_calculate_size
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline typename ::sake::range_traits<R>::size_type
adl(R const & r)
{ return range_calculate_size(r); }

} // namespace sake_range_size_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace size_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME size
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline typename sake::range_traits<R>::size_type
dispatch(R const & r, sake::int_tag<2>)
{ return r.size(); }

template< class R >
inline typename sake::range_traits<R>::size_type
dispatch(R const & r, sake::int_tag<1>)
{ return ::sake_range_size_private::adl(r); }

template< class R >
inline typename sake::range_traits<R>::size_type
dispatch(R const & r, sake::int_tag<0>)
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::range_traits<R>::traversal,
        boost::random_access_traversal_tag
    >::value));
    typedef sake::range_traits<R> traits_;
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    return static_cast< size_type >(traits_::end(r) - traits_::begin(r));
}

} // namespace size_private

template< class R >
inline typename sake::range_traits<R>::size_type
size(R const & r)
{
    SAKE_USING_TYPEDEF( typename sake::range_traits<R>, size_type );
    typedef typename boost_ext::mpl::
    if_<
        size_private::is_callable_mem_fun< R const &, size_type ( ) >,
        sake::int_tag<2>
    >::type::template
    else_if<
        ::sake_range_size_private::is_callable< size_type ( R const & ) >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type int_tag_;
    return size_private::dispatch(r, int_tag_());
}

template< class R, class Size >
struct has_intrinsic_size
    : boost_ext::mpl::or2<
          size_private::is_callable_mem_fun< R const &, Size ( ) >,
          ::sake_range_size_private::is_callable< Size ( R const & ) >
      >
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_SIZE_HPP
