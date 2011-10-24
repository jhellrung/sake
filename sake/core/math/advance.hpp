/*******************************************************************************
 * sake/core/math/advance.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * advance(T& x, D const & n) -> T&
 *
 * Essentially just extends std::advance to integral types.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ADVANCE_HPP
#define SAKE_CORE_MATH_ADVANCE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>

#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T, class D >
struct advance
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference<T>::value));
    typedef T type;
};

} // namespace result_of

namespace functional
{

struct advance
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::advance, 2 )

    template< class T, class D >
    T& operator()(T& x, D const & n) const
    { return advance_private::dispatch<T,D>::apply(x, n); }
};

} // namespace functional

functional::advance const advance = { };

namespace advance_private
{

template< class T, class D >
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 2) * sake::is_iterator<T>::value
      | (1 << 1) * (boost::is_integral<T>::value
                 && boost::is_integral<D>::value
                 && (sizeof( T ) < sizeof( D )))
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T, class D >
struct dispatch<T,D,2>
{
    static T&
    apply(T& i, D n, boost::incrementable_traversal_tag)
    {
        SAKE_ASSERT_RELATION( n, >=, sake::zero );
        if(n != 0)
            do { ++i; } while(!(--n == 0));
        return i;
    }

    static T&
    apply(T& i, D n, boost::bidirectional_traversal_tag)
    {
        if(n > 0)
            do { ++i; } while(!(--n == 0));
        else if(n < 0)
            do { --i; } while(!(++n == 0));
        return i;
    }

    static T&
    apply(T& i, D const n, boost::random_access_traversal_tag)
    { return i += n; }

    static T&
    apply(T& i, D const n)
    { return apply(i, n, typename boost::iterator_traversal<T>::type()); }
};

template< class T, class D >
struct dispatch<T,D,1>
{
    static T&
    apply(T& x, D const & n)
    { return x = static_cast<T>(x + n); }
};

template< class T, class D >
struct dispatch<T,D,0>
{
    static T&
    apply(T& x, D const & n)
    { return x += n; }
};

} // namespace advance_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ADVANCE_HPP
