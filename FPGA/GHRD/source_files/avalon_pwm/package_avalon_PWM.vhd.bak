library ieee;
	use ieee.math_real.all;
	use ieee.std_logic_1164.all;

	use ieee.numeric_std.all;		 -- casting int to unsigned
	use ieee.std_logic_textio.all; -- read std_vector_logic from a file

library std;
	use std.textio.all;  --include package textio.vhd




package avalon_PWM_constants is
constant ADDRESS_SPACE_SIZE : integer := 64; -- for max(QUANTITY) = 16
-- ADDRESS SPACE
-- 		addr | register
--			   0 | control register
--			   1 | status register
--          2 | prescaler register
--			   3 | info_0 register (QUANTITY)
--			   4 | info_1 register (PRESCALER_COUNTER_LENGTH)
--			   5 | info_2 register (PWM_COUNTER_LENGTH)
--			   6 | info_3 register (BLANK_COUNTER_LENGTH)
constant CONTROL_REG_OFFSET 	: integer := 0;
	-- control_reg
	constant RESET_BIT_N : integer := 0;		-- reset bit (low == active)
	constant ENABLE_BIT_OFFSET : integer := 1;	-- bits enabling the PWM output (hgn == enable, low == disable)
constant STATUS_REG_OFFSET 		: integer := 1;
constant PRESCALER_REG_OFFSET 	: integer := 2;
constant INFO_0_REG_OFFSET 		: integer := 3;
constant INFO_1_REG_OFFSET 		: integer := 4;
constant INFO_2_REG_OFFSET 		: integer := 5;
constant INFO_3_REG_OFFSET 		: integer := 6;

constant BLANK_REG_OFFSET : integer := 16; -- for max(QUANTITY) = 16
-- QUANTITY*1 | blank registers
constant PERIOD_REG_OFFSET : integer := 32; -- for max(QUANTITY) = 16
-- QUANTITY*1 | period registers
constant DUTY_REG_OFFSET : integer := 48; -- for max(QUANTITY) = 16
-- QUANTITY*1 | duty registers



constant AVALON_BUS_SIZE : integer := 32;
-- this is 32bit wide Avalon peripheral

end package;


