/*******************************************************************************
 * sake/boost_ext/type_traits/enclosing_is_const_copy_assignable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::enclosing_is_const_copy_assignable<T>
 *
 * This is a metafunction which determines whether a class enclosing T would
 * have an implicitly-defined copy assignment operator taking its parameter as
 * reference-to-const.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ENCLOSING_IS_CONST_COPY_ASSIGNABLE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ENCLOSING_IS_CONST_COPY_ASSIGNABLE_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/utility/has_private_operator_assign.hpp>
#include <sake/core/utility/convertible_from_any.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/true_false_tag.hpp>

namespace sake
{

namespace boost_ext
{

namespace enclosing_is_const_copy_assignable_private
{

template< class T, bool = sake::has_private_operator_assign<T>::value >
class dispatch;

template< class T >
class dispatch< T, true >
    : public boost::false_type
{ };

struct const_copy_assignable_detector { };
sake::true_tag  is_const_copy_assignable_detector(const_copy_assignable_detector);
sake::false_tag is_const_copy_assignable_detector(...);

template< class T >
class dispatch< T, false >
{
    struct detector
    {
        T dummy;
        const_copy_assignable_detector operator=(sake::convertible_from_any) const;
    };
public:
    static bool const value =
        SAKE_SIZEOF_FALSE_TAG
     == sizeof( is_const_copy_assignable_detector(
            sake::declval< detector& >() = sake::declval< detector const & >() ) );
    typedef dispatch type;
};

} // namespace enclosing_is_const_copy_assignable_private

template< class T >
struct enclosing_is_const_copy_assignable
    : enclosing_is_const_copy_assignable_private::dispatch<T>
{ };

template< class T >
struct enclosing_is_const_copy_assignable< T const >
    : boost::false_type
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ENCLOSING_IS_CONST_COPY_ASSIGNABLE_HPP
