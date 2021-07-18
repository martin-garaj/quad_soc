`ifndef AVALON_IF__SV
`define AVALON_IF__SV


interface avalon_if #(AW=32,DW=64,TW=2)(input bit clk,rst_n);
  logic  [AW-1:0]   	address;
  logic  [DW/8-1:0] 	byteenable;
  logic           	chipselect;
  logic           	read;
  logic           	write;
  logic  [DW-1:0] 	readdata;
  logic  [DW-1:0] 	writedata;
  logic           	waitrequest;
  logic           	readdatavalid;
  logic  [3:0]   	burstcount;
  logic           	beginbursttransfer;
  // Clocking block
  clocking m_cb @(posedge clk);
    default input #1step output #1;

    output address;
    output byteenable;
    output chipselect;
    output read;
    output write;
    output writedata;
    output burstcount;
    output beginbursttransfer;
    input  readdata;
    input  waitrequest;
    input  readdatavalid;
  endclocking
  // Slave Clocking block
  clocking s_cb @(posedge clk);
    default input #1step output #1;

    input address;
    input byteenable;
    input chipselect;
    input read;
    input write;
    input writedata;
    input burstcount;
    input beginbursttransfer;
    output  readdata;
    output  waitrequest;
    output  readdatavalid;
  endclocking
modport master (clocking m_cb);
modport slave (clocking s_cb);
//task read_transaction(logic [AW-1:0] addr);
//	wait(waitrequest ==1'b0);
//	address=addr;
//	byteenable=8'hFF;
//	chipselect=1'b1;
//	read=1'b1;
//	write=1'b0;
//	//readdata;
//	writedata='0;
//	//waitrequest;
//	//readdatavalid;
//	burstcount='0;
//	beginbursttransfer=1'b0;
//endtask
//task write_transaction(logic [AW-1:0] addr, logic [DW-1:0] data);
//	wait(waitrequest ==1'b0);
//	address=addr;
//	byteenable=8'hFF;
//	chipselect=1'b1;
//	read=1'b0;
//	write=1'b1;
//	//readdata;
//	writedata=data;
//	//waitrequest;
//	//readdatavalid;
//	burstcount='0;
//	beginbursttransfer=1'b0;
//endtask 
endinterface

`endif
