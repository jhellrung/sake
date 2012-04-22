/*******************************************************************************
 * sake/core/utility/noncopyable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_NONCOPYABLE( T )
 * struct noncopyable_base
 *
 * Use SAKE_NONCOPYABLE to declare a class to be non-copyable (deleted or
 * inaccessible copy constructor and copy assignment operator).  Similarly,
 * derive a class from noncopyable_base to achieve the same effect.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NONCOPYABLE_HPP
#define SAKE_CORE_UTILITY_NONCOPYABLE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/utility/memberwise/default_constructor.hpp>

#ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NONCOPYABLE( T ) \
    public: T(T&) = delete; \
    public: void operator=(T&) = delete;

#else // #ifndef BOOST_NO_DELETED_FUNCTIONS

#define SAKE_NONCOPYABLE( T ) \
    private: T(T&); \
    private: void operator=(T&); \
    public: typedef ::boost::true_type has_private_operator_assign_tag;

#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS

namespace sake
{

struct noncopyable_base
{
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR( noncopyable_base, BOOST_PP_SEQ_NIL )
    SAKE_NONCOPYABLE( noncopyable_base )
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_NONCOPYABLE_HPP
