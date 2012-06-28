/*******************************************************************************
 * sake/core/functional/operators/private/unary.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/curry.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_builtin_object.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_rv.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/private/unary_common.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

#ifndef SAKE_OPERATORS_NAME
#error SAKE_OPERATORS_NAME must be defined.
#endif // #ifndef SAKE_OPERATORS_NAME

#ifndef SAKE_OPERATORS_OP
#error SAKE_OPERATORS_OP must be defined.
#endif // SAKE_OPERATORS_OP

namespace sake
{

namespace operators
{

namespace result_of
{

template< class T >
struct SAKE_OPERATORS_NAME;

namespace extension
{
template< class T, class Enable = void >
struct SAKE_OPERATORS_NAME;
} // namespace extension

namespace default_impl
{
template< class T >
struct SAKE_OPERATORS_NAME;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::SAKE_OPERATORS_NAME<T>
 ******************************************************************************/

template< class T >
struct SAKE_OPERATORS_NAME
{
    typedef typename sake::operators::result_of::extension::
        SAKE_OPERATORS_NAME<
            typename boost_ext::remove_rvalue_reference<T>::type
        >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
private:
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    static bool const is_builtin_object_ =
        boost_ext::is_builtin_object<
            typename boost_ext::remove_qualifiers<T>::type
        >::value;
    // Sadly, MSVC tends to get lvalue/rvalue-ness wrong for unary operations
    // on lvalues of builtin types.
    typedef typename boost::mpl::if_c<
        is_builtin_object_,
        boost::mpl::quote2< boost_ext::is_same_sans_qualifiers >,
        boost::mpl::quote2< boost_ext::is_same_sans_rv >
    >::type is_same_type;
    typedef typename boost_ext::mpl::curry2< is_same_type >::template
        apply< type >::type assert_pred_type;
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    typedef typename boost_ext::mpl::curry_quote2< boost_ext::is_same_sans_rv >::
        apply< type >::type assert_pred_type;
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
    // MSVC9 workaround.
    static bool const is_same_sans_rv_ = SAKE_EXPR_APPLY( assert_pred_type,
        SAKE_OPERATORS_OP sake::declval<T>() );
    BOOST_STATIC_ASSERT((is_same_sans_rv_));
};

/*******************************************************************************
 * struct operators::result_of::extension::SAKE_OPERATORS_NAME< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct SAKE_OPERATORS_NAME
    : sake::operators::result_of::default_impl::
          SAKE_OPERATORS_NAME<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::SAKE_OPERATORS_NAME<T>
 ******************************************************************************/

namespace default_impl
{

namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )
{

template< class T >
struct impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename SAKE_OPERATORS_OP sake::declval<T>(),
        typename sake::operators::result_of::default_impl::unary_result_types<
            typename boost_ext::remove_qualifiers<T>::type
        >::type,
        type
    );
};

} // namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )

template< class T >
struct SAKE_OPERATORS_NAME
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    typedef typename BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::impl<T>::type type;
};

template< class T >
struct SAKE_OPERATORS_NAME< T& >
{
private:
    typedef typename BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::impl<T>::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        sake::operators::result_of::SAKE_OPERATORS_NAME<
            typename boost_ext::remove_qualifiers<T>::type
        >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::SAKE_OPERATORS_NAME(T&& x)
 *     -> operators::result_of::SAKE_OPERATORS_NAME<T>::type
 * struct operators::functional::SAKE_OPERATORS_NAME
 ******************************************************************************/

namespace functional
{

struct SAKE_OPERATORS_NAME
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::operators::result_of::SAKE_OPERATORS_NAME, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME<T>::type
    operator()(T&& x) const
    { return SAKE_OPERATORS_OP sake::forward<T>(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T& >::type
    operator()(T& x) const
    { return SAKE_OPERATORS_OP x; }

    template< class T >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T const & >::type
    operator()(T const & x) const
    { return SAKE_OPERATORS_OP x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

sake::operators::functional::SAKE_OPERATORS_NAME const SAKE_OPERATORS_NAME = { };

} // namespace operators

} // namespace sake

#undef SAKE_OPERATORS_NAME
#undef SAKE_OPERATORS_OP
