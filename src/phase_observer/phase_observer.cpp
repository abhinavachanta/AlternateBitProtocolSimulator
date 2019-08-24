/** \brief Records the active, passive and overall timing.
 *
 *  Authors      : Abhinav Achanta, Rajeev Jandhyala
 *  
 *  This function is used to record the active, passive and overall timing
 * of sender component in simulator. It takes the modified simulator output file 
 * as its input and determines these timings. It then stores the metrics in an
 * output file.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "../../include/phase_observer/phase_observer.hpp"

using namespace std;

/** 
 * Function to get the data from the input file
 * and write the timing metrics for each phase to the output file
 * @param ip_file_name input file name
 * @param op_file_name output file name
 */
void phase_observer(const char *ip_file_name, const char *op_file_name) {

  /** 
   * declaring string variables 
   * assigning the delimeters to parse
   */
  string srg, time1, time2, port_name = "packet_sent_out", port_name2 =
    "ack_received_out";

  /** 
   * declaring integer variables 
   */
  int start_secs1, end_secs2, h, m, s, no_of_packets = 0, no_of_receivers = 0,
    overall_time = 0, size_of_array = 0,
    pos1 = 0, pos2 = 0, time_diff, passive_time = 0, active_time = 0;
  int start_sec_arr[100], end_sec_arr[100];

  /**
   * ofstream cleans and writes a new output into existing file   
   * or creates one if not present.
   * initialize output stream
   */
  ofstream improvement_opf;
  improvement_opf.open(op_file_name);

  /**
   * file stream class used for file handling    
   * initialize input stream
   */

  ifstream filestream(ip_file_name);

  /**
   * checks if the file stream is   
   * valid and open
   */
  if (filestream.is_open()) {
    /**
     * checks for port name "packetsentout "and compares the it line by line 
     * and caluclates overall packets sentout 
     */
    while (getline(filestream, srg)) {
      if (srg.find(port_name) != string::npos) {
        no_of_packets++;
      }
      /**
       * again cheks for port_name "packetsentout", takes first 11 characters
       * of the string as time.
       */
      if (srg.find(port_name) != string::npos) {
        time1 = srg.substr(0, 11);
        if (sscanf(time1.c_str(), "%d:%d:%d", & h, & m, & s) >= 2) {
          start_secs1 = h * 3600 + m * 60 + s;
          start_sec_arr[pos1] = start_secs1;
        }
        pos1++;
      }
      /**
       * checks for acknowledgement received out string and 
       * calculates total number of receivers 
       */
      if (srg.find(port_name2) != string::npos) {
        no_of_receivers++;
        time2 = srg.substr(0, 11);
        if (sscanf(time2.c_str(), "%d:%d:%d", & h, & m, & s) >= 2) {
          /**
           * calculates the time and store it in array 
           */
          end_secs2 = h * 3600 + m * 60 + s;
          end_sec_arr[pos2] = end_secs2;
        }
        pos2++;
      }

    }

    /** 
     * caluclates the time difference between packets sent out and acknowledgemnet received.
     * calculates the total number of packets sentout and received .
     * time taken from one packet sentout to acknowledgement receivedout is called active_time.
     * difference between overall time  and active time gives passive time 
     */
    for (int j = 0; j < no_of_packets; j++) {
      cout << start_sec_arr[j] << endl;
    }
    for (int j = 0; j < no_of_receivers; j++) {
      cout << end_sec_arr[j] << endl;
    }
    if (no_of_packets > no_of_receivers) {
      size_of_array = no_of_receivers;
    } else {
      size_of_array = no_of_packets;
    }
    for (int j = 0; j < size_of_array; j++) {
      time_diff = end_sec_arr[j] - start_sec_arr[j];
      active_time = active_time + time_diff;
    }
    improvement_opf << "Sender active time = " << active_time << " ms " << endl;
    overall_time = end_sec_arr[no_of_receivers - 1] - start_sec_arr[0];
    improvement_opf << "Sender overall time = " << overall_time << " ms" << endl;
    passive_time = overall_time - active_time;
    improvement_opf << "Sender passive time = " << passive_time << " ms" << endl;
  }
  /** 
   *closing the stream if still open
   */
  improvement_opf.close();
}