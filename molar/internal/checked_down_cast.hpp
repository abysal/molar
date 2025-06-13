//
// Created by Akashic on 6/10/2025.
//

#ifndef CHECKED_DOWN_CAST_HPP
#define CHECKED_DOWN_CAST_HPP

namespace molar::details {
#if ((defined(_DEBUG) || !defined(NDEBUG) || defined(DEBUG)) &&                                \
     !defined(MOLAR_NODEBUG_RTTI)) ||                                                          \
    defined(MOLAR_FORCE_TYPE_ASSERTED_CAST)
    template <typename Derived, typename Current> Derived* type_asserted_cast(Current* p) {
        return std::addressof(dynamic_cast<Derived&>(*p));
    }

    template <typename Derived, typename Current> Derived& type_asserted_cast(Current& p) {
        return dynamic_cast<Derived&>(p);
    }
#else
    template <typename Derived, typename Current>
    Derived* type_asserted_cast(Current* p) noexcept {
        return static_cast<Derived*>(p);
    }

    template <typename Derived, typename Current>
    Derived& type_asserted_cast(Current& p) noexcept {
        return static_cast<Derived&>(p);
    }
#endif
} // namespace molar::details

#endif // CHECKED_DOWN_CAST_HPP
