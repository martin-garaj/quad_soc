import uvm_pkg::*;

class av_driver #(AW =32, DW=128, TW= 2) extends uvm_driver #(av_mm_transaction #(AW, DW, TW));


virtual avalon_if#(AW, DW, TW) mif;


semaphore outstanding;

function new(string name, uvm_component parent);
	super.new(name,parent);
	outstanding=new(1);
endfunction



function void build_phase(uvm_phase phase);
	super.build_phase(phase);
endfunction


task run_phase(uvm_phase phase);
  	//phase.raise_objection(this);
	fork
		req_loop();
		resp_loop();
	join_none
  	//phase.drop_objection(this);
endtask
task req_loop();
		forever
		if (!mif.rst_n) begin
			wait_transaction();
			repeat(5) @(posedge mif.m_cb);
		end 
		else begin
			av_mm_transaction#(AW,DW,TW) tx;
			seq_item_port.get_next_item(tx);
			// to record transaction see webinar questa uvm debug
			//begin_tr(tx,"txd");
			case(tx.dir)
				AVALON_RD: begin
						@(posedge mif.m_cb)
						 while(mif.waitrequest!==1'b0) @(posedge mif.m_cb);
						 wait_transaction();
						 outstanding.get(1);
						 read_transaction(tx);
						 //mif.read_transaction(tx.addr);
			 			$display ("RD ADDR=%d, stall_time=%f",tx.addr,tx.stall_time);
					   end
				AVALON_WR: begin
						@(posedge mif.m_cb)
						 while(mif.waitrequest!==1'b0) @(posedge mif.m_cb);
						 write_transaction(tx);
						 //mif.write_transaction(tx.addr,tx.data);
			 			`uvm_info ("","WR  ADDR=%d",tx.addr);
					   end
				AVALON_WAIT: begin
						@(posedge mif.m_cb)
						wait_transaction();
					     end
				default:   `uvm_fatal(get_type_name(),  $sformatf("Sequence item request unknown!") )
			endcase
		//end
			//end_tr(tx);
			seq_item_port.item_done();
		end
endtask		
task resp_loop();
		forever
		if (!mif.rst_n) begin
			repeat(5) @(posedge mif.m_cb);
		end 
		else begin
			@(posedge mif.m_cb);
			if(mif.readdatavalid ==1'b1) outstanding.put(1);
		end
	
endtask
task read_transaction(av_mm_transaction #(32,64,2) seq);
	mif.address<=seq.addr;
	mif.byteenable<=8'hFF;
	mif.chipselect<=1'b1;
	mif.read<=1'b1;
	mif.write<=1'b0;
	//mif.readdata;
	mif.writedata<='0;
	//mif.waitrequest;
	//mif.readdatavalid;
	mif.burstcount<='0;
	mif.beginbursttransfer<=1'b0;
endtask
task write_transaction(av_mm_transaction #(32,64,2) seq);
	mif.address<=seq.addr;
	mif.byteenable<=8'hFF;
	mif.chipselect<=1'b1;
	mif.read<=1'b0;
	mif.write<=1'b1;
	//mif.readdata;
	mif.writedata<=seq.data;
	//mif.waitrequest;
	//mif.readdatavalid;
	mif.burstcount<='0;
	mif.beginbursttransfer<=1'b0;
endtask 
task wait_transaction();
	//if(outstanding.try_get(4)) 
	begin
	mif.address<='0;
	mif.byteenable<=8'h00;
	mif.chipselect<=1'b0;
	mif.read<=1'b0;
	mif.write<=1'b0;
	//mif.readdata;
	mif.writedata<='0;
	//mif.waitrequest;
	//mif.readdatavalid;
	mif.burstcount<='0;
	mif.beginbursttransfer<=1'b0;
	end 
endtask 
endclass
