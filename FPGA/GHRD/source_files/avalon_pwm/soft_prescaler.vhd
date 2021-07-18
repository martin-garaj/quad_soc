-------------------------------------------------------------------------------------------------
-------------------------------------    AUTHOR & LICENSE    ------------------------------------
-------------------------------------------------------------------------------------------------
-- AUTHOR     : MARTIN GARAJ
-- DATE        : October 2017
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
-------------------------------------------------------------------------------------------------
-------------------------------------    INTENDED BEHAVIOUR   -----------------------------------
-------------------------------------------------------------------------------------------------
-- RUNTIME ADJUSTABLE PARAMETERS
-- MIDPOINT_VALUE 
--		values of prescaler clock reduction
--			MIDPOINT_VALUE | scaling
--                    0 | 2 (scaled_clk is half the clk frequency)
--							 1 | 4
--							 2 | 6
--							 3 | 8
--                    4 | 10 (scaled_clk is 1/10th of the clk frequency)
 

-- COMPILE-TIME PARAMETERS / GENERICS
-- COUNTER_LENGTH - the maximum length of the counter in bits

	 
-------------------------------------------------------------------------------------------------
------------------------------------    ENTITY - DEFINITION   -----------------------------------
-------------------------------------------------------------------------------------------------
entity soft_prescaler is
    generic (
				COUNTER_LENGTH        : integer := 16
            );
    port(
	         -- system Clock
                clk : in std_logic;
            -- reset Signal
                rst : in std_logic; 
			   -- enable signal
				    en : in std_logic; 
					 
            -- value inputs 
					 midpoint_value		 : in std_logic_vector( (COUNTER_LENGTH)-1 downto 0);   
                
            -- outputs
					 scaled_clk : out std_logic;
					 scaled_rst : out std_logic
					 
        );             
end entity;

-------------------------------------------------------------------------------------------------
---------------------------------------    ARCHITECTURE    --------------------------------------
-------------------------------------------------------------------------------------------------
architecture arch of soft_prescaler is
-- SIGNALS DECLARATION
    signal clock                    : std_logic;
    signal reset                    : std_logic;
	 signal scaled_clock             : std_logic := '0';
	 signal scaled_reset             : std_logic;
	 signal reset_mem                : std_logic_vector(1 downto 0);
	 signal en_signal                : std_logic;
	 signal counter_reg              : std_logic_vector( (COUNTER_LENGTH)-1 downto 0); 
	 signal midpoint_reg             : std_logic_vector( (COUNTER_LENGTH)-1 downto 0); 

-- COMPONENT DECLARATION
    
-------------------------------------------------------------------------------------------------
-----------------------------------    ARCHITECTURE - BEGIN    ----------------------------------
-------------------------------------------------------------------------------------------------
begin
-- CONCURENT STATEMENTS
clock <= clk;
reset <= rst;
en_signal <= en;

scaled_clk <= scaled_clock;
scaled_rst <= scaled_reset;

----------------------------------- PROCESS  
        counter_proc: process (clock, reset) begin
		      -- RESET
            if rising_edge(clock) and reset = '1' then
				    -- delete any stored values
                counter_reg   <= (others => '0');
					 midpoint_reg  <= (others => '0');
					 -- deassert scaled_clk output
                scaled_clock  <= '0';
					 -- reset reset_mem bit
					 reset_mem <= "00";
                
            -- ENABLED
            elsif rising_edge(clock) and (en_signal ='1') then
				    -- EVERY ITERATION
                -- increase counter_reg
                counter_reg <= counter_reg + 1;
					 
                -- MID-POINT
                if (unsigned(counter_reg)>=unsigned(midpoint_reg)) then
					     -- reset counter_reg
                    counter_reg <= (others => '0');
						  -- invert polarity / produce clock pulse
						  scaled_clock <= not(scaled_clock);
						  -- update prescaler midpoint value
						  midpoint_reg <= midpoint_value;
						  
						  -- scaled_rst logic
						  if (unsigned(reset_mem)<=to_unsigned(2,2)) then
								reset_mem <= reset_mem + 1;
								scaled_reset <= '1';
						  else
						      scaled_reset <= '0';
						  end if;
                end if;    
                
            -- DISABLED
            elsif rising_edge(clock) and (en_signal ='0') then
                -- stop and reset the coutner_reg
                counter_reg <= (others => '0');
					 
					 -- deassert scaled_clk output
					 scaled_clock  <= '0';
            end if;
            
        end process;

end architecture;