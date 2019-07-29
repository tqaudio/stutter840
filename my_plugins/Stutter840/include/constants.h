#pragma once

namespace Stutter840 {
namespace Constants {
constexpr double defaultRatio{0.0 /* percentage */};
constexpr double defaultRatioNormalized{defaultRatio / 100.0};

constexpr double maxDuration{10000.0 /* ms */};
constexpr double baseDenominator{256.0};

constexpr int defaultMinDenominator{8};
constexpr double defaultMinDenominatorNormalized{
    static_cast<double>(defaultMinDenominator) / baseDenominator};

constexpr int defaultMaxDenominator{32};
constexpr double defaultMaxDenominatorNormalized{
    static_cast<double>(defaultMaxDenominator) / baseDenominator};
} // namespace Constants
} // namespace Stutter840
