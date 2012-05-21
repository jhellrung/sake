/*******************************************************************************
 * sake/core/functional/default_construct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * default_construct<T>() -> T
 * struct functional::default_construct<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_DEFAULT_CONSTRUCT_HPP
#define SAKE_CORE_FUNCTIONAL_DEFAULT_CONSTRUCT_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

namespace sake
{

namespace functional
{

template< class T >
struct default_construct
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    typedef T result_type;

    T operator()() const
    { return T(); }
};

template<>
struct default_construct< void >
{
    typedef void result_type;
    void operator()() const { }
};

} // namespace functional

template< class T >
inline T
default_construct()
{ return sake::functional::default_construct<T>()(); }

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_DEFAULT_CONSTRUCT_HPP
