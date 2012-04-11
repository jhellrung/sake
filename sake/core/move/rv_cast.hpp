/*******************************************************************************
 * sake/core/move/rv_cast.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_RV_CAST( expression )
 *
 * In C++03, SAKE_RV_CAST( expression ) casts movable rvalues of type T to
 * boost::rv<T>&.  This is necessary to coerce generic forwarding functions to
 * capture rvalues as emulated rvalue references.  Non-movable rvalues are cast
 * to T const &, and lvalues are preserved.
 *
 * In C++11, SAKE_RV_CAST( expression ) expands to precisely ( expression ).
 *
 * Note: If expression accesses class members through this-> or by explicit
 * qualification, this might not work (MSVC9, at least, complains).  The
 * workaround is to bring class members into the current class scope with using
 * declarations, or assign "this" to a temporary variable and access members
 * through that temporary variable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_RV_CAST_HPP
#define SAKE_CORE_MOVE_RV_CAST_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_RV_CAST( expression ) ( expression )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#include <sake/core/move/move.hpp>

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_RV_CAST( expression ) \
    ::sake::rv_cast_private::impl( SAKE_EXPR_TYPE_TAG_OF( expression ), expression )

namespace sake
{

namespace rv_cast_private
{

template< class T >
inline T&
impl(sake::type_tag< T& >, T& x)
{ return x; }

template< class T >
inline typename sake::result_of::move<T>::type
impl(sake::type_tag<T>, typename sake::result_of::move<T>::type x)
{ return x; }

} // namespace rv_cast_private

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_RV_CAST_HPP
