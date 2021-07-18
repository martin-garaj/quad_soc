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
class wb_slave_agent_config extends uvm_object;

localparam string s_my_config_id = "wb_slave_agent_config";
localparam string s_no_config_id = "no config";
localparam string s_my_config_type_error_id = "config type error";

// UVM Factory Registration Macro
//
`uvm_object_utils(wb_slave_agent_config)

// Virtual Interface
virtual wb_if#(32, 64, 2) WB;

//------------------------------------------
// Data Members
//------------------------------------------
// Is the agent active or passive
uvm_active_passive_enum active = UVM_ACTIVE;

logic[31:0] start_address[15:0];
logic[31:0] range[15:0];

int wb_index = 0;
//------------------------------------------
// Methods
//------------------------------------------
extern static function wb_slave_agent_config get_config( uvm_component c );
// Standard UVM Methods:
extern function new(string name = "wb_slave_agent_config");

endclass: wb_slave_agent_config

function wb_slave_agent_config::new(string name = "wb_slave_agent_config");
  super.new(name);
endfunction

//
// Function: get_config
//
// This method gets the my_config associated with component c. We check for
// the two kinds of error which may occur with this kind of
// operation.
//
function wb_slave_agent_config wb_slave_agent_config::get_config( uvm_component c );
  uvm_object o;
  wb_slave_agent_config t;

  if( !c.get_config_object( s_my_config_id , o , 0 ) ) begin
    c.uvm_report_error( s_no_config_id ,
                        $sformatf("no config associated with %s" ,
                                  s_my_config_id ) ,
                        UVM_NONE , `uvm_file , `uvm_line  );
    return null;
  end

  if( !$cast( t , o ) ) begin
    c.uvm_report_error( s_my_config_type_error_id ,
                        $sformatf("config %s associated with config %s is not of type my_config" ,
                                   o.sprint() , s_my_config_id ) ,
                        UVM_NONE , `uvm_file , `uvm_line );
  end

  return t;
endfunction
