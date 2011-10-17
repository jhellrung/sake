/*******************************************************************************
 * core/math/advance.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * advance(T& x, D n) -> T&
 *
 * advance essentially just extends std::advance to integral types.
 *
 * advance is a BISFO; see docs/bisfo.txt.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ADVANCE_HPP
#define SAKE_CORE_MATH_ADVANCE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/math/arithmetic_operators.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/bisfo.hpp>

namespace sake
{

namespace functional
{

struct advance
{
private:
    template< class T, class D, class PlusResult = typename result_of::plus<T,D>::type >
    struct dispatch_on_plus_result
    {
        static T& apply(T& x, const D& n)
        { return x = static_cast<T>(x + n); }
    };

    template< class T, class D >
    struct dispatch_on_plus_result<T,D,T>
    {
        static T& apply(T& x, const D& n)
        { return x += n; }
    };

    template< class T, class D, bool = is_iterator<T>::value >
    struct dispatch_on_iterator
    {
        static T& apply(T& x, const D& n)
        { return dispatch_on_plus_result<T,D>::apply(x, n); }
    };

    template< class T, class D >
    struct dispatch_on_iterator< T, D, true >
    {
        static T& apply(T& x, D n, boost::incrementable_traversal_tag)
        {
            SAKE_ASSERT_RELATION( n, >=, sake::zero );
            if(sake::zero < n)
                do {
                    ++x;
                } while(!(--n == sake::zero));
            return x;
        }

        static T& apply(T& x, D n, boost::bidirectional_traversal_tag)
        {
            if(sake::zero < n)
                do {
                    ++x;
                } while(!(--n == sake::zero));
            else if(n < sake::zero)
                do {
                    --x;
                } while(!(++n == sake::zero));
            return x;
        }

        static T& apply(T& x, const D& n, boost::random_access_traversal_tag)
        { return x += n; }

        static T& apply(T& x, const D& n)
        { return apply(x, n, typename boost::iterator_traversal<T>::type()); }
    };
public:

    template<class> struct result;

    template< class This, class T, class D >
    struct result< This ( T&, D ) >
    { typedef T& type; };

    template< class T, class D >
    T& operator()(T& x, const D& n) const
    { return dispatch_on_iterator<T,D>::apply(x,n); }
};

} // namespace functional

SAKE_BISFO( advance )

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ADVANCE_HPP
