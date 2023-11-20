	component game is
		port (
			clk_clk                           : in  std_logic                     := 'X'; -- clk
			reset_reset_n                     : in  std_logic                     := 'X'; -- reset_n
			lcd_external_connection_export    : out std_logic_vector(11 downto 0);        -- export
			hex_external_connection_export    : out std_logic_vector(27 downto 0);        -- export
			key_external_connection_export    : in  std_logic                     := 'X'; -- export
			leds_external_connection_export   : out std_logic_vector(3 downto 0);         -- export
			switch_external_connection_export : in  std_logic                     := 'X'  -- export
		);
	end component game;

	u0 : component game
		port map (
			clk_clk                           => CONNECTED_TO_clk_clk,                           --                        clk.clk
			reset_reset_n                     => CONNECTED_TO_reset_reset_n,                     --                      reset.reset_n
			lcd_external_connection_export    => CONNECTED_TO_lcd_external_connection_export,    --    lcd_external_connection.export
			hex_external_connection_export    => CONNECTED_TO_hex_external_connection_export,    --    hex_external_connection.export
			key_external_connection_export    => CONNECTED_TO_key_external_connection_export,    --    key_external_connection.export
			leds_external_connection_export   => CONNECTED_TO_leds_external_connection_export,   --   leds_external_connection.export
			switch_external_connection_export => CONNECTED_TO_switch_external_connection_export  -- switch_external_connection.export
		);

