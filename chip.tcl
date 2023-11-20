# CLOCK
set_location_assignment PIN_Y2 -to clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to clk

# RESET_n
set_location_assignment PIN_M23 -to reset_n
set_instance_assignment -name IO_STANDARD "2.5 V" -to reset_n

# LCD (11: LCD_EN, 10: LCD_RW, 9: LCD_RS, 8: LCD_ON, 7-0: data) 
set_location_assignment PIN_L4 -to lcd[11]
set_location_assignment PIN_M1 -to lcd[10]
set_location_assignment PIN_M2 -to lcd[9]
set_location_assignment PIN_L5 -to lcd[8]
set_location_assignment PIN_M5 -to lcd[7]
set_location_assignment PIN_M3 -to lcd[6]
set_location_assignment PIN_K2 -to lcd[5]
set_location_assignment PIN_K1 -to lcd[4]
set_location_assignment PIN_K7 -to lcd[3]
set_location_assignment PIN_L2 -to lcd[2]
set_location_assignment PIN_L1 -to lcd[1]
set_location_assignment PIN_L3 -to lcd[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[3]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[4]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[5]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[6]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[7]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[8]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[9]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[10]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd[11]

# HEX (0-6: HEX0, 7-13: HEX1, 14-20: HEX2, 21-27: HEX3)
set_location_assignment PIN_G18 -to hex[0]
set_location_assignment PIN_F22 -to hex[1]
set_location_assignment PIN_E17 -to hex[2]
set_location_assignment PIN_L26 -to hex[3]
set_location_assignment PIN_L25 -to hex[4]
set_location_assignment PIN_J22 -to hex[5]
set_location_assignment PIN_H22 -to hex[6]
set_location_assignment PIN_M24 -to hex[7]
set_location_assignment PIN_Y22 -to hex[8]
set_location_assignment PIN_W21 -to hex[9]
set_location_assignment PIN_W22 -to hex[10]
set_location_assignment PIN_W25 -to hex[11]
set_location_assignment PIN_U23 -to hex[12]
set_location_assignment PIN_U24 -to hex[13]
set_location_assignment PIN_AA25 -to hex[14]
set_location_assignment PIN_AA26 -to hex[15]
set_location_assignment PIN_Y25 -to hex[16]
set_location_assignment PIN_W26 -to hex[17]
set_location_assignment PIN_Y26 -to hex[18]
set_location_assignment PIN_W27 -to hex[19]
set_location_assignment PIN_W28 -to hex[20]
set_location_assignment PIN_V21 -to hex[21]
set_location_assignment PIN_U21 -to hex[22]
set_location_assignment PIN_AB20 -to hex[23]
set_location_assignment PIN_AA21 -to hex[24]
set_location_assignment PIN_AD24 -to hex[25]
set_location_assignment PIN_AF23 -to hex[26]
set_location_assignment PIN_Y19 -to hex[27]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[0]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[1]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[2]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[3]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[4]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[5]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[6]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[7]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[8]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[9]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[10]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[11]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[12]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[13]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[14]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[15]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[16]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[17]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[18]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[19]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[20]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[21]
set_instance_assignment -name IO_STANDARD "2.5 V" -to hex[22]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to hex[23]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to hex[24]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to hex[25]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to hex[26]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to hex[27]

# KEY (2)
set_location_assignment PIN_N21 -to key
set_instance_assignment -name IO_STANDARD "2.5 V" -to key

# LEDS (0-3 green)
set_location_assignment PIN_E21 -to leds[0]
set_location_assignment PIN_E22 -to leds[1]
set_location_assignment PIN_E25 -to leds[2]
set_location_assignment PIN_E24 -to leds[3]
set_instance_assignment -name IO_STANDARD "2.5 V" -to leds[0]
set_instance_assignment -name IO_STANDARD "2.5 V" -to leds[1]
set_instance_assignment -name IO_STANDARD "2.5 V" -to leds[2]
set_instance_assignment -name IO_STANDARD "2.5 V" -to leds[3]

# SWITCH (17)
set_location_assignment PIN_Y23 -to switch
set_instance_assignment -name IO_STANDARD "2.5 V" -to switch
