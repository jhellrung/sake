/*******************************************************************************
 * sake/core/iterator/private/adaptor/member_storage.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_STORAGE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_STORAGE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/not.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/type_traits/has_default_constructor.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template< class Traversal, class T >
struct member_storage_bool
{
    static bool const value = boost_ext::mpl::and2<
        boost_ext::mpl::or2<
            boost::mpl::not_< boost_ext::is_convertible<
                Traversal, boost::forward_traversal_tag > >,
            sake::has_default_constructor<T>
        >,
        sake::has_operator_assign< T&, void ( T const & ) >
    >::value;
};

template< class Traversal, class T >
struct member_storage_bool< Traversal, T& >
{ static bool const value = false; };

template<
    class Traversal, class T,
    bool = member_storage_bool< Traversal, T >::value
>
struct member_storage;

template< class Traversal, class T >
struct member_storage< Traversal, T, false >
{
    typedef sake::optional<T> type;
    static typename boost_ext::add_reference<T>::type
    get(type& x)
    { return x.get(); }
    static typename boost_ext::add_reference_add_const<T>::type
    get(type const & x)
    { return x.get(); }
};

template< class Traversal, class T >
struct member_storage< Traversal, T, true >
{
    typedef T type;
    static T&
    get(T& x)
    { return x; }
    static T const &
    get(T const & x)
    { return x; }
};

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_STORAGE_HPP
