//interface apb_if(input PCLK,
//                 input PRESETn);
//
//  logic[31:0] PADDR;
//  logic[31:0] PRDATA;
//  logic[31:0] PWDATA;
//  logic[15:0] PSEL;
//  logic PENABLE;
//  logic PWRITE;
//  logic PREADY;
//  logic PSLVERR;
//endinterface: apb_if
`ifndef WISHBONE_B4_IF__SV
`define WISHBONE_B4_IF__SV
interface wb_if #(ADR_W  = 32,DAT_W  = 64, TAG_W  = 1) (input bit clk,rst_n);
    //parameter DAT_W  = 64;       // data port width
    //parameter ADR_W  = 32;       // address port width
    //parameter TAG_W  = 1;        // default tag widths are 1 bit
    
    localparam SEL_W = (DAT_W/8); // 1 select bit per data byte, divide by 8 
    
    /// common signals ///
    logic [DAT_W-1:0] dat_i;  // data in bus
    logic [DAT_W-1:0] dat_o;  // data out bus
    logic             rst_i;  // core reset
    logic [TAG_W-1:0] tgd_i;  // tag for data in. Contains information associated to dat_i (such as parity).
    logic [TAG_W-1:0] tgd_o;  // tag for data out. Contains information associated to dat_o
    
    /// signals direction is dependent on agent ///
    logic             ack;  // acknowledge, signals normal termination of the bus cycle
    logic [ADR_W-1:0] adr;  // address bus
    logic             cyc;  // cycle, when asserted, indicates that a valid bus cycle is in progress
    logic             err;  // error, indicates an abnormal cycle termination
    logic             lock; // lock, when asserted, indicates that the current bus cycle is uninterruptible
    logic             rty;  // retry, cycle should be retried
    logic [SEL_W-1:0] sel;  // select array, indicates where a dat_o/dat_i (write/read) byte is valid, each bit represents one data byte
    logic             stb;  // strobe, indicates a valid data transfer cycle
    logic [TAG_W-1:0] tga;  // tag for address, contains information associated to the adr signal
    logic [TAG_W-1:0] tgc;  // tag for cycle, contains information associated with cyc
    logic             we;   // write enable
    logic 	      stall; //signal added in wishbone B4, equivalet to AXI ready.
  
    // master driver
    clocking m_drv_cb @(posedge clk);
      default input #1step output #1;
      
      //common signals
      input  dat_i; 
      output dat_o;
      input  rst_i;
      input  tgd_i;
      output tgd_o;
    
      // direction for master agent
      input  ack;  
      output adr;
      output cyc;
      input  err;
      output lock;
      input  rty;
      output sel;
      output stb;
      output tga;
      output tgc;
      output we;
      input  stall;
        
    endclocking: m_drv_cb
    
    // slave driver
    clocking s_drv_cb @(posedge clk);
      default input #1step output #1;
      
      //common signals
      input  dat_i; 
      output dat_o;
      input  rst_i;
      input  tgd_i;
      output tgd_o;
    
      // direction for slave agent
      output ack;
      input  adr;
      input  cyc;
      output err;
      input  lock;
      output rty;
      input  sel;
      input  stb;
      input  tga;
      input  tgc;
      input  we;
      output stall;
        
    endclocking: s_drv_cb
    
    clocking mon_cb @(posedge clk);
      default input #1step output #1;
        
      //common signals
      input  dat_i; 
      input  dat_o;
      input  rst_i;
      input  tgd_i;
      input  tgd_o;
    
      // all monitor signals are inputs
      input  ack;  
      input  adr;
      input  cyc;
      input  err;
      input  lock;
      input  rty;
      input  sel;
      input  stb;
      input  tga;
      input  tgc;
      input  we;
      input  stall;
 
    endclocking: mon_cb

modport master (clocking m_drv_cb);
modport slave (clocking s_drv_cb);
modport monitor (clocking mon_cb);

endinterface: wb_if

`endif //WISHBONE_B3_IF__SV
