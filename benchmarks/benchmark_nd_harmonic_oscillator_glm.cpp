

#include <boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

#include <glm/glm.hpp>

#include "benchmark.hpp"

// using Scalar = float;
// using Scalar = double;
// using Scalar = long double;

using Scalar = double;

constexpr std::size_t DIM{2};
using Vec = glm::vec<DIM, Scalar, glm::defaultp>;
using PhaseVec = glm::vec<2 * DIM, Scalar, glm::defaultp>;

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
    runge_kutta4<PhaseVec, Scalar, PhaseVec, Scalar, vector_space_algebra>
        stepper;
    integrate_const(stepper, harmonic_oscillator, phase, 0.0, 10.0, 0.01);
}
