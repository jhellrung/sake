/*******************************************************************************
 * sake/core/math/inverse.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct inverse<T>
 *
 * Defines a simple wrapper class representing the (multiplicative) inverse of
 * a number.
 *
 * TODO: Consider making this fancier to account for UDTs...?
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERSE_HPP
#define SAKE_CORE_MATH_INVERSE_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/math/inv_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct inverse<T>
 ******************************************************************************/

template< class T >
struct inverse
{
    typedef T value_type;

    explicit inverse(T const & value_)
        : m_value(value_)
    { }

    T const & value() const
    { return m_value; }

    template< class U >
    operator U() const
    { return static_cast<U>(1) / static_cast<U>(m_value); }

    T inv() const
    { return m_value; }

private:
    T m_value;
};

/*******************************************************************************
 * struct is_inverse<T>
 ******************************************************************************/

template< class T >
struct is_inverse
    : boost::false_type
{ };

template< class T >
struct is_inverse< inverse<T> >
    : boost::true_type
{ };

/*******************************************************************************
 * struct result_of::extension::inv< inverse<T> >
 ******************************************************************************/

namespace result_of {
namespace extension {

template< class T >
struct inv< T,
    typename boost::enable_if_c<
        sake::is_inverse< typename boost_ext::remove_qualifiers<T>::type >::value
    >::type
>
{ typedef typename boost_ext::remove_qualifiers<T>::type::value_type type; };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERSE_HPP
