
module game (
	clk_clk,
	reset_reset_n,
	lcd_external_connection_export,
	hex_external_connection_export,
	key_external_connection_export,
	leds_external_connection_export,
	switch_external_connection_export);	

	input		clk_clk;
	input		reset_reset_n;
	output	[11:0]	lcd_external_connection_export;
	output	[27:0]	hex_external_connection_export;
	input		key_external_connection_export;
	output	[3:0]	leds_external_connection_export;
	input		switch_external_connection_export;
endmodule
