module av2wb #(AW=32,DW=64,TW=2,MAX_OUTSTANDING=2) (input clk,input rst_n,avalon_if.s_cb av, wb_if.m_drv_cb wb);
parameter STALL=2'b01,
	  TRANSFER=2'b10;

logic [1:0] state, n_state;
logic av_valid,wb_ready;
always @* av_valid = av.chipselect && (av.read || av.write);
logic [1:0] n_count,count,ack_reg;
//conversion of req-ack to enable
//always @(wb.stb or wb.ack) begin
always @* begin
//	case({wb.stb,wb.ack})
//		2'b10: if(state ==TRANSFER) n_count<=count+1;
//		2'b11: if(state ==TRANSFER) n_count<=count; else n_count<=count-1;
//		2'b01: n_count<=count-1;
//		default: n_count<=count;
//	endcase		
		if(n_count< MAX_OUTSTANDING)
			wb_ready <=1'b1;
		else if(state==STALL || state ==TRANSFER)
			wb_ready <=1'b0;
end


always @(posedge clk or negedge rst_n)
	if(!rst_n) begin
			state <=STALL;
			count<='0;
		   end
	else	begin
	 	   state <=n_state;
		   count<=n_count;
		end



//always @(state or av_valid or wb_ready or wb.stb) begin
always @* begin
	n_state<='bx;
	
	n_count<=count;
	//do_stall;
	case(state)
		STALL: begin
				if(av_valid && wb_ready) 
					n_state<=TRANSFER;
			  	else
					n_state<=STALL;
				//if(wb.ack) n_count<=count-1;
				//if(wb.ack) wb_ready<=1'b1;
				//else wb_ready<=1'b0;
				if(wb.ack==1'b1) 
					n_count<=count-1;
				else 
					n_count<=count;
					 
			  end
				
		TRANSFER: begin 
				if(av_valid && wb_ready)
					n_state<=TRANSFER;
			  	else
					n_state<=STALL;
				//do_transfer;
				//if(!wb.ack) n_count<=count+1;
				//if(wb.ack) wb_ready<=1'b1;
				//else wb_ready<=1'b0;
				if(wb.ack==1'b1) 
					n_count<=count;
				else 
					n_count<=count+1;
				
			  end	
	endcase
end

always @(posedge clk or negedge rst_n)
	if(!rst_n) do_stall;
	else begin
		case(state)
			STALL:do_stall();
			TRANSFER:do_transfer();
			default: do_stall();
		endcase
	end 
always @* begin
	av.waitrequest<=!(wb_ready);
	av.readdata<=wb.dat_i;
	av.readdatavalid<=wb.ack;
	
end
task do_stall;
	
	wb.dat_o<='0;
	wb.tgd_o<='0;
	wb.adr<='0;
	//wb.cyc<=!(wb_ready);
	wb.cyc<=count[1] || count[0];
	wb.lock<=1'b0;
	wb.sel<=av.byteenable;
        wb.stb<=!(wb_ready);
        //wb.stb<=1'b0;
        wb.tga<='0;
        wb.tgc<='0;
        wb.we<='0;
endtask
//task do_transfer(avalon_if.s_cb av, wishbone_b3_if.m_drv_cb wb);
task do_transfer;
	wb.dat_i<=av.writedata;
	//wb.tgd_o<= av.tag;
	wb.adr<=av.address;
	wb.cyc<=1'b1;
	wb.stb<=1'b1;
	if(av.read) wb.we<=1'b0;
	else	    wb.we<=1'b1;
endtask
endmodule
//task do_stall(avalon_if av, wishbone_b3_if  wb);
