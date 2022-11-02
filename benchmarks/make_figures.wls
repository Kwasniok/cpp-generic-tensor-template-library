#!/usr/bin/env wolframscript
(* ::Package:: *)

(* ::Text:: *)
(*IMPORTANT : This script is to be run from the directory ' benchmarks'!*)


(* ::Title:: *)
(*Prelude *)


$PlotTheme = "Default";


interpretAsQuantity[string_] := Interpreter["Quantity"
][string]
interpretAsNumber[string_] := Interpreter["Number"
][string]


(* ::Subtitle:: *)
(*Data Analysis (from JSON)*)


get[key_] := #[[key]]&

getAll[key_, value_] := Select[(#[[key]]) == value&]

readRuns[path_] := Import[path, "RawJSON"] [["runs"]]

getUniques[key_] :=(Map[ get[key],#] // DeleteDuplicates) &
getCompilers := getUniques["compiler"]
getOptimizationFlags:= getUniques["optimization_flags"]
getFrameworks := getUniques["framework"]
getNames := getUniques["name"]

selectWithName[name_] := getAll["name", name]
selectWithCompiler[name_] := getAll["compiler", name]
selectWithOptimizationFlags[flags_] := getAll["optimization_flags", flags]
selectWithFramework[framework_] := getAll["framework", framework]

gatherByCompilers[compilers_] := Table[# //selectWithCompiler[compiler], {compiler, compilers}]& 
gatherByOptimizationFlags[optimizationFlags_] := Table[# //selectWithOptimizationFlags[optFlags], {optFlags, optimizationFlags}]&
gatherByFramework[frameworks_] := Table[# //selectWithFramework[framework], {framework, frameworks}]&

gatherAsCompilerOptimizationFlagsMatrix[compilers_, optimizationFlags_] := 
Map[gatherByOptimizationFlags[optimizationFlags]] @* gatherByCompilers[compilers]


(*
must respect bar element format of either:
 - value
 - value -> meta
 
 here, meta = standard deviation
*)
makeStatistic[values_] := Mean[values] -> StandardDeviation[values]


(* ::Subtitle:: *)
(*Bar Chart Plot with Error Bar*)


getValue[value_Rule] := value // Keys
getValue[value_Quantity] := value

mono[str_] := Style[str, FontFamily->"Bitstream Vera Sans Mono"]
nicerValue[value_, targetUnit_] := N[UnitConvert[getValue[value] , targetUnit], 2] // DecimalForm

barLabel[targetUnit_][valueOrRule_] := Labeled[valueOrRule, angle[nicerValue[getValue[valueOrRule], targetUnit]], Above]
angle[x_] := Rotate[x, -((3\[Pi])/8)]

(* taken from Mathematica documentation and adapted *)
rectangleWithErrorBar[ {{x0_, x1_}, {y0_, y1_}}, value_, meta_] := Block[
	{stdDiv},

	stdDiv = Flatten[QuantityMagnitude[meta]] // First;
	If[stdDiv === {}, 
		{
			ChartElementData["Rectangle"][{{x0, x1}, {y0, y1}}, value, meta]
		},
		{
			ChartElementData["Rectangle"][{{x0, x1}, {y0, y1}}, value, meta],
			{
				Gray,
				Line[
					{
						{
							{(x0 + x1)/2, y1 - stdDiv},
							{(x0 + x1)/2, y1 + stdDiv}
						},
						{
							{1/4 (3 x0 + x1), y1 + stdDiv},
							{1/4 (x0 + 3 x1), y1 + stdDiv}
						},
						{
							{1/4 (3 x0 + x1), y1 - stdDiv},
							{1/4 (x0 + 3 x1), y1 - stdDiv}
						}
					}
				]
			}
		}
	]
]



barChart[title_, compilers_, frameworks_, times_, targetUnit_:Quantity["s"]] := BarChart[
	times // Map[Map[barLabel[targetUnit]]] // MapThread[Labeled, {#, (angle @* mono) /@ compilers}]&,
	ChartElementFunction->rectangleWithErrorBar,
	PlotLabel->title,
	ChartLegends->mono /@ frameworks,
	GridLines->Automatic,
	TargetUnits->{targetUnit},
	AxesLabel->Automatic,
	ImageSize->Medium
]


(* ::Text:: *)
(*Dummy Plot (for debugging)*)


(*
title = mono @ "Mat(float, 3, 3) x Mat(float, 3, 3) (\!\(\*SuperscriptBox[\(10\), \(9\)]\) reps.)";
frameworks = {"GTTL", "GLM"};
compilers = {"gcc", "clang"};
flags = {" -03"," -0fast"};
compilersWithFlags = Outer[StringJoin, compilers, flags] // Flatten;
(* times[[framework, compiler]] *)
times = {
(* GTTL *)
	{28637, 28637, 7553, 6653},
(* GLM *)
	{5105, 5217, 7143, 6688}
} // Map[Map[Quantity[#, "ms"]&]];

addDummyError[x_] := x -> Quantity[1000, "ms"];
times = times // Map[Map[addDummyError]]

barChart[title, compilersWithFlags, frameworks, times // Transpose]
Export["test.png", %];
*)


(* ::Subtitle:: *)
(*Benchmark Figures Functions*)


makePlotForBenchmark[databasePath_, benchmarkName_, title_, targetUnit_] := Block[
	{runs, compilers, optimizationFlags, frameworks, names, benchmarks, totalTimes, repititions, cycleTimes, cycleStats, compilersWithFlags},
	runs = readRuns[databasePath];
	compilers = runs // getCompilers;
	optimizationFlags = runs // getOptimizationFlags;
	frameworks = runs // getFrameworks;
	names = runs // getNames;
	
	benchmarks = runs // selectWithName[benchmarkName];
	benchmarks = benchmarks // gatherByFramework[frameworks]// Map[Flatten @* gatherAsCompilerOptimizationFlagsMatrix[compilers, optimizationFlags]];
	
	totalTimes = benchmarks // Map[Map[(#[["total_times"]] // interpretAsQuantity)&]];
	repititions = benchmarks // Map[Map[(#[["repititions"]] // interpretAsNumber)&]];
	cycleTimes = MapThread[MapThread[Divide, {#1, #2}]&, {totalTimes, repititions}];
	cycleTimes = cycleTimes // Map[Map[UnitConvert[#, targetUnit]&]];
	cycleStats = cycleTimes // Map[Map[makeStatistic]];
	
	compilersWithFlags = Outer[StringJoin, compilers, optimizationFlags] // Flatten;
	barChart[title, compilersWithFlags, frameworks, cycleStats // Transpose, targetUnit]
]


(* for convenience *)
makeAndExportStandardPlotForBenchmark[benchmarkPath_, benchmarkName_, benchmarkTitle_] := Block[
	{fig},
	fig = makePlotForBenchmark["results/" <> benchmarkPath <> ".json", benchmarkName, mono[benchmarkTitle], Quantity["ns"]];
	Export["figures/" <> benchmarkPath <> ".png", fig];
	fig
]


(* ::Title:: *)
(*Make Figure*)


(* ::Text:: *)
(*To make a figure for a benchmark call:*)
(**)
(*makeAndExportStandardPlotForBenchmark["path/to/benchmark_xyz", "xyz", "title"]*)
(**)
(*where*)
(**)
(*"path/to/benchmark_xyz" results in *)
(*	"benchmark/results/path/to/benchmark_xyz.json"*)
(*	"benchmark/figures/path/to/benchmark_xyz.png"*)
(*	*)
(*"xyz" is the name of the benchmark (suffix of the binary)*)
(**)
(*"title" is the plot title*)
(**)
