/*******************************************************************************
 * sake/core/iterator/is_const_iterator.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_const_iterator<T>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP

#include <boost/iterator/iterator_traits.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

template< class T >
struct is_const_iterator
{
    typedef typename boost::iterator_reference<T>::type reference;
    typedef typename boost_ext::remove_qualifiers<
        typename boost::iterator_value<T>::type
    >::type value_type;
    static bool const value =
        boost_ext::is_convertible< reference, value_type const & >::value
    && !boost_ext::is_convertible< reference, value_type& >::value;
    typedef is_const_iterator type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP
