import uvm_pkg::*;



class av_mm_transaction #(AW =32, DW=128, TW= 2) extends uvm_sequence_item;
`uvm_object_utils(av_mm_transaction);
rand t_direction dir;
rand logic [AW-1:0] addr;
rand logic [DW-1:0] data;
rand logic [TW-1:0] tags;
rand delay_t stall_time;
constraint DIR {dir dist {AVALON_RD:=50,AVALON_WAIT:=50};}
constraint ADDR_RANGE {addr inside {[32'h40000000:32'h80000000]};}
constraint DATA_RANGE {data inside {[0:32'hFFFFFFFF]};}
constraint TAG_RANGE {tags inside {[0:3]};}
constraint RANDOM_STALL {
		stall_time dist{
			0 :=50,
			[1:100]:=50
		};
}

covergroup cov;
	ADDR: coverpoint addr;
	TYPE: coverpoint dir;
	TAG:  coverpoint tags;
endgroup


function new(string name="av_mm_transaction");
	super.new(name);
	cov=new();
endfunction

//method to randomize real 
//function void post_randomize;
//	stall_time=$bitstoreal(stall_time_i);
//endfunction

function void do_record(uvm_recorder recorder);
	super.do_record(recorder);
	`uvm_record_attribute(recorder.tr_handle,"address",addr);
	`uvm_record_field("data",data);
	//if ((dir ==AVALON_RD) && (recorder.tr_handle !=0))
	if ((dir ==AVALON_RD))
		$add_color(recorder.tr_handle,"red");
	else
		$add_color(recorder.tr_handle,"green");
endfunction 
endclass
