/**  \brief This is the implemention of Receiver component
 *
 * This file imports the in-built libraries of cadmium 
 * and vendor library, accept the file as an input, 
 * perform simulator operations w.r.t. receiver,
 * store the output in a file and log the operations.
 *
 */

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include "../../../lib/vendor/NDTime.hpp"
#include "../../../lib/vendor/iestream.hpp"
#include "../../../include/data_structures/message.hpp"
#include "../../../include/atomics/receiver_cadmium.hpp"

using namespace std;
using high_resolution_clock = chrono::high_resolution_clock;
using TIME = NDTime;

/**
 * Setting input ports
 */
struct input_port: public cadmium::in_port <message_t> {};

/**
 * Setting output ports
 */
struct output_port: public cadmium::out_port <message_t> {};

/**
 * This is application generator class.
 * File path of the input file is passed as an
 * argument to this class and a generic template
 * @param message_t 
 * @param T
 */
template <typename T>
    class ApplicationGen: public iestream_input <message_t, T> {
        public: ApplicationGen() = default;

        /**
         * This is a parameterized contructor for application generator class 
         * It takes the file path of the input file as a parameter
         * @param file_path
         */
        ApplicationGen(const char *file_path): 
	iestream_input <message_t, T> (file_path) {}
    };

int main() {

    /** 
     * variable to measure simulation execution time
     */
    auto start = high_resolution_clock::now(); 

    /** 
     * The execution logs and messages are saved to the 
     * 'receiver_test_output.txt' file
     */
    static std::ofstream out_data("test/data/receiver_test_output.txt");

    /**
     * Structure which invokes the ostream sink function
     * and then returns the stored data
     */
    struct oss_sink_provider {
        static std::ostream &sink() {
            return out_data;
        }
    };

    /**
     * Storing the inbuilt cadmium logger definitions to variables
     */
    using info = cadmium::logger::logger <cadmium::logger::logger_info,
		                                  cadmium::dynamic::logger::formatter <TIME>, 
				                          oss_sink_provider> ;
    using debug = cadmium::logger::logger <cadmium::logger::logger_debug,
		                                   cadmium::dynamic::logger::formatter <TIME>, 
				                           oss_sink_provider> ;
    using state = cadmium::logger::logger <cadmium::logger::logger_state,
		                                   cadmium::dynamic::logger::formatter <TIME>, 
                                           oss_sink_provider> ;
    using log_messages = cadmium::logger::logger <cadmium::logger::logger_messages,
						                          cadmium::dynamic::logger::formatter <TIME>, 
						                          oss_sink_provider> ;
    using routing = cadmium::logger::logger<cadmium::logger::logger_message_routing,
					                        cadmium::dynamic::logger::formatter <TIME>,
					                        oss_sink_provider> ;
    using global_time = cadmium::logger::logger<cadmium::logger::logger_global_time,
						                        cadmium::dynamic::logger::formatter <TIME>, 
						                        oss_sink_provider> ;
    using local_time = cadmium::logger::logger <cadmium::logger::logger_local_time,
					                            cadmium::dynamic::logger::formatter <TIME>, 
					                            oss_sink_provider> ;
    using log_all = cadmium::logger::multilogger <info, 
                                                  debug,  
                                                  state, 
                                                  log_messages,
				                                  routing, 
                                                  global_time, 
                                                  local_time> ;
    using logger_top = cadmium::logger::multilogger <log_messages,global_time> ;

    /**
     * Specify the input control file and set it as 
     * input data control for execution of the receiver component
     */
    string input_data_control = "test/data/receiver_input_test.txt"; 
    const char *i_input_data_control = input_data_control.c_str();

    /**
     * This initializes the generator component which
     * generates the output taking in account the Time 
     * and input data control
     */
    std::shared_ptr <cadmium::dynamic::modeling::model> generator = 
    cadmium::dynamic::translate::make_dynamic_atomic_model
    < ApplicationGen, TIME, const char *> 
    ("generator", std::move(i_input_data_control));

    /**
     * Initialize the receiver component
     */
    std::shared_ptr <cadmium::dynamic::modeling::model> receiver1 = 
    cadmium::dynamic::translate::make_dynamic_atomic_model 
    <Receiver, TIME> ("receiver1");

    /**
     * Declare variables to hold top model 
     * components which are generated using receiver
     */
    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {
	   typeid(output_port)
    };
    cadmium::dynamic::modeling::Models submodels_TOP = {
        generator, receiver1
    };
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC 
	    <receiver_defs::out, output_port> ("receiver1")
    };
    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC <iestream_input_defs <message_t>::out, 
		                                      receiver_defs:: in> ("generator", 
                                                                   "receiver1")
    };
    std::shared_ptr <cadmium::dynamic::modeling::coupled <TIME>> TOP = 
    std::make_shared <cadmium::dynamic::modeling::coupled <TIME>> (
                                								   "TOP",
                                								   submodels_TOP,
                                							       iports_TOP,
                                								   oports_TOP,
                                								   eics_TOP,
                                								   eocs_TOP,
                                								   ics_TOP
                                								   );

    /**
     * Declare model to measure and store the time elapsed (in sec)  
     * since the start of the generator. Then print this in output file
     */
    auto elapsed1 = std::chrono::duration_cast <std::chrono::duration<double, 
                    std::ratio < 1>>> (high_resolution_clock::now() - start).count();
    cout << "Model Created. Elapsed time: " << elapsed1 << "sec" << endl;

    /**
     * Declare runner component to measure and store the time elapsed (in sec) 
     * since the start of the generator. Then print this in output file
     */
    cadmium::dynamic::engine::runner <NDTime, logger_top> r(TOP, { 0 });
    elapsed1 = std::chrono::duration_cast <std::chrono::duration <double, 
               std::ratio < 1>>> (high_resolution_clock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << elapsed1 << "sec" << endl;

    /**
     * Print message when the simulation starts.
     */
    cout << "Simulation starts" << endl;

    /**
     * Run the simulation until 04:00:00:000 time frame
     */
    r.run_until(NDTime("04:00:00:000"));

    /**
     * Measure the simulation time and print it
     */
    auto elapsed = std::chrono::duration_cast <std::chrono::duration <double, 
                   std::ratio < 1>>> (high_resolution_clock::now() - start).count();
    cout << "Simulation took:" << elapsed << "sec" << endl;
    return 0;
}