// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

// #define BUTTON_BLINK
// #define LIGHT_SCHEDULER
// #define TIME_RAND
// #define KEYPAD
// #define ROLLING_DICE
// #define ROLL_GET_CLUE
// #define KEYPAD_CONTROL

#include <stdbool.h> // booleans, i.e. true and false

//FUNCTIONS
char roll_the_dice();
int num_of_players();
char keypad_output();
void print_clue(int clueNum);
bool start_game();
int get_number();
int clue_number_sync(int codeNum);
bool code_verify (char guess_code[], char code []);

//void display_secret_code();

#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h> // for random num generator
#include <stdlib.h> // for random num generator


#include "ece198.h"

char roll_the_dice()
{
    //srand (time(0)); //seed
    //char *dice_numbers = "123456";
    int high = 6, low = 1;
    srand(HAL_GetTick());
    int r = (rand() % (high + 1 - low)) + low;
    char charVal = r + '0';
    return (charVal); //dice_numbers[rand() %6]
     
}

int num_of_players()
{
    bool players = true;
    char keypadSymbols[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
    int nums[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    InitializeKeypad();
    while (players)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if (key != 9 && key != 10 && key != 11)  // in case if user clicks *, 0, #
        {
            SerialPutc(keypadSymbols[key]); // output to console
            SerialPuts("\n");
            return nums[key];  // return int to loop 
            players = false;
        } else
        {
            SerialPuts("Invaid Number of Players. Re-enter:");
            SerialPuts("\n");
        }
        while (ReadKeypad() >= 0);  // wait until key is released
    }
    return 1;            
}

char keypad_output()
{
    return 'a';
}

bool start_game()
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

bool code_verify (char guess_code[], char code [])
{
    for (int i = 0; i < 3; i++){
        if (guess_code[i] != code[i]){
            return false;
        }
    }
    return true;
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

    // as mentioned above, only one of the following code sections will be used
    // (depending on which of the #define statements at the top of this file has been uncommented)

    //SOLVE THE UNKNOWN

    //TESTING
/*
    
*/

    bool GAME = start_game();

    while(GAME)
    {
        SerialPuts("Welcome to The Mystery of E7: Solve the Unknown!\nPlease enter the number of players:");

        //Initializations
        bool dice = true;
        int numOfPlayers = 0;
        char codeCh[4];
        int code [4]; 
        bool checker = true;

        numOfPlayers = num_of_players();
        
        SerialPuts("Let's start... Roll the dice!");

        for(int m = 0; m < 4; ++m) // FOUR TURNS -> FOUR CLUES
        {
            SerialPuts("\n");
            for(int k = 0; k < numOfPlayers; ++k) // players all roll dice
            {
                InitializeKeypad();
                while (ReadKeypad() < 0);   // wait for a valid key
                while(dice)
                {
                    int key = ReadKeypad();
                    if (key == 9)  // TO ROLL DICE, CLICK '*' KEY
                    {
                    //
                        SerialPutc(roll_the_dice());   // ROLLING DICE & OUTPUT NUM TO CONSOLE
                        SerialPuts("\n");
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
                if (m == 0) 
                {
                    checker = false;
                } else if(code[m] != code[m-1])// m > 0
                {
                    checker = false;
                } else
                {
                    code[m] = get_number();
                }
            }
            checker = true;

            int codeNumForSync = 0;
            codeNumForSync = clue_number_sync(code[m]);

            //storing values as characters for final display
            if(code[m] > 9)
            {
                code[m] = code[m] - 10;
            }
            codeCh[m] = code[m] + '0';
            SerialPuts("PASSWORD #:");
            SerialPutc(codeCh[m]);
            SerialPuts("\n");

            //Display Clue
            SerialPuts("Clue #");
            char clueNum = (m+1)+'0';
            SerialPutc(clueNum);
            SerialPuts(": ");
            print_clue(codeNumForSync);
        }//# of rounds loop

        for(int j = 0; j < 3; j++) // 3 trials in total
        {
            char guess_code [4];
            if (code_verify(guess_code,code)){
                SerialPutc("You escaped!");
                //LED
                j=3;
            }
            else
            {
                SerialPutc("Try again!");
            }

        }
        GAME = false;
    }// ONE GAME


#ifdef LIGHT_SCHEDULER
    // Turn on the LED five seconds after reset, and turn it off again five seconds later.

    while (true) {
        uint32_t now = HAL_GetTick();
        if (now > 5000 && now < 10000)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, true);   // turn on LED
        else
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false);  // turn off LED
    }
#endif

#ifdef TIME_RAND
    // This illustrates the use of HAL_GetTick() to get the current time,
    // plus the use of random() for random number generation.
    
    // Note that you must have "#include <stdlib.h>"" at the top of your main.c
    // in order to use the srand() and random() functions.

    // while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button press
    // srand(HAL_GetTick());  // set the random seed to be the time in ms that it took to press the button
    // if the line above is commented out, your program will get the same sequence of random numbers
    // every time you run it (which may be useful in some cases)

    while (true) // loop forever
    {
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button press

        // Display the time in milliseconds along with a random number.
        // We use the sprintf() function to put the formatted output into a buffer;
        // see https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm for more
        // information about this function
        char buff[100];
        sprintf(buff, "Time: %lu ms   Random = %ld\r\n", HAL_GetTick(), random());
        // lu == "long unsigned", ld = "long decimal", where "long" is 32 bit and "decimal" implies signed
        SerialPuts(buff); // transmit the buffer to the host computer's serial monitor in VSCode/PlatformIO

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));  // wait for button to be released
    }
#endif

#ifdef KEYPAD
    // Read buttons on the keypad and display them on the console.

    // this string contains the symbols on the external keypad
    // (they may be different for different keypads)
    char *keypad_symbols = "123456789*0#";
    // note that they're numbered from left to right and top to bottom, like reading words on a page

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        SerialPutc(keypad_symbols[ReadKeypad()]);  // look up its ASCII symbol and send it to the hsot
        while (ReadKeypad() >= 0);  // wait until key is released
    }
#endif

#ifdef ROLLING_DICE
    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if (key == 9)  // top-right key in a 4x4 keypad, usually 'A'
            SerialPutc(roll_the_dice());   // toggle LED on or off
         while (ReadKeypad() >= 0);  // wait until key is released
    }
#endif

#ifdef KEYPAD_CONTROL
    // Use top-right button on 4x4 keypad (typically 'A') to toggle LED.

    InitializeKeypad();
    while (true)
    {
        while (ReadKeypad() < 0);   // wait for a valid key
        int key = ReadKeypad();
        if (key == 3)  // top-right key in a 4x4 keypad, usually 'A'
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);   // toggle LED on or off
         while (ReadKeypad() >= 0);  // wait until key is released
    }
#endif
    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

void print_clue(int clueNum)
{
    if(clueNum == 0)
    {
        char C0[100] = "Solve the question on the board";
        SerialPuts(C0);
    } else if(clueNum == 1)
    {
        char C1[100] = "Tiration";//;{'T', 'i', 'r'}
        SerialPuts(C1);
    } else if(clueNum == 2)
    {
        char C2[100] = "# of blue chairs";
        SerialPuts(C2);
    } else if(clueNum == 3)
    {
        char C3[100] = "int a = 4; a = 3; std::cout<<a;";
        SerialPuts(C3);
    } else if(clueNum == 4)
    {
        char C4[100] = "# of working lightbulbs";
        SerialPuts(C4);
    } else if(clueNum == 5)
    {
        char C5[100] = "How many vertices are in the shape of the nano tile?";
        SerialPuts(C5);
    } else if(clueNum == 6)
    {
        char C6[100] = "I can grow on trees";
        SerialPuts(C6);
    } else if(clueNum == 7)
    {
        char C7[100] = "You can't walk up me";
        SerialPuts(C7);
    } else if(clueNum == 8)
    {
        char C8[100] = "Sugar cube";
        SerialPuts(C8);
    } else if(clueNum == 9)
    {
        char C9[100] = "How many countries do you see?";
        SerialPuts(C9);
    } else if(clueNum == 10)
    {
        char C10[100] = "Protect your head with me";
        SerialPuts(C10);
    } else if(clueNum == 11)
    {
        char C11[100] = "I sound like a car";
        SerialPuts(C11);
    } else if(clueNum == 12)
    {
        char C12[100] = "I'm smarted than you without a brain";
        SerialPuts(C12);
    } else if(clueNum == 13)
    {
        char C13[100] = "___ crash";
        SerialPuts(C13);
    } else if(clueNum == 14)
    {
        char C14[100] = "I am you in the future";
        SerialPuts(C14);
    } else if(clueNum == 15)
    {
        char C15[100] = "Red set of paper";
        SerialPuts(C15);
    } else if(clueNum == 16)
    {
        char C16[100] = "I keep you awake";
        SerialPuts(C16);
    } else if(clueNum == 17)
    {
        char C17[100] = "We move you without you moving";
        SerialPuts(C17);
    } else if(clueNum == 18)
    {
        char C18[100] = "You climb me to achieve goals";
        SerialPuts(C18);
    } else
    {
        char C19[100] = "Where engineers live";
        SerialPuts(C19);
    } // end if statements
}