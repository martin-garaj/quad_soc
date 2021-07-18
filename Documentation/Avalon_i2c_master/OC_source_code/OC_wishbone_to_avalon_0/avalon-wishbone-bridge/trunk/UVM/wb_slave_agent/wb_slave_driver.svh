//------------------------------------------------------------
//   Copyright 2010 Mentor Graphics Corporation
//   All Rights Reserved Worldwide
//
//   Licensed under the Apache License, Version 2.0 (the
//   "License"); you may not use this file except in
//   compliance with the License.  You may obtain a copy of
//   the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in
//   writing, software distributed under the License is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//   CONDITIONS OF ANY KIND, either express or implied.  See
//   the License for the specific language governing
//   permissions and limitations under the License.
//------------------------------------------------------------
`ifndef wb_slave_driver
`define wb_slave_driver

//
// Class Description:
//
//
class wb_slave_driver #(AW =32, DW=64, TW= 2) extends uvm_driver #(wb_slave_seq_item #(AW, DW, TW),wb_slave_seq_item#(AW,DW,TW));

// UVM Factory Registration Macro
//
`uvm_component_utils(wb_slave_driver)

// Virtual Interface
virtual wb_if #(AW, DW, TW) WB;

//------------------------------------------
// Data Members
//------------------------------------------
wb_slave_agent_config m_cfg;
mailbox slave_mbox;
//------------------------------------------
// Methods
//------------------------------------------
// Standard UVM Methods:
extern function new(string name = "wb_slave_driver", uvm_component parent = null);
extern function void build_phase(uvm_phase phase);
extern task run_phase(uvm_phase phase);
extern task req_loop();
extern task resp_loop(uvm_phase phase);
//task handle_req(wb_slave_seq_item #(AW, DW, TW) req);
//endtask
//
//task handle_rsp(wb_slave_seq_item #(AW, DW, TW) rsp,wb_slave_seq_item #(AW, DW, TW) req);
//endtask

endclass: wb_slave_driver

function wb_slave_driver::new(string name = "wb_slave_driver", uvm_component parent = null);
  super.new(name, parent);
  slave_mbox=new(1);
endfunction

function void wb_slave_driver::build_phase(uvm_phase phase);
  m_cfg = wb_slave_agent_config::get_config(this);
  WB = m_cfg.WB;
endfunction: build_phase

task wb_slave_driver::run_phase(uvm_phase phase);
  //wb_slave_seq_item #(AW, DW, TW) req;
  //wb_slave_seq_item #(AW, DW, TW) rsp;

//phase.raise_objection(this);
	fork
		this.req_loop();
		this.resp_loop(phase);
	join


      `uvm_info("** WB SLAVE RUN PHASE EXITED **", "", UVM_LOW)
//phase.drop_objection(this);
endtask: run_phase

task wb_slave_driver::req_loop();
	wb_slave_seq_item #(this.AW, this.DW, this.TW) req=new();	

        forever begin

	if (!this.WB.rst_n) begin 
		this.WB.stall<=1'b0;
     		@(posedge this.WB.clk);
	end else begin
		//@(posedge this.WB.clk);
		$strobe("waiting\n");
		//this.seq_item_port.get_next_item(req);
		while (!(this.WB.cyc === 1'b1 && this.WB.stb ===1'b1)) @(posedge this.WB.clk);
		//@((this.WB.clk ===1'b1 && this.WB.cyc === 1'b1));
		$strobe("got request %d\n",this.WB.adr);
		if(slave_mbox.try_put(req)) begin
			this.WB.stall<=1'b0;
			req.addr<= this.WB.adr;
        		req.rw<= this.WB.we;
			if (req.rw) req.wdata<= this.WB.dat_o;
			//this.seq_item_port.item_done();
			slave_mbox.put(req);
		end else 
			this.WB.stall<=1'b1;
		@(posedge this.WB.clk);
	end

	end
endtask


task wb_slave_driver::resp_loop(uvm_phase phase);
	wb_slave_seq_item #(this.AW, this.DW, this.TW) req1;	
	wb_slave_seq_item #(this.AW, this.DW, this.TW) req;	
	wb_slave_seq_item #(this.AW, this.DW, this.TW) rsp;
        forever begin

	if (!this.WB.rst_n) begin
     		this.WB.ack<= 1'b0;
     		this.WB.err<= 1'b0;
     		this.WB.rty<= 1'b0;
		this.WB.dat_i<='0;
     		@(posedge this.WB.clk);
   	end
   	else begin
		this.WB.ack<=1'b0;
     		this.WB.err<= 1'b0;
     		this.WB.rty<= 1'b0;
		this.WB.dat_i<='0;
		if(slave_mbox.try_get(req)) //begin
		//@(slave_mbox.try_get(req));
		begin
			$strobe("Processing req\n");
			slave_mbox.get(req);
			this.seq_item_port.get_next_item(req1);
			req1.copy(req);
			this.seq_item_port.item_done();
			this.seq_item_port.get_next_item(rsp);
				$strobe("wait cycle %d\n",rsp.delay);
			//phase.raise_objection(this);
     			repeat (rsp.delay) begin
				@(posedge this.WB.clk);
				this.WB.ack<=1'b0;
     				this.WB.err<= 1'b0;
     				this.WB.rty<= 1'b0;
				this.WB.dat_i<='0;
			end
			//phase.raise_objection(this);
					
     			if ( ! req.rw) this.WB.dat_i<= rsp.rdata;
				$strobe("rsp %d\n",rsp.rdata);
			this.WB.ack<=1'b1;
			this.WB.err<= rsp.slv_err;
			this.seq_item_port.item_done();
		end
		@(posedge this.WB.clk);
	end
	
	end

endtask


`endif // wb_slave_driver
