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
`ifndef wb_slave_sequencer
`define wb_slave_sequencer

//
// Class Description:
//
//
class wb_slave_sequencer extends uvm_sequencer #(wb_slave_seq_item#(32,64,2), wb_slave_seq_item#(32,64,2));
//I don't need two seq items req and rsp, only rsp is enough
//class wb_slave_sequencer extends uvm_sequencer #(wb_slave_seq_item#(32,64,2));

// UVM Factory Registration Macro
//
`uvm_component_utils(wb_slave_sequencer)

// Standard UVM Methods:
extern function new(string name="wb_slave_sequencer", uvm_component parent = null);

endclass: wb_slave_sequencer

function wb_slave_sequencer::new(string name="wb_slave_sequencer", uvm_component parent = null);
  super.new(name, parent);
endfunction

`endif // wb_slave_sequencer
