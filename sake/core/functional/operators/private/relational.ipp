/*******************************************************************************
 * sake/core/functional/operators/private/relational.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_rv.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/private/relational_common.hpp>
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

template< class T0, class T1 = T0 >
struct SAKE_OPERATORS_NAME;

namespace extension
{
template< class T0, class T1, class Enable = void >
struct SAKE_OPERATORS_NAME;
} // namespace extension

namespace default_impl
{
template< class T0, class T1 = T0 >
struct SAKE_OPERATORS_NAME;
} // namespace default_impl

/*******************************************************************************
 * struct operators::result_of::SAKE_OPERATORS_NAME< T0, T1 = T0 >
 ******************************************************************************/

namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )
{

template<
    class T0, class T1,
    class T0_ = typename boost_ext::remove_qualifiers< T0 >::type,
    class T1_ = typename boost_ext::remove_qualifiers< T1 >::type
>
struct dispatch
{
    typedef typename sake::operators::result_of::extension::
        SAKE_OPERATORS_NAME< T0, T1 >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
private:
    // MSVC9 workaround.
    static bool const is_same_sans_rv_ = SAKE_EXPR_APPLY(
        typename boost_ext::mpl::curry_quote2<
            boost_ext::is_same_sans_rv >::apply< type >::type,
        sake::declval< T0 >() SAKE_OPERATORS_OP sake::declval< T1 >()
    );
    BOOST_STATIC_ASSERT((is_same_sans_rv_));
};

template< class T0, class T1, class U0, class U1 >
struct dispatch< T0, T1, U0*, U1* >
{ typedef bool type; };

} // namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME
    : BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::dispatch<
          typename boost_ext::remove_rvalue_reference< T0 >::type,
          typename boost_ext::remove_rvalue_reference< T1 >::type
      >
{ };

/*******************************************************************************
 * struct operators::result_of::extension::SAKE_OPERATORS_NAME< T0, T1, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T0, class T1, class Enable >
struct SAKE_OPERATORS_NAME
    : sake::operators::result_of::default_impl::SAKE_OPERATORS_NAME< T0, T1 >
{ };

} // namespace extension

/*******************************************************************************
 * struct operators::result_of::default_impl::SAKE_OPERATORS_NAME< T0, T1 = T0 >
 ******************************************************************************/

namespace default_impl
{

namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )
{

template< class T0, class T1 >
struct impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval< T0 >() SAKE_OPERATORS_OP sake::declval< T1 >(),
        sake::operators::result_of::default_impl::relational_result_types,
        type
    );
};

template< class T0, class T1 >
class helper
{
    typedef typename impl< T0, T1 >::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        sake::operators::result_of::SAKE_OPERATORS_NAME<
            typename boost_ext::remove_qualifiers< T0 >::type,
            typename boost_ext::remove_qualifiers< T1 >::type
        >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

} // namespace BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T1 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T1 >::value));
    typedef typename BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::impl< T0, T1 >::type type;
};

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME< T0, T1& >
    : BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::helper< T0, T1& >
{ };

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME< T0&, T1 >
    : BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::helper< T0&, T1 >
{ };

template< class T0, class T1 >
struct SAKE_OPERATORS_NAME< T0&, T1& >
    : BOOST_PP_CAT( SAKE_OPERATORS_NAME, _private )::helper< T0&, T1& >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * operators::SAKE_OPERATORS_NAME(T0&& x0, T1&& x1)
 *     -> operators::result_of::SAKE_OPERATORS_NAME< T0, T1 >::type
 * struct operators::functional::SAKE_OPERATORS_NAME
 ******************************************************************************/

namespace functional
{

struct SAKE_OPERATORS_NAME
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::operators::result_of::SAKE_OPERATORS_NAME, 2 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T0, T1 >::type
    operator()(T0&& x0, T1&& x1) const
    { return sake::forward< T0 >(x0) SAKE_OPERATORS_OP sake::forward< T1 >(x1); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T0&, T1& >::type
    operator()(T0& x0, T1& x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

    template< class T0, class T1 >
    typename sake::operators::result_of::SAKE_OPERATORS_NAME< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return x0 SAKE_OPERATORS_OP x1; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    bool
    operator()(T0* const p0, T1* const p1) const
    { return static_cast< void* >(p0) SAKE_OPERATORS_OP static_cast< void* >(p1); }
};

} // namespace functional

sake::operators::functional::SAKE_OPERATORS_NAME const SAKE_OPERATORS_NAME = { };

} // namespace operators

} // namespace sake

#undef SAKE_OPERATORS_NAME
#undef SAKE_OPERATORS_OP
