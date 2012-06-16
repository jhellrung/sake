/*******************************************************************************
 * sake/core/cursor/adaptor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_ADAPTOR_HPP
#define SAKE_CORE_CURSOR_ADAPTOR_HPP

#include <boost/config.hpp>

#include <sake/core/cursor/adaptor_fwd.hpp>
#include <sake/core/cursor/private/adaptor/introversal_base.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/as_lvalue.hpp>

namespace sake
{

template<
    class I,
    class Introversal,
    class IntroversalMask /*= sake::null_introversal_tag*/
>
class cursor_adaptor
    : public cursor_adaptor_private::introversal_base<
          I, Introversal, IntroversalMask >
{
    typedef cursor_adaptor_private::introversal_base<
        I, Introversal, IntroversalMask
    > introversal_base_;
public:
    
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename cursor_adaptor,
        (( introversal_base_ ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    cursor_adaptor(R&& r, sake::begin_tag)
        : introversal_base_(r, sake::begin_tag())
    { }
    template< class R >
    cursor_adaptor(R&& r, sake::end_tag)
        : introversal_base_(r, sake::end_tag())
    { }
    template< class R >
    cursor_adaptor(R&& r, I const & i)
        : introversal_base_(r,i)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_ADAPTOR_HPP
