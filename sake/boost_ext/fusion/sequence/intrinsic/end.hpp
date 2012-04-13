/*******************************************************************************
 * boost_ext/fusion/sequence/intrisic/end.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::fusion::end(Sequence&& s)
 *     -> boost_ext::fusion::result_of::end< Sequence >::type
 *
 * TODO: This clearly needs some cleaning up...
 * TODO: Needs sake::move_iterator!
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP
#define SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/preprocessor/facilities/apply.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/fusion/iterator/move_owned_iterator.hpp>
#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace result_of
{

template< class Sequence >
class end
{
    typedef typename boost::fusion::result_of::end< Sequence >::type maybe_type;
    typedef boost::mpl::range_c<
        0, boost::fusion::result_of::size< Sequence >::value
    > indices_type;
public:
    typedef typename boost::mpl::if_c<
        boost_ext::mpl::and3<
            boost::mpl::not_< boost::fusion::traits::is_view< Sequence > >,
            boost::mpl::not_< boost_ext::mpl::all<
                indices_type,
                boost_ext::is_reference<
                    boost::fusion::result_of::value_at< Sequence, boost::mpl::_1 >
                >
            > >,
            boost::mpl::any<
                indices_type,
                boost_ext::is_lvalue_reference_to_nonconst<
                    boost::fusion::result_of::at< Sequence, boost::mpl::_1 >
                >
            >
        >::value,
        boost_ext::fusion::move_owned_iterator< maybe_type >,
        maybe_type
    >::type type;
};

template< class Sequence >
class end< Sequence& >
    : public boost::fusion::result_of::end< Sequence >
{ };

} // namespace result_of

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class Sequence >
inline typename result_of::end< Sequence >::type
end(Sequence&& s)
{
    typedef typename result_of::end< Sequence >::type result_type;
    return static_cast< result_type >(boost::fusion::end(s));
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class Sequence >
inline typename result_of::end< Sequence& >::type
end(Sequence& s)
{
    typedef typename result_of::end< Sequence& >::type result_type;
    return static_cast< result_type >(boost::fusion::end(s));
}

template< class Sequence >
inline typename result_of::end< Sequence const & >::type
end(Sequence const & s)
{
    typedef typename result_of::end< Sequence const & >::type result_type;
    return static_cast< result_type >(boost::fusion::end(s));
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SEQUENCE_INTRINSIC_BEGIN_HPP
