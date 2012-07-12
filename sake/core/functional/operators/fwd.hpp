/*******************************************************************************
 * sake/core/functional/operators/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_FWD_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_FWD_HPP

namespace sake
{

namespace operators
{

namespace functional
{

struct assign;
struct bit_and;
struct bit_and_assign;
struct bit_or;
struct bit_or_assign;
struct bit_xor;
struct bit_xor_assign;
struct bracket;
struct complement;
struct divide;
struct divide_assign;
struct equal;
struct greater;
struct greater_equal;
struct less;
struct less_equal;
struct logical_and;
struct logical_not;
struct logical_or;
struct minus;
struct minus_assign;
struct modulus;
struct modulus_assign;
struct multiply;
struct multiply_assign;
struct not_equal;
struct plus;
struct plus_assign;
struct post_decrement;
struct post_increment;
struct pre_decrement;
struct pre_increment;
struct shift_left;
struct shift_left_assign;
struct shift_right;
struct shift_right_assign;
struct star;
struct unary_minus;
struct unary_plus;

typedef logical_and and_;
typedef logical_not not_;
typedef logical_or or_;

} // namespace functional

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_FWD_HPP
