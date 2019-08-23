# **ALTERNATE BIT PROTOCOL SIMULATOR**
---
## ORGANIZATION: Carleton University
---
### *AUTHORS:*  
---
### Originally Developed by	: [Dr. Cristina Ruiz Martin](https://github.com/cruizm)
### Modified by				: [Rajeev Kuamr Jandhyala](https://github.com/jrajeev459) and [Abhinav Achanta](https://github.com/abhinavachanta)

### Table of Contents 
------
- [Project Description](#Project_Description)
- [File Structure](#File_Structure)
- [Installation Guide](#Installation_Guide)
- [Steps to Run Simulator](#Steps_to_Run_Simulator)

---

## Project Description
---
Alternate Bit Protocol(ABP) is for transmission of data packets from sender to reciver through subnet. In order to achieve reliable
transmission between sender and receiver through unreliable network the ABP protocol is used. when sender sends the packet,
it waits for particular to recieve acknowledgement from reviever. if it is not arrived in that time it re-sends packet until it gets
the expected acknowledgement before sending next packet. In order to distinguish from one packet to another this ABP helps to add
an additional bit on each packet.

There are three components of ABP simulator are 

**Sender** - Its behavior is pretty complex. When an external signal is received the sender changes from initial phase *passive*
to *active*. Once activated it start sending the packet with the alternating bit. Every time a packet is sent,it waits for an
acknowledgement during an specific waiting time. It the acknowledgment does not arrive within the time window, the sender will
re- send the previous packet with the alternating bit. If the expected acknowledgement is received within the time window, the 
sender will send the next packet. When there are no more packets to send, the sender will go again to the *passive* phase.

**subnet** - The subnets just pass the packets after a time delay. However, in order to simulate the unreliability of the network,
only 95% of the packets will be passed in each of the subnet, i.e. 5% of the data will be lost through the subnet

**Receiver** - The behavior of receiver is to receive the data and send back an acknowledgement extracted from the received
data after a time period.

---

## File Structure
---

##### bin [This folder containes the .exe files which are generated during program execution]
1. src [This folder contains the simulator .exe file]
	- ABP.exe
2. test [This folder contains the test .exe files]
	- SUBNET_TEST.exe
	- SENDER_TEST.exe
	- RECEIVER_TEST.exe
		
##### build [This folder contains .o files which are generated during program execution]
1. src [This folder contains simulator .o files]
	- main.o
	- message.o
2. test [This folder contains the test .o files]
	1. subnet
		- main.o
	2. sender
		- main.o
	3. receiver
		- main.o
			
##### data [This folder contains input data for testing,output and conversion]
1. input_abp_0.txt
2. input_abp_1.txt
3. abp_output.txt
4. converted_output.txt
	
##### doc [This folder contains documents related to project]
1. alternatebitprot.pdf
2. Cadmium_Documentation_Ubuntu.pdf
3. Cadmium_Windows.pdf
		
##### include [THis folder contains the header files and data structures]
1. atomics [This folder contains the header files]
	- receiverCadmium.hpp
	- senderCadmium.hpp
	- subnetCadmium.hpp
2. data_structures [This folder contains the data structures used in the project]
	- message.hpp
	- message.cpp
3. output_modification [This folder contains the header for output modification]
    - convert_output.hpp

##### lib [This folder contains the libraries/dependencies]
1. cadmium [This folder contains cadmium files]
2. DESTimes [This folder contains DESTimes files]
3. vendor [This folder contains 3rd party header files needed in the project]
	- iestream.hpp
		
##### src [This folder contains source code for the project]
1. data_structures [This folder contains the data structures used in the project]
	- message.cpp
2. top_model [This folder contains source code for the Alternate Bit Protocol simulator]	
	- main.cpp

##### test [This folder contains the unit test for the different include files]
1. data
	- receiver_input_test.txt
	- receiver_test_output.txt
	- sender_input_test_ack_In.txt
	- sender_input_test_control_In.txt
	- sender_test_output.txt
	- subnet_input_test.txt
	- subnet_test_output.txt

	1. modified [this folder contains the modified test outputs]
	  - converted_receiver_output.txt
		- converted_sender_output.txt
		- converted_subnet_output.txt

2. src
	1. receiver [This folder contains the unit test of the receiver]
		- main.cpp
	2. sender [This folder contains the unit test of the sender]
		- main.cpp
	3. subnet [This folder contains the unit test of the subnet]
		- main.cpp

--- 
## Installation Guide 
---

Following the corresponding link on instructions to install cadmium simulator dependencies on windows operating system:(https://github.com/abhinavachanta/AlternateBitProtocolSimulator/blob/master/doc/Cadmium_Windows.pdf)
Then run the following command from terminal to clone the project along with the submodules. 

	> Example: INCLUDECADMIUM=-I lib/cadmium/include

--- 
## Instructions to Run/compile the Project
---

1. Inside the doc folder you can find alternatebitprotocol.pdf which contains the explanation of this simulator

2. Update include paths in the makefile. You need to update the following lines:
	1. INCLUDECADMIUM=-I lib/cadmium/include
    2. Update the relative path to cadmium/include from the folder where the makefile is. You need to take relative path into account where you have copied the folder during the installation process

		> Example: git clone --recurse-submodules -b submodule_simulator_modification https://github.com/abhinavachanta/AlternateBitProtocolSimulator.git AlternateBitProtocol
    
3. **Run the tests**

	1. Run subnet test

		- Open the terminal. Press in your keyboard Ctrl+Alt+t

		- Set the command prompt to base folder where makefile is. Set the terminal path to this folder.
    
			> Example: cd ../AlternateBitProtocol/
		
		- To compile the test, type in the terminal:

		    > make clean_tests; make tests

		- To run the test, goto the test/src/subnet directory and type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this specific test you need to type:

			> ./SUBNET

		- To check the output of the test simulation, goto /test/data folder and you can see following files:
			"subnet_test_output.txt" - Initial subnet simulator output
			"modified/converted_subnet_output.txt" - Modified output for Part D of the project.
			
	2. To run receiver and sender tests, the steps are analogous to 1
			
4. **Run the simulator**
	1. Open the terminal. Press in your keyboard Ctrl+Alt+t

	2. Set the command prompt in the AlternateBitProtocol folder. Set the terminal path to this folder.

		> Example: cd ../AlternateBitProtocol/

	3. To compile the project, type in the terminal:

		> make clean_simulator; make simulator

	4. To run the simulation, goto the project root and type in the terminal 

		> ./NAME_OF_THE_COMPLIED_FILE path and NAME_OF_THE_INPUT_FILE. For this test you need to type: 

		> ./bin/src/ABP ./data/input_abp_1.txt

	5. To check the output of the simulation, goto /data folder and you can see following files:
		"abp_output.txt" - Initial simulator output
	    "converted_output.txt" - Modified output for Part D of the project.

	6.  To execute the simulator with different inputs
		1. Create new .txt files with the same structure as input_abp_0.txt or input_abp_1.txt
		2. Run the simulator using the instructions in step 4
		3. In order to save the output, rename abp_output.txt. To do so, goto test/data/ folder and
		   type in the terminal: "mv abp_output.txt NEW_NAME"	

			> Example: mv abp_output.txt abp_output_0.txt
			
**NOTE** - To run both the simulator and tests together, the following commands can be used from the project root. The output files will be according to the paths described above.
			
			> make clean_all; make all

		 		