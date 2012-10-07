/*******************************************************************************
 * sake/core/iterator/basic/empty.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_BASIC_EMPTY_HPP
#define SAKE_CORE_ITERATOR_BASIC_EMPTY_HPP

#include <boost/mpl/map/map10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/has_qualifier.hpp>

#include <sake/core/iterator/basic/fwd.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace basic
{

namespace empty_private
{

template< class T >
struct traits;

} // namespace empty_private

template< class T >
class empty
    : public empty_private::traits<T>::facade_
{
    typedef empty_private::traits<T> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:

    typedef boost::true_type has_trivial_default_constructor_tag;
    typedef boost::true_type has_trivial_copy_constructor_tag;
    typedef boost::true_type has_trivial_copy_assign_tag;
    typedef boost::true_type has_trivial_destructor_tag;

    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );

private:
    friend class sake::iterator::core_access;

    reference derived_dereference() const
    {
        SAKE_REQUIRE((false));
        return *static_cast< T* >(0);
    }

    void derived_increment()
    { SAKE_REQUIRE((false)); }
    void derived_decrement()
    { SAKE_REQUIRE((false)); }

    template< class U >
    bool derived_equal(U) const
    { return true; }

    void derived_advance_ip(difference_type const n)
    { SAKE_ASSERT_RELATION( n, ==, sake::zero ); }
    empty derived_advance(difference_type const n)
    {
        SAKE_ASSERT_RELATION( n, ==, sake::zero );
        return empty();
    }

    template< class U >
    difference_type derived_difference(U) const
    { return static_cast< difference_type >(0); }

    template< class U >
    bool derived_less(U) const
    { return false; }
    template< class U >
    bool derived_less_equal(U) const
    { return true; }
    template< class U >
    sake::sign_t derived_cmp(U) const
    { return sake::sign_t::zero_tag(); }

    template< class Introterminal >
    struct derived_relax
    { typedef empty type; };

    template< class U >
    void derived_at_ip(U)
    { }
    template< class U, class Introterminal >
    empty derived_at(U, Introterminal) const
    { return empty(); }
};

namespace empty_private
{

template< class T >
struct traits
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));
    typedef sake::iterator::facade<
        sake::iterator::basic::empty<T>,
        boost::mpl::map3<
            sake::iterator::keyword::value<T>,
            sake::iterator::keyword::traversal<
                boost::random_access_traversal_tag >,
            sake::iterator::keyword::introterminal<
                sake::begin_access_end_access_introterminal_tag >,
        >
    > facade_;
};

} // namespace empty_private

} // namespace basic

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_BASIC_EMPTY_HPP
