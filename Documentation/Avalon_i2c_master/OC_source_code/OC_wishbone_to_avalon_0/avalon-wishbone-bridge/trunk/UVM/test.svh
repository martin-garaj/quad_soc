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
class test extends uvm_test;

// UVM Factory Registration Macro
//
`uvm_component_utils(test)

//------------------------------------------
// Data Members
//------------------------------------------

//------------------------------------------
// Component Members
//------------------------------------------
// The environment class
env m_env;
// Configuration objects
env_config m_env_cfg;

//------------------------------------------
// Methods
//------------------------------------------
extern function void configure_wb_agent(wb_slave_agent_config cfg);
// Standard UVM Methods:
extern function new(string name = "test", uvm_component parent = null);
extern function void build_phase(uvm_phase phase);
extern task  main_phase(uvm_phase phase);

endclass: test

function test::new(string name = "test", uvm_component parent = null);
  super.new(name, parent);
endfunction

// Build the env, create the env configuration
// including any sub configurations and assigning virtural interfaces
function void test::build_phase(uvm_phase phase);
  // env configuration
  m_env_cfg = env_config::type_id::create("m_env_cfg");

  // WB configuration
  configure_wb_agent(m_env_cfg.slave_agent_cfg);

  if (!uvm_config_db #(virtual wb_if#(32,64,2))::get(this, "", "WB_vif", m_env_cfg.slave_agent_cfg.WB))
    `uvm_error("build_phase", "uvm_config_db #(virtual wb_if)::get(...) failed");

  m_env = env::type_id::create("m_env", this);

  uvm_config_db #(uvm_object)::set(this, "m_env*", "env_config", m_env_cfg);
  uvm_config_db #(uvm_object)::set(this, "m_env*", "wb_slave_agent_config", m_env_cfg.slave_agent_cfg);
endfunction: build_phase


//
// Convenience function to configure the wb agent
//
// This can be overloaded by extensions to this base class
function void test::configure_wb_agent(wb_slave_agent_config cfg);
  cfg.active = UVM_ACTIVE;
  cfg.start_address[0] = 32'h0;
  cfg.range[0] = 32'h18;
endfunction: configure_wb_agent

task test::main_phase(uvm_phase phase);
  wb_slave_sequence#(6) slave_seq = wb_slave_sequence#(6)::type_id::create("wb_slave_sequence");
  av_sequence#(6) seq1=new();

  phase.raise_objection(this);
  fork
    seq1.start(m_env.av_agent_h.av_sequencer_h);
    slave_seq.start(m_env.slave_agent.m_sequencer);
    #10000;
  //join_any
  join
  phase.drop_objection(this);
endtask
