	game u0 (
		.clk_clk                           (<connected-to-clk_clk>),                           //                        clk.clk
		.reset_reset_n                     (<connected-to-reset_reset_n>),                     //                      reset.reset_n
		.lcd_external_connection_export    (<connected-to-lcd_external_connection_export>),    //    lcd_external_connection.export
		.hex_external_connection_export    (<connected-to-hex_external_connection_export>),    //    hex_external_connection.export
		.key_external_connection_export    (<connected-to-key_external_connection_export>),    //    key_external_connection.export
		.leds_external_connection_export   (<connected-to-leds_external_connection_export>),   //   leds_external_connection.export
		.switch_external_connection_export (<connected-to-switch_external_connection_export>)  // switch_external_connection.export
	);

