/*******************************************************************************
 * sake/core/math/rcp.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * rcp(T&& x) -> result_of::rcp<T>::type
 * struct functional::rcp
 *
 * struct result_of::rcp<T>
 * struct result_of::extension::rcp< T, Enable = void >
 * struct result_of::default_impl::rcp<T>
 *
 * Returns the reciprocal (multiplicative inverse) of its argument.
 *
 * sake::rcp(T) is implemented in terms of
 * - T::rcp(), if available; else
 * - rcp(T) (unqualified, hence subject to ADL), if available; else
 * - T::rcp_ip(), if available and the argument is an rvalue; else
 * - rcp_ip(T) (unqualified, hence subject to ADL), if available and the
 *   argument is an rvalue; else
 * - returning a proxy.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_RCP_HPP
#define SAKE_CORE_MATH_RCP_HPP

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/has_qualifier.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/divide.hpp>
#include <sake/core/introspection/has_operator_divide.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/private/rcp_common.hpp>
#include <sake/core/math/rcp_fwd.hpp>
#include <sake/core/math/reciprocal.hpp>
#include <sake/core/math/reciprocal_fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace rcp_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename sake::result_of::rcp<T>::type,
    int = dispatch_index<T>::value
>
struct dispatch;

} // namespace rcp_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::rcp<T>
 ******************************************************************************/

template< class T >
struct rcp
{
    typedef typename sake::result_of::extension::rcp<
        typename boost_ext::remove_rvalue_reference<T>::type >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
};

/*******************************************************************************
 * struct result_of::extension::rcp< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct rcp
    : sake::result_of::default_impl::rcp<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::rcp<T>
 ******************************************************************************/

namespace default_impl
{

namespace rcp_private
{

template< class T, bool = sake::has_operator_divide<T>::value >
struct result_types_dispatch;

template< class T >
struct result_types_dispatch< T, false >
{
    typedef boost::mpl::vector1<
        typename boost_ext::remove_qualifiers<T>::type
    > type;
};

template< class T >
struct result_types_dispatch< T, true >
{
    typedef boost::mpl::vector2<
        typename boost_ext::remove_qualifiers<T>::type,
        typename boost_ext::remove_rvalue_reference<
            typename sake::operators::result_of::divide<T>::type >::type
    > type;
};

} // namespace rcp_private

template< class T >
struct rcp_result_types
    : sake::result_of::default_impl::rcp_private::result_types_dispatch<T>
{ };

template< class T >
struct rcp
    : sake::rcp_private::dispatch< T, void >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * rcp(T const & x) -> result_of::rcp<T>::type
 * struct functional::rcp
 ******************************************************************************/

namespace functional
{

struct rcp
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::rcp, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::rcp<T>::type
    operator()(T&& x) const
    { return sake::rcp_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::rcp< T& >::type
    operator()(T& x) const
    {
        return sake::rcp_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type >::apply(x);
    }

    template< class T >
    typename sake::result_of::rcp< T const & >::type
    operator()(T const & x) const
    { return sake::rcp_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace rcp_adl_barrier
{ sake::functional::rcp const rcp = { }; }
using namespace rcp_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::rcp const rcp = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_rcp_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME rcp
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T, class Result >
struct adl
{
    typedef typename ::sake::boost_ext::add_rvalue_reference<T>::type fwd_type;
    static Result apply(fwd_type x)
    { return rcp(static_cast< fwd_type >(x)); }
};

template< class T >
struct adl_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename rcp(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::rcp_result_types<T>::type,
        type
    );
};

template< class T >
struct adl< T, void >
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::has_qualifier<T>::value));
    typedef typename adl_impl<T>::type type;
};

template< class T >
struct adl< T&, void >
{
private:
    typedef typename adl_impl< T& >::type maybe_type;
public:
    typedef typename ::boost::mpl::eval_if_c<
        ::boost::is_void< maybe_type >::value,
        ::sake::result_of::rcp< typename boost::remove_cv<T>::type >,
        ::boost::mpl::identity< maybe_type >
    >::type type;
};

} // namespace sake_rcp_private

namespace sake
{

namespace rcp_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME rcp
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
struct dispatch_index
{
private:
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename boost_ext::add_reference<T>::type ref_type;
public:
    static int const value = boost_ext::mpl::
         if_< boost::is_floating_point< noqual_type >,
              sake::int_tag<5> >::type::template
    else_if < rcp_private::is_callable_mem_fun< T, void ( ) >,
              sake::int_tag<4> >::type::template
    else_if < ::sake_rcp_private::is_callable< void ( T ) >,
              sake::int_tag<3> >::type::template
#ifndef BOOST_NO_RVALUE_REFERENCES
    else_if < boost_ext::is_reference<T>,
              sake::int_tag<0> >::type::template
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    else_if_not< sake::has_move_emulation<T>,
              sake::int_tag<0> >::type::template
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
    else_if < rcp_ip_private::is_callable_mem_fun< ref_type, void ( ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_rcp_ip_private::is_callable< void ( ref_type ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
};

template< class T, class Result >
struct dispatch< T, Result, 5 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    static type apply(type const x)
    { return 1/x; }
};

template< class T >
struct dispatch4_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval<T>().rcp(),
        typename sake::result_of::default_impl::rcp_result_types<T>::type,
        type
    );
};

template< class T >
struct dispatch< T, void, 4 >
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));
    typedef typename dispatch4_impl<T>::type type;
};

template< class T >
struct dispatch< T&, void, 4 >
{
private:
    typedef typename dispatch4_impl< T& >::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        result_of::rcp< typename boost::remove_cv<T>::type >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

template< class T, class Result >
struct dispatch< T, Result, 4 >
{
    typedef typename boost_ext::add_rvalue_reference<T>::type fwd_type;
    static Result apply(fwd_type x)
    { return static_cast< fwd_type >(x).rcp(); }
};

template< class T, class Result >
struct dispatch< T, Result, 3 >
    : ::sake_rcp_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    typedef SAKE_RV_REF( T ) fwd_type;
    typedef T type;
    static type apply(fwd_type x)
    {
        SAKE_AS_LVALUE( x ).rcp_ip();
        return sake::move(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
{
    typedef SAKE_RV_REF( T ) fwd_type;
    typedef T type;
    static type apply(fwd_type x)
    {
        ::sake_rcp_ip_private::adl(SAKE_AS_LVALUE( x ));
        return sake::move(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename boost_ext::add_rvalue_reference<T>::type fwd_type;
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef sake::reciprocal< noqual_type > type;
    static type apply(fwd_type x)
    { return type(static_cast< fwd_type >(x)); }
};

} // namespace inv_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_RCP_HPP
