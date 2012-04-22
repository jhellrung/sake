/*******************************************************************************
 * sake/core/utility/noncopy_assignable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_NONCOPY_ASSIGNABLE( T )
 * struct noncopy_assignable_base
 *
 * Use SAKE_NONCOPY_ASSIGNABLE to declare a class to be non-copy-assignable
 * (deleted or inaccessible copy assignment operator).  Similarly, derive a
 * class from noncopy_assignable_base to achieve the same effect.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NONCOPY_ASSIGNABLE_HPP
#define SAKE_CORE_UTILITY_NONCOPY_ASSIGNABLE_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>

#ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NONCOPY_ASSIGNABLE( T ) \
    public: void operator=(T&) = delete; \

#else // #ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NONCOPY_ASSIGNABLE( T ) \
    private: void operator=(T&); \
    public: typedef ::boost::true_type has_private_operator_assign_tag;

#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS

namespace sake
{

struct noncopy_assignable_base
{ SAKE_NONCOPY_ASSIGNABLE( noncopy_assignable_base ) };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_NONCOPY_ASSIGNABLE_HPP
