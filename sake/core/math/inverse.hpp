/*******************************************************************************
 * sake/core/math/inverse.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct inverse<T>
 *
 * Defines a simple wrapper class representing the (multiplicative) inverse of
 * a number.
 *
 * TODO: Consider making this fancier to account for UDTs...?
 * TODO: Make inv available via ADL to allow efficient moving from rvalues.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERSE_HPP
#define SAKE_CORE_MATH_INVERSE_HPP

#include <sake/core/math/inv_fwd.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/define_natural/mem_fun.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct inverse<T>
 ******************************************************************************/

template< class T >
struct inverse
{
    typedef T value_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename inverse, ( T ) )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        inverse,
        ( move_ctor ) ( copy_assign ) ( move_assign ) ( swap ),
        BOOST_PP_SEQ_NIL, (( T, m_value ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< U >
    explicit inverse(U&& value_,
        typename boost::disable_if_c<
            boost::is_base_of<
                inverse,
                typename boost_ext::remove_qualifiers<U>::type
            >::value
        >::type* = 0)
        : m_value(emplacer_construct<T>(sake::forward<U>(value_))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    explicit inverse(T const & value_)
        : m_value(value_)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    T const & value() const
    { return m_value; }

    template< class U >
    operator U() const
    { return sake::one.as<U>() / static_cast<U>(m_value); }

    T inv() const
    { return m_value; }

private:
    T m_value;
};

/*******************************************************************************
 * struct result_of::extension::inv< inverse<T> >
 ******************************************************************************/

namespace result_of {
namespace extension {

template< class T >
struct inv< sake::inverse<T>, void >
{ typedef T type; };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERSE_HPP
