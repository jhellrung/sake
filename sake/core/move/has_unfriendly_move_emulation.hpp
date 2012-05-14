/*******************************************************************************
 * sake/core/move/has_unfriendly_move_emulation.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * [only for C++03]
 * struct has_unfriendly_move_emulation<T>
 *
 * This is a metafunction which evaluates to true only if T has "unfriendly"
 * move emulation enabled.  An "unfriendly" type is one which induces an
 * implicitly declared copy assignment operator taking a reference-to-non-const
 * within an enclosing class (one which derives from or contains T).  In the
 * absence of rvalue references, any type which enables move emulation via
 * SAKE_OPTIMAL_MOVABLE_COPYABLE* will, unfortunately, be unfriendly.
 *
 * has_unfriendly_move_emulation depends on the correctness of
 * has_move_emulation_and_copyable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_HAS_UNFRIENDLY_MOVE_EMULATION_HPP
#define SAKE_CORE_MOVE_HAS_UNFRIENDLY_MOVE_EMULATION_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/not.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/enclosing_is_const_copy_assignable.hpp>

#include <sake/core/move/has_move_emulation_and_copyable.hpp>

namespace sake
{

template< class T >
struct has_unfriendly_move_emulation
    : boost_ext::mpl::and2<
          sake::has_move_emulation_and_copyable<T>,
          boost::mpl::not_< boost_ext::enclosing_is_const_copy_assignable<T> >
      >
{ };

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_HAS_UNFRIENDLY_MOVE_EMULATION_HPP
