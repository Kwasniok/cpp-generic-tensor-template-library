#include <cmath>
#include <numbers>
#include <numeric>

#include <boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

#include <gttl.hpp>
#include <gttl/extensions/boost/numeric/odeint/tensor.hpp>

#include "benchmark.hpp"

using namespace gttl::literals; // for _D

// using Scalar = float;
// using Scalar = double;
// using Scalar = long double;

using Scalar = double;

template <std::size_t RANK>
using Dims = gttl::Dimensions<RANK>;

using Coords3 = std::array<Scalar, 3>;
using Vec3 = gttl::Tensor<Scalar, 1, Dims<1>{3_D}>;
using PhaseVec = gttl::Tensor<Scalar, 1, Dims<1>{6_D}>;
using Mat3x3 = gttl::Tensor<Scalar, 2, Dims<2>{3_D, 3_D}>;
using Ten3x3x3 = gttl::Tensor<Scalar, 3, Dims<3>{3_D, 3_D, 3_D}>;

// note: This code uses spherical coordinates

Mat3x3
metric(const Coords3& coords)
{
    using std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    // clang-format off
    return {
        1, 0,         0,
        0, pow(r, 2), 0,
        0, 0,         pow(r * sin(theta), 2),
    };
    // clang-format on
}

Mat3x3
inverse_metric(const Coords3& coords)
{
    using std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    // clang-format off
    return {
        1, 0,          0,
        0, pow(r, -2), 0,
        0, 0,          pow(r * sin(theta), -2),
    };
    // clang-format on
}

Ten3x3x3
christoffel_1(const Coords3& coords)
{
    using std::cos, std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    const auto stheta = sin(theta);
    const auto ctheta = cos(theta);
    const auto stheta2 = pow(stheta, 2);
    const auto r2 = pow(r, 2);
    // clang-format off
    return {
        0, 0, 0,
        0,-r, 0,
        0, 0, -r * stheta2,

        0, r, 0,
        r, 0, 0,
        0, 0, -r2 * stheta * ctheta,

        0,           0,                    r * stheta2,
        0,           0,                    r2 * stheta * ctheta,
        r * stheta2, r2 * stheta * ctheta, 0,
    };
    // clang-format on
}

Ten3x3x3
christoffel_2(const Coords3& coords)
{
    return gttl::contraction<1, 2>(
        inverse_metric(coords), christoffel_1(coords)
    );
}

void
geodesic_equation(const PhaseVec& p, PhaseVec& dpdt, const Scalar /* t */)
{
    constexpr std::size_t DIM{3};

    // dx^idt = v^i
    for (std::size_t i{0}; i < DIM; ++i) {
        dpdt[0 + i] = p[DIM + i];
    }

    // dv^idt = -Gamma^i_j_k v^j v^k
    const Coords3 coords{p[0], p[1], p[2]};
    const Vec3 velocity{p[3], p[4], p[5]};
    const Ten3x3x3 chris2 = christoffel_2(coords);

    const Vec3 fictitious_force =
        contraction<1, 2>(contraction<1, 3>(chris2, velocity), velocity);

    for (std::size_t i{0}; i < DIM; ++i) {
        dpdt[DIM + i] = -fictitious_force[i];
    }
}

PhaseVec phase;

void
benchmark_body()
{
    using namespace boost::numeric::odeint;
    using std::numbers::pi;

    // clang-format off
    phase = {
        1.0, pi / 4.0, pi / 2.0,
        0.0, 1.0,      1.0,
        };
    // clang-format oon
    runge_kutta4<PhaseVec, Scalar> stepper;
    integrate_const(stepper, geodesic_equation, phase, 0.0, 10.0, 0.1);
}
