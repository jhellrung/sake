/*******************************************************************************
 * sake/boost_ext/fusion/adapted/reference_wrapper.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_REFERENCE_WRAPPER_HPP
#define SAKE_BOOST_EXT_FUSION_ADAPTED_REFERENCE_WRAPPER_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/ref/is_reference_wrapper.hpp>
#include <sake/core/ref/unwrap_ref.hpp>
#include <sake/core/ref/unwrap_reference.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

struct reference_wrapper_tag;

} // namespace fusion

} // namespace boost_ext

} // namespace sake

namespace boost
{

namespace fusion
{

namespace traits
{

template< class T >
struct tag_of<
    T,
    typename boost::enable_if_c< sake::boost_ext::mpl::and2<
        sake::is_reference_wrapper<T>,
        boost::fusion::traits::is_sequence<
            typename sake::unwrap_reference<T>::type
        >
    >::value >::type
>
{ typedef sake::boost_ext::fusion::reference_wrapper_tag type; };

} // namespace traits

namespace extension
{

template<>
struct is_sequence_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{ template<class> struct apply : boost::true_type { }; };

template<>
struct is_view_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{ template<class> struct apply : boost::true_type { }; };

template<>
struct category_of_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This >
    struct apply
        : boost::fusion::traits::category_of<
              typename sake::unwrap_reference< This >::type
          >
    { };
};

/*******************************************************************************
 * Sequence requirements.
 * See
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/forward_sequence.html
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/bidirectional_sequence.html
 *     http://www.boost.org/libs/fusion/doc/html/fusion/sequence/concepts/random_access_sequence.html
 ******************************************************************************/

template<>
struct begin_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This >
    struct apply
    {
        typedef typename boost::fusion::result_of::begin<
            typename sake::unwrap_reference< This >::type
        >::type type;
        static type call(This& this_)
        { return boost::fusion::begin(sake::unwrap_ref(this_)); }
    };
};

template<>
struct end_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This >
    struct apply
    {
        typedef typename boost::fusion::result_of::end<
            typename sake::unwrap_reference< This >::type
        >::type type;
        static type call(This& this_)
        { return boost::fusion::end(sake::unwrap_ref(this_)); }
    };
};

template<>
struct size_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This >
    struct apply
        : boost::fusion::result_of::size<
              typename sake::unwrap_reference< This >::type
          >
    { };
};

template<>
struct at_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This, class Index >
    struct apply
    {
        typedef typename boost::fusion::result_of::at<
            typename sake::unwrap_reference< This >::type, Index >::type
        type;
        static type call(This& this_)
        { return boost::fusion::at< Index >(sake::unwrap_ref(this_)); }
    };
};

template<>
struct value_at_impl< sake::boost_ext::fusion::reference_wrapper_tag >
{
    template< class This, class Index >
    struct apply
        : boost::fusion::result_of::value_at<
              typename sake::unwrap_reference< This >::type,
              Index
          >
    { };
};

} // namespace extension

} // namespace fusion

} // namespace boost

#endif // SAKE_BOOST_EXT_FUSION_ADAPTED_REFERENCE_WRAPPER_HPP
