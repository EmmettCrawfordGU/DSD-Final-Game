--
-- top level (wrapper)
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity chip is
	port (
		clk      : in  std_logic;
		reset_n  : in  std_logic;
		lcd      : out std_logic_vector(11 downto 0);
		hex      : out std_logic_vector(27 downto 0);
		key      : in  std_logic;
		leds     : out std_logic_vector(3 downto 0);
		switch   : in  std_logic
	);
end entity chip;

architecture rtl_and_struct of chip is
	
	component game is
		port (
			clk_clk                           : in  std_logic := 'X'; -- clk
			reset_reset_n                     : in  std_logic := 'X'; -- reset_n
			lcd_external_connection_export    : out std_logic_vector(11 downto 0); -- export
			hex_external_connection_export    : out std_logic_vector(27 downto 0); -- export
			key_external_connection_export    : in  std_logic := 'X';              -- export
			leds_external_connection_export   : out std_logic_vector(3 downto 0);  -- export
			switch_external_connection_export : in  std_logic := 'X'               -- export
		);
	end component game;

begin
	u0 : component game
		port map (
			clk_clk                           => clk,
			reset_reset_n                     => reset_n,
			lcd_external_connection_export    => lcd,
			hex_external_connection_export    => hex,
			key_external_connection_export    => key,
			leds_external_connection_export   => leds,
			switch_external_connection_export => switch
	);
 
end architecture rtl_and_struct;
