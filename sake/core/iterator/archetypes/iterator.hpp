/*******************************************************************************
 * sake/core/iterator/archetypes/iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct archetypes::iterator< T, Traversal, Introversal, Access >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_ITERATOR_HPP

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_const_if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/private/introversal_base.hpp>
#include <sake/core/iterator/archetypes/private/operator_bracket_base.hpp>
#include <sake/core/iterator/archetypes/private/pointer_dispatch.hpp>
#include <sake/core/iterator/archetypes/private/reference_dispatch.hpp>
#include <sake/core/iterator/archetypes/private/traversal_base.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/category.hpp>

namespace sake
{

namespace archetypes
{

template< class T, class Traversal, class Introversal, class Access >
struct iterator
    : sake::archetypes::iterator_private::traversal_base<
          sake::archetypes::iterator< T, Traversal, Introversal, Access >,
          Traversal
      >,
      sake::archetypes::iterator_private::introversal_base<
          sake::archetypes::iterator< T, Traversal, Introversal, Access >,
          Introversal, Traversal
      >,
      sake::archetypes::iterator_private::operator_bracket_base<
          T, Traversal, Access >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    typedef T value_type;
private:
    typedef typename boost_ext::add_const_if_c<
        !boost_ext::is_convertible<
            Access,
            sake::iterator::archetypes::writable_tag
        >::value,
        T
    >::type const_value_type;
    typedef sake::archetypes::iterator_private::reference_dispatch<
        const_value_type, Access
    > reference_dispatch_;
    typedef sake::archetypes::iterator_private::pointer_dispatch<
        typename reference_dispatch_::type
    > pointer_dispatch_;
public:

    typedef typename reference_dispatch_::type reference;
    typedef typename pointer_dispatch_::type pointer;

    typedef Traversal iterator_traversal;
    typedef Introversal iterator_introversal;

    typedef typename sake::iterator::private_::category<
        Traversal, const_value_type
    >::type iterator_category;

    reference operator*() const
    { return reference_dispatch_::apply(); }
    pointer operator->() const
    { return pointer_dispatch_::apply(); }
};

template< class T, class Traversal, class Introversal, class Access >
inline typename boost::enable_if_c< boost_ext::is_convertible<
    Access, sake::iterator::archetypes::swappable_tag >::value >::type
iter_swap(
    sake::archetypes::iterator< T, Traversal, Introversal, Access >,
    sake::archetypes::iterator< T, Traversal, Introversal, Access >)
{ }

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_ITERATOR_HPP
