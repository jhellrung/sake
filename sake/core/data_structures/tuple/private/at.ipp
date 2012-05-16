/*******************************************************************************
 * sake/core/data_structures/tuple/private/at.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_TUPLE_INCLUDE_HEADERS

#include <cstddef>

#include <boost/mpl/at.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#endif // #ifdef SAKE_TUPLE_INCLUDE_HEADERS

#ifdef SAKE_TUPLE_DEFINE_MEMBERS

    struct result_of
    {
        template< class This, std::size_t I >
        struct at_c
            : boost_ext::add_reference<
                  typename boost_ext::propagate_const<
                      This, typename boost::mpl::at_c< value_types, I >::type
                  >::type
              >
        { };
        template< class This, class I >
        struct at
            : at_c< This, I::value >
        { };
    };

    template< std::size_t I >
    typename result_of::template at_c< tuple, I >::type
    at_c()
    { return private_::at_c_dispatch<I>::apply(*this); }
    template< std::size_t I >
    typename result_of::template at_c< tuple const, I >::type
    at_c() const
    { return private_::at_c_dispatch<I>::apply(*this); }

    template< class I >
    typename result_of::template at< tuple, I >::type
    at()
    { return at_c< I::value >(); }
    template< class I >
    typename result_of::template at< tuple const, I >::type
    at() const
    { return at_c< I::value >(); }

#endif // #ifdef SAKE_TUPLE_DEFINE_MEMBERS
