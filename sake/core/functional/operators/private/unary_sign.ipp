/*******************************************************************************
 * sake/core/functional/operators/private/unary_sign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>

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

template< class T >
struct SAKE_OPERATORS_NAME;

namespace extension
{
template< class T, class Enable = void >
struct SAKE_OPERATORS_NAME;
} // namespace extension

namespace default_impl
{
template< class T >
struct SAKE_OPERATORS_NAME;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::SAKE_OPERATORS_NAME<T>
 ******************************************************************************/

template< class T >
struct SAKE_OPERATORS_NAME
    : extension::SAKE_OPERATORS_NAME<T>
{ };

/*******************************************************************************
 * struct operators::result_of::extension::SAKE_OPERATORS_NAME< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct SAKE_OPERATORS_NAME
    : default_impl::SAKE_OPERATORS_NAME<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::SAKE_OPERATORS_NAME<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct SAKE_OPERATORS_NAME
    : boost_ext::remove_qualifiers<T>
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::SAKE_OPERATORS_NAME(T&& x)
 *     -> operators::result_of::SAKE_OPERATORS_NAME<T>::type
 * struct operators::functional::SAKE_OPERATORS_NAME
 ******************************************************************************/

namespace functional
{

struct SAKE_OPERATORS_NAME
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::SAKE_OPERATORS_NAME, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::SAKE_OPERATORS_NAME<T>::type
    operator()(T&& x) const
    { return SAKE_OPERATORS_OP sake::forward<T>(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::SAKE_OPERATORS_NAME<
        typename boost_ext::remove_rvalue_reference< T& >::type
    >::type
    operator()(T& x) const
    { return SAKE_OPERATORS_OP x; }

    template< class T >
    typename result_of::SAKE_OPERATORS_NAME< T const & >::type
    operator()(T const & x) const
    { return SAKE_OPERATORS_OP x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

functional::SAKE_OPERATORS_NAME const SAKE_OPERATORS_NAME = { };

} // namespace operators

} // namespace sake

#undef SAKE_OPERATORS_NAME
#undef SAKE_OPERATORS_OP
