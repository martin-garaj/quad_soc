
//`include "uvm_pkg.sv"
`include "uvm_macros.svh"
import uvm_pkg::*;

`ifndef DUT_TEST_PKG_SV
`define DUT_TEST_PKG_SV

package av_test_pkg ;
  	typedef enum {
  	  AVALON_WAIT = 0,
  	  AVALON_RD  = 1,
  	  AVALON_WR = 2
  	} t_direction;
	typedef int unsigned delay_t;
                    
	`include "./av_mm_transaction.sv"
	`include "./av_sequence.sv"
	`include "./av_driver.sv"
	`include "./av_config.sv"
	`include "./av_agent.sv"

endpackage
`endif
