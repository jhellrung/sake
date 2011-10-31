/*******************************************************************************
 * sake/core/utility/non_copyable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_NON_COPYABLE( T )
 * struct non_copyable_base
 *
 * Use SAKE_NON_COPYABLE to declare a class to be non-copyable (deleted or
 * inaccessible copy constructor and copy assignment operator).  Similarly,
 * derive a class from non_copyable_base to achieve the same effect.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NON_COPYABLE_HPP
#define SAKE_CORE_UTILITY_NON_COPYABLE_HPP

#include <boost/config.hpp>

#include <sake/core/utility/implicitly_defined/default_ctor.hpp>

#ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NON_COPYABLE( T ) \
    public: T(T&) = delete; \
    public: void operator=(T&) = delete;

#else // #ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NON_COPYABLE( T ) \
    private: T(T&); \
    private: void operator=(T&); \
    public: static bool const has_private_operator_assign = true;

#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS

namespace sake
{

struct non_copyable_base
{
protected:
    SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( non_copyable_base )
    SAKE_NON_COPYABLE( non_copyable_base )
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_NON_COPYABLE_HPP
