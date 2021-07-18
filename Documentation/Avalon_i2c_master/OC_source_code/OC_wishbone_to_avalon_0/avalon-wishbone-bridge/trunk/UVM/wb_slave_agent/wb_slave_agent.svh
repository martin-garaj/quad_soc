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
`ifndef wb_slave_agent
`define wb_slave_agent

//
// Class Description:
//
//
class wb_slave_agent#(AW =32, DW=64, TW= 2) extends uvm_component;

// UVM Factory Registration Macro
//
`uvm_component_utils(wb_slave_agent)

//------------------------------------------
// Data Members
//------------------------------------------
wb_slave_agent_config m_cfg;
//------------------------------------------
// Component Members
//------------------------------------------
uvm_analysis_port #(wb_slave_seq_item) ap;
wb_slave_monitor   m_monitor;
wb_slave_sequencer m_sequencer;
wb_slave_driver #(AW,DW,TW)    m_driver;
item_listener listener;
//------------------------------------------
// Methods
//------------------------------------------

// Standard UVM Methods:
extern function new(string name = "wb_slave_agent", uvm_component parent = null);
extern function void build_phase(uvm_phase phase);
extern function void connect_phase(uvm_phase phase);

endclass: wb_slave_agent


function wb_slave_agent::new(string name = "wb_slave_agent", uvm_component parent = null);
  super.new(name, parent);
endfunction

function void wb_slave_agent::build_phase(uvm_phase phase);
  m_cfg = wb_slave_agent_config::get_config(this);
  // Monitor is always present
  m_monitor = wb_slave_monitor::type_id::create("m_monitor", this);
  // Only build the driver and sequencer if active
  if(m_cfg.active == UVM_ACTIVE) begin
    m_driver = wb_slave_driver#(32,64,2) ::type_id::create("m_driver", this);
    m_sequencer = wb_slave_sequencer::type_id::create("m_sequencer", this);
  end
  listener = item_listener::type_id::create("item_listener", this);
endfunction: build_phase

function void wb_slave_agent::connect_phase(uvm_phase phase);
  ap = m_monitor.ap;
  ap.connect(listener.analysis_export);
  // Only connect the driver and the sequencer if active
  if(m_cfg.active == UVM_ACTIVE) begin
    m_driver.seq_item_port.connect(m_sequencer.seq_item_export);
  end
  
endfunction: connect_phase

`endif // wb_slave_agent
