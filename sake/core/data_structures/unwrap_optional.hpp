/*******************************************************************************
 * sake/core/data_structures/unwrap_optional.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct result_of::unwrap_optional<T>
 * unwrap_optional(T&& x) -> T&&
 * unwrap_optional(optional<T>&& x) -> T&&
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_UNWRAP_OPTIONAL_HPP
#define SAKE_CORE_DATA_STRUCTURES_UNWRAP_OPTIONAL_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>

#include <sake/core/data_structures/optional_fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

namespace result_of
{

template< class T >
struct unwrap_optional
    : boost_ext::add_rvalue_reference<T>
{ };

template< class T >
struct unwrap_optional< sake::optional<T> >
    : boost_ext::add_rvalue_reference<T>
{ };

template< class T >
struct unwrap_optional< sake::optional<T>& >
    : boost_ext::add_reference<T>
{ };

template< class T >
struct unwrap_optional< sake::optional<T> const & >
    : boost_ext::add_reference_add_const<T>
{ };

} // namespace result_of

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline T&&
unwrap_optional(T&& x)
{ return sake::forward<T>(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline T&
unwrap_optional(T& x)
{ return x; }

template< class T >
inline T const &
unwrap_optional(T const & x)
{ return x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
inline typename result_of::unwrap_optional< sake::optional<T>& >::type
unwrap_optional(sake::optional<T>& x)
{ return *x; }

template< class T >
inline typename result_of::unwrap_optional< sake::optional<T> const & >::type
unwrap_optional(sake::optional<T> const & x)
{ return *x; }

template< class T >
inline typename result_of::unwrap_optional< optional<T> >::type
unwrap_optional(SAKE_RV_REF( optional<T> ) x)
{ return sake::move(*x); }

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_UNWRAP_OPTIONAL_HPP
