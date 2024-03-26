/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 18 Mar 2024                *****************/
/******* Version   : 1.0.0                      *****************/
/******* File Name : KPD_config.h               *****************/
/****************************************************************/
#ifndef KPD_CONFIG_H_
#define KPD_CONFIG_H_

/**
 * @brief Available ports for the rows and columns of the keypad.
 * Users can choose from DIO_PORTA to DIO_PORTD.
 */
#define KPD_ROWS_PORT           DIO_PORTB

/**
 * @brief Available ports for the rows and columns of the keypad.
 * Users can choose from DIO_PORTA to DIO_PORTD.
 */
#define KPD_COLS_PORT           DIO_PORTD

/**
 * @brief Pins for the rows of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_R1_PIN              DIO_PIN4

/**
 * @brief Pins for the rows of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_R2_PIN              DIO_PIN5

/**
 * @brief Pins for the rows of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_R3_PIN              DIO_PIN6

/**
 * @brief Pins for the rows of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_R4_PIN              DIO_PIN7

/**
 * @brief Pins for the columns of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_C1_PIN              DIO_PIN2

/**
 * @brief Pins for the columns of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_C2_PIN              DIO_PIN3

/**
 * @brief Pins for the columns of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_C3_PIN              DIO_PIN4

/**
 * @brief Pins for the columns of the keypad.
 * Users can choose pins from 0 to 7.
 */
#define KPD_C4_PIN              DIO_PIN5

/**
 * @brief Definition of the keypad keys.
 * Users can configure the keyboard layout using this definition.
 */
#define KPD_KEYS                {                       \
                                    {'1','2','3','+'},  \
                                    {'4','5','6','-'},  \
                                    {'7','8','9','*'},  \
                                    {'0','=','/','%'}   \
                                }

#endif /**< KPD_CONFIG_H_ */
