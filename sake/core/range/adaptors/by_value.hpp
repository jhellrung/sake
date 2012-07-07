/*******************************************************************************
 * sake/core/range/adaptors/by_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::adaptors::by_value<R>
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_BY_VALUE_HPP
#define SAKE_CORE_RANGE_ADAPTORS_BY_VALUE_HPP

#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/by_value/movable.hpp>
#include <sake/core/range/adaptor.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/value_constructor.hpp>

namespace sake
{

namespace range
{

namespace adaptors
{

template< class R >
class by_value
    : public sake::range::adaptor< by_value<R>, R >
{
    typedef sake::range::adaptor< by_value, R > adaptor_;
public:
    SAKE_USING_TYPEDEF( typename adaptor_, difference_type );
    SAKE_USING_TYPEDEF( typename adaptor_, size_type );

    SAKE_MOVABLE_NONCOPYABLE( by_value )
    by_value(this_rvalue_param_type other)
        : adaptor_(sake::move(static_cast< adaptor_& >(other)))
    { }

    SAKE_MEMBERWISE_SWAP( by_value, (( adaptor_ )) )

    // template< class S >
    // explicit by_value(S&& r)
    //     : adaptor_(sake::forward<S>(r))
    // { }
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME by_value
#define SAKE_VALUE_CONSTRUCTOR_TYPES      ( R )
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    adaptor_
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

private:
    friend class sake::range::core_access;

    template< class This, class Introterminal >
    struct derived_iterator_with_of
        : adaptor_::template base_iterator_with_of< This, Introterminal >
    { };

    template< class This, class T, class Introterminal >
    static typename adaptor_::template
        iterator_with_of< This, Introterminal >::type
    derived_iter_at(This& this_, T const & x, Introterminal)
    { return adaptor_::base_iter_at(this_, x, Introterminal()); }

    template< class This, class Begin, class End >
    struct derived_subrange_with_of
        : adaptor_::template base_subrange_with_of< This, Begin, End >
    { };

    template< class This, class Begin, class End >
    static typename adaptor_::template
        subrange_with_of< This, Begin, End >::type
    derived_sub(This& this_, Begin const & b, End const & e)
    { return adaptor_::base_sub(this_, b, e); }

    template< class This, class T >
    static typename adaptor_::template reference_of< This >::type
    derived_at(This& this_, T const x)
    { return adaptor_::base_at(this_, x); }

    bool derived_empty() const
    { return adaptor_::base_empty(); }
    difference_type derived_distance() const
    { return adaptor_::base_distance(); }
    size_type derived_size() const
    { return adaptor_::base_size(); }
};

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_BY_VALUE_HPP
