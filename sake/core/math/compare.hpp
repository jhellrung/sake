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
 * compare compares its arguments (determines their relative order), returning
 * either a sake::sign_t (the default) or a sake::fuzzy_sign_t (when the
 * comparison could be indeterminate).  It automatically uses ADL to find
 * custom overloads, falling back to a default implementation if ADL fails.
 *
 * The default implementation of compare(T0 const &, T1 const &)
 * - forwards to T0::compare, if available; else
 * - forwards to T1::compare, if available; else
 * - compares the arguments via operator<.
 *
 * The default implementation of result_of::compare metafunction is evaluated
 * based on the compare overload found via ADL, and is able to detect result
 * types convertible to either sake::sign_t (including int's) and
 * sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_COMPARE_HPP
#define SAKE_CORE_MATH_COMPARE_HPP

#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/less.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/compare_fwd.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/indeterminate.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_compare0
#define SAKE_INTROSPECTION_FUNCTION_NAME compare0
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_compare1
#define SAKE_INTROSPECTION_FUNCTION_NAME compare1
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_compare
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME compare
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

namespace default_impl
{

namespace compare_private
{

template<
    class T0, class T1,
    bool S01    = sake::is_callable_mem_fun_compare< T0 const &, sake::sign_t ( T1 const & ) >::value
               || sake::is_callable_mem_fun_compare< T0 const &, int ( T1 const & ) >::value,
    bool SorF01 = S01 || sake::is_callable_mem_fun_compare< T0 const &, sake::fuzzy_sign_t ( T1 const & ) >::value,
    bool S10    = sake::is_callable_mem_fun_compare< T1 const &, sake::sign_t ( T0 const & ) >::value
               || sake::is_callable_mem_fun_compare< T1 const &, int ( T0 const & ) >::value,
    bool SorF10 = S10 || sake::is_callable_mem_fun_compare< T1 const &, sake::fuzzy_sign_t ( T0 const & ) >::value
>
struct dispatch_on_mem_fun;

template<
    class T0, class T1,
    class B01 = typename sake::operators::result_of::less< T0, T1 >::type,
    class B10 = typename sake::operators::result_of::less< T1, T0 >::type
>
struct dispatch_on_less_result;

} // namespace compare_private

} // namespace default_impl

namespace result_of
{

/*******************************************************************************
 * struct result_of::compare< T0, T1 = T0 >
 ******************************************************************************/

template< class T0, class T1 /*= T0*/ >
struct compare
    : extension::compare0<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::compare0< T0, T1, Enable = void >
 * struct result_of::extension::compare1< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable /*= void*/ >
struct compare0
    : extension::compare1< T0, T1 >
{ };
template< class T0, class T1, class Enable /*= void*/ >
struct compare1
    : default_impl::compare< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::compare< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

namespace compare_private
{

template<
    class T0, class T1,
    bool S01    = sake::is_callable_compare0< sake::sign_t ( T0 const &, T1 const & ) >::value
               || sake::is_callable_compare0< int ( T0 const &, T1 const & ) >::value,
    bool SorF01 = S01 || sake::is_callable_compare0< sake::fuzzy_sign_t ( T0 const &, T1 const & ) >::value,
    bool S10    = sake::is_callable_compare1< sake::sign_t ( T0 const &, T1 const & ) >::value
               || sake::is_callable_compare1< int ( T0 const &, T1 const & ) >::value,
    bool SorF10 = S10 || sake::is_callable_compare1< sake::fuzzy_sign_t ( T0 const &, T1 const & ) >::value
>
struct dispatch_on_fun;

template< class T0, class T1, bool S01, bool S10, bool SorF10 >
struct dispatch_on_fun< T0, T1, S01, true, S10, SorF10 >
    : boost::mpl::if_c< S01, sake::sign_t, sake::fuzzy_sign_t >
{ };

template< class T0, class T1, bool S10 >
struct dispatch_on_fun< T0, T1, false, false, S10, true >
    : boost::mpl::if_c< S10, sake::sign_t, sake::fuzzy_sign_t >
{ };

template< class T0, class T1 >
struct dispatch_on_fun< T0, T1, false, false, false, false >
    : sake::default_impl::compare_private::dispatch_on_mem_fun< T0, T1 >
{ };

} // namespace compare_private

template< class T0, class T1 /*= T0*/ >
struct compare
    : compare_private::dispatch_on_fun<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace default_impl

} // namespace result_of

namespace default_impl
{

template< class T0, class T1 >
inline typename sake::result_of::compare< T0, T1 >::type
compare0(T0 const & x0, T1 const & x1);

template< class T0, class T1 >
inline typename sake::result_of::compare< T0, T1 >::type
compare1(T0 const & x0, T1 const & x1);

} // namespace default_impl

} // namespace sake

namespace sake_compare_private
{

template< class T0, class T1 >
inline typename ::sake::result_of::compare< T0, T1 >::type
impl0(T0 const & x0, T1 const & x1)
{
    using ::sake::default_impl::compare0;
    typedef typename ::sake::result_of::compare< T0, T1 >::type result_type;
    return static_cast< result_type >(compare0(x0, x1));
}

template< class T0, class T1 >
inline typename ::sake::result_of::compare< T0, T1 >::type
impl1(T0 const & x0, T1 const & x1)
{
    using ::sake::default_impl::compare1;
    typedef typename ::sake::result_of::compare< T0, T1 >::type result_type;
    return static_cast< result_type >(compare1(x0, x1));
}

} // namespace sake_compare_private

namespace sake
{

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
    { return ::sake_compare_private::impl0(x0, x1); }
};

} // namespace functional

functional::compare const compare = { };

namespace default_impl
{

template< class T0, class T1 >
inline typename compare_private::dispatch_on_mem_fun< T0, T1 >::type
compare(T0 const & x0, T1 const & x1)
{ return compare_private::dispatch_on_mem_fun< T0, T1 >::apply(x0, x1); }

template< class T0, class T1 >
inline typename sake::result_of::compare< T0, T1 >::type
compare0(T0 const & x0, T1 const & x1)
{ return ::sake_compare_private::impl1(x0, x1); }

template< class T0, class T1 >
inline typename sake::result_of::compare< T0, T1 >::type
compare1(T0 const & x0, T1 const & x1)
{ return default_impl::compare(x0, x1); }

namespace compare_private
{

template< class T0, class T1, bool S01, bool S10, bool SorF10 >
struct dispatch_on_mem_fun< T0, T1, S01, true, S10, SorF10 >
{
    typedef typename boost::mpl::if_c< S01, sake::sign_t, sake::fuzzy_sign_t >::type type;
    static type apply(T0 const & x0, T1 const & x1)
    { return static_cast< type >(x0.compare(x1)); }
};

template< class T0, class T1, bool S10 >
struct dispatch_on_mem_fun< T0, T1, false, false, S10, true >
{
    typedef typename boost::mpl::if_c< S10, sake::sign_t, sake::fuzzy_sign_t >::type type;
    static type apply(T0 const & x0, T1 const & x1)
    { return static_cast< type >(-x1.compare(x0)); }
};

template< class T0, class T1 >
struct dispatch_on_mem_fun< T0, T1, false, false, false, false >
    : dispatch_on_less_result< T0, T1 >
{ };

template< class T0, class T1, class B01, class B10 >
struct dispatch_on_less_result
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
               sake::fuzzy_sign_t(
                   static_cast< int >(static_cast< bool >(b10)) -
                   static_cast< int >(static_cast< bool >(b01))
               );
    }
};

template< class T0, class T1 >
struct dispatch_on_less_result< T0, T1, bool, bool >
{
    typedef sake::sign_t type;
    static type apply(T0 const & x0, T1 const & x1)
    {
        return sake::sign_t(
            static_cast< int >(sake::operators::less(x1, x0))
          - static_cast< int >(sake::operators::less(x0, x1))
        );
    }
};

} // namespace compare_private

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_COMPARE_HPP
