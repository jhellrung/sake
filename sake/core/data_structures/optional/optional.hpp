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
 * struct result_of::extension::get< optional<T>        , void >
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

#include <boost/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/and.hpp>
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
#include <sake/core/emplacer/construct.hpp>
#include <sake/core/emplacer/traits.hpp>
#include <sake/core/functional/forwarding/core_access.hpp>
#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/type_traits/has_nothrow_copy_assign.hpp>
#include <sake/core/type_traits/has_nothrow_copy_constructor.hpp>
#include <sake/core/type_traits/has_nothrow_destructor.hpp>
#include <sake/core/type_traits/has_nothrow_move_assign.hpp>
#include <sake/core/type_traits/has_nothrow_move_constructor.hpp>
#include <sake/core/type_traits/has_trivial_copy_assign.hpp>
#include <sake/core/type_traits/has_trivial_copy_constructor.hpp>
#include <sake/core/type_traits/has_trivial_destructor.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/define_friend_function.hpp>
#include <sake/core/utility/explicit_operator_bool.hpp>
#include <sake/core/utility/get_fwd.hpp>
#include <sake/core/utility/hash_value.hpp>
#include <sake/core/utility/swap.hpp>

#define SAKE_OPTIONAL_INCLUDE_HEADERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/construct.ipp>
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
        (sake::has_move_emulation<T>::value)
    )

    typedef boost::true_type has_nothrow_default_constructor_tag;
    optional()
        BOOST_NOEXCEPT;

    typedef sake::has_nothrow_copy_constructor< nocv_type >
        has_nothrow_copy_constructor_tag;
    typedef sake::has_trivial_copy_constructor< nocv_type >
        has_trivial_copy_constructor_tag;
    optional(optional const & other)
        BOOST_NOEXCEPT_IF((has_nothrow_copy_constructor_tag::value));

    typedef sake::has_nothrow_move_constructor< nocv_type >
        has_nothrow_move_constructor_tag;
    optional(this_rvalue_param_type other)
        BOOST_NOEXCEPT_IF((has_nothrow_move_constructor_tag::value));

    typedef sake::has_nothrow_destructor< nocv_type >
        has_nothrow_destructor_tag;
    typedef sake::has_trivial_destructor< nocv_type >
        has_trivial_destructor_tag;
    ~optional()
        BOOST_NOEXCEPT_IF((has_nothrow_destructor_tag::value));

    struct has_nothrow_copy_assign_tag;
    struct has_trivial_copy_assign_tag;
    optional& operator=(this_copy_assign_param_type other)
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value));

    struct has_nothrow_move_assign_tag;
    optional& operator=(this_rvalue_param_type other)
        BOOST_NOEXCEPT_IF((has_nothrow_move_assign_tag::value));

    optional(sake::optional<>);

    optional& operator=(sake::optional<>);

    void destruct();

    void reset();
    void reset(sake::optional<>);

#if 0 // for exposition purposes only
    template< class U >
    optional(U&& x,
        typename implicit_constructor_enabler<U>::type* = 0);

    template< class U >
    optional(U&& x, bool const initialize,
        typename initialize_constructor_enabler<U>::type* = 0);

    template< class U >
    typename operator_assign_enabler<U>::type // -> optional&
    operator=(U&& x);

    template< class U >
    typename construct_enabler<U>::type // -> void
    construct(U&& x);

    template< class U >
    typename reset_enabler<U>::type // -> void
    reset(U&& x);
#endif // #if 0
#define SAKE_OPTIONAL_DEFINE_MEMBERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/construct.ipp>
#include <sake/core/data_structures/optional/private/reset.ipp>
#undef SAKE_OPTIONAL_DEFINE_MEMBERS

    optional(T* const p);
    template< class Signature >
    explicit optional(sake::emplacer< Signature > e,
        typename emplacer_enabler< sake::emplacer< Signature > >::type* = 0);

    void swap(optional& other);
    SAKE_DEFINE_FRIEND_SWAP( optional )

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
    SAKE_DEFINE_FRIEND_HASH_VALUE( optional )

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

    typedef boost::true_type has_nothrow_default_constructor_tag;
    optional() BOOST_NOEXCEPT;
    typedef boost::true_type has_trivial_copy_constructor_tag;
    //optional(optional const &);
    typedef boost::true_type has_trivial_destructor_tag;
    //~optional();

    typedef boost::true_type has_trivial_copy_assign_tag;
    //optional& operator=(optional const &);

    optional(sake::optional<>);

    optional& operator=(sake::optional<>);

    void destruct();

    void reset();
    void reset(sake::optional<>);

#if 0 // for exposition purposes only
    template< class U >
    optional(U&& x,
        typename implicit_constructor_enabler<U>::type* = 0);

    template< class U >
    optional(U&& x, bool const initialize,
        typename initialize_constructor_enabler<U>::type* = 0);

    template< class U >
    typename operator_assign_enabler<U>::type // -> optional&
    operator=(U&& x);

    template< class U >
    typename construct_enabler<U>::type // -> void
    construct(U&& x);

    template< class U >
    typename reset_enabler<U>::type // -> void
    reset(U&& x);
#endif // #if 0
#define SAKE_OPTIONAL_REFERENCE
#define SAKE_OPTIONAL_DEFINE_MEMBERS
#include <sake/core/data_structures/optional/private/common_members.ipp>
#include <sake/core/data_structures/optional/private/implicit_constructor.ipp>
#include <sake/core/data_structures/optional/private/initialize_constructor.ipp>
#include <sake/core/data_structures/optional/private/operator_assign.ipp>
#include <sake/core/data_structures/optional/private/construct.ipp>
#include <sake/core/data_structures/optional/private/reset.ipp>
#undef SAKE_OPTIONAL_REFERENCE
#undef SAKE_OPTIONAL_DEFINE_MEMBERS

    optional(T* p);
    template< class Signature >
    explicit optional(sake::emplacer< Signature > e,
        typename emplacer_enabler< sake::emplacer< Signature > >::type* = 0);

    void swap(optional& other);
    SAKE_DEFINE_FRIEND_SWAP( optional )

    T& get() const;
    T* get_ptr() const;
    T& operator*() const;
    T* operator->() const;

    bool initialized() const;
    SAKE_EXPLICIT_OPERATOR_BOOL() const { return initialized(); }

    std::size_t hash_value() const;
    SAKE_DEFINE_FRIEND_HASH_VALUE( optional )

private:
    T* m_p;
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
    BOOST_NOEXCEPT
    : m_initialized(false)
{ }

template< class T >
inline
optional<T>::
optional(optional const & other)
    BOOST_NOEXCEPT_IF((has_nothrow_copy_constructor_tag::value))
    : m_initialized(other.m_initialized)
{ implicit_constructor_dispatch(other); }

template< class T >
inline
optional<T>::
optional(this_rvalue_param_type other)
    BOOST_NOEXCEPT_IF((has_nothrow_move_constructor_tag::value))
    : m_initialized(other.m_initialized)
{ implicit_constructor_dispatch(sake::move(other)); }

template< class T >
inline
optional<T>::
~optional()
    BOOST_NOEXCEPT_IF((has_nothrow_destructor_tag::value))
{ destruct_dispatch_::apply_if(*this); }

template< class T >
struct optional<T>::
has_nothrow_copy_assign_tag
    : boost_ext::mpl::and2<
          has_nothrow_copy_constructor_tag,
          boost::mpl::if_<
              sake::has_operator_assign< T&, void ( T const & ) >,
              sake::has_nothrow_copy_assign<T>,
              has_nothrow_destructor_tag
          >
      >::type
{ };

template< class T >
struct optional<T>::
has_trivial_copy_assign_tag
    : boost_ext::mpl::and2<
          sake::has_trivial_copy_assign<T>,
          sake::has_trivial_copy_constructor<T>
      >
{ };

template< class T >
inline optional<T>&
optional<T>::
operator=(this_copy_assign_param_type other)
    BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value))
{ return operator_assign_dispatch(SAKE_AS_LVALUE(other)); }

template< class T >
struct optional<T>::
has_nothrow_move_assign_tag
    : boost::mpl::if_c<
          sake::has_operator_assign< T&, void ( nocv_type ) >::value,
          sake::has_nothrow_move_assign<T>,
          boost_ext::mpl::and2<
              has_nothrow_destructor_tag,
              has_nothrow_move_constructor_tag
          >
      >::type
{ };

template< class T >
inline optional<T>&
optional<T>::
operator=(this_rvalue_param_type other)
    BOOST_NOEXCEPT_IF((has_nothrow_move_assign_tag::value))
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
destruct()
{
    SAKE_ASSERT((m_initialized));
    destruct_dispatch_::apply_reset(*this);
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
optional(sake::emplacer< Signature > e,
    typename emplacer_enabler< sake::emplacer< Signature > >::type*)
    : m_initialized(true)
{ sake::emplacer_construct< nocv_type >(e, m_storage._); }

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
        else {
            // nothing
        }
    }
}

template< class T >
inline T&
optional<T>::
get()
{
    SAKE_ASSERT((m_initialized));
    return *static_cast< T* >(static_cast< void* >(m_storage._));
}

template< class T >
inline T const &
optional<T>::
get() const
{
    SAKE_ASSERT((m_initialized));
    return *static_cast< T const * >(static_cast< void const * >(m_storage._));
}

template< class T >
inline T*
optional<T>::
get_ptr()
{ return m_initialized ? static_cast< T* >(static_cast< void* >(m_storage._)) : 0; }

template< class T >
inline T const *
optional<T>::
get_ptr() const
{ return m_initialized ? static_cast< T const * >(static_cast< void const * >(m_storage._)) : 0; }

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
optional() BOOST_NOEXCEPT
    : m_p(0)
{ }

template< class T >
inline
optional< T& >::
optional(sake::optional<>)
    : m_p(0)
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
destruct()
{
    SAKE_ASSERT((m_p));
    reset();
}

template< class T >
inline void
optional< T& >::
reset()
{ m_p = 0; }

template< class T >
inline void
optional< T& >::
reset(sake::optional<>)
{ reset(); }

template< class T >
inline
optional< T& >::
optional(T* const p)
    : m_p(p)
{ }

template< class T >
template< class Signature >
inline
optional< T& >::
optional(sake::emplacer< Signature > e,
    typename emplacer_enabler< sake::emplacer< Signature > >::type*)
    : m_p(get_ptr_dispatch(e))
{ }

template< class T >
inline void
optional< T& >::
swap(optional& other)
{ sake::swap(m_p, other.m_p); }

template< class T >
inline T&
optional< T& >::
get() const
{
    SAKE_ASSERT((m_p));
    return *m_p;
}

template< class T >
inline T*
optional< T& >::
get_ptr() const
{ return m_p; }

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
{ return m_p != 0; }

template< class T >
inline std::size_t
optional< T& >::
hash_value() const
{ return m_p ? sake::hash_value(*m_p) : 0; }

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPTIONAL_HPP
