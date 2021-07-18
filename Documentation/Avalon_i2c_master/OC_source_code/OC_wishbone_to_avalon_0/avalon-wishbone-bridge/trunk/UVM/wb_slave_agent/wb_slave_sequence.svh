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
`ifndef wb_slave_sequence
`define wb_slave_sequence


//
// Class Description:
//
//
class wb_slave_sequence #(N=100) extends uvm_sequence #(wb_slave_seq_item);

// UVM Factory Registration Macro
//
`uvm_object_utils(wb_slave_sequence#(N))

//------------------------------------------
// Data Members (Outputs rand, inputs non-rand)
//------------------------------------------
bit [31:0] memory [int];

//------------------------------------------
// Constraints
//------------------------------------------



//------------------------------------------
// Methods
//------------------------------------------

// Standard UVM Methods:
extern function new(string name = "wb_slave_sequence");
extern task body;

endclass:wb_slave_sequence

function wb_slave_sequence::new(string name = "wb_slave_sequence");
  super.new(name);
endfunction

task wb_slave_sequence::body;
  wb_slave_agent_config m_cfg = wb_slave_agent_config::get_config(m_sequencer);
  wb_slave_seq_item req;
  wb_slave_seq_item rsp;

  req = wb_slave_seq_item#(32,64,2) ::type_id::create("req");
  rsp = wb_slave_seq_item#(32,64,2) ::type_id::create("rsp");

  //wait (m_cfg.WB.rst_n);
  // Slave sequence finishes after 60 transfers:
  repeat(N) begin

    // Get request
    start_item(req);
    finish_item(req);
    $display ("req item emitted\n");	
    // Prepare memory for response:
    if (req.rw) begin
      memory[req.addr] = req.wdata;
    end
    else begin
      if(!memory.exists(req.addr)) begin
        memory[req.addr] = 32'hdeadbeef;
        memory[req.addr] = req.addr;
      end
    end

    // Respond:
    start_item(rsp);
    rsp.copy(req);
    assert (rsp.randomize() with {if(!rsp.rw) rsp.rdata == memory[rsp.addr];});
    finish_item(rsp);
    $display ("rsp item emitted\n");	
  end

endtask:body

`endif // wb_slave_sequence
