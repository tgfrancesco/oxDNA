/*2
 * OxpyManager.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: lorenzo
 */

#include "OxpyManager.h"

/**
 * First converts the argument to an array of char * and then builds the input_file with it. The first element of the new array is
 * initialised to "oxpy" because SimManager expectes an argv-like vector, with the first element containing the name of the program.
 *
 * @param to_convert
 * @return
 */
input_file _input_file_from_filename(std::string input_filename) {
	input_file input;
	input.init_from_filename(input_filename.c_str());

	return input;
}

OxpyManager::OxpyManager(std::string input_filename) :
				SimManager(_input_file_from_filename(input_filename)) {

}

OxpyManager::OxpyManager(input_file input) :
				SimManager(input) {

}

OxpyManager::~OxpyManager() {

}

std::shared_ptr<ConfigInfo> OxpyManager::config_info() {
	return CONFIG_INFO;
}

number OxpyManager::system_energy() {
	return CONFIG_INFO->interaction->get_system_energy(CONFIG_INFO->particles(), CONFIG_INFO->lists);
}

void OxpyManager::print_configuration(bool also_last) {
    // prints the trajectory configuration
    _backend->print_conf(_cur_step);
    // prints the last configuration
    _backend->print_conf(_cur_step, false, true);
}

void OxpyManager::run(llint steps, bool print_output) {
	if(_cur_step < _start_step) {
		_cur_step = _start_step;
	}

	_backend->apply_changes_to_simulation_data();

	for(llint i = 0; i < steps && !SimManager::stop; i++, _cur_step++) {
		if(_cur_step == _time_scale_manager.next_step) {
			if(print_output && _cur_step > _start_step) {
				_backend->print_conf(_cur_step);
			}
			setTSNextStep(&_time_scale_manager);
		}

		if(_cur_step > 0 && _cur_step % _fix_diffusion_every == 0) {
			_backend->fix_diffusion();
		}

		if(print_output) {
			_backend->print_observables(_cur_step);
		}

		_backend->sim_step(_cur_step);
	}

	_backend->apply_simulation_data_changes();
}

void export_SimManager(py::module &m) {
	pybind11::class_<SimManager, std::shared_ptr<SimManager>> manager(m, "SimManager");

	manager.def("load_options", &SimManager::load_options, R"pbdoc(
		Load the options from the input file.
	)pbdoc");

	manager.def("init", &SimManager::init, R"pbdoc(
		Initialise the simulation manager.
	)pbdoc");

	manager.def("run_complete", &SimManager::run, R"pbdoc(
        Run the simulation till completion (that is, till the simulation has run number of steps specified in the input file).
	)pbdoc");
}

void export_OxpyManager(py::module &m) {
	py::class_<OxpyManager, SimManager, std::shared_ptr<OxpyManager>> manager(m, "OxpyManager", R"pbdoc(
		The object in charge of initialising and running a simulation. 
	)pbdoc");

	manager.def(py::init<std::string>(), py::arg("args"), R"pbdoc(
The default constructor takes the input filename as its only parameter.

Parameters
----------
input_filename: str
    The name of the input file that will be used to run the simulation
	)pbdoc");

	manager.def(py::init<input_file>(), py::arg("input"), R"pbdoc(
This constructor takes an :class:`InputFile` as its only argument.

Parameters
----------
input: :class:`InputFile`
    The object storing the simulations' options.
	)pbdoc");

	manager.def("print_configuration", &OxpyManager::print_configuration, py::arg("also_last") = true, R"pbdoc(
        Append the current configuration to the trajectory file and, by default, print it in the "lastconf_file".

        Parameters
        ----------
            also_last : bool
                If True (default value) prints the current configuration also in the "lastconf_file" file.  
	)pbdoc");

	manager.def("config_info", &OxpyManager::config_info, R"pbdoc(
		Return the simulation's :class:`ConfigInfo`.

        Returns
        -------
        :class:`ConfigInfo`
            The object that stores all the simulation's details (particles, interaction, `etc`).
	)pbdoc");

	manager.def("system_energy", &OxpyManager::system_energy, R"pbdoc(
		Compute the potential energy of the system in its current state.

        Returns
        -------
        float
            The system's potential energy.
	)pbdoc");

	manager.def("run", &OxpyManager::run, pybind11::arg("steps"), pybind11::arg("print_output") = true, R"pbdoc(
		Run the simulation for the given number of steps. The second argument controls whether the simulations output (configurations and observables) should be printed or not.

        Parameters
        ----------
            steps: int
                The number of steps to be run.
            print_output: bool
                If True (the default value) the simulation output will be printed.
	)pbdoc");
}
