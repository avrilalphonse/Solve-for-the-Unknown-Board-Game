// FINAL CODE FOR GROUP 39

#include "stm32f4xx_hal.h"
#include "LiquidCrystal.h"
#include "ece198.h"

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h> // for random num generator
#include <stdlib.h> // for random num generator

//FUNCTIONS
char roll_the_dice();
int num_of_players();
void print_clue(int clueNum);
bool start_game();
int get_number();
int clue_number_sync(int codeNum);
bool code_verify (char guessCode[], char codeCh []);
bool next_round();
void print_moving_screen();

char roll_the_dice()
// Function called roll_the_dice that generates random number from 1 to 6
// it returns the character value of the random number
{
    int high = 6, low = 1;
    srand(HAL_GetTick());
    int r = (rand() % (high + 1 - low)) + low;
    char charVal = r + '0';
    return (charVal); 
}

int num_of_players()
// Function called num_of_numbers that recieve input from user, the number of players
// it displays the number of players on the screen, and if user inputs the invalid number, it makes them to input again
{
    bool players = true;
    int nums[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    char keypadSymbols[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
    char displayPlayers[2];
    displayPlayers[0] = ' ';

    InitializeKeypad();
    while (players)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if (key != 9 && key != 10 && key != 11)  // in case if user clicks *, 0, #
        {
            displayPlayers[1] = keypadSymbols[key];
            //output to LCD
            setCursor(14,1);
            print(displayPlayers);
            HAL_Delay(1000);
            clear();
            return nums[key];  // return int to loop 
            players = false;
        } else
        {
            clear();
            setCursor(1,0);
            print("Invaid Number");
            setCursor(3,1);
            print("Re-enter:");
            HAL_Delay(2000);
        }
        while (ReadKeypad() >= 0);  // wait until key is released
    }
    return 1;            
}

bool start_game()
// Function that startts the game when user clicks any button from the keypad
{
    bool begin = true;
    InitializeKeypad();
    while (begin)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if(key >= 0)
        {
            begin = false;
        }
        while (ReadKeypad() >= 0);  // wait until key is released
    }
    return true;
}

int get_number()
// Function named get_number that get a random number from 0 to 19
// This number choose the clueNum that will provide to user
{
    int high = 19, low = 0;
    int ret_Val = 0;
    srand(HAL_GetTick());
    int r = (rand() % (high + 1 - low)) + low;
    int clueNumArr[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    ret_Val = clueNumArr[r];
    return ret_Val;
}

int clue_number_sync(int codeNum)
{
    int numForClue = 0;
    numForClue = codeNum;
    return numForClue;
}

bool code_verify (char guessCode[], char codeCh []) 
// Function code_verify compares if the answer and the guessed code by user are same
{
    for (int i = 0; i < 3; i++){
        if (guessCode[i] != codeCh[i]){
            return false;
        }
    }
    return true;
}

bool next_round()
// Function called next_round that goes to the next round to roll a dice again and get new clues
// Through this function, it provides more time for user to guess clues.
// Also, if the user click hashtag, it will go to the next round, and otherwise it will not do anything
{
    bool repeat = true;
    InitializeKeypad();
    while (ReadKeypad() < 0);   // wait for a valid key
    while(repeat)
    {
        int key = ReadKeypad();
        if (key == 11)  // TO PLAY AGAIN, CLICK '#' KEY
        {
            return true;
        }
    }//end repeat game
    while (ReadKeypad() >= 0);  // wait until key is released
    return false;
}

bool terminate()
// Function called terminate that restarts the game when user clicks the hashtag button, and ends the game when 
// the user clicks the other buttons
{ 
    bool repeat = true;
    InitializeKeypad();
    while (ReadKeypad() < 0);   // wait for a valid key
    while(repeat)
    {
        int key = ReadKeypad();
        if (key == 11)  // TO PLAY AGAIN, CLICK '#' KEY
        {
            return true;
        }
        else
        {
            return false; 
        }
    }//end repeat game
    while (ReadKeypad() >= 0);  // wait until key is released
    return false;
}

void print_moving_screen() 
// Function named print_moving_screen, it helps the printed sentences to move to the left side
{
    bool test = true;
    while(test) //Code  inspired from: https://github.com/SayidHosseini/STM32LiquidCrystal/blob/master/examples/Scroll/main.c 
	{
		// scroll 16 positions (display length + string length) to the left
		// to move it back to center:
		for (int positionCounter = 0; positionCounter < 16; positionCounter++) 
        {
		    // scroll one position left:
		    scrollDisplayLeft();
		    // wait a bit:
	        HAL_Delay(350);
		}
        // delay at the end of the full loop:
	    HAL_Delay(1000);
        test = false;
	}
    clear();
}

int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED

    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

    SerialSetup(9600);

    LiquidCrystal(GPIOB, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6); // LCD
    
    //START GAME KEY MESSAGE
    setCursor(2,0);
    print("Press any key");
    setCursor(4,1);
    print("to start");
    HAL_Delay(3000);
    bool GAME = start_game();
    clear();

    //LOOP FOR ONE COMPLETE GAME
    while(GAME)
    {
        //Initializations
        int numOfPlayers = 0, codeDoubleDigits = 0;
        char codeCh[4], diceOutput[2];
        diceOutput[1]= ' ';
        int code [4]; 
        bool checker = true, dice = true;

        //Welcome Message LCD
        setCursor(1, 0);
        print("Welcome to The");
        setCursor(1,1);
        print("Mystery of E7!");
        HAL_Delay(3000);
        print_moving_screen();
        
        //Number of Players LCD
        setCursor(0,0);
        print("Please enter the");
        setCursor(1,1);
        print("# of players:");
        numOfPlayers = num_of_players();
        HAL_Delay(3000);
        clear();
        
        setCursor(3,0);
        print("Let's start");
        HAL_Delay(3000);
        clear();

        for(int m = 0; m < 4; ++m) // FOUR TURNS -> FOUR CLUES
        {
            setCursor(1,0);
            print("Roll the dice!");
            HAL_Delay(3000);
            clear();

            for(int k = 0; k < numOfPlayers; ++k) // players all roll dice
            {
                InitializeKeypad();
                while (ReadKeypad() < 0);   // wait for a valid key
                while(dice)
                {
                    int key = ReadKeypad();
                    if (key == 9)  // TO ROLL DICE, CLICK '*' KEY
                    {
                        diceOutput[0] = roll_the_dice();
                        // ROLLING DICE & OUTPUT NUM TO LCD
                        setCursor(7,0);
                        print(diceOutput);
                        HAL_Delay(3000);
                        clear();
                        dice = false;
                    }
                }//end rolling dice
                while (ReadKeypad() >= 0);  // wait until key is released
                dice = true;
            }// # of players rolling dice

            //Storing Integer Values of Code/Password
            code[m] = get_number();
            while(checker)
            {
                //To ensure that NO clues repeat in ONE game
                if (m == 0) 
                {
                    checker = false;
                } else if(m == 1)
                {
                    if(code[m] != code[m-1])
                        checker = false;
                    else
                        code[m] = get_number();
                } else if(m == 2)
                {
                    if(code[m] != code[m-1] && code[m] != code[m-2])
                        checker = false;
                    else
                        code[m] = get_number();
                } else
                {
                    if(code[m] != code[m-1] && code[m] != code[m-2] && code[m] != code[m-3])
                        checker = false;
                    else
                        code[m] = get_number();
                }//end assigning int values for the 4-digit code
            }//end while loop for checker
            checker = true;

            //Assigning int value to match with clue
            int codeNumForSync = 0;
            codeNumForSync = clue_number_sync(code[m]);

            //Storing values as characters for final display
            if(code[m] > 9) // Change double digits to single digits for FINAL 4-digit code
            {
                codeDoubleDigits = code[m] - 10;
                codeCh[m] = codeDoubleDigits + '0';
            } else
            {
                codeCh[m] = code[m] + '0';
            }

            //Display Clue
            print_clue(codeNumForSync);

            bool hashtag = true; //For '#' key
            if(m == 3) // Last round in one game -> leads to guessing the final code
            {
                //Display Move to Guess Code Msg
                setCursor(0, 0);
                print("Click # to guess");
                setCursor(3,1);
                print("the code!");
                while(hashtag)
                {
                    if (next_round() == true)
                        hashtag = false;
                }
                HAL_Delay(3000);
                clear();
            } else
            {
                //Display Move to Next Round Msg
                setCursor(0, 0);
                print("Click # for the");
                setCursor(2,1);
                print("next round!");
                while (hashtag)
                {
                    if (next_round() == true)
                        hashtag = false;
                }
                HAL_Delay(3000);
                clear();
            }
        } //# of rounds loop
        
        //Initililizations
        char keypadSymbols[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
        bool guessCheck = true;
        char guessCode[5];

        for(int j = 0; j < 3; j++) // 3 trials in total to guess code
        {
            //Initializing Values to ' ' to print string to LCD
            guessCode[1] = ' ';
            guessCode[2] = ' ';
            guessCode[3] = ' ';
            guessCode[4] = ' ';
            clear();

            //Enter 4-giti Message LCD
            setCursor(1, 0);
            print("Enter 4-digit");
            setCursor(5,1);
            print("code: ");
            for(int p = 0; p < 4; p++) // get 4-digit code from player through keypad
            {
                InitializeKeypad();
                while (guessCheck)
                {
                    while (ReadKeypad() < 0);   // wait for a valid key
                    int key = ReadKeypad();
                    if(key >= 0)
                    {
                        guessCode[p] = keypadSymbols[ReadKeypad()];
                        guessCheck = false;
                    }
                    while (ReadKeypad() >= 0);  // wait until key is released
                }
                //Show user which number they clicked on LCD
                setCursor(11,1);
                print(guessCode);
                guessCheck = true;
            }//end for loop
            HAL_Delay(3000);
            clear();
            if (code_verify(guessCode, codeCh)) //calling function to check if user-inputted code matches stored final code
            {
                //Winning Message
                setCursor(2, 0);
                print("You escaped!");
                HAL_Delay(3000);
                clear();
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, true);   // turn on LED
                j=3; //leaves for loop
            } else
            {
                //Code is wrong Message
                setCursor(3, 0);
                print("Not quite");
                HAL_Delay(3000);
                clear();
                for (int o = 0; o< 10; o++) // FOR LED
                {
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                    if(j == 2) // If user gets code wrong 3 times, LED blinks faster at 3rd attempt
                        HAL_Delay(200);
                    else
                        HAL_Delay(300); 
                }//end for loop for LED blinking speed
            }
        }//end for loop for FINAL GUESS                
        setCursor(3, 0);
        print("Game Over.");
        HAL_Delay(4000);
        clear();

        //Play Again Message
        setCursor(1, 0);
        print("To Play Again,");
        setCursor(4, 1);
        print("Press #");
        if(terminate()) // calls terminate() function that returns true if '#' key is pressed
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false);   // turn off LED
            GAME = true; // while loop continues to be true; repeats
        } else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false);   // turn off LED
            GAME = false;     
        }//end play again if statement
        HAL_Delay(3000);
        clear();
    }//END OF LOOP FOR ONE GAME
    return 0;
}//end main

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); 
    // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

void print_clue(int clueNum) 
// Function called print_clue, that gets a value of clueNum which is a randomly generated number from 0 to 19
// As it gets the number from main, it prints the clue to the screen that corresponds to the clue number.
{
    //MATCHING INT NUM WITH ACCORDING CLUE
    if(clueNum == 0)
    {
        setCursor(1,0);
        print("Solve question");
        setCursor(3,1);
        print("on the board");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 1)
    {
        setCursor(4,0);
        print("Tiration");
        HAL_Delay(6000);
        clear();

    } else if(clueNum == 2)
    {
        setCursor(0,0);
        print("# of blue chairs");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 3)
    {
        setCursor(0,0);
        print("int a = 4; a = 3");
        setCursor(1,1);
        print("std::cout<<a;");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 4)
    {
        setCursor(2,0);
        print("# of working");
        setCursor(3,1);
        print("lightbulbs");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 5)
    {
        setCursor(2,0);
        print("# of vertices");
        setCursor(0,1);
        print("on the nano tile");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 6)
    {
        setCursor(1,0);
        print("I can grow on");
        setCursor(5,1);
        print("trees");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 7)
    {
        setCursor(1,0);
        print("You can't walk");
        setCursor(5,1);
        print("up me");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 8)
    {
        setCursor(3,0);
        print("Sugar cube");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 9)
    {
        setCursor(1,0);
        print("# of countries");
        setCursor(4,1);
        print("you see");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 10)
    {
        setCursor(2,0);
        print("Protect your");
        setCursor(2,1);
        print("head with me");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 11)
    {
        setCursor(1,0);
        print("I sound like a");
        setCursor(6,1);
        print("car");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 12)
    {
        setCursor(2,0);
        print("I'm smarter ");
        setCursor(0,1);
        print("without a brain");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 13)
    {
        setCursor(3,0);
        print("___ crash");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 14)
    {
        setCursor(0,0);
        print("I am you in the");
        setCursor(5,1);
        print("future");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 15)
    {
        setCursor(0,0);
        print("Red set of paper");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 16)
    {
        setCursor(0,0);
        print("I keep you awake");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 17)
    {
        setCursor(0,0);
        print("Move you without");
        setCursor(3,1);
        print("you moving");
        HAL_Delay(6000);
        clear();
    } else if(clueNum == 18)
    {
        setCursor(2,0);
        print("Climb me to ");
        setCursor(1,1);
        print("achieve goals");
        HAL_Delay(6000);
        clear();
    } else
    {
        setCursor(0,0);
        print("Where engineers");
        setCursor(6,1);
        print("live");
        HAL_Delay(6000);
        clear();
    } // end if statements
}//end print_clue 