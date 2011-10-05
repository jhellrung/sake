/*******************************************************************************
 * sake/core/move/as_lvalue.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_AS_LVALUE( x )
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_AS_LVALUE_HPP
#define SAKE_CORE_MOVE_AS_LVALUE_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_lvalue_reference.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_AS_LVALUE( x ) x

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_AS_LVALUE( x ) ::sake::as_lvalue( x )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace sake
{

template< class T >
inline typename boost_ext::add_lvalue_reference<T>::type
as_lvalue(T& x)
{ return x; }

} // namespace sake

#endif // #ifndef SAKE_CORE_MOVE_AS_LVALUE_HPP
