/* 
 * File:   application.c
 * Author: Mohamed_Nagy
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on January 27, 2023, 8:41 PM
 */



#include "application.h"
#include "MCAL_Layer/I2C/hal_i2c.h"

void MSSP_I2C_Send_1_Byte(uint8 slave_add, uint8 _data);

#define SLAVE_1     0x60
#define SLAVE_2     0x62
Std_ReturnType ret = E_NOT_OK;

uint8 slave_ack, master_rec_data, counter;

mssp_i2c_t i2c_obj =
{
    .i2c_clock = 100000,
    .i2c_cfg.i2c_mode = I2C_MSSP_MASTER_MODE,
    .i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK,
    .i2c_cfg.i2c_SMBus_control = I2C_SMBus_DISABLE,
    .i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE,
    .I2C_DefaultInterruptHandler = NULL,
    .I2C_Report_Receive_Overflow = NULL,
    .I2C_Report_Write_Collision = NULL
};

int main() 
{   
    ret = MSSP_I2C_Init(&i2c_obj);
    while(1)
    {
        MSSP_I2C_Send_1_Byte(SLAVE_1, 'a');
        __delay_ms(500);
        MSSP_I2C_Send_1_Byte(SLAVE_2, 'b');
        __delay_ms(500);
        MSSP_I2C_Send_1_Byte(SLAVE_1, 'c');
        __delay_ms(500);
        MSSP_I2C_Send_1_Byte(SLAVE_2, 'd');
        __delay_ms(500);
    }
    return (EXIT_SUCCESS);
}


void MSSP_I2C_Send_1_Byte(uint8 slave_add, uint8 _data)
{
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, slave_add, &slave_ack);
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, _data, &slave_ack);
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
}
