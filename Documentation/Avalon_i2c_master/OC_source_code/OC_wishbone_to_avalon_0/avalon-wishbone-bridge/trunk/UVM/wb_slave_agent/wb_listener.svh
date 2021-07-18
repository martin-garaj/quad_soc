class item_listener extends uvm_subscriber #( wb_slave_seq_item#(32,64,2) );

  `uvm_component_utils( item_listener );

  int transfers;

  function new( string name , uvm_component parent );
    super.new( name , parent );
  endfunction

  function void write( input wb_slave_seq_item t );
    transfers++;
    $display("No. %d\n",transfers);
  endfunction

  function void report_phase(uvm_phase phase);
    if(transfers == 6) begin
      `uvm_info("** UVM TEST PASSED **", "Correct number of transfers occured before timeout", UVM_LOW)
    end
    else begin
      `uvm_error("** UVM TEST FAILED **", "Too few transfers occured before the timeout")
    end
  endfunction: report_phase

endclass
