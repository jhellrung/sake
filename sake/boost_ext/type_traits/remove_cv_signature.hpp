/*******************************************************************************
 * sake/boost_ext/type_traits/remove_cv_signature.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::remove_cv_signature<T>
 *
 * remove_cv_signature<T> -> T
 * remove_cv_signature< T const > -> remove_cv_signature<T>
 * remove_cv_signature< T volatile > -> remove_cv_signature<T>
 * remove_cv_signature< T const volatile > -> remove_cv_signature<T>
 * remove_cv_signature< T* > -> remove_cv_signature<T> *
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_CV_SIGNATURE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_CV_SIGNATURE_HPP

namespace sake
{

namespace boost_ext
{

template< class T >
struct remove_cv_signature
{ typedef T type; };

template< class T > struct remove_cv_signature< T const > : remove_cv_signature<T> { };
template< class T > struct remove_cv_signature< T volatile > : remove_cv_signature<T> { };
template< class T > struct remove_cv_signature< T const volatile > : remove_cv_signature<T> { };

template< class T >
struct remove_cv_signature< T* >
{ typedef typename remove_cv_signature<T>::type * type; };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_CV_SIGNATURE_HPP
