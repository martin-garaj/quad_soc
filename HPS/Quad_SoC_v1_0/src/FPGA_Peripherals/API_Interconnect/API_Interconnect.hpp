/*
 * FPGA_access.hpp
 *
 *  Created on: 2.3.2017
 *      Author: gartin
 */

#ifndef API_INTERCONNECT_HPP_
#define API_INTERCONNECT_HPP_



#define soc_cv_av
// header files from Altera HPS tutorial //
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>		// text input and output
#include <soc_cv_av/socal/socal.h>
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include <hwlib.h>


//macros to define hardware directions
#define MMAP_BASE 0xC0000000 				//default start for HPS-FPGA High performance bridge
#define MMAP_SPAN 0x20000000		 		// 0x20000000 -> 512MB = half of h2f memory address space
#define MMAP_MASK ( MMAP_SPAN - 1 )			// MMAP_MASK = 0x3BFFFFFF




class API_Interconnect
{
// VARIABLES //
public:

private:

	static void *virtual_base;
	static int instance_counter;
	int fd;

// FUNCTIONS //
public:
	//constructor and destructor
	API_Interconnect();
	~API_Interconnect();


	int mmap_FPGA_memory();
	uint32_t * convert_address(uint32_t FPGA_address);
	int munmap_FPGA_memory();

};

void * API_Interconnect::virtual_base = NULL;
int API_Interconnect::instance_counter = 0;


API_Interconnect::API_Interconnect(){
	// increment instance coutner
	instance_counter = instance_counter + 1;

	if(instance_counter == 1){
		// open /dev/mem
		if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
			#ifdef DEBUG_0
			std::cout << CP_misc << CP_0 << CP_CRIT_ERROR << CP_FPGA_ACCESS << "ERROR: could not open \"/dev/mem\"...\n" << std::endl;
			#endif
		}else{
			// THROW EXCEPTION
		}
		// find beginning of virtual address space
		virtual_base = mmap( NULL, MMAP_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, MMAP_BASE );
		// check the value
		if( virtual_base == MAP_FAILED ) {
			#ifdef DEBUG_0
			std::cout << CP_misc << CP_0 << CP_CRIT_ERROR << CP_FPGA_ACCESS << "ERROR: mmap() failed...\n" << std::endl;
			#endif
			close( fd );
		}else{
			// THROW EXCEPTION
		}
	}else if(instance_counter > 1){
		// THROW EXCEPTION
	}

};


API_Interconnect::~API_Interconnect(){

	// increment instance coutner
	instance_counter = instance_counter - 1;

	if(instance_counter == 0){
		//Clean mmap
		if( munmap( virtual_base, MMAP_SPAN ) != 0 ) {
			#ifdef DEBUG_0
			std::cout << CP_misc << CP_0 << CP_CRIT_ERROR << CP_FPGA_ACCESS << "ERROR: munmap() failed..." << std::endl;
			#endif
			close( fd );
		}else{
			// THROW EXCEPTION
		}
		#ifdef DEBUG_2
		std::cout << CP_misc << CP_0 << CP_OUT << CP_FPGA_ACCESS << "munmap()" << std::endl;
		#endif
		close( fd );
		#ifdef DEBUG_2
		std::cout << CP_misc << CP_0 << CP_OUT << CP_FPGA_ACCESS << "close fd" << std::endl;
		#endif
	}

};


uint32_t * API_Interconnect::convert_address(uint32_t FPGA_address){
	// returns FPGA_interface as pointer to program memory
	#ifdef DEBUG_2
	std::cout << CP_misc << CP_0 << CP_OUT << CP_FPGA_ACCESS << "interface " << FPGA_address << " registered" << std::endl;
	#endif
	return (uint32_t *)(virtual_base + ( ( unsigned long  )( FPGA_address ) & ( unsigned long)( MMAP_MASK ) ));

};





#endif /* FPGA_ACCESS_HPP_ */
