import uvm_pkg::*;
class av_config extends uvm_object;

virtual avalon_if mif;

function new(string name="av_config");
	super.new(name);
endfunction

endclass
