//`include "uvm_pkg.sv"
//`include "uvm_macros.svh"
//`include "avalon_m_if.sv"
//`include "av_test_pkg.sv"
//`include "../Download/uvm_agents-master/src/wishbone_b3/wishbone_b3_if.sv"
//`include "../../av2wb/av2wb.sv"
import env_pkg::*;
module tb;
logic clock,rst_n;
logic [0:2] S;
logic x;
integer count=0;

wb_if #(32,64,2) wb(clock,rst_n);
avalon_if #(32,64,2) av(clock,rst_n);
//always @(posedge clock) begin 
//	if(count==3) count<=0; 
//	else if(wb.stb && wb.cyc) count<=count+1;
//	else count<=count;
//end
//assign wb.ack=count[1] && count[0];
//assign wb.ack=wb.stb && wb.cyc;
//assign dut_if_h.slave.read=1'b1;
//assign dut_if_h.slave.read=1'b1;
av2wb  i_dut(clock,rst_n,av,wb);

import uvm_pkg::*;
import questa_uvm_pkg::*;
import av_test_pkg::*;


initial begin rst_n<=1'b0;#10 rst_n<=1'b0;#20 rst_n<=1'b1; end
always begin 
	clock<=1'b0 ;
	forever
	#10 clock=~clock;
end


initial begin

//uvm_config_db#(virtual dut_if)::set(null,"*.dut_agent_h.*","dut_if",dut_if_h);
//				     cntxt,inst_name,field_name,value    
uvm_config_db#(int)            ::set(null, "", "recording_detail", 400);
uvm_config_db#(uvm_bitstream_t)::set(null, "", "recording_detail", 400);
//uvm_config_db#(virtual avalon_if #(32,64,2))::set(null,"uvm_test_top.av_env_h.av_agent_h","DUT_IF",av);
uvm_config_db#(virtual avalon_if #(32,64,2))::set(null,"uvm_test_top.m_env.av_agent_h","DUT_IF",av);
uvm_config_db #(virtual wb_if#(32,64,2))::set(null, "uvm_test_top", "WB_vif" , wb);
// cntxt null becasue tb is a module not a class.
run_test("test");
end
always@* begin
#10000 rst_n<=1'b0;
#10000 clock<=1'b0;
end

endmodule
