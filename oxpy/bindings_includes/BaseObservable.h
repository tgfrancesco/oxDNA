/*
 * BaseObservable.h
 *
 *  Created on: Mar 21, 2020
 *      Author: lorenzo
 */

#ifndef OXPY_BUILD_DOCS_BINDINGS_INCLUDES_BASEOBSERVABLE_H_
#define OXPY_BUILD_DOCS_BINDINGS_INCLUDES_BASEOBSERVABLE_H_

#include "../python_defs.h"

#include <Observables/BaseObservable.h>

// trampoline class for BaseObservable
class PyBaseObservable : public BaseObservable {
public:
	using BaseObservable::BaseObservable;

	std::string get_output_string(llint curr_step) override {
		PYBIND11_OVERLOAD_PURE( // @suppress("Unused return value")
				std::string,
				BaseObservable,
				get_output_string,
				curr_step
		);

		return std::string();
	}
};

void export_BaseObservable(py::module &m) {
	py::class_<BaseObservable, PyBaseObservable, std::shared_ptr<BaseObservable>> obs(m, "BaseObservable", R"pbdoc(
		The interface class for observables.
	)pbdoc");

	obs.def(py::init<>(), R"pbdoc(
        The default constructor takes no parameters.
	)pbdoc");

	obs.def("get_settings", &BaseObservable::get_settings, py::arg("my_inp"), py::arg("sim_inp"), R"pbdoc(
		Computes the quantity/quantities of interest and returns the output string.

		Parameters
		---------- 
		my_inp: :class:`input_file`
			The input file of the observable.
		sim_inp: :class:`input_file`
			The general input file of the simulation.
	)pbdoc");

	obs.def("init", &BaseObservable::init, py::arg("config_info"), R"pbdoc(
		Initialises the observable.

		Parameters
		---------- 
		config_info: :class:`ConfigInfo`
			The singleton object storing the simulation details.
	)pbdoc");

	obs.def("get_output_string", &BaseObservable::get_output_string, py::arg("curr_step"), R"pbdoc(
		Computes the quantity/quantities of interest and returns the output string.

        Parameters
        ---------- 
        curr_step: int
            The current simulation step.

        Returns
        -------
        str
            The output of the observable.
	)pbdoc");
}

#endif /* OXPY_BUILD_DOCS_BINDINGS_INCLUDES_BASEOBSERVABLE_H_ */