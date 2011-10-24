/*******************************************************************************
 * sake/core/functional/operators/private/shift.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

#ifndef SAKE_OPERATORS_NAME
#error SAKE_OPERATORS_NAME must be defined.
#endif // #ifndef SAKE_OPERATORS_NAME

#ifndef SAKE_OPERATORS_OP
#error SAKE_OPERATORS_OP must be defined.
#endif // SAKE_OPERATORS_OP

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T0, class T1 = T0 >
struct SAKE_OPERATORS_NAME;

namespace extension
{
template< class T0, class T1, class Enable = void >
struct BOOST_PP_CAT( SAKE_OPERATORS_NAME, 0 );
template< class T0, class T1, class Enable = void >
struct BOOST_PP_CAT( SAKE_OPERATORS_NAME, 1 );
} // namespace extension

namespace default_impl
{
template< class T0, class T1 = T0 >
struct SAKE_OPERATORS_NAME;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::SAKE_OPERATORS_NAME< T0, T1 = T0 >
 ******************************************************************************/

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME
    : extension::BOOST_PP_CAT( SAKE_OPERATORS_NAME, 0 )< T0, T1 >
{ };

/*******************************************************************************
 * struct operators::result_of::extension::SAKE_OPERATORS_NAME0< T0, T1, Enable = void >
 * struct operators::result_of::extension::SAKE_OPERATORS_NAME1< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable >
struct BOOST_PP_CAT( SAKE_OPERATORS_NAME, 0 )
    : extension::BOOST_PP_CAT( SAKE_OPERATORS_NAME, 1 )< T0, T1 >
{ };

template< class T0, class T1, class Enable >
struct BOOST_PP_CAT( SAKE_OPERATORS_NAME, 1 )
    : default_impl::SAKE_OPERATORS_NAME< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::SAKE_OPERATORS_NAME< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )
{

template< class T, bool = boost_ext::is_integral_or_enum<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, true >
    : boost::integral_promotion<T>
{ };

template< class T >
struct dispatch< T, false >
{ typedef T type; };

} // namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME
    : BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::SAKE_OPERATORS_NAME(T0&& x0, T1&& x1)
 *     -> operators::result_of::SAKE_OPERATORS_NAME< T0, T1 >::type
 * struct operators::functional::SAKE_OPERATORS_NAME
 ******************************************************************************/

namespace functional
{

struct SAKE_OPERATORS_NAME
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::SAKE_OPERATORS_NAME, 2 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename result_of::SAKE_OPERATORS_NAME< T0, T1 >::type
    operator()(T0&& x0, T1&& x1) const
    { return sake::forward< T0 >(x0) SAKE_OPERATORS_OP sake::forward< T1 >(x1); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename result_of::SAKE_OPERATORS_NAME<
        typename boost_ext::remove_rvalue_reference< T0& >::type,
        typename boost_ext::remove_rvalue_reference< T1& >::type
    >::type
    operator()(T0& x0, T1& x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename result_of::SAKE_OPERATORS_NAME<
        typename boost_ext::remove_rvalue_reference< T0& >::type,
        T1 const &
    >::type
    operator()(T0& x0, T1 const & x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename result_of::SAKE_OPERATORS_NAME<
        T0 const &,
        typename boost_ext::remove_rvalue_reference< T1& >::type
    >::type
    operator()(T0 const & x0, T1& x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename result_of::SAKE_OPERATORS_NAME<
        T0 const &,
        T1 const &
    >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

functional::SAKE_OPERATORS_NAME const SAKE_OPERATORS_NAME = { };

} // namespace operators

} // namespace sake

#undef SAKE_OPERATORS_NAME
#undef SAKE_OPERATORS_OP