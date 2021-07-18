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

// Questa recording macro:

//`define uvm_record_field(NAME,VALUE) \
//   $add_attribute(recorder.tr_handle,VALUE,NAME);

//
// Class Description:
//
//
class wb_slave_seq_item #(AW =32, DW=64, TW= 2) extends uvm_sequence_item;

// UVM Factory Registration Macro
//
`uvm_object_utils(wb_slave_seq_item)

//------------------------------------------
// Data Members (Outputs rand, inputs non-rand)
//------------------------------------------
logic[AW-1:0] addr;
logic[DW-1:0] wdata;
logic[TW-1:0] tag;
logic rw;

rand logic[DW-1:0] rdata;
rand logic slv_err;
rand int delay;

constraint delay_bounds {
  delay inside {[0:2]};
}

constraint error_dist {
  slv_err dist {0 := 80, 1 := 20};
}

//------------------------------------------
// Methods
//------------------------------------------

// Standard UVM Methods:
extern function new(string name = "wb_slave_seq_item");
extern function void do_copy(uvm_object rhs);
extern function bit do_compare(uvm_object rhs, uvm_comparer comparer);
extern function string convert2string();
extern function void do_print(uvm_printer printer);
extern function void do_record(uvm_recorder recorder);

endclass:wb_slave_seq_item

function wb_slave_seq_item::new(string name = "wb_slave_seq_item");
  super.new(name);
endfunction

function void wb_slave_seq_item::do_copy(uvm_object rhs);
  wb_slave_seq_item rhs_;

  if(!$cast(rhs_, rhs)) begin
    `uvm_fatal("do_copy", "cast of rhs object failed")
  end
  super.do_copy(rhs);
  // Copy over data members:
  addr = rhs_.addr;
  wdata = rhs_.wdata;
  rw = rhs_.rw;
  slv_err = rhs_.slv_err;
  rdata = rhs_.rdata;
  delay = rhs_.delay;

endfunction:do_copy

function bit wb_slave_seq_item::do_compare(uvm_object rhs, uvm_comparer comparer);
  wb_slave_seq_item rhs_;

  if(!$cast(rhs_, rhs)) begin
    `uvm_error("do_copy", "cast of rhs object failed")
    return 0;
  end
  return super.do_compare(rhs, comparer) &&
         addr == rhs_.addr &&
         wdata == rhs_.wdata &&
         rw   == rhs_.rw &&
         slv_err == rhs_.slv_err &&
         rdata == rhs_.rdata;
  // Delay is not relevant to the comparison
endfunction:do_compare

function string wb_slave_seq_item::convert2string();
  string s;

  $sformat(s, "%s\n", super.convert2string());
  // Convert to string function reusing s:
  $sformat(s, "%s\n addr\t%0h\n wdata\t%0h\n rw\t%0b\n", s, addr, wdata, rw);
  $sformat(s, "%s\n slv_err\t%0b\n rdata\t%0h\n delay\t%0d\n", s, slv_err, rdata, delay);
  return s;

endfunction:convert2string

function void wb_slave_seq_item::do_print(uvm_printer printer);
  if(printer.knobs.sprint == 0) begin
    $display(convert2string());
  end
  else begin
    printer.m_string = convert2string();
  end
endfunction:do_print

function void wb_slave_seq_item:: do_record(uvm_recorder recorder);
  super.do_record(recorder);

  // Use the record macros to record the item fields:
  `uvm_record_field("addr", addr)
  `uvm_record_field("wdata", wdata)
  `uvm_record_field("rdata", rdata)
  `uvm_record_field("rw", rw)
  `uvm_record_field("slv_err", slv_err)
  `uvm_record_field("delay", delay)
endfunction:do_record

