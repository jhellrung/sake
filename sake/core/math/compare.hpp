/*******************************************************************************
 * sake/core/math/compare.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * compare(T0 const & x0, T1 const & x1) -> result_of::compare< T0, T1 >::type
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
 * sake::compare(T0 const &, T1 const &) is implemented in terms of
 * - T0::compare(T1 const &), if available; else
 * - T1::compare(T0 const &), if available; else
 * - compare(T0 const &, T1 const &) (unqualified, hence subject to ADL), if
 *   available; else
 * - compare(T1 const &, T0 const &) (unqualified, hence subject to ADL), if
 *   available; else
 * - comparison via operator<.
 *
 * The default implementation of the result_of::compare metafunction is
 * evaluated based on the result of first available implemention above and is
 * able to detect result types convertible to int, sake::sign_t, and
 * sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_COMPARE_HPP
#define SAKE_CORE_MATH_COMPARE_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/compare_fwd.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/indeterminate.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

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
    : extension::compare<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

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

typedef boost::mpl::vector3<
    int,
    sake::sign_t,
    sake::fuzzy_sign_t
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

    template< class T0, class T1 >
    typename result_of::compare< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return compare_private::dispatch< T0, T1 >::apply(x0, x1); }
};

} // namespace functional

functional::compare const compare = { };

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
    static Result apply(T0 const & x0, T1 const & x1)
    { return static_cast< Result >(compare(x0, x1)); }
};

template< class T0, class T1 >
struct adl< T0, T1, void >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        compare(::sake::declcref< T0 >(), ::sake::declcref< T1 >()),
        ::sake::result_of::default_impl::compare_result_types,
        nominal_type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< nominal_type >::value));
    typedef typename boost::mpl::if_c<
        ::boost::is_same< nominal_type, int >::value,
        ::sake::sign_t,
        nominal_type
    >::type type;
};

} // namespace sake_compare_private

namespace sake
{

namespace compare_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME compare
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T0, class T1 >
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 4) * is_callable_mem_fun< T0 const &, void ( T1 const & ) >::value
      | (1 << 3) * is_callable_mem_fun< T1 const &, void ( T0 const & ) >::value
      | (1 << 2) * sake_compare_private::is_callable< void ( T0 const &, T1 const & ) >::value
      | (1 << 1) * sake_compare_private::is_callable< void ( T1 const &, T0 const & ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 4 >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        sake::declcref< T0 >().compare(sake::declcref< T1 >()),
        result_of::default_impl::compare_result_types,
        nominal_type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< nominal_type >::value));
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        sake::sign_t,
        nominal_type
    >::type type;
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 4 >
{
    static Result apply(T0 const & x0, T1 const & x1)
    { return static_cast< Result >(x0.compare(x1)); }
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 3 >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        sake::declcref< T1 >().compare(sake::declcref< T0 >()),
        result_of::default_impl::compare_result_types,
        nominal_type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< nominal_type >::value));
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        sake::sign_t,
        nominal_type
    >::type type;
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 3 >
{
    static Result apply(T0 const & x0, T1 const & x1)
    { return -static_cast< Result >(x1.compare(x0)); }
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 2 >
    : ::sake_compare_private::adl< T0, T1, Result >
{ };

template< class T0, class T1 >
struct dispatch< T0, T1, void, 1 >
    : ::sake_compare_private::adl< T1, T0, void >
{ };

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 1 >
{
    static Result apply(T0 const & x0, T1 const & x1)
    { return -::sake_compare_private::adl< T1, T0, Result >::apply(x1, x0); }
};

template<
    class T0, class T1,
    class B01 = typename sake::operators::result_of::less< T0, T1 >::type,
    class B10 = typename sake::operators::result_of::less< T1, T0 >::type
>
struct dispatch_on_result_of_less;

template< class T0, class T1, class B01, class B10 >
struct dispatch_on_result_of_less
{
    typedef sake::fuzzy_sign_t type;
    static type apply(T0 const & x0, T1 const & x1)
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< B01, bool >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< B10, bool >::value));
        B01 const b01 = sake::operators::less(x0, x1);
        B10 const b10 = sake::operators::less(x1, x0);
        return sake::indeterminate(b01) || sake::indeterminate(b10) ?
               sake::fuzzy_sign_t() :
               sake::fuzzy_sign_t(static_cast< int >(static_cast< bool >(b10))
                                - static_cast< int >(static_cast< bool >(b01)));
    }
};

template< class T0, class T1 >
struct dispatch_on_result_of_less< T0, T1, bool, bool >
{
    typedef sake::sign_t type;
    static type apply(T0 const & x0, T1 const & x1)
    {
        return sake::sign_t(static_cast< int >(sake::operators::less(x1, x0))
                          - static_cast< int >(sake::operators::less(x0, x1)));
    }
};

template< class T0, class T1, class Result >
struct dispatch< T0, T1, Result, 0 >
    : dispatch_on_result_of_less< T0, T1 >
{ };

} // namespace compare_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_COMPARE_HPP
