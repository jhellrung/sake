/*******************************************************************************
 * sake/core/utility/forwarding_unary_function.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_FORWARDING_UNARY_FUNCTION[_R]( [r,] T, V, body )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_FORWARDING_UNARY_FUNCTION_HPP
#define SAKE_CORE_UTILITY_FORWARDING_UNARY_FUNCTION_HPP

#include <boost/config.hpp>
// #include <boost/mpl/vector/vector10.hpp>
// #include <boost/type_traits/is_base_of.hpp>
// #include <boost/type_traits/remove_const.hpp>
// #include <boost/preprocessor/cat.hpp>
// #include <boost/preprocessor/detail/is_unary.hpp>
// #include <boost/preprocessor/punctuation/comma_if.hpp>
// #include <boost/preprocessor/repetition/deduce_r.hpp>
// #include <boost/preprocessor/seq/enum.hpp>
// #include <boost/preprocessor/seq/for_each_i.hpp>
// #include <boost/preprocessor/seq/size.hpp>
// #include <boost/preprocessor/tuple/elem.hpp>
// #include <boost/utility/enable_if.hpp>

// #include <sake/boost_ext/mpl/all.hpp>
// #include <sake/boost_ext/mpl/any.hpp>
// #include <sake/boost_ext/mpl/or.hpp>
// #include <sake/boost_ext/preprocessor/keyword/cv.hpp>
// #include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
// #include <sake/boost_ext/type_traits/is_convertible.hpp>
// #include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
// #include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>
// #include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

// #include <sake/core/move/any_rv.hpp>
// #include <sake/core/move/as_lvalue.hpp>
// #include <sake/core/move/forward.hpp>
// #include <sake/core/move/movable.hpp>
// #include <sake/core/utility/call_traits.hpp>
// #include <sake/core/utility/void.hpp>

namespace sake
{

#define SAKE_FORWARDING_UNARY_FUNCTION( value_types, body ) \
    SAKE_FORWARDING_UNARY_FUNCTION_R( \
        BOOST_PP_DEDUCE_R(), \
        class_name, \
        value_types, \
        body \
    )

#ifndef BOOST_NO_RVALUE_REFERENCES


#else // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace forwarding_unary_function_private
{

template< class T >
struct rparam_type
{
    typedef typename sake::call_traits<
        typename boost_ext::add_rvalue_reference<T>::type
    >::param_type type;
};

template< class T >
struct is_same_sans_qualifiers_as
{
    template< class U >
    struct apply
        : boost_ext::is_same_sans_qualifiers<T,U>
    { };
};

template< class T >
struct not_is_same_sans_qualifiers_as
{
    template< class U >
    struct apply
    {
        static bool const value = !boost_ext::is_same_sans_qualifiers<T,U>::value;
        typedef apply type;
    };
};

template< class Sequence, class T >
struct any_is_same_sans_qualifiers
    : boost_ext::mpl::any<
          Sequence,
          is_same_sans_qualifiers_as<T>
      >
{ };

template< class Sequence >
struct not_any_is_same_sans_qualifiers
{
    template< class T >
    struct apply
        : boost_ext::mpl::all<
              Sequence,
              not_is_same_sans_qualifiers_as<T>
          >
    { };
};

} // namespace forwarding_unary_function_private

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_FORWARDING_UNARY_FUNCTION_HPP
