/*******************************************************************************
 * sake/core/functional/identity.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * identity(T&& x) -> T&&
 * struct functional::identity
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_IDENTITY_HPP
#define SAKE_CORE_FUNCTIONAL_IDENTITY_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>

namespace sake
{

namespace functional
{

struct identity
{
    template< class > struct result;
    template< class This, class T >
    struct result< This ( T ) >
        : boost_ext::add_rvalue_reference<T>
    { };

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    T&& operator()(T&& x) const
    { return static_cast< T&& >(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    T& operator()(T& x) const
    { return x; }

    template< class T >
    T const & operator()(T const & x) const
    { return x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

functional::identity const identity = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_IDENTITY_HPP
