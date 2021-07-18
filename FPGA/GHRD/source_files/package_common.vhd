library ieee;
	use ieee.math_real.all;
	use ieee.std_logic_1164.all;

	use ieee.numeric_std.all;		 -- casting int to unsigned
	use ieee.std_logic_textio.all; -- read std_vector_logic from a file

library std;
	use std.textio.all;  --include package textio.vhd




package common is
	--data types
	type array_of_std_logic_vector is array(integer range <>) of std_logic_vector;
	type array2D_of_std_logic_vector is array (integer range <>) of array_of_std_logic_vector;
	
	--functions
	function ceil_log2(i: natural) return natural;
	impure function seek_line (constant n_line, bits : Natural; file read_file : text) return std_logic_vector;
	function int2str0dec (N, L: Natural) return String;
end package;

package body common is
-- ceil_log2
	function ceil_log2(i: natural) return natural is
	begin
		return integer(ceil(log2(real(i)))); 
	end function;
	
	
	
-- seek_line
	impure function seek_line (constant n_line, bits : Natural; file read_file : text) return std_logic_vector
	-- returns specific line from a file
	-- HOW TO :
	--		parameters : 
	--				n_line 		- number of line to be returned (starts from 0)
	--				bits			- bits stored on the line (multiples of 4, only hexagonal values, "bits" has to fit the length of data stored on the line in the .txt file)
	--				read_file 	- file including data (has to be opened before the seek_line function call)
	-- REQUIREMENTS :
	--		library ieee;   
	--			use ieee.std_logic_textio.all; 	-- read std_vector_logic from a file
	--		library std;
	--			use std.textio.all;  				-- include package textio.vhd
	is
		variable read_line	: line; 				-- buffer between the program and file
		variable read_data 	: std_logic_vector((bits-1) downto 0); -- 32 bit float
	   variable counter		: integer;
	begin
		counter := 0;
		
		L1 : while (counter <= n_line) loop
			readline(read_file, read_line);       	-- reading a line from the file.
			counter := counter + 1;
			
			exit L1 when counter = 9999;
		end loop;
		
		hread(read_line, read_data); 					-- reah hex value
		
		return read_data;
	end seek_line;
	
	
	
-- int2str0dec
	-- function to get integers into string in the following form : 0001, 0002, ... (preceeding '0')
	function int2str0dec (N, L: Natural) return String
	-- Accepts a number N to convert into a string of length L.  If the resulting
	-- string is too small, you get the least-significant digits of N in the
	-- returned string.
	is
	  variable Temp: Natural := N;
	  variable Result: String(1 to L) := (others => '0');
	begin
	  for i in Result'reverse_range loop
		 Result(i) := Character'Val((Temp mod 10) + Character'Pos('0'));
		 Temp := Temp/10;
		 exit when Temp = 0;
	  end loop;
	  return Result;
	end int2str0dec;

end package body;

