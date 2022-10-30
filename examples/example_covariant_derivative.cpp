// clang-format off

/*
* Example for calculating the covariant derivative in spherical coordinates.*
*/

#include <cmath>
#include <iostream>
#include <numbers>
#include <gttl.hpp>

using namespace gttl::literals; // for _D

using Scalar = double; 

template<std::size_t RANK>
using Dims = gttl::Dimensions<RANK>;

using Coords3 = std::array<Scalar, 3>;
using Vec3 = gttl::Tensor<Scalar, 1, Dims<1>{3_D}>;
using Mat3x3 = gttl::Tensor<Scalar, 2, Dims<2>{3_D, 3_D}>;
using Ten3x3x3 = gttl::Tensor<Scalar, 3, Dims<3>{3_D, 3_D, 3_D}>;

Mat3x3 metric(const Coords3& coords) {
    using std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    return {
        1, 0,         0,
        0, pow(r, 2), 0,
        0, 0,         pow(r * sin(theta), 2),
    };
}

Mat3x3 inverse_metric(const Coords3& coords) {
    using std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    return {
        1, 0,          0,
        0, pow(r, -2), 0,
        0, 0,          pow(r * sin(theta), -2),
    };
}

Ten3x3x3 christoffel_1(const Coords3& coords) {
    using std::cos, std::sin, std::pow;
    const auto& [r, theta, phi] = coords;
    const auto stheta = sin(theta);
    const auto ctheta = cos(theta);
    const auto stheta2 = pow(stheta, 2);
    const auto r2 = pow(r, 2);
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
}

Ten3x3x3 christoffel_2(const Coords3& coords) {
    return gttl::contraction<1, 2>(inverse_metric(coords), christoffel_1(coords));
}

Vec3 covariant_derivative(const Coords3& coords, const Vec3& v, const Vec3& u, const Mat3x3& du_dx) {

    // du_dx^i_j = du^i/dx^j
    //       0 1

    // contraction scheme:
    // gamma^i_j_k u^j u^k
    //       0 1 2   3   4   (contract over i <-> 1 and 3)
    //       0   1       2   (contract over j <-> 1 and 2)

    return (
        contraction<1, 2>(du_dx, v) +
        contraction<1, 2>(contraction<1, 3>(christoffel_2(coords), u), v)
    );
}

int main()
{
    using std::cout, std::numbers::pi;

    // (r, theta, phi) (ISO 31-11 spherical coordinates)
    Coords3 coords{2, pi / 4, pi / 8};

    cout << "metric(coords)" << '\n';
    cout << (metric(coords)) << '\n';

    cout << '\n';
    cout << "christoffel_1(coords)" << '\n';
    cout << (christoffel_1(coords)) << '\n';

    cout << '\n';
    cout << "christoffel_2(coords)" << '\n';
    cout << (christoffel_2(coords)) << '\n';

    cout << '\n';
    Vec3 velocity{0.5, 1, 1.5};
    Mat3x3 acceleration{
        0.1, 0.2, 0.3,
        0.4, 0.5, 0.6,
        0.7, 0.8, 0.9,
    };
    cout << "covariant_derivative(coords, velocity, velocity, acceleration)" << '\n';
    cout << (covariant_derivative(coords, velocity, velocity, acceleration)) << '\n';
}