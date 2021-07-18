import uvm_pkg::*; 

class av_sequence #(N=100) extends uvm_sequence #(av_mm_transaction #(32,64,2));


//av_mm_transaction#(32,64,2) tx;

function new (string name="av_sequence");
	super.new(name);
endfunction


function void build_phase(uvm_phase phase);
endfunction

task body;
	
	av_mm_transaction#(32,64,2) tx=new();
	repeat(2*N) begin
		start_item(tx);
		assert (tx.randomize());
		finish_item(tx);
	end
	//end with a wait transaction
	start_item(tx);
	//assert (tx.randomize() with {tx.dir==AVALON_WAIT;});
	tx.dir=AVALON_WAIT;
	finish_item(tx);
endtask 



endclass
