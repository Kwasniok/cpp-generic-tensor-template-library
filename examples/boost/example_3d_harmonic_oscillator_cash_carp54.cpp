// clang-format off
#include <iostream>

#include <gttl/extensions/boost/numeric/odeint.hpp>

#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/stepper/generation/generation_runge_kutta_cash_karp54.hpp>
#include <boost/numeric/odeint/stepper/generation/make_controlled.hpp>

#include <gttl.hpp>

using namespace gttl::literals; // for _D

using Scalar = double;

template <std::size_t N>
using Dims = gttl::Dimensions<N>;

using Vec = gttl::Tensor<Scalar, 1, Dims<1>{3_D}>;
using PhaseVec = gttl::Tensor<Scalar, 1, Dims<1>{6_D}>;

constexpr Vec k{0.1, 0.2, 0.3};
constexpr Vec gam{0.25, 0.15, 0.25};

void
harmonic_oscillator(const PhaseVec& p, PhaseVec& dpdt, const Scalar /* t */)
{
    constexpr std::size_t DIM{3};

    for (std::size_t i{0}; i < DIM; ++i) {
        // dxdt = v
        dpdt[0 + i] = p[DIM + i];
        // dvdt = -k * x -2 * gamma * v
        dpdt[DIM + i] = -k[i] * p[0 + i] - 2 * gam[i] * p[DIM + i];
    }
}

int main()
{
    using namespace boost::numeric::odeint;
    using std::cout;

    constexpr Scalar initial_dt = 0.01;
    constexpr Scalar t_start = 0.0;
    constexpr Scalar t_stop = 10.0;
    constexpr Scalar error_abs = 1.0e-2;
    constexpr Scalar error_rel = 1.0e-2;

    PhaseVec phase = {1, 1, 1, 0, 0, 0};

    cout << "phase(t=" << t_start << "):  ";
    cout << phase << '\n';

    auto stepper = make_controlled(
        error_abs, error_rel, runge_kutta_cash_karp54<PhaseVec, Scalar>()
    );
    auto steps = integrate_adaptive(
        stepper, harmonic_oscillator, phase, t_start, t_stop, initial_dt
    );

    cout << "phase(t=" << t_stop << "): ";
    cout << phase << '\n';
    cout << "(in " << steps << " steps)" << '\n';
}
