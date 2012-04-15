/*******************************************************************************
 * sake/core/ref/ref_if_nbvo.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * ref_if_nbvo(T& x)                           -> result_of::ref_if_nbvo<T>::type
 * ref_if_nbvo(T& x, type_tag< Tags >)         -> result_of::ref_if_nbvo< T, type_tag< Tags > >::type
 * cref_if_nbvo(T const & x)                   -> result_of::cref_if_nbvo<T>::type
 * cref_if_nbvo(T const & x, type_tag< Tags >) -> result_of::cref_if_nbvo< T, type_tag< Tags > >::type
 *
 * NBVO = Not By-Value Optimal
 *
 * ref_if_nbvo will wrap its argument in a sake::reference_wrapper if passing by
 * value is not optimal (as given by is_by_value_optimal).  In particular,
 * reference wrappers are not rewrapped.
 *
 * The default tag set is boost::mpl::set1< ref_tag::parameter >.
 ******************************************************************************/

#ifndef SAKE_CORE_REF_REF_OPTIMAL_HPP
#define SAKE_CORE_REF_REF_OPTIMAL_HPP

#include <boost/mpl/set/set10.hpp>
#include <boost/type_traits/add_const.hpp>

#include <sake/core/ref/ref_tag.hpp>
#include <sake/core/ref/reference_wrapper.hpp>
#include <sake/core/utility/type_tag.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>

namespace sake
{

namespace result_of
{

namespace ref_if_nbvo_private
{

template<
    class T, class Tags,
    bool = sake::is_by_value_optimal<T>::value
>
struct dispatch;

template< class T, class Tags >
struct dispatch< T, Tags, false >
{ typedef sake::reference_wrapper< T, Tags > type; };

template< class T, class Tags >
struct dispatch< T, Tags, true >
{ typedef T& type; };

} // namespace ref_if_nbvo_private

template<
    class T,
    class Tags = boost::mpl::set1< sake::ref_tag::parameter >
>
struct ref_if_nbvo
    : ref_if_nbvo_private::dispatch< T, Tags >
{ };

template< class T, class Tags >
struct ref_if_nbvo< T&, Tags >
    : ref_if_nbvo< T, Tags >
{ };

template<
    class T,
    class Tags = boost::mpl::set1< sake::ref_tag::parameter >
>
struct cref_if_nbvo
    : ref_if_nbvo< typename boost::add_const<T>::type, Tags >
{ };

template< class T, class Tags >
struct cref_if_nbvo< T&, Tags >
    : cref_if_nbvo< T, Tags >
{ };

} // namespace result_of

template< class T >
inline typename sake::result_of::ref_if_nbvo<T>::type
ref_if_nbvo(T& x)
{ return static_cast< typename sake::result_of::ref_if_nbvo<T>::type >(x); }

template< class T >
inline typename sake::result_of::ref_if_nbvo< T const >::type
ref_if_nbvo(T const & x)
{ return static_cast< typename sake::result_of::ref_if_nbvo< T const >::type >(x); }

template< class T, class Tags >
inline typename sake::result_of::ref_if_nbvo< T, Tags >::type
ref_if_nbvo(T& x, sake::type_tag< Tags >)
{ return static_cast< typename sake::result_of::ref_if_nbvo< T, Tags >::type >(x); }

template< class T, class Tags >
inline typename sake::result_of::ref_if_nbvo< T const, Tags >::type
ref_if_nbvo(T const & x, sake::type_tag< Tags >)
{ return static_cast< typename sake::result_of::ref_if_nbvo< T const, Tags >::type >(x); }

template< class T >
inline typename sake::result_of::cref_if_nbvo<T>::type
cref_if_nbvo(T const & x)
{ return static_cast< typename sake::result_of::cref_if_nbvo<T>::type >(x); }

template< class T, class Tags >
inline typename sake::result_of::cref_if_nbvo< T, Tags >::type
cref_if_nbvo(T const & x, sake::type_tag< Tags >)
{ return static_cast< typename sake::result_of::cref_if_nbvo< T, Tags >::type >(x); }

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_REF_OPTIMAL_HPP
