# DSD-Final-Game By Hayato Tsujii, Emmett Crawford, Matthew VonWahlde, Cameron Zhang

This game runs on the Altera DE2-115.

# Objective
1. Avoid the asteroids that come your way!
2. Survive as long as you can!


## How to run on your computer 
1. Download ZIP file from Github and unzip
2. Open Quartus
3. Open project in Quartus
4. Run TCL script
5. Compile code
6. Upload the code to the Programmer
7. Open Nios II Software Build tools for Eclipse
8. Build project
9. Run project as Nios II hardware
10. Start game!
   

## How to play
1. To start game
  - Key 2
2. Move up and down
  - Switch 17 
3. Reset game 
  - Key 0
    
## Troubleshooting
### ELF process failed
![image](https://github.com/EmmettCrawfordGU/DSD-Final-Game/assets/97589878/64e848a2-e586-48fc-a1a0-836b11fca29c)
- Click run
- Click run Configuration
- Make sure you are under "Nios II Hardware" -> "blinly-sw Nios II Hardware" on the left side
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

- Right click blinky-sw_bsp
- Hover over NIOS II
- Click "Generate QSF

