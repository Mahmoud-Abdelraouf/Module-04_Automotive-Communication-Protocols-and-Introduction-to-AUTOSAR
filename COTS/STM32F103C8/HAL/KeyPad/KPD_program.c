/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 18 Mar 2024                *****************/
/******* Version   : 1.0.0                      *****************/
/******* File Name : KPD_program.c              *****************/
/****************************************************************/

/*****************************< LIB *****************************/
#include "STD_TYPES.h"
#include "util/delay.h"
/*****************************< MCAL *****************************/
#include "DIO_interface.h"
/*****************************< HAL *****************************/
#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"

/**
 * @brief Matrix representing the keypad keys.
 * Each row corresponds to a row of the physical keypad, and each column corresponds to a column.
 * Users can initialize this matrix with the desired keypad layout using KPD_KEYS.
 */
const u8 KPD_Keys[4][4] = KPD_KEYS;

/**
 * @brief Array representing the pins connected to the rows of the keypad.
 * Users need to specify the corresponding pins in the order of physical connection.
 */
const u8 KPD_rowsPins[4] = {KPD_R1_PIN, KPD_R2_PIN, KPD_R3_PIN, KPD_R4_PIN};

/**
 * @brief Array representing the pins connected to the columns of the keypad.
 * Users need to specify the corresponding pins in the order of physical connection.
 */
const u8 KPD_colsPins[4] = {KPD_C1_PIN, KPD_C2_PIN, KPD_C3_PIN, KPD_C4_PIN};
/*****************************< Function Implementations *****************************/
Std_ReturnType KPD_GetKeyState(u8 *returnedKey) 
{
    Std_ReturnType FunctionState = E_NOT_OK; /**< Initialize function state to "not OK" */
    u8 rowsCounter = 0, colsCounter = 0, pinValue = 0, flag = 0; /**< Initialize loop counters and pin value */
    
    if (NULL != returnedKey) /**< Check if the returnedKey pointer is not NULL */
    {   
        *returnedKey = KPD_KEY_NOT_PRESSED; /**< Set the returnedKey to indicate no key is pressed */
        
        /**< Active Each Row => For loop on pins of the rows */
        for (rowsCounter = 0; rowsCounter < 4; rowsCounter++) /**< Loop through each row */
        {
            DIO_SetPinValue(KPD_ROWS_PORT, KPD_rowsPins[rowsCounter], DIO_LOW); /**< Activate the current row */
            
            /**< Check which input pin has a low value (i.e., which key is pressed) */
            for (colsCounter = 0; colsCounter < 4; colsCounter++) /**< Loop through each column */
            {
                DIO_GetPinValue(KPD_COLS_PORT, KPD_colsPins[colsCounter], &pinValue); /**< Read the value of the current column pin */
                if (pinValue == DIO_LOW) /**< Check if the pin value is low */
                {
                    /**< Debouncing */
                    _delay_ms(20); /**< Delay for debouncing */
                    DIO_GetPinValue(KPD_COLS_PORT, KPD_colsPins[colsCounter], &pinValue); /**< Get pin value again */
                    /**< check if the pin is still equal low */
                    while (pinValue == DIO_LOW) /**< Wait until the pin value becomes high (debounced) */
                    {
                        DIO_GetPinValue(KPD_COLS_PORT, KPD_colsPins[colsCounter], &pinValue); /**< Get pin value */
                    }
                    *returnedKey = KPD_Keys[rowsCounter][colsCounter]; /**< Store the pressed key */
                    flag = 1; /**< Set flag to indicate that a key is pressed */
                    break; /**< Exit the loop */
                }
            }
            
            if (flag == 1) /**< Check if a key is pressed */
            {
            	/**< Deactivate Rows */
				DIO_SetPinValue(KPD_ROWS_PORT, KPD_rowsPins[rowsCounter], DIO_HIGH); /**< Deactivate the current row */

            	FunctionState = E_OK;
                break; /**< Exit the loop */
            }
        }
    }
    else
    {
        FunctionState = E_NOT_OK; /**< Set function state to "not OK" if returnedKey pointer is NULL */
    }
    
    return FunctionState; /**< Return the function state */
}
