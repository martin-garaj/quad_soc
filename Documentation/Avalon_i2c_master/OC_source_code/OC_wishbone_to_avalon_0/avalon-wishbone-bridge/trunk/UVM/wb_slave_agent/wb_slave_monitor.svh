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

//
// Class Description:
//
//
class wb_slave_monitor extends uvm_component;

// UVM Factory Registration Macro
//
`uvm_component_utils(wb_slave_monitor);

// Virtual Interface
virtual wb_if #(32,64,2)  WB;

//------------------------------------------
// Data Members
//------------------------------------------
wb_slave_agent_config m_cfg;

//------------------------------------------
// Component Members
//------------------------------------------
uvm_analysis_port #(wb_slave_seq_item) ap;

//------------------------------------------
// Methods
//------------------------------------------

// Standard UVM Methods:

extern function new(string name = "wb_slave_monitor", uvm_component parent = null);
extern function void build_phase(uvm_phase phase);
extern task run_phase(uvm_phase phase);
extern function void report_phase(uvm_phase phase);

endclass: wb_slave_monitor

function wb_slave_monitor::new(string name = "wb_slave_monitor", uvm_component parent = null);
  super.new(name, parent);
endfunction

function void wb_slave_monitor::build_phase(uvm_phase phase);
  ap = new("ap", this);
  m_cfg = wb_slave_agent_config::get_config(this);
  WB = m_cfg.WB;
endfunction: build_phase

task wb_slave_monitor::run_phase(uvm_phase phase);
  wb_slave_seq_item #(32,64,2)  item;
  wb_slave_seq_item #(32,64,2) cloned_item;

  item = wb_slave_seq_item#(32,64,2)::type_id::create("item");

//phase.raise_objection(this);
  forever begin
    // Detect the protocol event on the TBAI virtual interface
    @(posedge WB.clk);
    if(WB.cyc && WB.stb)
    // Assign the relevant values to the analysis item fields
      begin
        item.addr = WB.adr;
        item.rw = WB.we;
        if(WB.we)
          begin
            item.wdata = WB.dat_o;
          end
        else
          begin
            item.rdata = WB.dat_i;
          end
        // Clone and publish the cloned item to the subscribers
        $cast(cloned_item, item.clone());
        ap.write(cloned_item);
      end
  end
//phase.drop_objection(this);
endtask: run_phase

function void wb_slave_monitor::report_phase(uvm_phase phase);
// Might be a good place to do some reporting on no of analysis transactions sent etc

endfunction: report_phase
