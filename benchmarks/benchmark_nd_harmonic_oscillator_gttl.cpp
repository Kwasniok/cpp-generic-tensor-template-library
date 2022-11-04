

#include <boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

#include <gttl.hpp>
#include <gttl/extensions/boost/numeric/odeint.hpp>

#include "benchmark.hpp"

using namespace gttl::literals; // for _D

// using Scalar = float;
// using Scalar = double;
// using Scalar = long double;

using Scalar = double;

constexpr std::size_t DIM{2};

template <std::size_t N>
using Dims = gttl::Dimensions<N>;

using Vec = gttl::Tensor<Scalar, 1, Dims<1>{DIM}>;
using PhaseVec = gttl::Tensor<Scalar, 1, Dims<1>{2 * DIM}>;

constexpr Vec k{0.1, 0.2};
constexpr Vec gam{0.25, 0.15};

void
harmonic_oscillator(const PhaseVec& p, PhaseVec& dpdt, const Scalar /* t */)
{
    for (std::size_t i{0}; i < DIM; ++i) {
        // dxdt = v
        dpdt[0 + i] = p[DIM + i];
        // dvdt = -k * x -2 * gamma * v
        dpdt[DIM + i] = -k[i] * p[0 + i] - 2 * gam[i] * p[DIM + i];
    }
}

PhaseVec phase;

void
benchmark_body()
{
    using namespace boost::numeric::odeint;

    phase = {1, 1, 0, 0};
    runge_kutta4<PhaseVec, Scalar> stepper;
    integrate_const(stepper, harmonic_oscillator, phase, 0.0, 10.0, 0.01);
}
