
#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <iostream>

/*
 * to be defined later by individual benchmark
 */
void
benchmark_body();

void
measure(unsigned long repitions, unsigned long warm_up_repitions)
{
    using namespace std::chrono;
    using std::cout;

    // disable function inlining
    // works for gcc 12.2.0 and clang 14.0.6 (linux, i5-4590)
    // IMPORTANT: Needs to be confirmed for each hardware/software combination!
    // Simple check: Empty bechmark_body should take something in the order of
    //               ms for 10^7 repitions.
    volatile auto func = benchmark_body;

    // warm-up
    for (auto i{0ul}; i < warm_up_repitions; ++i) {
        func();
    }

    // measurement
    const auto time_begin{steady_clock::now()};
    for (auto i{0ul}; i < repitions; ++i) {
        func();
    }
    const auto time_end{steady_clock::now()};

    // display results
    // note: Measurements below 1s total time are useless.
    cout << duration_cast<milliseconds>(time_end - time_begin);
}

int
main(int argc, const char** argv)
{
    constexpr auto DEFAULT_REPITITIONS{1000ul};
    constexpr auto DEFAULT_WARM_UP_REPITITIONS{10ul};

    unsigned long repititions = DEFAULT_REPITITIONS;
    unsigned long warm_up_repititions = DEFAULT_WARM_UP_REPITITIONS;

    if (argc >= 2) {
        repititions = std::stol(*(argv + 1));
    }
    if (argc >= 3) {
        warm_up_repititions = std::stol(*(argv + 2));
    }

    measure(repititions, warm_up_repititions);
}

#endif
