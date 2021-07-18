import uvm_pkg::*;

typedef uvm_sequencer#(av_mm_transaction#(32,64,2)) av_sequencer;
class av_agent #(AW =32, DW=64, TW= 2) extends uvm_agent;

av_config av_config_h;
av_sequencer av_sequencer_h;
av_driver#(AW,DW,TW) av_driver_h;


function new(string name, uvm_component parent);
	super.new(name,parent);
endfunction


function void build_phase(uvm_phase phase);
	super.build_phase(phase);
	av_config_h=new();
	av_sequencer_h=new("av_sequencer_h",this);
	av_driver_h=new("av_driver_h",this);
						// cntxt,inst_name,field_name,value    
	if(!(uvm_config_db #(virtual avalon_if#(AW,DW,TW))::get(this,"","DUT_IF",av_config_h.mif)))
		 `uvm_fatal("VIF CONFIG", "cannot get av interface");
endfunction

function void connect_phase(uvm_phase phase);
	av_driver_h.seq_item_port.connect(av_sequencer_h.seq_item_export);
	av_driver_h.mif=av_config_h.mif;
endfunction


endclass
