/** @file convert_output.hpp
 *  @brief Function prototypes for the file format
 *
 *  This contains the prototypes of converter output function used
 *  to convert output of a given input file
 */
#define __CONVERT_OUTPUT_HPP__
#ifdef __CONVERT_OUTPUT_HPP__

/** @brief get converted output.
 *
 *  This function is invoked to get the  name from the output file
 *  @param inputfilename filename The input data is obtained when it is called.
 *  @param openfilename  the extracted output
 *  @return void
 *
 */
void convert_output(const char *ip_file_name, const char *op_file_name);

#endif // __CONVERT_OUTPUT_HPP__