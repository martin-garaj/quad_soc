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
package wb_slave_agent_pkg;

import uvm_pkg::*;
`include "uvm_macros.svh"

//import register_layering_pkg::*;

`include "wb_slave_seq_item.svh"
`include "wb_slave_agent_config.svh"
`include "wb_slave_driver.svh"
`include "wb_slave_monitor.svh"
`include "wb_slave_sequencer.svh"
`include "wb_listener.svh"
`include "wb_slave_agent.svh"

// Utility Sequences
`include "wb_slave_sequence.svh"

endpackage: wb_slave_agent_pkg
