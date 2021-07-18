library ieee; --Set visible the ieee library
	use ieee.std_logic_1164.all; --Needed to use the ieee types


entity avalon_i2c_master is
	port(
			--Avalon MM slave ports
				AB		: in std_logic_vector(2 downto 0);	--Address bus
				DBin	: in std_logic_vector(31 downto 0);	--Input Data Bus (or Write Data Bus)
				DBout	: out std_logic_vector(31 downto 0);--Output Data Bus (or Read Data Bus)
				wr		: in std_logic;							--Write Signal (active when the master wants to write to this peripheral)
				rd		: in std_logic;							--Read Signal (active when the master wants to read from this peripheral)
				wrq	: out std_logic;							--Waitrequest
					
			--System Clock
				clk	: in std_logic;
			--Reset Signal
				rst	: in std_logic;	
			--Interrupt
				irq 	: out std_logic;
				
			-- i2c lines
				scl   : inout std_logic;
				sda   : inout std_logic
		);			 
end entity;

architecture arch of avalon_i2c_master is
-- SIGNALS DECLARATION
	signal wb_clk_i      : std_logic;
	signal wb_rst_i      : std_logic;
	signal arst_i        : std_logic;
	signal wb_adr_i      : std_logic_vector(2 downto 0);
	signal wb_dat_i      : std_logic_vector(7 downto 0);
	signal wb_dat_o      : std_logic_vector(7 downto 0);
	signal wb_we_i       : std_logic;
	signal wb_stb_i      : std_logic;
	signal wb_cyc_i      : std_logic;
	signal wb_ack_o      : std_logic;
	signal wb_inta_o     : std_logic;
	
	signal avalon_address		: std_logic_vector(2 downto 0);
	signal avalon_writedata		: std_logic_vector(7 downto 0);
	signal avalon_readdata		: std_logic_vector(7 downto 0);
	signal avalon_write			: std_logic;
	signal avalon_read			: std_logic;
	signal avalon_waitrequest	: std_logic;
	
	signal clock					: std_logic;
	signal reset					: std_logic;
	signal interrupt 				: std_logic;
	
	signal scl_pad_i 		: std_logic;
	signal scl_pad_o  	: std_logic;
	signal scl_padoen_o 	: std_logic;
	signal sda_pad_i 		: std_logic;
	signal sda_pad_o 		: std_logic;
	signal sda_padoen_o 	: std_logic;

-- COMPONENT DECLARATION
	component i2c_master_top
		generic(
			ARST_LVL : std_logic := '0'              			  -- asynchronous reset level
		);
		port   (
			-- wishbone signals
			wb_clk_i      : in  std_logic;                    -- master clock input
			wb_rst_i      : in  std_logic := '0';             -- synchronous active high reset
			arst_i        : in  std_logic := not ARST_LVL;    -- asynchronous reset
			wb_adr_i      : in  std_logic_vector(2 downto 0); -- lower address bits
			wb_dat_i      : in  std_logic_vector(7 downto 0); -- Databus input
			wb_dat_o      : out std_logic_vector(7 downto 0); -- Databus output
			wb_we_i       : in  std_logic;                    -- Write enable input
			wb_stb_i      : in  std_logic;                    -- Strobe signals / core select signal
			wb_cyc_i      : in  std_logic;                    -- Valid bus cycle input
			wb_ack_o      : out std_logic;                    -- Bus cycle acknowledge output
			wb_inta_o     : out std_logic;                    -- interrupt request output signal

			-- i2c lines
			scl_pad_i     : in  std_logic;                    -- i2c clock line input
			scl_pad_o     : out std_logic;                    -- i2c clock line output
			scl_padoen_o  : out std_logic;                    -- i2c clock line output enable, active low
			sda_pad_i     : in  std_logic;                    -- i2c data line input
			sda_pad_o     : out std_logic;                    -- i2c data line output
			sda_padoen_o  : out std_logic                     -- i2c data line output enable, active low
		);
	end component;
	
begin
-- CONCURENT STATEMENTS
	-- inputs & outputs
	avalon_address <= AB;
	avalon_writedata <= DBin(7 downto 0);
	DBout <= x"000000" & avalon_readdata;
	avalon_write <= wr;
	avalon_read <= rd;
	wrq <= avalon_waitrequest;
	irq <= interrupt;
	clock <= clk;
	reset <= rst;
	irq <= interrupt;
	
	-- implying the use of tri-state buffers
	scl <= scl_pad_o when (scl_padoen_o = '0') else 'Z';
	sda <= sda_pad_o when (sda_padoen_o = '0') else 'Z';
	scl_pad_i <= scl;
	sda_pad_i <= sda;	
	
	-- adapter wishbone->avalon
	wb_clk_i				<= clock;
	wb_rst_i				<= reset;
	arst_i				<= '0';		-- async reset is not used, and is set to opposite value of ARST_LVL i2c_master_top generic
	wb_adr_i				<= avalon_address;
	wb_dat_i 			<= avalon_writedata;
	avalon_readdata 	<= wb_dat_o;
	wb_we_i 				<= avalon_write AND NOT(avalon_read);
	wb_stb_i				<= avalon_read OR avalon_write;
	wb_cyc_i 			<= avalon_read OR avalon_write;
	avalon_waitrequest<= NOT(wb_ack_o);
	interrupt 			<= wb_inta_o;

-- COMPONENT INSTANTIATION
	I2C_master : i2c_master_top
		generic map ( ARST_LVL => '1')		-- opposite value of arst_i signal
		port map (
			wb_clk_i      => wb_clk_i,          -- master clock input
			wb_rst_i      => wb_rst_i,		      -- synchronous active high reset
			arst_i        => arst_i,    			-- asynchronous reset
			wb_adr_i      => wb_adr_i, 			-- lower address bits
			wb_dat_i      => wb_dat_i, 			-- Databus input
			wb_dat_o      => wb_dat_o, 			-- Databus output
			wb_we_i       => wb_we_i,           -- Write enable input
			wb_stb_i      => wb_stb_i,          -- Strobe signals / core select signal
			wb_cyc_i      => wb_cyc_i,          -- Valid bus cycle input
			wb_ack_o      => wb_ack_o,          -- Bus cycle acknowledge output
			wb_inta_o     => wb_inta_o,         -- interrupt request output signal

			-- i2c lines
			scl_pad_i     => scl_pad_i,         -- i2c clock line input
			scl_pad_o     => scl_pad_o,         -- i2c clock line output
			scl_padoen_o  => scl_padoen_o,      -- i2c clock line output enable, active low
			sda_pad_i     => sda_pad_i,         -- i2c data line input
			sda_pad_o     => sda_pad_o,         -- i2c data line output
			sda_padoen_o  => sda_padoen_o       -- i2c data line output enable, active low
		);

end architecture;