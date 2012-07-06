/*******************************************************************************
 * sake/core/iterator/adapt_introversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPT_INTROVERSAL_HPP
#define SAKE_CORE_ITERATOR_ADAPT_INTROVERSAL_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/set/set10.hpp>

#include <sake/core/iterator/adapt_introversal_fwd.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/introversal.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

template<
    class I, class Introversal,
    class IntroversalMask /*= null_introversal_tag*/
>
class adapt_introversal
{
    static unsigned int const begin_mask_value =
        sake::begin_access_introversal_tag::value;
    static unsigned int const end_mask_value =
        sake::end_access_introversal_tag::value;

    static unsigned int const implicit_value =
        sake::iterator_introversal<I>::value
      & Introversal::value
      & IntroversalMask::value;
    static unsigned int const nominal_explicit_value =
        Introversal::value & ~implicit_value;

    static bool const explicit_begin =
        0 != (nominal_explicit_value & begin_mask_value);
    static bool const explicit_end =
        0 != (nominal_explicit_value & end_mask_value);
    static unsigned int const explicit_value =
        (explicit_begin * begin_mask_value) | (explicit_end * end_mask_value);

    static unsigned int const nominal_relax_value =
        implicit_value & ~explicit_value;
    typedef typename sake::iterator_relax<
        I, sake::introversal_tag_c< nominal_relax_value >
    >::type relax_type;
    static unsigned int const relax_value =
        sake::iterator_introversal< relax_type >::value;
    static unsigned int const adapt_mask_value =
        relax_value & IntroversalMask::value;
    typedef typename boost::mpl::eval_if_c<
        explicit_begin,
        boost::mpl::if_c<
            explicit_end,
            boost::mpl::set2< sake::begin_tag, sake::end_tag >,
            boost::mpl::set1< sake::begin_tag >
        >,
        boost::mpl::if_c<
            explicit_end,
            boost::mpl::set1< sake::end_tag >,
            boost::mpl::set0<>
        >
    >::type tag_types;
    typedef sake::iterator::adaptors::introversal<
        relax_type,
        tag_types,
        sake::introversal_tag_c< adapt_mask_value >
    > adaptors_introversal_;

public:
    static bool const wrap_cond =
        0 != explicit_value || relax_value != adapt_mask_value;
    typedef typename boost::mpl::if_c<
        wrap_cond, adaptors_introversal_, relax_type >::type type;
};

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPT_INTROVERSAL_HPP
