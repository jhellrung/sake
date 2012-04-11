/*******************************************************************************
 * sake/core/math/compare.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * compare(T0&& x0, T1&& x1) -> result_of::compare< T0, T1 >::type
 * struct functional::compare
 *
 * struct result_of::compare< T0, T1 = T0 >
 * struct result_of::extension::compare< T0, T1, Enable = void >
 * struct result_of::default_impl::compare< T0, T1 = T0 >
 *
 * Compares its arguments (determines their relative order), returning either a
 * sake::sign_t (the default) or a sake::fuzzy_sign_t (when the comparison could
 * be indeterminate).
 *
 * sake::compare(T0, T1) is implemented in terms of
 * - T0::compare(T1), if available; else
 * - T1::compare(T0), if available; else
 * - compare(T0, T1) (unqualified, hence subject to ADL), if available; else
 * - compare(T1, T0) (unqualified, hence subject to ADL), if available; else
 * - comparison via operator<.
 *
 * The default implementation of the result_of::compare metafunction is
 * evaluated based on the result of first available implemention above and is
 * able to detect result types int, sake::sign_t, and sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_COMPARE_HPP
#define SAKE_CORE_MATH_COMPARE_HPP

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/unary_minus.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/compare_fwd.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/indeterminate.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace compare_private
{

template< class T0, class T1 >
struct dispatch_index;

template<
    class T0, class T1,
    class Result = typename result_of::compare< T0, T1 >::type,
    unsigned int = dispatch_index< T0, T1 >::value
>
struct dispatch;

} // namespace compare_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::compare< T0, T1 = T0 >
 ******************************************************************************/

template< class T0, class T1 /*= T0*/ >
struct compare
{
    typedef typename extension::compare<
        typename boost_ext::remove_rvalue_reference< T0 >::type,
        typename boost_ext::remove_rvalue_reference< T1 >::type
    >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
};

/*******************************************************************************
 * struct result_of::extension::compare< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable /*= void*/ >
struct compare
    : default_impl::compare< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::compare< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

typedef boost::mpl::vector9<
    int,
    sake::sign_t,
    sake::fuzzy_sign_t,
    boost::integral_constant< int, -1 >,
    boost::integral_constant< int,  0 >,
    boost::integral_constant< int, +1 >,
    boost::mpl::int_<-1>,
    boost::mpl::int_< 0>,
    boost::mpl::int_<+1>
> compare_result_types;

template< class T0, class T1 /*= T0*/ >
struct compare
    : compare_private::dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type,
          void
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * compare(T0 const & x0, T1 const & x1) -> result_of::compare< T0, T1 >::type
 * struct functional::compare
 ******************************************************************************/

namespace functional
{

struct compare
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::compare, 2 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename result_of::compare< T0, T1 >::type
    operator()(T0&& x0, T1&& x1) const
    {
        return compare_private::dispatch< T0, T1 >::apply(
            sake::forward< T0 >(x0),
            sake::forward< T1 >(x1)
        );
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename result_of::compare< T0&, T1& >::type
    operator()(T0& x0, T1& x1) const
    {
        return compare_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T0& >::type,
            typename boost_ext::remove_rvalue_reference< T1& >::type
        >::apply(x0, x1);
    }

    template< class T0, class T1 >
    typename result_of::compare< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1) const
    {
        return compare_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T0& >::type,
            T1 const &
        >::apply(x0, x1);
    }

    template< class T0, class T1 >
    typename result_of::compare< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1) const
    {
        return compare_private::dispatch<
            T0 const &,
            typename boost_ext::remove_rvalue_reference< T1& >::type
        >::apply(x0, x1);
    }

    template< class T0, class T1 >
    typename result_of::compare< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return compare_private::dispatch< T0 const &, T1 const & >::apply(x0, x1); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace compare_adl_barrier
{ functional::compare const compare = { }; }
using namespace compare_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::compare const compare = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_compare_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME compare
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T0, class T1, class Result >
struct adl
{
    template< class T0_, class T1_ >
    static Result apply(SAKE_FWD2_REF( T0_ ) x0, SAKE_FWD2_REF( T1_ ) x1)
    {
        return static_cast< Result >(compare(
            ::sake::forward< T0_ >(x0),
            ::sake::forward< T1_ >(x1)
        ));
    }
};

template< class T0, class T1 >
struct adl_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename compare(::sake::declval< T0 >(), ::sake::declval< T1 >()),
        ::sake::result_of::default_impl::compare_result_types,
        type
    );
};

template< class T0, class T1 >
struct adl< T0, T1, void >
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference< T0 >::value));
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference< T1 >::value));
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_cv_or< T0 >::value));
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_cv_or< T1 >::value));
private:
    typedef typename adl_impl< T0, T1 >::type nominal_type;
public:
    typedef typename ::boost::mpl::if_c<
        ::boost::is_same< nominal_type, int >::value,
        ::sake::sign_t,
        nominal_type
    >::type type;
};

template< class T0, class T1 >
class adl_helper
{
    typedef typename adl_impl< T0, T1 >::type maybe_type;
public:
    typedef typename ::boost::mpl::eval_if_c<
        ::boost::is_void< maybe_type >::value,
        ::sake::result_of::compare<
            typename ::sake::boost_ext::remove_qualifiers< T0 >::type,
            typename ::sake::boost_ext::remove_qualifiers< T1 >::type
        >,
        ::boost::mpl::identity< maybe_type >
    >::type type;
};

template< class T0, class T1 >
struct adl< T0, T1&, void >
    : adl_helper< T0, T1& >
{ };

template< class T0, class T1 >
struct adl< T0&, T1, void >
    : adl_helper< T0&, T1 >
{ };

template< class T0, class T1 >
struct adl< T0&, T1&, void >
    : adl_helper< T0&, T1& >
{ };

} // namespace sake_compare_private

namespace sake
{

namespace compare_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME compare
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

using boost_ext::mpl::uint;

template< class T0, class T1 >
struct dispatch_index
{
    static unsigned int const value = boost_ext::mpl::
             if_< is_callable_mem_fun< T0, void ( T1 ) >, uint<3> >::type::template
        else_if < is_callable_mem_fun< T1, void ( T0 ) >, uint<2> >::type::template
        else_if < ::sake_compare_private::is_callable< void ( T0, T1 ) >, uint<1> >::type::template
        else_if < ::sake_compare_private::is_callable< void ( T1, T0 ) >, uint<2> >::type::template
        else_   < uint<0> >::type::value;
};

template< class T0, class T1 >
struct dispatch3_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval< T0 >().compare(sake::declval< T1 >()),
        result_of::default_impl::compare_result_types,
        type
    );
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 3 >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T1 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T1 >::value));
private:
    typedef typename dispatch3_impl< T0, T1 >::type nominal_type;
public:
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        sake::sign_t,
        nominal_type
    >::type type;
};

template< class T0, class T1 >
class dispatch3_helper
{
    typedef typename dispatch3_impl< T0, T1 >::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        result_of::compare<
            typename boost_ext::remove_qualifiers< T0 >::type,
            typename boost_ext::remove_qualifiers< T1 >::type
        >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

template< class T0, class T1 >
struct dispatch< T0, T1&, void, 3 >
    : dispatch3_helper< T0, T1& >
{ };

template< class T0, class T1 >
struct dispatch< T0&, T1, void, 3 >
    : dispatch3_helper< T0&, T1 >
{ };

template< class T0, class T1 >
struct dispatch< T0&, T1&, void, 3 >
    : dispatch3_helper< T0&, T1& >
{ };

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 3 >
{
    template< class T0_, class T1_ >
    static Result apply(SAKE_FWD2_REF( T0_ ) x0, SAKE_FWD2_REF( T1_ ) x1)
    {
        return static_cast< Result >(
            sake::forward< T0_ >(x0).compare(
            sake::forward< T1_ >(x1))
        );
    }
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 2 >
    : boost_ext::remove_rvalue_reference<
          typename operators::result_of::unary_minus<
              typename result_of::compare< T1, T0 >::type
          >::type
      >
{ };

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 2 >
{
    template< class T0_, class T1_ >
    static Result apply(SAKE_FWD2_REF( T0_ ) x0, SAKE_FWD2_REF( T1_ ) x1)
    {
        return -sake::compare(
            sake::forward< T1_ >(x1),
            sake::forward< T0_ >(x0)
        );
    }
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 1 >
    : ::sake_compare_private::adl< T0, T1, Result >
{ };

template<
    class T0, class T1,
    class B01 = typename boost_ext::remove_rvalue_reference<
                    typename operators::result_of::less< T0, T1 >::type
                >::type,
    class B10 = typename boost_ext::remove_rvalue_reference<
                    typename operators::result_of::less< T1, T0 >::type
                >::type
>
struct dispatch0_impl;

template< class T0, class T1, class B01, class B10 >
struct dispatch0_impl
{
    typedef sake::fuzzy_sign_t type;
    static type apply(T0 const & x0, T1 const & x1)
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< B01, bool >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< B10, bool >::value));
        B01 const b01 = operators::less(x0, x1);
        B10 const b10 = operators::less(x1, x0);
        return sake::indeterminate(b01)
            || sake::indeterminate(b10) ?
               sake::fuzzy_sign_t() :
               sake::fuzzy_sign_t(
                   static_cast< int >(static_cast< bool >(b10))
                 - static_cast< int >(static_cast< bool >(b01))
               );
    }
};

template< class T0, class T1 >
struct dispatch0_impl< T0, T1, bool, bool >
{
    typedef sake::sign_t type;
    static type apply(T0 const & x0, T1 const & x1)
    {
        return sake::sign_t(
            static_cast< int >(operators::less(x1, x0))
          - static_cast< int >(operators::less(x0, x1))
        );
    }
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 0 >
    : dispatch0_impl<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace compare_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_COMPARE_HPP
