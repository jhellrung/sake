/*******************************************************************************
 * sake/core/utility/non_copy_assignable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_NON_COPY_ASSIGNABLE( T )
 * struct non_copy_assignable_base
 *
 * Use SAKE_NON_COPY_ASSIGNABLE to declare a class to be non-copy-assignable
 * (deleted or inaccessible copy assignment operator).  Similarly, derive a
 * class from non_copy_assignable_base to achieve the same effect.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NON_COPY_ASSIGNABLE_HPP
#define SAKE_CORE_UTILITY_NON_COPY_ASSIGNABLE_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>

#ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NON_COPY_ASSIGNABLE( T ) \
    public: void operator=(T&) = delete; \

#else // #ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NON_COPY_ASSIGNABLE( T ) \
    private: void operator=(T&); \
    public: typedef ::boost::true_type has_private_operator_assign;

#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS

namespace sake
{

struct non_copy_assignable_base
{ SAKE_NON_COPY_ASSIGNABLE( non_copy_assignable_base ) };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_NON_COPY_ASSIGNABLE_HPP
