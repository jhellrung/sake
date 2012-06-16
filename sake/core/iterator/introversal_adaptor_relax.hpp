/*******************************************************************************
 * sake/core/iterator/introversal_adaptor_relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_RELAX_HPP
#define SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_RELAX_HPP

#include <boost/mpl/if.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/introversal_adaptor_fwd.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

template<
    class I, class Introversal,
    class IntroversalMask = sake::null_introversal_tag
>
class introversal_adaptor_relax
{
    static unsigned int const begin_mask_value =
        sake::begin_access_introversal_tag::value;
    static unsigned int const end_mask_value =
        sake::end_access_introversal_tag::value;
    static unsigned int const mask_value = IntroversalMask::value;
    static unsigned int const base_value = sake::iterator_introversal<I>::value;
    static unsigned int const implicit_value = base_value & mask_value;
    static unsigned int const nominal_explicit_value =
        Introversal::value & ~implicit_value;
    static bool const explicit_begin =
        0 != (nominal_explicit_value & begin_mask_value);
    static bool const explicit_end =
        0 != (nominal_explicit_value & end_mask_value);
    static unsigned int const explicit_value =
        (explicit_begin * begin_mask_value) | (explicit_end * end_mask_value);
    static unsigned int const nominal_relax_base_value =
        Introversal::value & implicit_value & ~explicit_value;
    typedef typename sake::iterator_relax<
        I, sake::introversal_tag_c< nominal_relax_base_value >
    >::type relax_base_type;
    static unsigned int const relax_base_value =
        sake::iterator_introversal< relax_base_type >::value;
    typedef sake::iterator::introversal_adaptor<
        relax_base_type,
        explicit_begin, explicit_end,
        IntroversalMask
    > introversal_adaptor_;
public:
    typedef typename boost::mpl::if_c<
        (0 != explicit_value || 0 != (~mask_value & relax_base_value)),
        introversal_adaptor_,
        relax_base_type
    >::type type;
};

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_RELAX_HPP
