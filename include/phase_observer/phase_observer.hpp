/** @file phase_observer.hpp
 *  @brief Function prototypes for the sender phase time recorder
 *
 *  This contains the prototypes of phase observer function used
 *  to calculate sender timing in each phase.
 */
#define __SENDER_PHASE_OBSERVER_HPP__
#ifdef __SENDER_PHASE_OBSERVER_HPP__

/** @brief get sender timing in each phase
 *
 *  This function is invoked to get the timing of sender in each phase
 *  @param ip_file_name  The input data
 *  @param op_file_name  the output file
 *  @return void
 *
 */
void phase_observer(const char *ip_file_name, const char *op_file_name);

#endif // __SENDER_PHASE_OBSERVER_HPP__