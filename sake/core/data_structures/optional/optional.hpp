/*******************************************************************************
 * sake/core/data_structures/optional/optional.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct optional<T>
 *
 * optional_adl::swap(optional<T>& x, optional<T>& y) -> void
 * optional_adl::hash_value(optional<T> const & x) -> std::size_t
 *
 * struct result_of::extension::get< optional<T>       &, void >
 * struct result_of::extension::get< optional<T> const &, void >
 * struct result_of::extension::get< optional<T>      &&, void >
 * optional_adl::get(optional<T>       & x) -> ...
 * optional_adl::get(optional<T> const & x) -> ...
 * optional_adl::get(optional<T>      && x) -> ...
 *
 * See the Boost.Optional documentation
 *     http://www.boost.org/libs/optional/doc/html/index.html
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPTIONAL_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPTIONAL_HPP

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/aligned_storage.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/data_structures/optional/private/destruct_dispatch.hpp>
#include <sake/core/data_structures/optional/private/forwarding_base.hpp>
#include <sake/core/data_structures/optional/private/initialized_type.hpp>
#include <sake/core/data_structures/optional/private/operator_assign_dispatch.hpp>
#include <sake/core/data_structures/optional/private/swap_dispatch.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/emplacer/traits.hpp>
#include <sake/core/utility/explicit_operator_bool.hpp>
#include <sake/core/utility/get_fwd.hpp>
#include <sake/core/utility/hash_value.hpp>
#include <sake/core/utility/swap.hpp>

#define SAKE_OPTIONAL_INCLUDE_HEADERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/reset.ipp>
#undef SAKE_OPTIONAL_INCLUDE_HEADERS

namespace sake
{

namespace result_of {
namespace extension {

template< class T >
struct get< sake::optional<T>&, void >
    : boost_ext::add_reference<T>
{ };

template< class T >
struct get< sake::optional<T> const &, void >
    : boost_ext::add_reference_add_const<T>
{ };

template< class T >
struct get< sake::optional<T>, void >
    : boost_ext::add_rvalue_reference<T>
{ };

} // namespace extension
} // namespace result_of

namespace optional_adl
{

/*******************************************************************************
 * struct optional<>
 ******************************************************************************/

template<>
struct optional<>
{ typedef void value_type; };

/*******************************************************************************
 * struct optional<T>
 ******************************************************************************/

template< class T >
struct optional
    : private_::forwarding_base<T>
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    typedef T value_type;
private:
    typedef typename boost::remove_cv<T>::type nocv_type;
public:

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_C(
        typename optional,
        (sake::is_movable<T>::value)
    )

    optional();
    typedef boost::true_type has_default_constructor_tag;
    optional(optional const & other);
    optional(this_rvalue_param_type other);

    ~optional();

    optional& operator=(this_copy_assign_param_type other);
    optional& operator=(this_rvalue_param_type other);

    optional(sake::optional<>);

    optional& operator=(sake::optional<>);

    void reset();
    void reset(sake::optional<>);

#if 0 // for exposition purposes only
    template< class U >
    optional(U&& x,
        typename enable_implicit_constructor<U>::type* = 0);

    template< class U >
    optional(U&& x, bool const initialize);

    template< class U >
    typename enable_operator_assign<U>::type
    operator=(U&& x);

    template< class U >
    void reset(U&& x);
#endif // #if 0
#define SAKE_OPTIONAL_DEFINE_MEMBERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/reset.ipp>
#undef SAKE_OPTIONAL_DEFINE_MEMBERS

    optional(T* const p);
    template< class Signature >
    explicit optional(sake::emplacer< Signature > e);

    void swap(optional& other);
    inline friend
    void swap(optional& x, optional& y) { return x.swap(y); }

    T       & get();
    T const & get() const;
    T       * get_ptr();
    T const * get_ptr() const;

    T       & operator*();
    T const & operator*() const;
    T       * operator->();
    T const * operator->() const;

    bool initialized() const;
    SAKE_EXPLICIT_OPERATOR_BOOL() const { return initialized(); }

    std::size_t hash_value() const;
    inline friend
    std::size_t hash_value(optional const & x) { return x.hash_value(); }

private:
    friend class sake::forwarding::core_access;

    friend struct private_::destruct_dispatch< nocv_type >;
    typedef private_::destruct_dispatch< nocv_type > destruct_dispatch_;
    template< class, class, bool > friend struct private_::operator_assign_dispatch;

    typename boost_ext::aligned_storage< nocv_type >::type m_storage;
    typename private_::initialized_type< nocv_type >::type m_initialized;
};

/*******************************************************************************
 * struct optional< T& >
 ******************************************************************************/

template< class T >
struct optional< T& >
    : private_::forwarding_base< T& >
{
    typedef T& value_type;

    optional();
    typedef boost::true_type has_default_constructor_tag;
    //optional(optional const &);
    //~optional();

    //optional& operator=(optional const &);

    optional(sake::optional<>);

    optional& operator=(sake::optional<>);

    void reset();
    void reset(sake::optional<>);

#if 0 // for exposition purposes only
    template< class U >
    optional(U&& x,
        typename enable_implicit_constructor<U>::type* = 0);

    template< class U >
    optional(U&& x, bool const initialize);

    template< class U >
    typename enable_operator_assign<U>::type
    operator=(U&& x);

    template< class U >
    void reset(U&& x);
#endif // #if 0
#define SAKE_OPTIONAL_REFERENCE
#define SAKE_OPTIONAL_DEFINE_MEMBERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/reset.ipp>
#undef SAKE_OPTIONAL_REFERENCE
#undef SAKE_OPTIONAL_DEFINE_MEMBERS

    optional(T* p);
    template< class Signature >
    explicit optional(sake::emplacer< Signature > e);

    void swap(optional& other);
    inline friend
    void swap(optional& x, optional& y) { x.swap(y); }

    T& get() const;
    T* get_ptr() const;
    T& operator*() const;
    T* operator->() const;

    bool initialized() const;
    SAKE_EXPLICIT_OPERATOR_BOOL() const { return initialized(); }

    std::size_t hash_value() const;
    inline friend
    std::size_t hash_value(optional x) { return x.hash_value(); }

private:
    T* mp;
};

/*******************************************************************************
 * get(optional<T>       & x) -> result_of::get< optional<T>       & >::type
 * get(optional<T> const & x) -> result_of::get< optional<T> const & >::type
 * get(optional<T>      && x) -> result_of::get< optional<T>         >::type
 ******************************************************************************/

template< class T >
inline typename sake::result_of::get< sake::optional<T>& >::type
get(sake::optional<T>& x)
{ return x.get(); }

template< class T >
inline typename sake::result_of::get< sake::optional<T> const & >::type
get(sake::optional<T> const & x)
{ return x.get(); }

template< class T >
inline typename sake::result_of::get< sake::optional<T> >::type
get(SAKE_RV_REF( optional<T> ) x)
{
    typedef typename sake::result_of::get< sake::optional<T> >::type result_type;
    return static_cast< result_type >(x.get());
}

/*******************************************************************************
 * optional<T> member definitions
 ******************************************************************************/

template< class T >
inline
optional<T>::
optional()
    : m_initialized(false)
{ }

template< class T >
inline
optional<T>::
optional(optional const & other)
    : m_initialized(other.m_initialized)
{ implicit_constructor_dispatch(other); }

template< class T >
inline
optional<T>::
optional(this_rvalue_param_type other)
    : m_initialized(other.m_initialized)
{ implicit_constructor_dispatch(sake::move(other)); }

template< class T >
inline
optional<T>::
~optional()
{ destruct_dispatch_::apply_if(*this); }

template< class T >
inline optional<T>&
optional<T>::
operator=(this_copy_assign_param_type other)
{ return operator_assign_dispatch(SAKE_AS_LVALUE(other)); }

template< class T >
inline optional<T>&
optional<T>::
operator=(this_rvalue_param_type other)
{ return operator_assign_dispatch(sake::move(other)); }

template< class T >
inline
optional<T>::
optional(sake::optional<>)
    : m_initialized(false)
{ }

template< class T >
inline optional<T>&
optional<T>::
operator=(sake::optional<>)
{
    reset();
    return *this;
}

template< class T >
inline void
optional<T>::
reset()
{ destruct_dispatch_::apply_if_reset(*this); }

template< class T >
inline void
optional<T>::
reset(sake::optional<>)
{ reset(); }

template< class T >
inline
optional<T>::
optional(T* const p)
    : m_initialized(!!p)
{
    if(p)
        new(m_storage._) nocv_type(*p);
}

template< class T >
template< class Signature >
inline
optional<T>::
optional(sake::emplacer< Signature > e)
    : m_initialized(true)
{ sake::emplacer_construct< nocv_type >(sake::forward<U>(x), m_storage._); }

template< class T >
inline void
optional<T>::
swap(optional& other)
{
    if(m_initialized) {
        if(other.m_initialized)
            private_::swap_dispatch<T>::apply(*this, other);
        else {
            other.reset(sake::move(get()));
            destruct_dispatch_::apply_reset(*this);
        }
    }
    else {
        if(other.m_initialized) {
            reset(sake::move(other.get()));
            destruct_dispatch_::apply_reset(other);
        }
        else
            // nothing
    }
}

template< class T >
inline T&
optional<T>::
get()
{
    SAKE_ASSERT((m_initialized));
    return *reinterpret_cast< T* >(m_storage._);
}

template< class T >
inline T const &
optional<T>::
get() const
{
    SAKE_ASSERT((m_initialized));
    return *reinterpret_cast< T const * >(m_storage._);
}

template< class T >
inline T*
optional<T>::
get_ptr()
{ return m_initialized ? reinterpret_cast< T* >(m_storage._) : 0; }

template< class T >
inline T const *
optional<T>::
get_ptr() const
{ return m_initialized ? reinterpret_cast< T const * >(m_storage._) : 0; }

template< class T >
inline T&
optional<T>::
operator*()
{ return get(); }

template< class T >
inline T const &
optional<T>::
operator*() const
{ return get(); }

template< class T >
inline T*
optional<T>::
operator->()
{
    SAKE_ASSERT((m_initialized));
    return reinterpret_cast< T* >(m_storage._);
}

template< class T >
inline T const *
optional<T>::
operator->() const
{
    SAKE_ASSERT((m_initialized));
    return reinterpret_cast< T const * >(m_storage._);
}

template< class T >
inline bool
optional<T>::
initialized() const
{ return m_initialized == true; }

template< class T >
inline std::size_t
optional<T>::
hash_value() const
{ return m_initialized ? sake::hash_value(get()) : 0; }

/*******************************************************************************
 * optional< T& > member definitions
 ******************************************************************************/

template< class T >
inline
optional< T& >::
optional()
    : mp(0)
{ }

template< class T >
inline
optional< T& >::
optional(sake::optional<>)
    : mp(0)
{ }

template< class T >
inline optional< T& >&
optional< T& >::
operator=(sake::optional<>)
{
    reset();
    return *this;
}

template< class T >
inline void
optional< T& >::
reset()
{ mp = 0; }

template< class T >
inline void
optional< T& >::
reset(sake::optional<>)
{ reset(); }

template< class T >
inline
optional< T& >::
optional(T* const p)
    : mp(p)
{ }

template< class T >
template< class Signature >
inline
optional< T& >::
optional(sake::emplacer< Signature > e)
    : mp(get_ptr_dispatch(e))
{ }

template< class T >
inline void
optional< T& >::
swap(optional& other)
{ sake::swap(mp, other.mp); }

template< class T >
inline T&
optional< T& >::
get() const
{ SAKE_ASSERT((mp)); return *mp; }

template< class T >
inline T*
optional< T& >::
get_ptr() const
{ return mp; }

template< class T >
inline T&
optional< T& >::
operator*() const
{ return get(); }

template< class T >
inline T*
optional< T& >::
operator->() const
{ return get_ptr(); }

template< class T >
inline bool
optional< T& >::
initialized() const
{ return mp != 0; }

template< class T >
inline std::size_t
optional< T& >::
hash_value() const
{ return mp ? sake::hash_value(*mp) : 0; }

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPTIONAL_HPP
