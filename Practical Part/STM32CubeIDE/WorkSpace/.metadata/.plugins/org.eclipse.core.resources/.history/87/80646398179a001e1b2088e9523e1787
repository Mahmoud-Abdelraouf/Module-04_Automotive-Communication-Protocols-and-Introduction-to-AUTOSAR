/**
 ********************************************************************************************************************************** 
 * @file SPI_program.c
 * @brief Implementation file for the SPI driver.
 * 
 * This file contains the implementation of the functions for the SPI driver.
 ********************************************************************************************************************************** 
 * @author Mahmoud Abdelraouf Mahmoud
 * @date 10 Dec 2023
 * @version V01
 * 
 * @copyright Copyright (c) 2023
 ********************************************************************************************************************************** 
 */

/*****************************< LIB *****************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/*****************************< MCAL *****************************/
/**< GPIO */
#include "GPIO_interface.h"
/**< MCAL_SPI */
#include "SPI_interface.h"
#include "SPI_private.h"
#include "SPI_config.h"

/**
 * @addtogroup SPI_Functions
 * @{
 */

void SPI_Init(const SPI_config_t *Copy_SPIConfig)
{
  /********************************< Configure the SPI peripheral ********************************/
  if(Copy_SPIConfig == NULL)
  {
    SPI_DefaultInitiation();
  }
  else
  {
    /**< Set the data frame format */
    if (Copy_SPIConfig->DataFrame == SPI_DATA_FRAME_8BIT)
    {
      CLR_BIT(SPI->CR1, SPI_CR1_DFF);
    }
    else if (Copy_SPIConfig->DataFrame == SPI_DATA_FRAME_16BIT)
    {
	    SET_BIT(SPI->CR1, SPI_CR1_DFF);
    }
    else
    {
      return;
    }

    /**< Set the frame format */
    if(Copy_SPIConfig->FrameFormat == SPI_LSB_FIRST)
    {
      SET_BIT(SPI->CR1,SPI_CR1_LSBFIRST);
    }
    else if(Copy_SPIConfig->FrameFormat == SPI_MSB_FIRST)
    {
      CLR_BIT(SPI->CR1,SPI_CR1_LSBFIRST);
    }
    else
    {
      return;
    }

    /**< Set the clock polarity */
    if (Copy_SPIConfig->ClockPolarity == SPI_IDLE_HIGH)
    {
      SET_BIT(SPI->CR1, SPI_CR1_CPOL);
    }
    else if(Copy_SPIConfig->ClockPolarity == SPI_IDLE_LOW)
    {
	    CLR_BIT(SPI->CR1, SPI_CR1_CPOL);
    }
    else
    {
      return;
    }

    /**< Set the clock phase */
    if (Copy_SPIConfig->ClockPhase == SPI_WRITE_READ)
    {
      SET_BIT(SPI->CR1, SPI_CR1_CPHA);
    }
    else if(Copy_SPIConfig->ClockPhase == SPI_READ_WRITE)
    {
      CLR_BIT(SPI->CR1, SPI_CR1_CPHA);
    }
    else
    {
      return;
    }

    /**< Set the clock speed */
    SPI->CR1 &= ~SPI_CR1_BR_MSK;
    SPI->CR1 |= Copy_SPIConfig->BaudRateDIV;

    #if SPI_MODE == SPI_MASTER_MODE
      /**< Config the SPI to mater mode */
      SET_BIT(SPI->CR1, SPI_CR1_SSM);   /**< Set the SSM to manage the slave bit by software */
      SET_BIT(SPI->CR1, SPI_CR1_SSI);   /**< Set the SSI to work in the Master mode */

      /**< Set the master mode */
      SET_BIT(SPI->CR1, SPI_CR1_MSTR);

      #elif SPI_MODE == SPI_SLAVE_MODE
        /**< Config the SPI to slave mode */
        CLR_BIT(SPI->CR1, SPI_CR1_SSM);   /* Clear the SSM to manage the slave bit by hardware */

        /**< Set the slave mode */
        CLR_BIT(SPI->CR1, SPI_CR1_MSTR);  /* Clear the MSTR to select the spi to work in slave mode */
      #else
        #error "ERROR!! Wrong choice"
    #endif

    /**< Enable the SPI peripheral */
    SET_BIT(SPI->CR1, SPI_CR1_SPE);

  }
}

void SPI_Transfer(u8 *Copy_TxData, u8 *Copy_RxData, u16 Copy_Size)
{

  /**< Iterator to loop on the data */
  u16 Local_Iterator;

  #if SPI_MODE == SPI_MASTER_MODE
    /**< Clear the slave select pin -> Enable the slave select pin */
    MCAL_GPIO_SetPinValue(GPIO_PORTA, GPIO_PIN4, GPIO_LOW);
  #endif

  /**< Send and receive the data */
  for (Local_Iterator = 0; Local_Iterator < Copy_Size; Local_Iterator++)
  {
    /**< Send the data */
    SPI_SendByte(Copy_TxData[Local_Iterator]);

    /**< Receive the data */
    Copy_RxData[Local_Iterator] = SPI_ReceiveByte();
  }

  /* Wait for the transmission to complete */
  SPI_WaitForTransmissionComplete();

  #if SPI_MODE == SPI_MASTER_MODE
    /* Set the slave select pin -> Disable the slave select pin */
    MCAL_GPIO_SetPinValue(GPIO_PORTA, GPIO_PIN4, GPIO_HIGH);
  #endif

}

/**
 * @} SPI_Functions
 */

/**
 * @addtogroup SPI_Private_Functions
 * @{
 */

static void SPI_SendByte(u8 Copy_Data)
{
  /* Wait for the transmit buffer to be empty */
  while (!GET_BIT(SPI->SR, SPI_SR_TXE));

  /* Send the data */
  *((u8*)&(SPI->DR)) = Copy_Data;
}

static u8 SPI_ReceiveByte(void)
{
  /* Wait for the receive buffer to be full */
  while (!GET_BIT(SPI->SR, SPI_SR_RXNE));

  /* Return the received data */
  return *((u8*)&(SPI->DR));
}

static void SPI_WaitForTransmissionComplete(void)
{
  /* Wait for the transmission to complete */
  while (GET_BIT(SPI->SR, SPI_SR_BSY));
}

static void SPI_DefaultInitiation(void)
{ 
  /**< Set the data frame format to be 8-bit data frame */
  CLR_BIT(SPI_Default->CR1, SPI_CR1_DFF);

  /**< Set the frame format to be the LSB first */
  SET_BIT(SPI_Default->CR1, SPI_CR1_LSBFIRST);

  /**< Set the clock polarity to be clock polarity high at idle state */
  SET_BIT(SPI_Default->CR1, SPI_CR1_CPOL);

  /* Set the clock phase to be write the read */
  SET_BIT(SPI_Default->CR1, SPI_CR1_CPHA);

  /* Set the clock speed to be divided by two */
  SPI_Default->CR1 &= ~SPI_CR1_BR_MSK;
  SPI_Default->CR1 |= SPI_BAUD_RATE_DIV2;

  #if SPI_MODE == SPI_MASTER_MODE
    /* Config the SPI to mater mode */
    SET_BIT(SPI_Default->CR1, SPI_CR1_SSM);   /* Set the SSM to manage the slave bit by software */
    SET_BIT(SPI_Default->CR1, SPI_CR1_SSI);   /* Set the SSI to work in the Master mode */

    /* Set the master mode */
    SET_BIT(SPI_Default->CR1, SPI_CR1_MSTR);

    #elif SPI_MODE == SPI_SLAVE_MODE
      /* Config the SPI to slave mode */
      CLR_BIT(SPI_Default->CR1, SPI_CR1_SSM);   /* Clear the SSM to manage the slave bit by hardware */

      /* Set the slave mode */
      CLR_BIT(SPI_Default->CR1, SPI_CR1_MSTR);  /* Clear the MSTR to select the spi to work in slave mode */
    #else
      #error "ERROR!! Wrong choice"  
  #endif

  /* Enable the SPI peripheral */
  SET_BIT(SPI_Default->CR1, SPI_CR1_SPE);
}

/**
 * @}
 */

