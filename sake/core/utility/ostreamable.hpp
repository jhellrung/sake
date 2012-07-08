/*******************************************************************************
 * sake/core/utility/ostreamable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct ostreamable<T>
 * construct::ostreamable(T const & x) -> ostreamable<T>
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

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/is_ostreamable.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

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
 * construct::ostreamable(T const & x) -> ostreamable<T>
 ******************************************************************************/

namespace construct
{

namespace result_of
{

template< class T >
struct ostreamable
{ typedef sake::ostreamable<
    typename boost_ext::remove_qualifiers<T>::type > type; };

} // namespace result_of

namespace functional
{

struct ostreamable
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::construct::result_of::ostreamable, 1 )

    template< class T >
    inline sake::ostreamable<T>
    operator()(T const & x) const
    { return sake::ostreamable<T>(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace ostreamable_adl_barrier
{ sake::construct::functional::ostreamable const ostreamable = { }; }
using namespace ostreamable_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::construct::functional::ostreamable const ostreamable = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace construct

/*******************************************************************************
 * operator<<(std::ostream& o, ostreamable<T> x) -> std::ostream&
 ******************************************************************************/

namespace ostreamable_private
{

template< class T >
inline std::ostream&
dispatch(std::ostream& o, T const & x, sake::int_tag<1>)
{ return o << x; }

template< class T >
inline std::ostream&
dispatch(std::ostream& o, T const & x, sake::int_tag<0>)
{
    void const * const p = sake::address_of(x);
    return o << '{' << typeid( T ).name() << "@" << p << '}';
}

template< class T, bool = sake::is_ostreamable< T const & >::value >
struct operator_ostream_dispatch;

} // namespace ostreamable_private

template< class T >
inline std::ostream&
operator<<(std::ostream& o, sake::ostreamable<T> x)
{
    typedef sake::int_tag< sake::is_ostreamable< T const & >::value > int_tag_;
    return ostreamable_private::dispatch(o, x.value, int_tag_());
}

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_OSTREAMABLE_HPP
