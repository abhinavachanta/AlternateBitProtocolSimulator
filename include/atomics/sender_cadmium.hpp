/**
 *Cristina Ruiz Martin
 *ARSLab - Carleton University
 */

#ifndef _BOOST_SIMULATION_PDEVS_SENDER_HPP_

#define _BOOST_SIMULATION_PDEVS_SENDER_HPP_
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

//Port definition
struct sender_defs{
	struct packet_sent_out: public out_port <message_t> {};
    struct ack_received_out: public out_port <message_t> {};
    struct data_out: public out_port <message_t> {};
    struct control_in: public in_port <message_t> {};
    struct ack_in: public in_port <message_t> {};
};

template <typename TIME>
    class Sender
    {
        using definitions = sender_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME PREPARATION_TIME;
			TIME TIME_OUT;
			// default constructor
			Sender() noexcept{\
				PREPARATION_TIME = TIME("00:00:10");
				TIME_OUT = TIME("00:00:20");
				state.alt_bit = 0;
				state.next_internal = std::numeric_limits <TIME>::infinity();
				state.model_active = false;
			}

			// state definition
			struct state_type{
				bool ack;
				int packet_number;
				int total_packet_number;
				int alt_bit;
				bool sending;
				bool model_active;
				TIME next_internal;
			};
			state_type state;
			// ports definition
			using input_ports = std::tuple <typename definitions::control_in, 
											typename definitions::ack_in> ;
			using output_ports = std::tuple <typename definitions::packet_sent_out, 
											typename definitions::ack_received_out, 
											typename definitions::data_out> ;

			// internal transition
			void internal_transition(){			
				if (state.ack){
					if (state.packet_number < state.total_packet_number){
						state.packet_number++;
						state.ack = false;
						state.alt_bit = (state.alt_bit + 1) % 2;
						state.sending = true;
						state.model_active = true;
						state.next_internal = PREPARATION_TIME;
					}else{
						state.model_active = false;
						state.next_internal = std::numeric_limits <TIME>::infinity();
					}
				}else{
				    if (state.sending){
					    state.sending = false;
						state.model_active = true;
						state.next_internal = TIME_OUT;
					}else{
						state.sending = true;
						state.model_active = true;
						state.next_internal = PREPARATION_TIME;
					}
				}
			}

			// external transition
			void external_transition(TIME e,
								     typename make_message_bags <input_ports>::type mbs){
        
				if ((get_messages <typename definitions::control_in> (mbs).size() +
					get_messages <typename definitions::ack_in> (mbs).size()) > 1){
					assert(false && "one message per time uniti");
				}
				for (const auto &x: get_messages <typename definitions::control_in> (mbs)){
					if (state.model_active == false){
						state.total_packet_number = static_cast <int> (x.value);
						if (state.total_packet_number > 0){
							state.packet_number = 1;
							state.ack = false;
							state.sending = true;
							state.alt_bit = state.packet_number % 2; //set initial alt_bit
							state.model_active = true;
							state.next_internal = PREPARATION_TIME;
						}else{
							if (state.next_internal != std::numeric_limits <TIME>::infinity()){                        
								state.next_internal = state.next_internal - e;
							}
						}
				    }
				}
				for (const auto &x: get_messages <typename definitions::ack_in> (mbs)){       
					if (state.model_active == true){
						if (state.alt_bit == static_cast <int> (x.value)){
							state.ack = true;
							state.sending = false;
							state.next_internal = TIME("00:00:00");
						}else{                    
	                        if (state.next_internal != std::numeric_limits <TIME>::infinity()){                        
	                            state.next_internal = state.next_internal - e;
	                        }
	                    }
	                }
	            }
	        }

			// confluence transition
			void confluence_transition(TIME e, 
								       typename make_message_bags <input_ports>::type mbs){
			    internal_transition();
				external_transition(TIME(), std::move(mbs));
			}

			// output function
			typename make_message_bags <output_ports>::type output() const {
				typename make_message_bags <output_ports>::type bags;
				message_t out;
				if (state.sending)
				{
					out.value = state.packet_number * 10 + state.alt_bit;
					get_messages <typename definitions::data_out> (bags).push_back(out);
					out.value = state.packet_number;
					get_messages <typename definitions::packet_sent_out> (bags).push_back(out);
				}else{
					if (state.ack){
						out.value = state.alt_bit;
						get_messages <typename definitions::ack_received_out> (bags).push_back(out);
				    }
				}
				return bags;
			}
	        
			// time_advance function
			TIME time_advance() const {
				return state.next_internal;
			}
			friend std::ostringstream &operator << (std::ostringstream &os, 
												    const typename Sender <TIME>::state_type &i){
				os << "packet_number: " << i.packet_number << " &total_packet_number: " <<
				i.total_packet_number;
				return os;
			}
		};

#endif // _BOOST_SIMULATION_PDEVS_SENDER_HPP_