/*******************************************************************************
 * sake/core/ref/ref.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * ref(T& x)                           -> result_of::ref<T>::type
 * ref< Tags >(T& x)                   -> result_of::ref< T, Tags >::type
 * ref(T& x, type_tag< Tags >)         -> result_of::ref< T, Tags >::type
 * cref(T const & x)                   -> result_of::cref< T >::type
 * cref< Tags >(T const & x)           -> result_of::cref< T, Tags >::type
 * cref(T const & x, type_tag< Tags >) -> result_of::cref< T, Tags >::type
 ******************************************************************************/

#ifndef SAKE_CORE_REF_REF_HPP
#define SAKE_CORE_REF_REF_HPP

#include <boost/mpl/set/set10.hpp>
#include <boost/type_traits/add_const.hpp>

#include <sake/core/ref/ref_tag.hpp>
#include <sake/core/ref/reference_wrapper.hpp>
#include <sake/core/utility/referent.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template<
    class T,
    class Tags = boost::mpl::set1< sake::ref_tag::parameter >
>
struct ref
{
    typedef sake::reference_wrapper<
        typename sake::referent<T>::type, Tags
    > type;
};

template<
    class T,
    class Tags = boost::mpl::set1< sake::ref_tag::parameter >
>
struct cref
{
    typedef sake::reference_wrapper<
        typename boost::add_const< typename sake::referent<T>::type >::type, Tags
    > type;
};

} // namespace result_of

template< class T >
inline typename sake::result_of::ref<T>::type
ref(T& x)
{ return typename sake::result_of::ref<T>::type(x); }

template< class T >
inline typename sake::result_of::ref< T const >::type
ref(T const & x)
{ return typename sake::result_of::ref< T const >::type(x); }

template< class Tags, class T >
inline typename sake::result_of::ref< T, Tags >::type
ref(T& x)
{ return typename sake::result_of::ref< T, Tags >::type(x); }

template< class Tags, class T >
inline typename sake::result_of::ref< T const, Tags >::type
ref(T const & x)
{ return typename sake::result_of::ref< T const, Tags >::type(x); }

template< class T, class Tags >
inline typename sake::result_of::ref< T, Tags >::type
ref(T& x, sake::type_tag< Tags >)
{ return typename sake::result_of::ref< T, Tags >::type(x); }

template< class T, class Tags >
inline typename sake::result_of::ref< T const, Tags >::type
ref(T const & x, sake::type_tag< Tags >)
{ return typename sake::result_of::ref< T const, Tags >::type(x); }

template< class T >
inline typename sake::result_of::cref<T>::type
cref(T const & x)
{ return typename sake::result_of::cref<T>::type(x); }

template< class Tags, class T >
inline typename sake::result_of::cref< T, Tags >::type
cref(T const & x)
{ return typename sake::result_of::cref< T, Tags >::type(x); }

template< class T, class Tags >
inline typename sake::result_of::cref< T, Tags >::type
cref(T const & x, sake::type_tag< Tags >)
{ return typename sake::result_of::cref< T, Tags >::type(x); }

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_REF_HPP
