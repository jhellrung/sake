/*******************************************************************************
 * core/utility/ostreamable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct ostreamable<T>
 * make_ostreamable(T const & x) -> ostreamable<T>
 * operator<<(std::ostream& o, ostreamable<T> x) -> std::ostream&
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

/*******************************************************************************
 * struct ostreamable<T>
 ******************************************************************************/

template< class T >
struct ostreamable
{
    T const & value;
    explicit ostreamable(T const & value_)
        : value(value_)
    { }
};

/*******************************************************************************
 * make_ostreamable(T const & x) -> ostreamable<T>
 ******************************************************************************/

template< class T >
inline ostreamable<T>
make_ostreamable(T const & x)
{ return ostreamable<T>(x); }

/*******************************************************************************
 * operator<<(std::ostream& o, ostreamable<T> x) -> std::ostream&
 ******************************************************************************/

namespace ostreamable_private
{

template< class T, bool = sake::is_ostreamable< T const & >::value >
struct operator_ostream_dispatch;

} // namespace ostreamable_private

template< class T >
inline std::ostream&
operator<<(std::ostream& o, ostreamable<T> x)
{ return ostreamable_private::operator_ostream_dispatch<T>::apply(o, x); }

namespace ostreamable_private
{

template< class T >
struct operator_ostream_dispatch< T, true >
{
    static std::ostream& apply(std::ostream& o, ostreamable<T> x)
    { return o << x.value; }
};

template< class T >
struct operator_ostream_dispatch< T, false >
{
    
    static std::ostream& apply(std::ostream& o, ostreamable<T> x)
    {
        void const * const address = static_cast< void const * >(sake::address_of(x.value));
        return o << '{' << typeid(T).name() << "@" << address << '}';
    }
};

} // namespace ostreamable_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_OSTREAMABLE_HPP
