/*******************************************************************************
 * sake/core/expr_traits/private/type_tag_inducer.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_PRIVATE_TYPE_TAG_INDUCER_HPP
#define SAKE_CORE_EXPR_TRAITS_PRIVATE_TYPE_TAG_INDUCER_HPP

#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace expr_traits_private
{

struct type_tag_inducer
{
    template< class T >
    operator sake::type_tag<T>() const
    { return sake::type_tag<T>(); }
};

} // namespace expr_traits_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_PRIVATE_TYPE_TAG_INDUCER_HPP
