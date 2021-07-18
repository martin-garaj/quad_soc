-------------------------------------------------------------------------------------------------
-------------------------------------    AUTHOR & LICENSE    ------------------------------------
-------------------------------------------------------------------------------------------------
-- AUTHOR     : MARTIN GARAJ
-- DATE        : June 2017
-- Copyright (c) 2017 Martin Garaj
-- LICENSE    : MIT license

-------------------------------------------------------------------------------------------------
----------------------------------------    KNOWN ISSUE   ---------------------------------------
-------------------------------------------------------------------------------------------------
-- Qsys requires the following directive to understand VHDL 2008
-- (https://www.altera.com/support/support-resources/knowledge-base/solutions/rd05312011_49.html)
-- altera vhdl_input_version vhdl_2008
-------------------------------------------------------------------------------------------------
-----------------------------------------    LIBRARIES   ----------------------------------------
-------------------------------------------------------------------------------------------------
library ieee; --Set visible the ieee library
    use ieee.std_logic_1164.all; --Needed to use the ieee types
    use ieee.numeric_std.all;    -- conversion to_unsigned
    use ieee.std_logic_unsigned.all; -- airthmetic operation +/- and logic =
    
library work;
    use work.common.all;    -- type array_of_std_logic_vector, function ceil_log2() 
	 use work.avalon_PWM_constants.all; -- constants

-------------------------------------------------------------------------------------------------
------------------------------------    ENTITY - DEFINITION   -----------------------------------
-------------------------------------------------------------------------------------------------
entity avalon_PWM is
    generic (
            QUANTITY          		  : integer range 1 to 16 := 4;
				PRESCALER_COUNTER_LENGTH  : integer range 1 to 32 := 16;
				PWM_COUNTER_LENGTH        : integer range 1 to 32 := 12;
				BLANK_COUNTER_LENGTH      : integer range 1 to 8  := 6
            );
    port(
            --Avalon MM slave ports
                AB        : in std_logic_vector( (ceil_log2(ADDRESS_SPACE_SIZE))-1 downto 0);  
                DBin      : in std_logic_vector((AVALON_BUS_SIZE-1) downto 0);  --Input Data Bus (or Write Data Bus)
                DBout     : out std_logic_vector((AVALON_BUS_SIZE-1) downto 0);--Output Data Bus (or Read Data Bus)
                wr        : in std_logic;                            --Write Signal (active when the master wants to write to this peripheral)
                rd        : in std_logic;                            --Read Signal (active when the master wants to read from this peripheral)
                    
            --System Clock
                clk : in std_logic;
            --Reset Signal
                rst : in std_logic;    
                
            -- PWM outputs
                pwm_out : out std_logic_vector((QUANTITY-1) downto 0)
        );             
end entity;

-------------------------------------------------------------------------------------------------
---------------------------------------    ARCHITECTURE    --------------------------------------
-------------------------------------------------------------------------------------------------
architecture arch of avalon_PWM is
-- SIGNALS DECLARATION

    -- CHIP SELECT
    -- converts binary avalon_address into one-hot vector (e.g. avalon_address = "010" -> chip_select = "00000100")
    -- vector length is determined according to : QUANTITY*period_reg + QUANTITY*duty_reg + 1*control_reg + 1*status_reg
    signal chip_select              : std_logic_vector(ADDRESS_SPACE_SIZE-1 downto 0);
    
    -- REGISTERS
        -- control register
        signal control_reg          : std_logic_vector(QUANTITY downto 0);
		-- bits : high - [PWM_enable_X, PWM_enable_ ... , PWM_enable_0, reset] - low
		signal reset_bit 			  		: std_logic;
		signal enable_bits            : std_logic_vector(QUANTITY-1 downto 0);
        
	  -- status register
	  signal status_reg        	   : std_logic_vector((AVALON_BUS_SIZE-1) downto 0); -- maximum period value
	  
	  -- prescaler register
	  signal prescaler_reg        	: std_logic_vector((PRESCALER_COUNTER_LENGTH-1) downto 0); -- maximum period value	  
	  
--	  -- info_0_reg
--	  signal info_0_reg        	   : std_logic_vector((ceil_log2(QUANTITY)-1) downto 0) 						:= std_logic_vector(to_unsigned(QUANTITY, ceil_log2(QUANTITY))); -- stores constatnt value
--	  	  -- info_1_reg
--	  signal info_1_reg        	   : std_logic_vector((ceil_log2(PRESCALER_COUNTER_LENGTH)-1) downto 0) := std_logic_vector(to_unsigned(PRESCALER_COUNTER_LENGTH, ceil_log2(PRESCALER_COUNTER_LENGTH))); -- stores constatnt value
--	  	  -- info_2_reg
--	  signal info_2_reg        	   : std_logic_vector((ceil_log2(PWM_COUNTER_LENGTH)-1) downto 0) 		:= std_logic_vector(to_unsigned(PWM_COUNTER_LENGTH, ceil_log2(PWM_COUNTER_LENGTH))); -- stores constatnt value
--	  	  -- info_3_reg
--	  signal info_3_reg        	   : std_logic_vector((ceil_log2(BLANK_COUNTER_LENGTH)-1) downto 0) 		:= std_logic_vector(to_unsigned(BLANK_COUNTER_LENGTH, ceil_log2(BLANK_COUNTER_LENGTH))); -- stores constatnt value

	  -- period register
	  -- stores the length of 1 PWM period as number of FPGA clock cycles
	  signal blank_reg            : array_of_std_logic_vector((QUANTITY-1) downto 0)((BLANK_COUNTER_LENGTH-1) downto 0);
	  
	  -- period register
	  -- stores the length of 1 PWM period as number of FPGA clock cycles
	  signal period_reg            : array_of_std_logic_vector((QUANTITY-1) downto 0)((PWM_COUNTER_LENGTH-1) downto 0);

	  -- duty cycle register
	  -- stores the length of active part of duty cycle as number of FPGA clock cycles (duty_reg <= period_reg)
	  signal duty_reg                : array_of_std_logic_vector((QUANTITY-1) downto 0)((PWM_COUNTER_LENGTH-1) downto 0);
    
    -- avalon MM
    signal avalon_address        : std_logic_vector( (ceil_log2(ADDRESS_SPACE_SIZE))-1 downto 0);
    signal avalon_writedata        : std_logic_vector((AVALON_BUS_SIZE-1) downto 0);
    signal avalon_readdata        : std_logic_vector((AVALON_BUS_SIZE-1) downto 0);
    signal avalon_write            : std_logic;
    signal avalon_read            : std_logic;
    signal avalon_waitrequest    : std_logic;
    
    -- system sugnals
    signal clock                    : std_logic;
    signal reset                    : std_logic;
	 
	 -- interconnecting signals
	 signal scaled_clk              : std_logic;
	 signal scaled_rst              : std_logic;
        -- stores the ON/OFF state within 1 PWM period
    signal pwm_signal        : std_logic_vector((QUANTITY-1) downto 0);
	 
-- COMPONENT DECLARATION
   -- software adjustable prescaler 
	COMPONENT soft_prescaler
		GENERIC (
				COUNTER_LENGTH        : integer := PRESCALER_COUNTER_LENGTH
            );
		PORT (
				clk : IN STD_LOGIC;
				en : IN STD_LOGIC;
				midpoint_value : IN STD_LOGIC_VECTOR((PRESCALER_COUNTER_LENGTH-1) DOWNTO 0);
				rst : IN STD_LOGIC;
				scaled_clk : OUT STD_LOGIC;
				scaled_rst : OUT STD_LOGIC
		);
	END COMPONENT;
	 -- PWM counter with blank register
	COMPONENT pwm_counter_blank
		GENERIC (
				PWM_COUNTER_LENGTH        : integer := PWM_COUNTER_LENGTH; -- 10=1024 / 11=2048 / 12=4096 / ...
				BLANK_COUNTER_LENGTH      : integer := BLANK_COUNTER_LENGTH
            );
		PORT (
			blank_value : IN STD_LOGIC_VECTOR((BLANK_COUNTER_LENGTH-1) DOWNTO 0);
			clk : IN STD_LOGIC;
			duty_value : IN STD_LOGIC_VECTOR((PWM_COUNTER_LENGTH-1) DOWNTO 0);
			en : IN STD_LOGIC;
			end_out : OUT STD_LOGIC;
			period_value : IN STD_LOGIC_VECTOR((PWM_COUNTER_LENGTH-1) DOWNTO 0);
			pwm_out : OUT STD_LOGIC;
			rst : IN STD_LOGIC
	);
	END COMPONENT;
    -- altera ALTIOBUF (outbut buffer, no open drain)
    component output_buffer 
        port(
            datain    : IN STD_LOGIC_VECTOR (0 DOWNTO 0);
            oe            : IN STD_LOGIC_VECTOR (0 DOWNTO 0);
            dataout    : OUT STD_LOGIC_VECTOR (0 DOWNTO 0)
        );
    end component;
    
-------------------------------------------------------------------------------------------------
-----------------------------------    ARCHITECTURE - BEGIN    ----------------------------------
-------------------------------------------------------------------------------------------------
begin
-- CONCURENT STATEMENTS
    -- inputs & outputs
    -- port-to-port connections are sometimes difficult to track in simulation software, 
    --    intermediate signals with verbose names are nice and easy to track
    avalon_address <= AB;
    avalon_writedata <= DBin;
    DBout <= avalon_readdata;
    avalon_write <= wr;
    avalon_read <= rd;
    clock <= clk;
    reset <= rst;
	
	-- internal signals
	reset_bit <= not(control_reg(RESET_BIT_N));
	enable_bits <= control_reg(QUANTITY downto ENABLE_BIT_OFFSET);

----------------------------------- PROCESS
-- CONTROL REGISTER
    -- read/write
    control_reg_proc: process (clock, reset) begin
        if rising_edge(clock) and reset='1' then
            -- reset
            control_reg <= (others => '0');
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- write
            if chip_select(CONTROL_REG_OFFSET)='1' and avalon_write='1' then
                control_reg <= avalon_writedata(QUANTITY downto 0);
            end if;
            -- read
            if chip_select(CONTROL_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <= ( (AVALON_BUS_SIZE-1) downto (QUANTITY+1) => '0' ) & control_reg; 
                report "avalon_readdata CONTROL";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
    
-- STATUS REGISTER
    -- read-only
    status_reg_proc: process (clock, reset) begin 
        if rising_edge(clock) and reset='1' then
            -- reset
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- read
            if chip_select(STATUS_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <=  status_reg; 
                report "avalon_readdata STATUS";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
	 
-- PRESCALER REGISTER
    -- read/write
    prescaler_reg_proc: process (clock, reset) begin
        if rising_edge(clock) and reset='1' then
            -- reset
            prescaler_reg <= (others => '0');
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- write
            if chip_select(PRESCALER_REG_OFFSET)='1' and avalon_write='1' then
                prescaler_reg <= avalon_writedata(PRESCALER_COUNTER_LENGTH-1 downto 0);
            end if;
            -- read
            if chip_select(CONTROL_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <= ( (AVALON_BUS_SIZE-1) downto PRESCALER_COUNTER_LENGTH => '0' ) & prescaler_reg; 
                report "avalon_readdata PRESCALER";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
	 
-- INFO_0_REGISTER
    -- read-only
    info_0_reg_proc: process (clock, reset) begin 
        if rising_edge(clock) and reset='1' then
            -- reset
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- read
            if chip_select(INFO_0_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <=  ( (AVALON_BUS_SIZE-1) downto ceil_log2(QUANTITY)+1 => '0' ) & std_logic_vector(to_unsigned(QUANTITY, ceil_log2(QUANTITY)+1));
                report "avalon_readdata INFO_0";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
	 
-- INFO_1_REGISTER
    -- read-only
    info_1_reg_proc: process (clock, reset) begin 
        if rising_edge(clock) and reset='1' then
            -- reset
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- read
            if chip_select(INFO_1_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <=  ( (AVALON_BUS_SIZE-1) downto ceil_log2(PRESCALER_COUNTER_LENGTH)+1 => '0' ) & std_logic_vector(to_unsigned(PRESCALER_COUNTER_LENGTH, ceil_log2(PRESCALER_COUNTER_LENGTH)+1)); 
                report "avalon_readdata INFO_1";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
    
-- INFO_2_REGISTER
    -- read-only
    info_2_reg_proc: process (clock, reset) begin 
        if rising_edge(clock) and reset='1' then
            -- reset
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- read
            if chip_select(INFO_2_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <=  ( (AVALON_BUS_SIZE-1) downto ceil_log2(PWM_COUNTER_LENGTH)+1 => '0' ) & std_logic_vector(to_unsigned(PWM_COUNTER_LENGTH, ceil_log2(PWM_COUNTER_LENGTH)+1)); 
                report "avalon_readdata INFO_2";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
	 
-- INFO_3_REGISTER
    -- read-only
    info_3_reg_proc: process (clock, reset) begin 
        if rising_edge(clock) and reset='1' then
            -- reset
            avalon_readdata <= (others=>'Z');
        elsif rising_edge(clock) then
            -- read
            if chip_select(INFO_3_REG_OFFSET)='1' and avalon_read='1' then
                avalon_readdata <=  ( (AVALON_BUS_SIZE-1) downto ceil_log2(BLANK_COUNTER_LENGTH)+1 => '0' ) & std_logic_vector(to_unsigned(BLANK_COUNTER_LENGTH, ceil_log2(BLANK_COUNTER_LENGTH)+1)); 
                report "avalon_readdata INFO_3";
            else 
                avalon_readdata <= (others=>'Z'); 
            end if;
        end if;
    end process;
	 
----------------------------------- GENERATE
    -- CHIP SELECT
    chip_select_gen: for I in 0 to ADDRESS_SPACE_SIZE-1 generate
        chip_select(I) <= '1' when (I=avalon_address) else '0';
    end generate chip_select_gen;
	 
    -- PWM REGISTERS
    pwm_regs_gen: for I in 0 to (QUANTITY-1) generate
    
        -- avalon MM blank registers
        blank_reg_X_proc: process (clock, reset) begin
            if rising_edge(clock) and ((reset = '1') or (reset_bit = '1')) then
                -- reset
                blank_reg(I) <= (others => '0');
                avalon_readdata <= (others=>'Z');
            elsif rising_edge(clock) then
                -- write
                if chip_select(I+BLANK_REG_OFFSET)='1' and avalon_write='1' then
                    blank_reg(I) <= avalon_writedata((BLANK_COUNTER_LENGTH-1) downto 0);
                end if;
                -- read
                if chip_select(I+BLANK_REG_OFFSET)='1' and avalon_read='1' then
                    avalon_readdata <= ( (AVALON_BUS_SIZE-1) downto BLANK_COUNTER_LENGTH => '0' ) & blank_reg(I); -- append '0's & period_reg
                else 
                    avalon_readdata <= (others=>'Z'); 
                end if;
            end if;
        end process;
	 
        -- avalon MM period registers
        period_reg_X_proc: process (clock, reset) begin
            if rising_edge(clock) and ((reset = '1') or (reset_bit = '1')) then
                -- reset
                period_reg(I) <= (others => '0');
                avalon_readdata <= (others=>'Z');
            elsif rising_edge(clock) then
                -- write
                if chip_select(I+PERIOD_REG_OFFSET)='1' and avalon_write='1' then
                    period_reg(I) <= avalon_writedata((PWM_COUNTER_LENGTH-1) downto 0);
                end if;
                -- read
                if chip_select(I+PERIOD_REG_OFFSET)='1' and avalon_read='1' then
                    avalon_readdata <= ( (AVALON_BUS_SIZE-1) downto PWM_COUNTER_LENGTH => '0' ) & period_reg(I); -- append '0's & period_reg
                else 
                    avalon_readdata <= (others=>'Z'); 
                end if;
            end if;
        end process;

        -- avalon MM duty registers
        duty_reg_X_proc: process (clock, reset) begin
            if rising_edge(clock) and ((reset = '1') or (reset_bit = '1')) then
                -- reset
                duty_reg(I) <= (others => '0');
                avalon_readdata <= (others=>'Z');
            elsif rising_edge(clock) then
                -- write
                if chip_select(I+DUTY_REG_OFFSET)='1' and avalon_write='1' then
                    duty_reg(I) <= avalon_writedata((PWM_COUNTER_LENGTH-1) downto 0);
                end if;
                -- read
                if chip_select(I+DUTY_REG_OFFSET)='1' and avalon_read='1' then
                    avalon_readdata <= ( (AVALON_BUS_SIZE-1) downto PWM_COUNTER_LENGTH => '0' ) & duty_reg(I); -- append '0's & duty_reg
                else 
                    avalon_readdata <= (others=>'Z'); 
                end if;
            end if;
        end process;

    end generate pwm_regs_gen;
	 
	 -- PRESCALER
	 	soft_prescaler_inst : soft_prescaler
		generic map (
				COUNTER_LENGTH  => PRESCALER_COUNTER_LENGTH
		)
		PORT MAP (
			-- list connections between master ports and signals
				clk => clock,
				en => '1',
				midpoint_value => prescaler_reg,
				rst => reset_bit,
				scaled_clk => scaled_clk,
				scaled_rst => scaled_rst
		);
	 
    -- PWM LOGIC
    pwm_logic_gen: for I in 0 to (QUANTITY-1) generate

		pwm_counter_blank_X_inst : pwm_counter_blank
		generic map (
				PWM_COUNTER_LENGTH  => PWM_COUNTER_LENGTH, -- 10=1024 / 11=2048 / 12=4096 / ...
				BLANK_COUNTER_LENGTH => BLANK_COUNTER_LENGTH
				)
		PORT MAP (
			-- list connections between master ports and signals
				blank_value => blank_reg(I),
				clk => scaled_clk,
				duty_value => duty_reg(I),
				en => enable_bits(I),
				--end_out => end_out,		-- no use in current design
				period_value => period_reg(I),
				pwm_out => pwm_signal(I),
				rst => scaled_rst
		);
	 
	 end generate pwm_logic_gen;
	
    -- BUFFER GENERATION
    buffer_gen: for I in 0 to (QUANTITY-1) generate
        output_buffer_X_inst : output_buffer
            port map(
                datain(0)     => pwm_signal(I),
                oe(0)          => enable_bits(I),
                dataout(0)     => pwm_out(I)
            );
    end generate buffer_gen;

end architecture;