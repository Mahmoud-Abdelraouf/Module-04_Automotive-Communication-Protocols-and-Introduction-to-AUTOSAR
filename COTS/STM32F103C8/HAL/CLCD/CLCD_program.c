/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 16 Sep 2023                *****************/
/******* Version   : 0.1                        *****************/
/******* File Name : CLCD_program.c             *****************/
/****************************************************************/

/*****************************< LIB *****************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
/*****************************< MCAL *****************************/
#include "GPIO_interface.h"
#include "STK_interface.h"
/*****************************< HAL *****************************/
#include "CLCD_private.h"
#include "CLCD_interface.h"
#include "CLCD_config.h"
/*****************************< Function Implementations *****************************/
void LCD_Init(const LCD_Config_t *config) 
{
    if(config == NULL)
    {
        return;
    }

    if(config->mode == LCD_4BitMode)
    {
        for(uint8_t i = 0; i < 4; i++)
        {
            MCAL_GPIO_SetPinMode(config->dataPins[i].LCD_PortId, config->dataPins[i].LCD_PinId, GPIO_OUTPUT_PUSH_PULL_2MHZ);
        }

        MCAL_STK_SetDelay_ms(1000);

        LCD_SendCommand(config, LCD_8BIT_MODE_COMMAND);
        LCD_SendCommand(config, LCD_CLEAR_COMMAND);
        LCD_SendCommand(config, LCD_DISPLAY_ON_COMMAND);
    }
    else if(LCD_8BitMode)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            MCAL_GPIO_SetPinMode(config->dataPins[i].LCD_PortId, config->dataPins[i].LCD_PinId, GPIO_OUTPUT_PUSH_PULL_2MHZ);
        }

        MCAL_STK_SetDelay_ms(1000);

        LCD_SendCommand(config, LCD_4BIT_MODE_COMMAND_1);
        LCD_SendCommand(config, LCD_4BIT_MODE_COMMAND_2);
        LCD_SendCommand(config, LCD_4BIT_MODE_COMMAND_3);
        LCD_SendCommand(config, LCD_CLEAR_COMMAND);
        LCD_SendCommand(config, LCD_DISPLAY_ON_COMMAND);
    }
    else
    {
        return;
    }
}

void LCD_SendCommand(const LCD_Config_t *config, uint8_t command) 
{
    /**< Set RS pin to low for command --> RS = 0 */
    MCAL_GPIO_SetPinValue(config->rsPin.LCD_PortId, config->rsPin.LCD_PinId, GPIO_LOW);
    /**< Set RW pin to low for write  --> RW = 0 */
    MCAL_GPIO_SetPinValue(config->rwPin.LCD_PortId, config->rwPin.LCD_PinId, GPIO_LOW);

    if(config->mode == LCD_4BitMode)
    {
        LCD_Send4Bits(config, command);
    }
    else if(config->mode == LCD_8BitMode)
    {
        LCD_Send8Bits(config, command);
    }
    else
    {
        return;
    }
}

void LCD_SendChar(const LCD_Config_t *config, uint8_t character) 
{
    /**< Set RS pin to low for data --> RS = 1 */
    MCAL_GPIO_SetPinValue(config->rsPin.LCD_PortId, config->rsPin.LCD_PinId, GPIO_LOW);
    /**< Set RW pin to low for write  --> RW = 0 */
    MCAL_GPIO_SetPinValue(config->rwPin.LCD_PortId, config->rwPin.LCD_PinId, GPIO_LOW);

    if(config->mode == LCD_4BitMode)
    {
        LCD_Send4Bits(config, character);
    }
    else if(config->mode == LCD_8BitMode)
    {
        LCD_Send8Bits(config, character);
    }
    else
    {
        return;
    }
}

void LCD_SendString(const LCD_Config_t *config, const uint8_t *string) 
{
    uint8_t Local_Counter = 0;

    while(string[Local_Counter] != '\0')
    {
        LCD_SendChar(config, string[Local_Counter]);
        Local_Counter++;
    }
}

void LCD_SendIntegerPart(const LCD_Config_t *config, s32 number) {
    u8 Local_Integer[11] = {0};
    s8 Local_Counter = 0;

    /**< Handle negative numbers */ 
    if (number < 0) {
        LCD_SendChar(config, '-');
        number *= -1;
    }

    /**< Extract digits of the integer part */ 
    do {
        Local_Integer[Local_Counter] = number % 10;
        number /= 10;
        Local_Counter++;
    } while (number != 0);

    Local_Counter--;

    /**< Display each digit */ 
    for (; Local_Counter >= 0; Local_Counter--) {
        LCD_SendChar(config, Local_Integer[Local_Counter] + '0');
    }
}

void LCD_SendNumber(const LCD_Config_t *config, double number) {
    /**< Display integer part */ 
    u32 integerPart = (u32)number;

    LCD_SendIntegerPart(config, (s32)integerPart); // Function to handle integer part

    /**< Display decimal point */ 
    LCD_SendChar(config, '.');

    /**< Display fractional part with 3 decimal points */ 
    double fractionalPart = number - integerPart;
    u8 decimalPlaces = 3;

    for (u8 i = 0; i < decimalPlaces; ++i) {
        fractionalPart *= 10;
        u8 digit = (u8)fractionalPart;
        LCD_SendChar(config, digit + '0');
        fractionalPart -= digit;
    }
}

void LCD_Clear(const LCD_Config_t *config) 
{
    LCD_SendCommand(config, LCD_8BIT_MODE_COMMAND);
}

void LCD_GoToXYPos(const LCD_Config_t *config, uint8_t x, uint8_t y) {
    /**< Check if the coordinates are within bounds */ 
    if ((x < 2) && (y >= 0 && y <= 15)) {
        u8 localAddress = 0;

        switch (x) {
            case 0:
                localAddress = y;
                break;
            case 1:
                localAddress = y + DDRAM_ADDRESS_OFFSET;
                break;
            default:
                break;
        }

        // Calculate the final address to move the cursor
        u8 command = localAddress | (1 << 7); // Or simply: localAddress + 0x80;
        LCD_voidSendCommand(config, command);
    }
    else
    {
        return;
    }
}

/**< Private helper function to send 4 bits */ 
static void LCD_Send4Bits(const LCD_Config_t *config, uint8_t value) 
{
    /**< Send the 4-MSB */
    for(uint8_t i = 0; i < 4; i++)
    {
        MCAL_GPIO_SetPinValue(config->dataPins[i].LCD_PinId, config->dataPins[i].LCD_PinId, value >> (4 + i) & 0x01);
    }

    /**< Set the enable pin to high */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_HIGH);
    /**< Set the Pulse time to be 5msec */
    MCAL_STK_SetDelay_ms(5);
    /**< Set the enable pin to low */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_LOW);

    /**< Shift the 4-LSB command to the 4-MSB */
    value <<= 4;

    /**< Send the 4-LSB */
    for(uint8_t i = 0; i < 4; i++)
    {
        MCAL_GPIO_SetPinValue(config->dataPins[i].LCD_PinId, config->dataPins[i].LCD_PinId, value >> (4 + i) & 0x01);
    }

    /**< Set the enable pin to high */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_HIGH);
    /**< Set the Pulse time to be 5msec */
    MCAL_STK_SetDelay_ms(5);
    /**< Set the enable pin to low */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_LOW);
}

/**< Private helper function to send 8 bits */ 
static void LCD_Send8Bits(const LCD_Config_t *config, uint8_t value) 
{
     /**< Send the 8-Bit */
    for(uint8_t i = 0; i < 8; i++)
    {
        MCAL_GPIO_SetPinValue(config->dataPins[i].LCD_PinId, config->dataPins[i].LCD_PinId, value >> i & 0x01);
    }

    /**< Set the enable pin to high */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_HIGH);
    /**< Set the Pulse time to be 5msec */
    MCAL_STK_SetDelay_ms(5);
    /**< Set the enable pin to low */
    MCAL_GPIO_SetPinValue(config->enablePin.LCD_PortId, config->enablePin.LCD_PinId, GPIO_LOW);
}
