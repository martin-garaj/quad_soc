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
-- PERIOD - number of cycles after which the PWM signal repeats (refer to BLANK)
-- DUTY CYCLE - length of positive pulse within PERIOD
	-- FULL DUTY CYCLE = PERIOD + 1
	-- ZERO DUTY CYCLE = 0
-- BLANK - inserts blank periods , after every period
   -- reason : Servos and ESC require PWM signal, with very low ration of duty cycle and period (duty cycle / period = 1 / 10).
	--          To save counter bits, the counter doesnt need to run through whole period.
	--          Instead, every "active" period (when the period counter is checked against duty cycle value, to produce positive pulse),
	--          is followed by BLANK number of blank/"passive" periods, where there is no poisitive signal asserted.
	--          This solution can offer very high precision of pulses, while offering direct compatibility with Servo/ESC interfaces.

-- COMPILE-TIME PARAMETERS / GENERICS
-- PWM_COUNTER_LENGTH - the length of period / duty cycle counter in bits (for resolution of 4096, 12 bits are required)
-- BLANK_COUNTER_LENGTH - the length of blank counter (if there are 8 blank/"passive" periods, the counter needs 4 bits)

	 
-------------------------------------------------------------------------------------------------
------------------------------------    ENTITY - DEFINITION   -----------------------------------
-------------------------------------------------------------------------------------------------
entity pwm_counter_blank is
    generic (
				PWM_COUNTER_LENGTH        : integer := 12; -- 10=1024 / 11=2048 / 12=4096 / ...
				BLANK_COUNTER_LENGTH      : integer := 6
            );
    port(
	         --System Clock
                clk : in std_logic;
            --Reset Signal
                rst : in std_logic; 
			   --Enable signal
				    en : in std_logic; 
					 
            --value inputs 
					 period_value		 : in std_logic_vector( (PWM_COUNTER_LENGTH)-1 downto 0);   
					 duty_value        : in std_logic_vector( (PWM_COUNTER_LENGTH)-1 downto 0);  
					 blank_value       : in std_logic_vector( (BLANK_COUNTER_LENGTH)-1 downto 0); 
                
            --Outputs
                pwm_out : out std_logic;
					 end_out : out std_logic
					 
        );             
end entity;

-------------------------------------------------------------------------------------------------
---------------------------------------    ARCHITECTURE    --------------------------------------
-------------------------------------------------------------------------------------------------
architecture arch of pwm_counter_blank is
-- SIGNALS DECLARATION
    signal clock                    : std_logic;
    signal reset                    : std_logic;
	 signal pwm_signal               : std_logic;
	 signal end_signal               : std_logic;
	 signal en_signal                : std_logic;
	 
	 signal counter_reg              : std_logic_vector( (PWM_COUNTER_LENGTH)-1 downto 0); 
	 signal duty_reg                 : std_logic_vector( (PWM_COUNTER_LENGTH)-1 downto 0); 
	 signal period_reg               : std_logic_vector( (PWM_COUNTER_LENGTH)-1 downto 0); 
	 signal blank_counter_reg        : std_logic_vector( (BLANK_COUNTER_LENGTH)-1 downto 0); 
	 signal blank_reg                : std_logic_vector( (BLANK_COUNTER_LENGTH)-1 downto 0); 
    
-- COMPONENT DECLARATION
    
-------------------------------------------------------------------------------------------------
-----------------------------------    ARCHITECTURE - BEGIN    ----------------------------------
-------------------------------------------------------------------------------------------------
begin
-- CONCURENT STATEMENTS
clock <= clk;
reset <= rst;

pwm_out <= pwm_signal;
end_out <= end_signal;
en_signal <= en;

----------------------------------- PROCESS  
        pwm_counter_proc: process (clock, reset) begin
		      -- RESET
            if rising_edge(clock) and reset = '1' then
                counter_reg   <= (others => '0');
					 blank_counter_reg   <= (others => '0');
					 duty_reg      <= (others => '0');
					 period_reg    <= (others => '0');
					 blank_reg     <= (others => '0');
                pwm_signal    <= '0';
					 end_signal    <= '0';
                
            -- ENABLED
            elsif rising_edge(clock) and (en_signal ='1') then
				    -- EVERY ITERATION
                -- increase counter_reg
                counter_reg <= counter_reg + 1;
					 -- deassert END signal
					 end_signal    <= '0';
                
                -- PERIOD END
                if (unsigned(counter_reg)>=unsigned(period_reg)) then
					     -- reset counter_reg
                    counter_reg <= (others => '0');
                    -- update registers with new values
						  period_reg <= period_value;
                    duty_reg <= duty_value;
						  blank_reg <= blank_value;
						  -- assert End signal
						  end_signal    <= '1';
						  
						  -- increment blank counter
						  blank_counter_reg <= blank_counter_reg+1;
						  
						 -- BLANK END
						 if (unsigned(blank_counter_reg)>=unsigned(blank_reg)) then
							  -- reset counter_reg
							  blank_counter_reg <= (others => '0');
					    end if;  
                end if;    
                
                -- PERIOD BEGIN
                if (unsigned(counter_reg) >= 0) then
					     -- condition : only if the blank counter is 0
						  if( unsigned(blank_counter_reg) = to_unsigned(0,blank_counter_reg'length) ) then
								-- assert PWM signal
							   pwm_signal <= '1';
						  end if;
                end if;    
                
                -- DUTY END
                if (unsigned(counter_reg)>=unsigned(duty_reg))then
					     -- deassert PWM signal
                    pwm_signal <= '0';
                end if;    
					 
					 -- BLANK =! 0
							-- this is necessary, because the counter_reg resets, when == period_reg, but as I know, fot 100% duty cycle, duty_reg = period_reg+1,
							-- therefore, the DUTY END condition above is NEVER true, if duty_reg > period_reg (that is 100%)
                if ( unsigned(blank_counter_reg) /= to_unsigned(0,blank_counter_reg'length) ) then
					     -- deassert PWM signal
                    pwm_signal <= '0';
                end if;    
					 
					 
            -- DISABLED
            elsif rising_edge(clock) and (en_signal ='0') then
                -- stop and reset the coutner_reg
                counter_reg <= (others => '0');
					 blank_counter_reg <= (others => '0');
                
                -- deassert PWM signal
                pwm_signal <= '0';                
					 -- deassert END signal
					 end_signal    <= '0';
            end if;
            
        end process;

end architecture;