/*******************************************************************************
 * sake/core/move/move_if_noexcept.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * move_if_noexcept(T& x) -> result_of::move_if_noexcept< T& >::type
 * struct functional::move_if_noexcept
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_MOVE_IF_NOEXCEPT_HPP
#define SAKE_CORE_MOVE_MOVE_IF_NOEXCEPT_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

#include <sake/core/type_traits/has_copy_constructor.hpp>
#include <sake/core/type_traits/has_nothrow_move_constructor.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T >
struct move_if_noexcept;

template< class T >
struct move_if_noexcept< T& >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_rvalue_reference< T& >::value));
    typedef typename boost_ext::add_reference<
        typename boost::mpl::if_c<
            boost_ext::mpl::or2<
                sake::has_nothrow_move_constructor<T>,
                boost::mpl::not_< sake::has_copy_constructor<T> >
            >::value,
            typename boost_ext::add_rvalue_reference<T>::type,
            T
        >::type
    >::type type;
};

template< class T >
struct move_if_noexcept< T const & >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_rvalue_reference< T& >::value));
    typedef T const & type;
};

} // namespace result_of

namespace functional
{

struct move_if_noexcept
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::move_if_noexcept, 1 )

    template< class T >
    typename sake::result_of::move_if_noexcept< T& >::type
    operator()(T& x) const
    {
        typedef typename sake::result_of::move_if_noexcept< T& >::type result_type;
        return static_cast< result_type >(x);
    }

};

} // namespace functional

sake::functional::move_if_noexcept const move_if_noexcept = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MOVE_MOVE_IF_NOEXCEPT_HPP
