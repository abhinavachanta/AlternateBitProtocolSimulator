/** \brief Output Format Conversion source file
 *
 *  Authors      : Abhinav Achanta, Rajeev Jandhyala
 *  
 *  This function is used to transform the ABP simulator generated output 
 *  to a tabular and readable format. It takes the file as input and 
 *  can transform and store in a new output file provided the name.
 *   
 *  
 *  This function can be used for the formatted output of components
 *  ABP Simulator, Sender,Receiver, Subnet
 */

#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

/** 
 * Function to get the data from the input file
 * and write the data in new format to the output file
 * @param ip_file_name input file name string
 * @param op_file_name output file name string
 */
void convert_output(char * ip_file_name, char * op_file_name) {

    /** 
     * declaring string variables 
     * assigning the delimeters to parse
     */
    string srg, val1, val, time, port1, port, comp_name,
    port_delimiter = "::", comp_delimiter = "model";

    /** 
     * declaring integer variables 
     */
    int vala, valb, valc, start, portc, portd, porta, portb,
    vale, valf, porte, portf, comp;

    /**
     * file stream class used for file handling    
     *initialize input stream
     */
    ifstream filestream(ip_file_name);

    /**
     * ofstream cleans and writes a new output into existing file   
     * or creates one if not present.
     *initialize output stream
     */
    ofstream myfile;
    myfile.open(op_file_name);

    /**
     * checks if the file stream is   
     * valid and open
     */
    if (filestream.is_open()) {

        /**
         * setting the column names for the output file.
         * setw() function is used to give indentation in the output file.
         */
        myfile << setw(6) << "Time" << setw(20) << "Value" << setw(20) <<
            "Port" << setw(25) << "Component" << endl;

        /**
         *For each line of simulator output, 
         *transforming in required format
         */
        while (getline(filestream, srg)) {
            /**
             * Check if line starts with digit, 
             * it means it is time component
             */
            if (isdigit(srg[0]) == true) {
                time.assign(srg);
            }
            start = 0;

            /**
             * Fetch the value inside {} 
             */
            vala = srg.find('{');

            while (vala > 0) {
                valb = srg.find('}', start);

                /**
                 * If value between {} is greater than 1,
                 * then parse the port and component
                 */
                if (valb - vala > 1) {
                    porta = srg.find(port_delimiter, start);
                    portb = srg.find(":", porta + 2);
                    comp = srg.find(comp_delimiter, start);
                    val = srg.substr(vala + 1, valb - vala - 1);
                    port = srg.substr(porta + 2, portb - porta - 2);
                    comp_name = srg.substr(comp + 6);

                    /**
                     *Save the processed result in output file
                     */
                    myfile << setw(6) << time << setw(12) << val << setw(24) <<
                        port << setw(24) << comp_name << endl;

                }
                start = valb + 1;
                vala = srg.find('{', start);
            }
        }
        /** 
         *closing the input stream if still open
         */
        filestream.close();
    } else {
        cout << "File opening  failed." << endl;
    }
}