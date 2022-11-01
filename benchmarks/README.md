# Benchmarks

## Organization
- `./benchmarks`: everything related to benchmarking and the source code/scripts.
- `./benchmarks/results`: all measurement results as `.json`.
- `./benchmarks/figures`: all figures made from results e.g. as `.png`.
- `./benchmarks/run_current_benchmark`: script to execute a benchmark
- `./benchmarks/make_figures`: script to to create figures

note: It is recommended to create subdirectories in the `results` and `figures` directories for individual hardware architectures and/or CPUs.

## Run Benchmark Batch
1. Configure `run_current_benchmark` for the current benchmark.
2. Taylor `GENERAL_FLAGS` in this script to your needs (e.g. include additional frameworks).
2. Execute the following command:
```bash
./benchmarks/run_current_benchmark > ./benchmarks/results/<architecture>/<benchmark_name>.json
```
This creates a database file with the runtimes and meta data.

## Make Figure
1. Open Mathematica (v13+)
2. Change the current working directory via `SetDirectory[]` to `./benchmarks`.
3. Evaluate the entire file (or import it).
4. Adapt the command listed under the 'Make Figure' section to your needs (see further explanation there).
5. Run the adapted command.

This reads from the `results` and writes to the `figures` directory.

note: Custome figures can be created from the 'Benchmark Figures Function' section as well.