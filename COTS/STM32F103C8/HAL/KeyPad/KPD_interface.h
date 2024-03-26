/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 18 Mar 2024                *****************/
/******* Version   : 1.0.0                      *****************/
/******* File Name : KPD_interface.h            *****************/
/****************************************************************/
#ifndef KPD_INTERFACE_H_
#define KPD_INTERFACE_H_

/**
 * @brief Constant value representing that no key is pressed on the keypad.
 */
#define KPD_KEY_NOT_PRESSED        0xFF

/**
 * @brief Get the state of the pressed key on the keypad.
 * 
 * @param[out] returnedKey Pointer to store the pressed key.
 *                         If no key is pressed, it will store KPD_KEY_NOT_PRESSED.
 * @return Std_ReturnType Standard return type indicating function execution status:
 *                         - E_OK: Success
 *                         - E_NOT_OK: Failure
 */
Std_ReturnType KPD_GetKeyState(u8 *returnedKey);


#endif /**< KPD_INTERFACE_H_ */