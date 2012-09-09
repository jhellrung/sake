/*******************************************************************************
 * sake/core/utility/is_template_base_of.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_IS_TEMPLATE_BASE_OF_HPP
#define SAKE_CORE_UTILITY_IS_TEMPLATE_BASE_OF_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/utility/sizeof_t.hpp>
#include <sake/core/utility/true_false_tag.hpp>

#ifndef SAKE_IS_TEMPLATE_BASE_OF_MAX_ARITY
#define SAKE_IS_TEMPLATE_BASE_OF_MAX_ARITY 8
#endif // #ifndef SAKE_IS_TEMPLATE_BASE_OF_MAX_ARITY

namespace sake
{

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_IS_TEMPLATE_BASE_OF_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/utility/is_template_base_of.hpp>
#include BOOST_PP_ITERATE()

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_TEMPLATE_BASE_OF_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

namespace is_template_base_of_private
{

template< template< class_T0N > class B, class BasePred >
struct BOOST_PP_CAT( helper, N )
{
    template< class B_ >
    struct result
    {
        static bool const value = boost::mpl::apply1< BasePred, B_ >::type::value;
        typedef sake::sizeof_t< 1 + value > type;
    };

    template< class_T0N >
    static typename result< B< T0N > >::type apply(B< T0N >*);
    static sake::false_tag apply(void*);
};

} // namespace is_template_base_of_private

template<
    template< class_T0N > class B, class D,
    class BasePred = boost::mpl::always< boost::true_type >
>
class BOOST_PP_CAT( is_template_base_of, N )
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<D>::value));
    typedef is_template_base_of_private::BOOST_PP_CAT( helper, N )< B, BasePred > helper_;
public:
    static bool const value =
        SAKE_SIZEOF_TRUE_TAG
     == sizeof( helper_::apply(static_cast< typename boost::remove_cv<D>::type* >(0)) );
    typedef BOOST_PP_CAT( is_template_base_of, N ) type;
};

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
