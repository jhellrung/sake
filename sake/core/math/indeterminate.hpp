/*******************************************************************************
 * sake/core/math/indeterminate.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * indeterminate(sign_t s) -> bool
 * indeterminate(bool b) -> bool
 * indeterminate(fuzzy_sign_t s) -> bool
 * indeterminate(boost::logic::tribool b) -> bool
 * struct functional::indeterminate
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INDETERMINATE_HPP
#define SAKE_CORE_MATH_INDETERMINATE_HPP

#include <boost/logic/tribool.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/indeterminate_fwd.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/sign_t.hpp>

namespace sake
{

namespace functional
{

inline bool
indeterminate::
operator()(sake::sign_t const) const
{ return false; }

inline bool
indeterminate::
operator()(int const) const
{ return false; }

inline bool
indeterminate::
operator()(bool const) const
{ return false; }

inline bool
indeterminate::
operator()(sake::fuzzy_sign_t const s) const
{ return s.m_value == sake::fuzzy_sign_t::indeterminate_sign_value; }

inline bool
indeterminate::
operator()(boost::logic::tribool const b) const
{ return boost::logic::indeterminate(b); }

inline
indeterminate::
operator boost::logic::tribool() const
{ return boost::logic::tribool(boost::logic::indeterminate); }

inline bool
indeterminate::
operator!() const
{ return false; }

#define define_equality_operators( lhs, rhs ) \
inline indeterminate operator==(lhs, rhs) { return indeterminate(); } \
inline indeterminate operator!=(lhs, rhs) { return indeterminate(); }
define_equality_operators( indeterminate, indeterminate )
define_equality_operators( indeterminate, bool )
define_equality_operators( bool, indeterminate )
define_equality_operators( indeterminate, boost::logic::tribool )
define_equality_operators( boost::logic::tribool, indeterminate )
#undef define_equality_operators

inline indeterminate operator&&(indeterminate, indeterminate) { return indeterminate(); }
inline indeterminate operator||(indeterminate, indeterminate) { return indeterminate(); }

inline boost::logic::tribool operator&&(indeterminate, bool const b)
{ return b ? boost::logic::tribool(boost::logic::indeterminate) : boost::logic::tribool(false); }
inline boost::logic::tribool operator||(indeterminate, bool const b)
{ return b ? boost::logic::tribool(true) : boost::logic::tribool(boost::logic::indeterminate); }
inline boost::logic::tribool operator&&(bool const b, indeterminate)
{ return b ? boost::logic::tribool(boost::logic::indeterminate) : boost::logic::tribool(false); }
inline boost::logic::tribool operator||(bool const b, indeterminate)
{ return b ? boost::logic::tribool(true) : boost::logic::tribool(boost::logic::indeterminate); }

inline boost::logic::tribool operator&&(indeterminate, boost::logic::tribool const b)
{ return !b ? boost::logic::tribool(false) : boost::logic::tribool(boost::logic::indeterminate); }
inline boost::logic::tribool operator||(indeterminate, boost::logic::tribool const b)
{ return b ? boost::logic::tribool(true) : boost::logic::tribool(boost::logic::indeterminate); }
inline boost::logic::tribool operator&&(boost::logic::tribool const b, indeterminate)
{ return !b ? boost::logic::tribool(false) : boost::logic::tribool(boost::logic::indeterminate); }
inline boost::logic::tribool operator||(boost::logic::tribool const b, indeterminate)
{ return b ? boost::logic::tribool(true) : boost::logic::tribool(boost::logic::indeterminate); }

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace indeterminate_adl_barrier
{ sake::functional::indeterminate const indeterminate = { }; }
using namespace indeterminate_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::indeterminate const indeterminate = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INDETERMINATE_HPP
