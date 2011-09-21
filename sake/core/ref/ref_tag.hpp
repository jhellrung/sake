/*******************************************************************************
 * sake/core/ref/ref_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct ref_tag::assignable;
 * struct ref_tag::default_constructible;
 * struct ref_tag::implicitly_convertible;
 * struct ref_tag::parameter;
 *
 * This provides several tags for use when specifying a sake::reference_wrapper
 * instantiation.  The semantics of each tag are as follows.
 *
 * assignable
 *     [ optional ]
 *     Specifies assignable (by default, sake::reference_wrapper's are not
 *     assignable, like real C++ references).  The semantics of assignation are
 *     rebinding.
 * default_constructible
 *     [ optional ]
 *     Specifies default constructible (by default, sake::reference_wrapper's
 *     are not default constructible, like real C++ references).  Implies
 *     assignable.
 * implicitly_convertible
 *     [ optional ]
 *     Specifies implicitly convertible from T&.  By default,
 *     sake::reference_wrapper's are only explicitly convertible from T&.
 * parameter
 *     [ optional ]
 *     Specifies a wrapper for a by-reference parameter, indicating that the
 *     referent itself is the actual parameter, rather than the wrapper.
 ******************************************************************************/

#ifndef SAKE_CORE_REF_REF_TAG_HPP
#define SAKE_CORE_REF_REF_TAG_HPP

#include <boost/mpl/set/set0.hpp>

namespace sake
{

namespace ref_tag
{

struct assignable;
struct default_constructible;
struct implicitly_convertible;
struct parameter;

typedef boost::mpl::set0<> default_tags;

} // namespace ref_tag

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_REF_TAG_HPP
