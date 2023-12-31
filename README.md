# DSD-Final-Game By Hayato Tsujii, Emmett Crawford, Matthew VonWahlde, Cameron Zhang

This game runs on the Altera DE2-115.

# Objective
1. Avoid the asteroids that come your way!
2. Survive as long as you can!

# Features
1. 10Hz LCD Gameplay
2. 4 Lives indicated on LEDG 0-3
3. Death animation played on death
4. 3 Different asteroid sprites
5. Special 1/16 chance black hole : instant Game Over
6. Randomly generated asteroids
7. Score 0 to 9999 on 7-Segment Display

## How to run on your computer 
- Download ZIP file.
- Unzip the folder by right clicking the ZIP file and selecting "Extract All..."
- Select the source destination and click "Extract."
- Once that is complete, open Quartus Prime 18.0.
- In the home tab, select "Open Project."
![Open Project](image-1.png)
- Navigate to the downloaded folder, and double click to enter.
- Open the file .../DSD-Final-Game-main/chip.qpf
- Now, in Quartus, navigate to Tools->Programmer.
- Quartus should identify the chip.sof file automatically.
![Programmer](image-2.png)
- If it did not, click "Add File..."
- Open the file .../DSD-Final-Game-main/output_files/chip.sof
- Make sure Altera DE2-115 board is turned on.
- If the Programmer has not identified the target board, click "Hardware Setup..." then double click USB-Blaster. Close that window.
- Click start to upload the custom chip to the FPGA board.
- Sometimes, the upload fails, so click start again if that is the case.
- After the upload is successful, close the programmer.
- A window may pop up. Select Yes to save changes to the chip.cdf file.
![Save Changes](image-3.png)
- Now, navigate to Tools->Nios II Software Build Tools for Eclipse.
- Once Eclipse is open, it will prompt you to select a workspace.
- Select "Browse...", then select the folder where the projects were extracted .../DSD-Final-Game-main/software. Click "OK" to select that workspace.
![Software folder](image-4.png)
 - Click "OK" again to open the workspace.
 - Navigate to Run->Run Configurations... 
 - Expand "Nios II Hardware" on the right and select game-sw Nios II Hardware configuration.
![Hardware config](image-6.png)
 - Click the "..." in the "File system ELF file name:" section.
 ![ELF file browse](image-7.png)
 - Return to where the files were extracted, from the folder, select ...DSD-Final-Game-main/software/game-sw/game-sw.elf and click "Select."
 - Click "Apply" and then "Run."
 - The executable and linkable format file for the game should now upload to the board.
 - Enjoy the game!
   
## How to play
1. To start game
  - Press Key 2 to start playing
2. Move up and down
  - Use Switch 17 to move the spaceship up and down to avoid the asteroids coming right to left
3. Reset game 
  - To reset the game press Key 0
    
## Troubleshooting in Eclipse
### ELF process failed
![image](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/97589878/64e848a2-e586-48fc-a1a0-836b11fca29c)
- Click run
- Click run Configuration
- Make sure you are under "Nios II Hardware" -> "game-sw Nios II Hardware" on the left side
- Check "Enable browse for file system ELF file"
- Click the ... button
- Click DSD-Final-Game-main folder
- Click DSD-Final-Game-main folder again
- Click Software folder
- Click game-sw folder
- Click game-sw.elf and press select on the bottom right
- Click Apply Then Click Run 

### BSP error
![image](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/97589878/487f85dd-3177-447c-b069-914981ddf773)

- Right click game-sw_bsp folder on the left
- Hover over NIOS II
- Click "Generate QSF"

## Troubleshooting Hardware
### Black bars on LCD
- Go to the file hw.c on line 134 (should be in the function lcdInit on the line setTimer(LCD_TIMER,5000))
![lcdInit](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/101604242/027f543b-7d12-4290-af4d-7396a66f1916)
- Increase the value 5000 in that line in increments of 1000.
- If that doesn't work, redownload the zip file and start from the beginning.

### Random characters printing on the LCD
- Go to the file hw.c on line 204 (should be in the function lcdWrite on the line setTimer(LCD_TIMER,1000))
![lcdWrite](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/101604242/81ae8bf3-dfd9-4c67-8f20-aa58fb19e221)
- Increase the value 1000 in that line in increments of 500.
- If that doesn't work, redownload the zip file and start from the beginning.

### Characters printing in the wrong spot 
- Go to the file hw.c on line 159 (should be in the function lcdWrite on the line setTimer(LCD_TIMER,1000))
![lcdSetAddr](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/101604242/e40a7efd-4e57-42a6-a767-2b08ba91826d)
- Increase the value 1000 in that line in increments of 500.
- If that doesn't work, redownload the zip file and start from the beginning.

### Timers are not working properly
- If the timers are not working properly, check these following lines.
- Make sure that you are setting a time (setTimer) and starting the timer (startTimer).
![set and start timers](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/101604242/39011f4b-ac31-4b2c-87b0-5825b7ef56f9)
- In the interrupt handlers (handleGameTimer & handleLCDTimer), make sure the interrupt is acknowledged by writing 0 to the proper timer base address in the function IOWR_ALTERA_AVALON_TIMER_STATUS.
![Acknowledge](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/101604242/fa39118d-28b7-4394-96d9-f28e612fb1f5)

