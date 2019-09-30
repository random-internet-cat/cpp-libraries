#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>

#include "randomcat/units/detail/quantity_def.hpp"

namespace randomcat::units {
    template<typename Rep, typename Unit>
    constexpr auto abs(quantity<Rep, Unit> _q) {
        using std::abs;
        return make_quantity<Unit>(abs(_q.count()));
    }

    template<std::intmax_t Power, typename Rep, typename Unit>
    constexpr auto pow(quantity<Rep, Unit> const& _quantity) {
        using std::pow;
        return units::make_quantity<power_unit<Unit, Power>>(pow(_quantity.count(), Power));
    }

    template<typename Rep1, typename Unit1, typename Rep2, typename Unit2, typename = std::enable_if_t<unit_tags_are_equal_v<Unit1, Unit2>>>
    constexpr std::common_type_t<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>> operator+(quantity<Rep1, Unit1> const& _first,
                                                                                         quantity<Rep2, Unit2> const& _second) noexcept {
        using common = std::common_type_t<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>>;
        return common(common(_first).count() + common(_second).count());
    }

    template<typename Rep1, typename Unit1, typename Rep2, typename Unit2>
    constexpr auto operator*(quantity<Rep1, Unit1> const& _first, quantity<Rep2, Unit2> const& _second) noexcept {
        return make_quantity<product_unit<Unit1, Unit2>>(_first.count() * _second.count());
    }

    template<typename ValueRep, typename QuantityRep, typename Unit, typename = std::enable_if_t<!is_quantity_v<ValueRep>>>
    constexpr auto operator*(ValueRep const& _val, quantity<QuantityRep, Unit> const& _quantity) noexcept {
        return unitless_quantity(_val) * _quantity;
    }

    template<typename ValueRep, typename QuantityRep, typename Unit, typename = std::enable_if_t<!is_quantity_v<ValueRep>>>
    constexpr auto operator*(quantity<QuantityRep, Unit> const& _quantity, ValueRep const& _val) noexcept {
        return _quantity * unitless_quantity(_val);
    }

    template<typename Rep1, typename Unit1, typename Rep2, typename Unit2>
    constexpr auto operator-(quantity<Rep1, Unit1> const& _first, quantity<Rep2, Unit2> const& _second) noexcept {
        using common = std::common_type_t<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>>;
        return common(common(_first).count() - common(_second).count());
    }

    template<typename Rep1, typename Unit1, typename Rep2, typename Unit2>
    constexpr quantity<std::common_type_t<Rep1, Rep2>, quotient_unit<detail::unit_without_scale_t<Unit1>, detail::unit_without_scale_t<Unit2>>> operator/(
        quantity<Rep1, Unit1> const& _first,
        quantity<Rep2, Unit2> const& _second) noexcept {
        using common_rep = std::common_type_t<Rep1, Rep2>;
        using result = quantity<common_rep, quotient_unit<detail::unit_without_scale_t<Unit1>, detail::unit_without_scale_t<Unit2>>>;

        using scale_gcd = detail::ratio_gcd_t<detail::unit_scale_t<Unit1>, detail::unit_scale_t<Unit2>>;
        using first_scaled = quantity<common_rep, unit<detail::unit_tag_counts_t<Unit1>, scale_gcd>>;
        using second_scaled = quantity<common_rep, unit<detail::unit_tag_counts_t<Unit2>, scale_gcd>>;

        return result{static_cast<common_rep>(first_scaled{_first}.count()) / static_cast<common_rep>(second_scaled{_second}.count())};
    }

    template<typename ValueRep, typename QuantityRep, typename QuantityUnit, typename = std::enable_if_t<!is_quantity_v<ValueRep>>>
    constexpr auto operator/(quantity<QuantityRep, QuantityUnit> const& _q, ValueRep const& _v) {
        return _q / unitless_quantity(_v);
    }
}    // namespace randomcat::units