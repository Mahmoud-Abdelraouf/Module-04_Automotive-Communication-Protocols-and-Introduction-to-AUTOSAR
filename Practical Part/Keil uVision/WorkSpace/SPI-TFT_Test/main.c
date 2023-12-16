/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 13 Dec 2023                *****************/
/******* Version   : 0.1                        *****************/
/******* File Name : main.c                     *****************/
/****************************************************************/

/****************************< LIB ****************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
/****************************< MCAL ****************************/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "SPI_interface.h"
/****************************< HAL ****************************/
#include "TFT_interface.h"
/****************************< APP ****************************/
//#include "image.h" 

int main(void)
{
	
	/**< init RCC */
	MCAL_RCC_InitSysClock();
	/**< init GPIO */
	MCAL_RCC_EnablePeripheral(RCC_APB2, RCC_APB2ENR_AFIOEN);
	MCAL_RCC_EnablePeripheral(RCC_APB2, RCC_APB2ENR_IOPAEN);
	MCAL_RCC_EnablePeripheral(RCC_APB2, RCC_APB2ENR_SPI1EN);
	
	/**< config spi 1 */
	SPI_Config_t spi_1;
	
	spi_1.BaudRateDIV = SPI_BAUD_RATE_DIV2;
	spi_1.ClockPolarity = SPI_IDLE_HIGH;
	spi_1.ClockPhase = SPI_WRITE_READ;
	spi_1.DataFrame = SPI_DATA_FRAME_8BIT;
	spi_1.FrameFormat = SPI_MSB_FIRST;
	
	SPI_Init(&spi_1);
	
	
	/**< config tft */
	TFT_Config_t tft_1;
	
	/**< Data/Command config ==> gpio */
	tft_1.TFT_DCPin.TFT_Port = GPIO_PORTA;
	tft_1.TFT_DCPin.TFT_Pin = GPIO_PIN1;
	/**< Reset pin ==> gpio */
	tft_1.TFT_RESPin.TFT_Port = GPIO_PORTA;
	tft_1.TFT_RESPin.TFT_Pin = GPIO_PIN2;

	/**< set gpio mode */
	MCAL_GPIO_SetPinMode(tft_1.TFT_DCPin.TFT_Port, tft_1.TFT_DCPin.TFT_Pin, GPIO_OUTPUT_PUSH_PULL_10MHZ);

	MCAL_GPIO_SetPinMode(tft_1.TFT_RESPin.TFT_Port, tft_1.TFT_RESPin.TFT_Pin, GPIO_OUTPUT_PUSH_PULL_10MHZ);

	MCAL_GPIO_SetPinMode(GPIO_PORTA, GPIO_PIN7, GPIO_OUTPUT_AF_PUSH_PULL_10MHZ);

	MCAL_GPIO_SetPinMode(GPIO_PORTA, GPIO_PIN5, GPIO_OUTPUT_AF_PUSH_PULL_10MHZ);

	
	TFT_Init(&tft_1);
	
	
	//TFT_DisplayImage(&tft_1, image);
	

	for(;;);
	
	
}


