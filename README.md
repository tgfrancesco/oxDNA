# oxDNA

**This repository contains a new (and not fully tested) version of oxDNA. If you are not a developer or a tester we advise you to use the [sourceforge](https://sourceforge.net/projects/oxdna/) repo.**

oxDNA is a simulation code that was initially conceived as an implementation of the coarse-grained DNA model introduced by [T. E. Ouldridge, J. P. K. Doye and A. A. Louis](http://dx.doi.org/10.1063/1.3552946). It has been since reworked and it is now an extensible simulation+analysis framework. It natively supports DNA, RNA, Lennard-Jones and patchy particle simulations of different kinds on both CPU and NVIDIA GPUs.

The development of this software has been partially supported by the European Commission through the Marie Skłodowska−Curie Fellowship No. 702298-DELTAS.

## Requirements

The code requires `cmake` and a c++-14-compliant `g++` (any version >= 4.9 *should* work). The code should be also compilable with the Intel compiler (with the `-DIntel=ON` `cmake` flag, see below), although this has not been tested with newer oxDNA versions. 

### CUDA

Compiling with CUDA support requires `cmake` >= 3.5 and the CUDA toolkit (>= 4.0).

### Documentation

Generating the documentation (with `make docs`, see below) requires [doxygen](http://www.doxygen.nl/). 

## Compiling oxDNA

Extract the oxDNA archive and then:

```
cd oxDNA         # enter the oxDNA folder
mkdir build      # create a new build folder. It is good practice to compile out-of-source
cd build
cmake ..         # here you can specify additional options, see next section
make -j4         # compile oxDNA. The -jX make option makes it compile the code in parallel by using X threads.
```

At the end of the compilation three executables (oxDNA, DNAnalysis and confGenerator) will be placed
in the build/bin folder.

## `cmake` options
-------------

* `-DCUDA=ON` Enables CUDA support
* `-DCUDA_COMMON_ARCH=ON` Choose the target CUDA compute architecture based on the nvcc version. Set it to off to autodetect the CUDA compute arch GPU installed.
* `-DDebug=ON` Compiles with debug symbols and without optimisation flags
* `-DG=ON` Compiles with debug symbols + optimisation flags
* `-DINTEL=ON` Uses INTEL's compiler suite
* `-DMPI=ON` Compiles oxDNA with MPI support
* `-DCXX11=ON` Compiles oxDNA with c++11 support
* `-DSIGNAL=OFF` Handling system signals is not always supported. Set this flag to OFF to remove this feature
* `-DMOSIX=ON` Makes oxDNA compatible with MOSIX
* `-DDOUBLE=OFF` Set the numerical precision of the CPU backend to `float`
* `-DCUDA_DOUBLE=ON` Set the numerical precision of the CUDA backend to `double`. This is incompatible with the `mixed` precision

## `make` targets

* `make` compiles oxDNA
* `make docs` Produces html doxygen documentation for oxDNA (`DOCS/html_oxDNA/index.html`) and for the UTILS folder (`DOCS/html_UTILS/index.html`)
* `make rovigatti` Compiles the observables and interactions in contrib/rovigatti
* `make romano` Compiles the observables and interactions in contrib/romano
	
## Testing

* `make test_run` Runs quick tests to check whether oxDNA has been correctly compiled or not.	
* `make test_quick` Runs longer tests to check that oxDNA works (not fully implemented yet, but the main features are supported). 

## Usage

`oxDNA input_file`

Most of the options that can be specified in the input file can be found in the `input_options.md` file.

## Output files

The energy.dat (default name, can be changed in the configuration file) has this layout for MD:
time potential_energy kinetic_energy total_energy

The energy.dat (default name, can be changed in the configuration file) has this layout for MC:
time potential_energy hydrogen_bonding_energy acc_trasl acc_rot [acc_volume]

Mind that potential, kinetic and total energies are divided by the number of particles.

Configurations are saved in the trajectory file (appended one after the other).

## Observables

The observable infrastructure was devised to help building customized output from `oxDNA` (and `DNAnalysis`) without having to dive in the simulation code itself. 

The relevant keys in the input file are analysis_data_output_* and data_output_* (see below). These take as an argument, between curly brackets, a series of lines that is interpreted as an input file.
An example is:

```
data_output_1 = {
name = caca.dat
print_every = 100
col_1 = {
type = potential_energy
}
col_2 = {
type = step
units = MD
}
}
```

this will print in caca.dat two colums, the first with the potential energy and the second with the steps in MD units (`dt`-aware). 

The lines in between curly brackets are interpreted as input files by the single observables.

See also the doxygen documentation for `Observables/ObservableOutput.h`

oxDNA provides a plugin infrastructure that supports, among other things, Observables. See the doxygen documentation for `PluginManagement/PluginManager.h`