#include "application.h"
#include "MCAL_Layer/I2C/hal_i2c.h"
Std_ReturnType ret = E_NOT_OK;

static volatile uint8 i2c_slave1_rec_data;
static volatile uint8 slave_flag = 0, ack;
led_t led1 = {.port_name = PORTD_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};


void MSSP_I2C_DefaultInterruptHandler(void)
{
    /* Holds clock low (clock stretch) */
    I2C_CLOCK_STRETCH_ENABLE();
    if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0))
    { /* Master Will Write Data, So i need to read it */
        uint8 dummy_buffer = SSPBUF; /* Read The Last Byte To Clear The Buffer */
        while(!SSPSTATbits.BF);
        i2c_slave1_rec_data = SSPBUF; /* read data */
    }
    else if(SSPSTATbits.R_nW == 1)
    { /* Master needs to read Data, So i need to write it */
        uint8 dummy_buffer = SSPBUF; /* Read The Last Byte To Clear The Buffer */
        if(0 == slave_flag)
        {
            SSPBUF = 0x55; /* Write Data */
            slave_flag = 1;
        }
        else if(1 == slave_flag)
        {
            SSPBUF = 0x66; /* Write Data */
            slave_flag = 0;
        }
        else{}
    }
    else { /* Nothing */ }
    /* Releases clock */
    I2C_CLOCK_STRETCH_DISABLE();
}


mssp_i2c_t i2c_obj =
{
    .I2C_DefaultInterruptHandler = MSSP_I2C_DefaultInterruptHandler,
    .i2c_cfg.i2c_SMBus_control = I2C_SMBus_DISABLE,
    .i2c_cfg.i2c_general_call = I2C_GENERAL_CALL_DISABLE,
    .i2c_cfg.i2c_mode = I2C_MSSP_SLAVE_MODE,
    .i2c_cfg.i2c_mode_cfg = I2C_SLAVE_MODE_7BIT_ADDRESS,
    .i2c_cfg.i2c_slave_address = 0x62,
    .i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE
};


int main() 
{   
    ret = MSSP_I2C_Init(&i2c_obj);
    ret = led_initialize(&led1);
    while(1)
    {
        if('b' == i2c_slave1_rec_data)
        {
            led_turn_on(&led1);
        }
        else if('d' == i2c_slave1_rec_data)
        {
            led_turn_off(&led1); 
        }
    }
    return (EXIT_SUCCESS);
}
