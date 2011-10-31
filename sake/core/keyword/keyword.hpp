/*******************************************************************************
 * sake/core/keyword/keyword.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_KEYWORD_HPP
#define SAKE_CORE_KEYWORD_KEYWORD_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>

#include <sake/core/keyword/tagged_value.hpp>
#include <sake/core/keyword/tagged_lazy.hpp>

#define SAKE_KEYWORD( name ) \
namespace tag { struct name; } \
::sake::keyword::keyword_< tag::name > const BOOST_PP_CAT( _, name ) = { };

namespace sake
{

namespace keyword
{

template< class Tag >
struct keyword_
{
    typedef Tag tag;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    keyword::tagged_value< Tag, T&& >
    operator=(T&& value) const
    { return static_cast< T&& >(value); }

    template< class T >
    keyword::tagged_value< Tag, T&& >
    operator|(T&& value) const
    { return static_cast< T&& >(value); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    keyword::tagged_value< Tag, T& >
    operator=(T& value) const
    { return value; }

    template< class T >
    keyword::tagged_value< Tag, T const & >
    operator=(T const & value) const
    { return value; }

    template< class T >
    keyword::tagged_value< Tag, T& >
    operator|(T& value) const
    { return value; }

    template< class T >
    keyword::tagged_value< Tag, T const & >
    operator|(T const & value) const
    { return value; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    keyword::tagged_lazy< Tag, T >
    operator||(T const & value) const
    { return keyword::tagged_lazy< Tag, T >(value); }
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_KEYWORD_HPP
