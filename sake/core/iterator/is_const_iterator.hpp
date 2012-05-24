/*******************************************************************************
 * sake/core/iterator/is_const_iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class is_const_iterator<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP

#include <boost/mpl/not.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class I >
class is_const_iterator
{
    typedef sake::iterator_traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, value_type );
public:
    static const bool value = boost_ext::mpl::and2<
        boost_ext::is_convertible< reference, value_type const & >,
        boost::mpl::not_< boost_ext::is_convertible< reference, value_type & > >
    >::value;
    typedef is_const_iterator type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_CONST_ITERATOR_HPP
