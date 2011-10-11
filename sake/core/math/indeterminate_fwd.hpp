/*******************************************************************************
 * sake/core/math/indeterminate_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * This header is necessary since both fuzzy_sign_t and
 * functional::indeterminate use each other as by-value parameters to member
 * functions.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INDETERMINATE_FWD_HPP
#define SAKE_CORE_MATH_INDETERMINATE_FWD_HPP

#include <boost/logic/tribool.hpp>

#include <sake/core/utility/explicit_operator_bool.hpp>

namespace sake
{

struct sign_t;
struct fuzzy_sign_t;

namespace functional
{

struct indeterminate
{
    typedef bool result_type;
    bool operator()(sake::sign_t const s) const;
    bool operator()(int const s) const; // s in { -1, 0, +1 }
    bool operator()(bool const b) const;
    bool operator()(sake::fuzzy_sign_t const s) const;
    bool operator()(boost::logic::tribool const b) const;
    bool operator()(indeterminate) const;

    operator boost::logic::tribool() const;
    SAKE_EXPLICIT_OPERATOR_BOOL() const { return false; }
    bool operator!() const;
};

#define declare_equality_operators( lhs, rhs ) \
inline indeterminate operator==(lhs, rhs); \
inline indeterminate operator!=(lhs, rhs);
declare_equality_operators( indeterminate, indeterminate )
declare_equality_operators( indeterminate, bool )
declare_equality_operators( bool, indeterminate )
declare_equality_operators( indeterminate, boost::logic::tribool )
declare_equality_operators( boost::logic::tribool, indeterminate )
#undef declare_equality_operators

inline indeterminate operator&&(indeterminate, indeterminate);
inline indeterminate operator||(indeterminate, indeterminate);

#define declare_logical_operators( lhs, rhs ) \
inline boost::logic::tribool operator&&(lhs, rhs); \
inline boost::logic::tribool operator||(lhs, rhs);
declare_logical_operators( indeterminate, bool const b )
declare_logical_operators( bool const b, indeterminate )
declare_logical_operators( indeterminate, boost::logic::tribool const b )
declare_logical_operators( boost::logic::tribool const b, indeterminate )
#undef declare_logical_operators

} // namespace functional

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INDETERMINATE_FWD_HPP
