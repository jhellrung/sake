/*******************************************************************************
 * sake/core/move/has_move_emulation.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * [only for C++03]
 * struct has_move_emulation<T>
 *
 * has_move_emulation is a Boost.MPL metafunction that determines if the given
 * type has move emulation enabled.
 *
 * Implementation notes:
 * - Since this is widely used (indirectly) throughout Sake, even among some
 *   lower-level components, we aim to minimize dependencies.
 * - MSVC9 seems to have some strange, unpredictable, and not easily replicable
 *   issues with the use of has_mem_fun_operator, so we fall back to testing for
 *   convertibility to boost::rv<T>&.
 * - GCC <= 4.6.3 has issues with has_mem_fun_operator when the target type has
 *   the __may_alias__ attribute, so we fall back to testing for convertibility
 *   to boost::rv<T>&.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_HAS_MOVE_EMULATION_HPP
#define SAKE_CORE_MOVE_HAS_MOVE_EMULATION_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

template< class T >
struct has_move_emulation;

template< class T >
struct has_move_emulation< T const >
    : boost::false_type
{ };

template< class T >
struct has_move_emulation< T& >
    : boost::false_type
{ };

} // namespace sake

#if SAKE_MSC_VERSION <= 1500 \
 || SAKE_GNUC_VERSION <= SAKE_GNUC_VERSION_OF(4,6,3)

#include <sake/core/utility/true_false_tag.hpp>

namespace sake
{

namespace has_move_emulation_private
{

template< class T, bool = boost::is_class<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, false >
    : boost::false_type
{ };

template< class T >
struct dispatch< T, true >
{
private:
    static T& ref();
    static sake::true_tag test(boost::rv<T>&);
    static sake::false_tag test(...);
public:
    static bool const value = SAKE_SIZEOF_TRUE_TAG == sizeof( test(ref()) );
    typedef dispatch type;
};

} // namespace has_move_emulation_private

template< class T >
struct has_move_emulation
    : has_move_emulation_private::dispatch<T>
{ };

} // namespace sake

#else // #if XXX_VERSION <= ...

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_mem_fun_operator.hpp>

namespace sake
{

template< class T >
struct has_move_emulation
    : boost_ext::mpl::and2<
          boost::is_class<T>,
          sake::has_mem_fun_operator< T, boost::rv<T>& >
      >
{ };

} // namespace sake

#endif // #if XXX_VERSION <= ...

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_HAS_MOVE_EMULATION_HPP
