/** \brief Header file for receiver 
*
* Behaviour of receive is ro receive the data and send back an acknowledgement extracted 
* from the received data after a time period.
* receiver have two phases: active and passive
* They are in passive phase initially. Whenever they receive a packet, they will be in active phase, 
* and send out acknowledgement. once it is send it goes back into passive phase.
* the delay of the receiver is a constant 
*/

/**
* Author            : Dr. Cristina Ruiz Martin
* Organization      : ARSLab - Carleton University
* Modified by       : Rajeev kumar Jandhyala
*/

#ifndef _BOOST_SIMULATION_PDEVS_RECEIVER_HPP_
#define _BOOST_SIMULATION_PDEVS_RECEIVER_HPP_


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

/** 
* namespace is used to organize code into logical groups and to prevent name collisions.
*/
using namespace cadmium;
using namespace std;

/**
* Structure consists of class public output and input Messages
* Port definition 
*/
    struct receiver_defs {
		struct out : public out_port<message_t> {
		};
		struct in : public in_port<message_t> {
		};
	};
   

    template<typename TIME>
    
    class Receiver {

        
        using defs = receiver_defs;
        /** 
        * Putting reciever definitions in context. 
        */  
        public:
        
        /** 
        * Time constant Parameter
        */
            TIME PREPARATION_TIME;
        
            /** 
            * receiver constructor.
            * initializing acknowledgement to zero and sending to false values.
            */

            /**
            * @brief Constructs the receiver object.
            * Delay constant is initialised, state acknowledgement is set to value zero
            * and  sending state to false.
            */
            Receiver() noexcept{    
                                
            	PREPARATION_TIME  = TIME("00:00:10");
				state.acknowledgement_num    = 0;
				state.sending    = false;
            }   
            
            /** 
            * structure state definition and declaring acknowledgment number
            */ 
            struct state_type {
				int acknowledgement_num;    /** <alternate bits are send by acknowledgement number */ 
                                            /** < Acknowledge Number */

				bool sending;               /** < sending state of the receiver */
                                            /** < sending state */
            }; 
		    state_type state;

            /** 
            * port definitions 
            */
            using input_ports = std::tuple<typename defs::in>;
            using output_ports = std::tuple<typename defs::out>;

            /** 
            * Internal_transition function returns void  and intializing sending variable 
            */
            void internal_transition() {
				state.sending = false; 
            }

            /** 
            * @brief Funciton external_transition gets the number of messages is greater than 1.
            * assert to false and concatenate with one message per time unit 
            * initialize the state acknowledgement value to x.value and sending to true
            * @param e  type time 
    		* @param mbs type message bags
            */
            void external_transition(
				TIME e, typename make_message_bags<input_ports>::type mbs) { 
				if(get_messages<typename defs::in>(mbs).size()>1) {
					assert(false && "one message per time uniti");
				}
				for(const auto &x : get_messages<typename defs::in>(mbs)){
					state.acknowledgement_num = static_cast<int>(x.value);
					state.sending = true;
				}  
                           
            }

            /** 
            * confluence_transition function contains two functions internal_transistion 
            * and external_transition
            * @param e  type time 
    		* @param mbs type message bags 
            */ 
            void confluence_transition(TIME e,
									   typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
			}

            /** 
            * output function sends remainder value of acknowledgement number and the value is send to output port
            * @return message bags
            */
            typename make_message_bags<output_ports>::type output() const {
				typename make_message_bags<output_ports>::type bags;
				message_t out;              
				out.value = state.acknowledgement_num % 10;
				get_messages<typename defs::out>(bags).push_back(out);
            	return bags;
			}

            /** time_advance function declares the variable next_internal.
            * if state.sending is true then variable set to preparation time 
            * or else to infinity
            * @return next_interval
            */
            TIME time_advance() const {  
				TIME next_internal;
				if (state.sending) {
                next_internal = PREPARATION_TIME;
                }else {
                next_internal = std::numeric_limits<TIME>::infinity();
					}    
            	return next_internal;
            }
            
            /** 
            * friend is a function returns output acknowledge number to ostring stream 
            * @param os output stream
            * @param i 
            * @return os 
            */
            friend std::ostringstream& operator<<(std::ostringstream& os,
												  const typename Receiver<TIME>::state_type& i) {		
                os << "acknowledgement_num: " << i.acknowledgement_num; 
				return os;
            }
    };     
  
	
#endif /** _BOOST_SIMULATION_PDEVS_RECEIVER_HPP_ *\