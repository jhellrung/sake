/*******************************************************************************
 * sake/core/utility/address_of.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * address_of(T& x) -> result_of::address_of<T>::type
 * struct functional::address_of
 *
 * This is identical to boost::addressof, except that it should work with
 * emulated rvalue references as well.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_ADDRESS_OF_HPP
#define SAKE_CORE_UTILITY_ADDRESS_OF_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/propagate_cv.hpp>

#include <sake/core/move/rv.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/bisfo.hpp>

namespace sake
{

namespace result_of
{

template< class T >
struct address_of;

template< class T >
struct address_of< T& >
{ typedef T* type; };

#ifdef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct address_of< boost::rv<T>& >
{ typedef T* type; };

// disable the following bindings
template< class T > struct address_of< boost::rv<T> const & > { };
template< class T > struct address_of< boost::rv<T> volatile & > { };
template< class T > struct address_of< boost::rv<T> const volatile & > { };

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace result_of

namespace functional
{

struct address_of
{
    SAKE_RESULT_FROM_METAFUNCTION( 1, result_of::address_of )

    template< class T >
    typename result_of::address_of< T& >::type
    operator()(T& x) const
    {
        typedef typename result_of::address_of< T& >::type result_type;
        typedef typename boost_ext::propagate_cv< T, unsigned char >::type cv_uchar_type;
        return reinterpret_cast< result_type >(&reinterpret_cast< cv_uchar_type& >(x));
    }
};

} // namespace functional

SAKE_BISFO( address_of )

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_ADDRESS_OF_HPP
