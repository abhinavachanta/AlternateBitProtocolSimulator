/** \brief Header file for subnet
*
*subnets passes the packets after some a time delay.
* in order to simulate the unreliability of the network,
* only 95% of the packets will be passed in each of the subnet,
* 5% of the data will be lost through the subnet.
* subnet has 2 phases active and passive. intially it is in passive phase
* whenever packet is received subnet changes to active phase. 
* when there is no data transmission or acknowledgement it goes to passive phase
*/

/**
* Author            : Dr. Cristina Ruiz Martin
* Organization      : ARSLab - Carleton University
* Modified by       : Rajeev kumar Jandhyala
*/

#ifndef _BOOST_SIMULATION_PDEVS_SUBNET_HPP_
#define _BOOST_SIMULATION_PDEVS_SUBNET_HPP_

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

	/** Port definition 
	* structure consists of class public output and input Messages,
    */

	struct subnet_defs{
		struct out : public out_port<message_t> {
		};
		struct in : public in_port<message_t> {
        };
    };
	 
	
    template<typename TIME>

    class Subnet{

    	using defs = subnet_defs; 
        /** 
        * putting reciever definitions in context.
        */
        public:
         
        	Subnet() noexcept{
                /*subnet constructor initializing state trasmiting to false and index to zero.
            /**
            * subnet constructor initializing state trasmiting to false and index to zero.
            */
				state.transmiting = false; 

				state.index = 0;
            }
                
            /**
            * structure state_type definition and declaring packet,index and transmiting values
            */
            
            struct state_type{
				bool transmiting;
				int packet;
				int index;
            };
            state_type state;

            /** 
            * ports definition 
            */
            using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<typename defs::out>;

            /** 
            * internal_transition function returns void  and intializing sending variable. 
            */
            void internal_transition() {
				state.transmiting = false;  
            }
			/** 
            * funciton external_transition gets the number of messages is greater than 1.
            * assert to false and concatenate with one message per time unit 
            * initialize the state acknowledgement value to x.value and sending to true 
            */ 
            void external_transition(TIME e,
							         typename make_message_bags<input_ports>::type mbs) { 
                state.index ++;
                if(get_messages<typename defs::in>(mbs).size()>1) {
					assert(false && "One message at a time");
				}	
                for (const auto &x : get_messages<typename defs::in>(mbs)) {
					state.packet = static_cast < int > (x.value);
					state.transmiting = true; 
                }               
            }

            /** 
            * confluence_transition function contains two functions internal_transistion 
            * and external_transition 
            * @param e  type time 
            * @param mbs type message bag
            */ 
            void confluence_transition(TIME e,
				                      typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }

 			/** 
            * output function sends message with probability 95%.
 			* with packet loss 5% to ouptut port
 			* @return message bags
            */

            typename make_message_bags<output_ports>::type output() const {
						typename make_message_bags<output_ports>::type bags;
				message_t out;
				if ((double)rand() / (double) RAND_MAX  < 0.95){
					out.value = state.packet;
					get_messages<typename defs::out>(bags).push_back(out);
				}
				return bags;
            }

            /** 
            * time_advance function declares the variable next_internal.
            * if state.transmiting  is true then variable set to next_internal
            * time value obtained from distribution function  else value is infinity 
            * @return next_interval
            */

            TIME time_advance() const {
				std::default_random_engine generator;
				std::normal_distribution<double> distribution(3.0, 1.0); 
				TIME next_internal;
				if (state.transmiting) {
					std::initializer_list<int> time = {0, 0,static_cast < int > 
											          (round(distribution(generator)))};
					/** 
                    * time is hour min and second.
                    */
					next_internal = TIME(time);
				}else {
					next_internal = std::numeric_limits<TIME>::infinity();
					}    
                return next_internal;
            }

            /** 
            * friend is a type grant member-level access to functions to return output is index and transmitting to ostring stream.
            * @param os output stream
            * @param i 
            * @return os  
            */

            friend std::ostringstream& operator<<(std::ostringstream& os,
												  const typename Subnet<TIME>::state_type& i) {
                os << "index: " << i.index << " & transmiting: " << i.transmiting; 
                return os;
            }
        };    
#endif // _BOOST_SIMULATION_PDEVS_SUBNET_HPP_