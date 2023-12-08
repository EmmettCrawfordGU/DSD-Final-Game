// Main file for Astrododge
// Authors: Matthew VonWahlde, Cameron Zheng, Hayato Tsujii, Emmett Crawford
// Last modified: 12/7/2023
// Description : The main game loop for the program Astrododge.
//               This program is meant to run on a Nios II processor that is on an FPGA.

#include "hw.h"
#include <time.h>


#define LCD_ROWS 2      /* Number of rows on the LCD */
#define LCD_COLS 16     /* Number of columns on the LCD */

#define PLAYER_COLUMN 1 /* The column that the player is located on the LCD */

#define BLANK      0x20 /* ASCII for a space character */
#define PLAYER     0xF6 /* ASCII for the player */
#define ASTEROID_0 0x2A /* ASCII for Asteroid 0 */
#define ASTEROID_1 0xEF /* ASCII for Asteroid 1 */
#define ASTEROID_2 0xF2 /* ASCII for Asteroid 2 */
#define BLACK_HOLE 0xFF /* ASCII for the Black Hole */


// The matrix that stores the locations of the player and asteroids for the game
uint8_t game_matrix[LCD_ROWS][LCD_COLS] = {
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
	{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK}
};

// Other globals
uint8_t lives = 4;  // Number of lives remaining
uint32_t score = 0; // Player's score (increments every 1/10 of a second
uint32_t count = 0; // Increments on the start screen until the player pushes KEY2
					// 		Used to seed the random number generator

// Function prototypes (declared below)
void update_matrix(void);
void death(void);
void delayMs(uint32_t milliseconds);
void gameOver(void);
uint8_t updatePlayer(uint8_t data);
void updateLCD(void);
void start(void);
uint8_t **createAsteroid(void);
void free_array(uint8_t** array);
void draw_player(uint8_t data);
void deathAnimation(uint8_t row);


// Entry point for the program
int main(){
	uint8_t switch_data; // The value read in from switch 17
	uint8_t hit;         // Collision detection, 0 - Not hit, 1 - hit by asteroid, 2 - hit by black hole

	// Initializations
	initTimers();
	lcdInit();

	// Clear the hex display
	IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASE, 0x0FFFFFFF);

	// Left in here from the tutorial for nostalgia
	alt_putstr("Ciao from Nios II!\n");

	// Set the LEDs
	uint8_t led_pattern = 0x00;
	for(int i = 0; i < lives; i++){led_pattern |= 0x01 << i;}
	setLEDs(led_pattern);

	// Set the game timer to 100ms and start
	setTimer(GAME_TIMER, 100000);
	startTimer(GAME_TIMER);

	// Enter the start screen and wait for player to press Key 2
	start();

	// Clear the lcd and draw the player
	lcdClear();
	switch_data = readSwitch();
	draw_player(switch_data);

	// Seed the random number generator
	srand(count);

	// Main loop
	while(1) {
		// Clear the semaphore and wait until the game timer sets it
		// This controls the framerate to 10 Hz
		clearSema(GAME_TIMER);
		while(getSema(GAME_TIMER) == 0){};

		// Set the score
		if(score >= 9999){score = 9999;}
		else{score++;}
		setSevenSeg(score);

		// Read in the state of switch 17
		switch_data = readSwitch();

		// Overwrite the player
		if(game_matrix[0][PLAYER_COLUMN] == PLAYER){game_matrix[0][PLAYER_COLUMN] = BLANK;}
		if(game_matrix[1][PLAYER_COLUMN] == PLAYER){game_matrix[1][PLAYER_COLUMN] = BLANK;}

		// Move matrix 1 column to the left, generate new column on the right
		update_matrix();

		// Update the player and check if the player has been hit
		hit = updatePlayer(switch_data);

		// Update the LCD
		updateLCD();

		// If the player has been hit
		if(hit != 0x00){
			if(hit == 0x02){ // If hit by a black hole, game over
				lives = 0;
			} else { // If hit by an asteroid, lose a life
				lives--;
			}

			// Set the LEDs
			uint8_t led_pattern = 0x00;
			for(int i = 0; i < lives; i++){led_pattern |= 0x01 << i;}
			setLEDs(led_pattern);

			// Go to function that handles the player losing a life
			death();
		}
	}

	return 0;
}


// Displays the starting screen and waits for the player to press Key 2
void start(void){
	uint8_t key_data = 1;

	// Clears LCD and displays title
	lcdClear();
	lcdSetAddr(0, 0);
	lcdWriteStr("** ASTRODODGE **\0");

	// Display the next line
	lcdSetAddr(1, 2);
	lcdWriteStr("Press KEY2!");

	// Wait for player to press Key 2
	while(key_data == 1) {
		key_data = readKey();

		// Increment count to seed the random number generator
		count++;
		if(count >= 1000000){
			count = 0;
		}
	}
}


// Creates a 2x1 array with an asteroid randomly in one of the rows
uint8_t **createAsteroid(void){
	// Allocate space for the new asteroid matrix
	uint8_t **newAsteroid = (uint8_t**)malloc(2 * sizeof(uint8_t*));

	// Array to index the different asteroids and black hole
	uint8_t asteroids[4] = {ASTEROID_0, ASTEROID_1, ASTEROID_2, BLACK_HOLE};

	// Randomly generate an asteroid and a row
	uint8_t randomAsteroid = rand() % 4;
	uint8_t randomRow = rand() % 2;

	// If it is a black hole, randomly generate a number out of 4 again
	if (randomAsteroid == 3)
		randomAsteroid = rand() % 4;

	// Clear the newAsteroid array
    for(int i = 0; i < 2; ++i) {
    	newAsteroid[i] = (uint8_t*)malloc(sizeof(uint8_t));
    	newAsteroid[i][0] = BLANK;	// blank spaces
    }

    // Insert the asteroid in the random row
	newAsteroid[randomRow][0] = asteroids[randomAsteroid];

	// Return the matrix
	return newAsteroid;
}


// Call after the 2x1 array is done being used
void free_array(uint8_t** array){
    // Don't forget to free the allocated memory
    for (int i = 0; i < 2; ++i) {
        free(array[i]);
    }
    free(array);
}


// Shifts the game matrix to the left and adds a new column on the right
void update_matrix(void){
	// Container for the new column
	uint8_t** new_col;
	static int count = 1; // Used as a timer to generate new asteroids to leave room for the player to dodge

	// Shift each value to the left
	for (int i = 0; i < LCD_ROWS; i++) {
		for (int j = 0; j < LCD_COLS - 1; j++) {
			game_matrix[i][j] = game_matrix[i][j + 1];
		}
	}

	// Check if it is time to generate a new asteroid
	if(count <= 0){
		// Set the count again
		count = (rand() % 4) + 2;

		// Get the new column with the asteroid
		new_col = createAsteroid();
		game_matrix[0][LCD_COLS - 1] = new_col[0][0]; // update top right bit
		game_matrix[1][LCD_COLS - 1] = new_col[1][0]; // update bottom left bit
		free_array(new_col); // Free the memory
	} else {
		game_matrix[0][LCD_COLS - 1] = BLANK; // update top right bit
		game_matrix[1][LCD_COLS - 1] = BLANK; // update bottom right bit
		count--;
	}
}


// Draws the player on the screen
void draw_player(uint8_t data){
	// Sets the LCD address to the player location
	lcdSetAddr((~data) & 0x01, PLAYER_COLUMN);
	lcdWrite(PLAYER);
}


// Update the player and check for collision with an asteroid
// Returns 0 if not hit, 1 if hit by an asteroid, and 2 if hit by a black hole
uint8_t updatePlayer(uint8_t data){
	// Switch is backwards from the LCD (LCD 1 = lower row, switch 1 = upwards direction)
	uint8_t row = (~data) & 0x01;

	// Check if the player moved
	// Set the location of the player in the game matrix
	// Return collision status
	if(game_matrix[row][PLAYER_COLUMN] != BLANK){
		// Check what the player was hit by
		if(game_matrix[row][PLAYER_COLUMN] == BLACK_HOLE){
			game_matrix[row][PLAYER_COLUMN] = PLAYER;
			return 0x02; // hit by a black hole
		} else {
			game_matrix[row][PLAYER_COLUMN] = PLAYER;
			return 0x01; // hit by an asteroid
		}
	} else {
		game_matrix[row][PLAYER_COLUMN] = PLAYER;
		return 0x00; // not hit
	}
}


// Delays in increments of 100 ms using the game timer
void delayMs(uint32_t milliseconds) {
	// Delay for the appropriate number of milliseconds
	for(uint32_t i = 100; i <= milliseconds; i += 100){
		clearSema(GAME_TIMER);
		while(getSema(GAME_TIMER) == 0){};
	}
}


// Handles the death of the player
void death(void){
	// Check where player dies
	uint8_t row;
	if(game_matrix[0][PLAYER_COLUMN] == PLAYER)
		row = 0;
	else
		row = 1;

	// Play the death animation
	deathAnimation(row);

	// Check number of lives remaining
	if(lives == 0){
		gameOver(); // Never returns
	} else {
		// Write the number of lives remaining to the LCD
		lcdClear();
		lcdSetAddr(0,4);
		lcdWriteStr("LIVES: \0");
		lcdWrite(lives+48);

		// Tell the player to press Key 2
		lcdSetAddr(1,0);
		lcdWriteStr("   PRESS KEY2   \0");

		// Wait until Key 2 is pressed
		while(readKey() == 1);

		// Clear the LCD and game matrix
		lcdClear();
		for(int i = 0; i < LCD_ROWS; i++){
			for(int j = 0; j < LCD_COLS; j++){
				game_matrix[i][j] = BLANK;
			}
		}

		// Draw the player
		uint8_t switch_data = readSwitch();
		draw_player(switch_data);
	}
}


// Displays game over message and never returns
void gameOver(void){
	// Clear the LCD
	lcdClear();

	// Display game over message
	lcdSetAddr(0,4);
	lcdWriteStr("GAMEOVER\0");

	// Prompt the player to restart the board
	lcdSetAddr(1,1);
	lcdWriteStr("KEY0 TO RESTART\0");

	// Wait forever
	while(1);
}


// Plays the death animation of the player on the LCD
void deathAnimation(uint8_t row){
	uint32_t delay = 100;

	// Small
	lcdSetAddr(row,1);
	lcdWrite(0xA1);
	delayMs(delay);
	// Medium
	lcdSetAddr(row,1);
	lcdWrite(0x6F);
	delayMs(delay);
	// Large
	lcdSetAddr(row,1);
	lcdWrite(0x4F);
	delayMs(delay);
	// XL
	lcdSetAddr(row,1);
	lcdWrite('X');
	delayMs(delay);
	// Large
	lcdSetAddr(row,1);
	lcdWrite(0x4F);
	delayMs(delay);
	// Medium
	lcdSetAddr(row,1);
	lcdWrite(0x6F);
	delayMs(delay);
	// Small
	lcdSetAddr(row,1);
	lcdWrite(0xA1);
	delayMs(delay);
}


// Updates the LCD from the game matrix
void updateLCD(void){
	// Stores the previous state of the game matrix to delete past images
	static uint8_t prev_matrix[LCD_ROWS][LCD_COLS] = {
			{BLANK, BLANK,  BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK},
			{BLANK, BLANK,  BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK}
	};

	// Get what row the player is on in the current and previous matrices
	uint8_t newPlayerRow  = (game_matrix[0][PLAYER_COLUMN] == PLAYER) ? (0) : (1);
	uint8_t prevPlayerRow = (prev_matrix[0][PLAYER_COLUMN] == PLAYER) ? (0) : (1);

	// Update the player first (this will look smoother)
	if(newPlayerRow != prevPlayerRow){
		// Write the new player to the LCD
		lcdSetAddr(newPlayerRow, PLAYER_COLUMN);
		lcdWrite(PLAYER);

		// Clear previous player's image
		lcdSetAddr(prevPlayerRow, PLAYER_COLUMN);
		lcdWrite(BLANK);
	}

	// Drawing asteroids
	for(int i = 0; i < LCD_ROWS; i++){
		for(int j = 0; j < LCD_COLS; j++){
			uint8_t prev = prev_matrix[i][j];
			uint8_t new = game_matrix[i][j];

			// Player is already updated
			if(new == PLAYER){
				continue;
			}

			// If there is an asteroid
			if(new != BLANK){
				// Draw asteroid
				lcdSetAddr(i, j);
				lcdWrite(new);
			} else {
				// Check for old asteroids
				if(prev != BLANK && prev != PLAYER){
					lcdSetAddr(i, j);
					lcdWrite(BLANK);
				}
			}
		}
	}

	// Copy frame into the previous matrix for next iteration
	for(int i = 0; i < LCD_ROWS; i++){
		for(int j = 0; j < LCD_COLS; j++){
			prev_matrix[i][j] = game_matrix[i][j];
		}
	}
}
