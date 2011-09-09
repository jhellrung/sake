/*******************************************************************************
 * core/utility/ostreamable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct ostreamable<T>
 * make_ostreamable(const T& x) -> ostreamable<T>
 *
 * ostreamable<T> wraps a reference to T.  Its operator<< overload to a
 * std::ostream is defined such that if T ostreamable, then the wrapped
 * reference is streamed, else typeid(T).name() and the address of the wrapped
 * reference is streamed.
 *
 * This mostly exists for logging and debugging purposes.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_OSTREAMABLE_HPP
#define SAKE_CORE_UTILITY_OSTREAMABLE_HPP

#include <ostream>
#include <typeinfo>

#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/is_ostreamable.hpp>
#include <sake/core/utility/address_of.hpp>

namespace sake
{

template< class T >
struct ostreamable
{
    T const & value;
    explicit ostreamable(T const & value_)
        : value(value_)
    { }
};

template< class T >
typename boost::enable_if_c<
    sake::is_ostreamable< T const & >::value,
    std::ostream&
>::type
operator<<(std::ostream& out, ostreamable<T> x)
{ return out << x.value; }

template< class T >
typename boost::disable_if_c<
    sake::is_ostreamable< T const & >::value,
    std::ostream&
>::type
operator<<(std::ostream& out, ostreamable<T> x)
{
    return out << '{'
               << typeid(T).name()
               << " @ "
               << static_cast< void const * >(sake::address_of(x.value))
               << '}';
}

template< class T >
inline ostreamable<T>
make_ostreamable(T const & x)
{ return ostreamable<T>(x); }

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_OSTREAMABLE_HPP
