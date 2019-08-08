#define MPU6000_C

/****************** Includes ********************/

#include "mpu6000.h"

#include <stdbool.h>
#include <string.h>

/******************* Defines ********************/

#define REG_GYRO_CFG    0x1B // Register - GYRO_CONFIG   - Gyroscope Configuration
#define REG_ACC_CFG     0x1C // Register - ACCEL_CONFIG  - Accelerometer Configuration
#define REG_INT_PIN_CFG 0x37 // Register - INT_PIN_CFG   - INT Pin / Bypass Enable Configuration
#define REG_INT_ENABLE  0x38 // Register - INT_ENABLE    - Interrupt Enable
#define REG_INT_STATUS  0x3A // Register - INT_STATUS    - Interrupt Status
#define REG_ACCEL_XOUT  0x3B // Register - ACCEL_XOUT    - Accelerometer Measurements X axis
#define REG_TEMP_OUT    0x41 // Register - TEMP_OUT      - Temperature Measurements
#define REG_GYRO_XOUT   0x43 // Register - GYRO_XOUT     - Gyro Measurements X axis
#define REG_PWR_MGMT_1  0x6B // Register - PWR_MGMT_1    - Power Management 1
#define REG_WHO_AM_I    0x75 // Register - WHO_AM_I      - Device Identification

#define EARTH_G 9.81274     // Gravitational acceleration constant g [m/s²]

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

// HAL
extern void iic_mpu6000_send( uint8_t *bufOut_pu8, uint8_t ByteCnt_u8 ); // Send data via I²C
extern void iic_mpu6000_recv( uint8_t *bufIn_pu8 , uint8_t ByteCnt_u8 ); // Receive data via I²C

// Register Access (polled)
static uint8_t mpu6000_readReg ( uint8_t addr_u8 );
static void    mpu6000_writeReg( uint8_t addr_u8, uint8_t data_u8 );

/**************** Implementation ****************/

// Initialize MPU-6000 chip
uint8_t mpu6000_init( void )
{
  // Initialize privates
  memset( &mpu6000_s.prv_s, 0, sizeof(mpu6000_s.prv_s) );

  // Initialize outputs
  memset( &mpu6000_s.outp_s, 0, sizeof(mpu6000_s.outp_s) );

  // Test "who am I" register
  uint8_t reg_u8 = mpu6000_readReg( 0x75 );
  if( reg_u8 != 0x68 )
  {
    // No connection to IMU
    return 1;
  }

  // Set clock source - PLL with X axis gyro reference - CLKSEL = 1
  // Wake-up device - SLEEP = 0
  mpu6000_writeReg( REG_PWR_MGMT_1, 0x01 );

  // Configure to 250 °/s gyro resolution
  mpu6000_writeReg( REG_GYRO_CFG, 0x00 );
  mpu6000_s.prv_s.currRate_p1degps_s16 = 2500;

  // Done
  return 0;
}

// Get measurement from sensor
void mpu6000( void )
{
  // Set address
  mpu6000_s.prv_s.buf_au8[0] = REG_GYRO_XOUT; // 7-bit Register Address
  iic_mpu6000_send( mpu6000_s.prv_s.buf_au8, 1 );

  // Read 6 Bytes gyro data from device
  iic_mpu6000_recv( mpu6000_s.prv_s.buf_au8, 6 );

  // Extract RAW data from buffer
  int16_t rotX_s16 = ((int16_t)mpu6000_s.prv_s.buf_au8[0] << 8) | mpu6000_s.prv_s.buf_au8[1];
  int16_t rotY_s16 = ((int16_t)mpu6000_s.prv_s.buf_au8[2] << 8) | mpu6000_s.prv_s.buf_au8[3];
  int16_t rotZ_s16 = ((int16_t)mpu6000_s.prv_s.buf_au8[4] << 8) | mpu6000_s.prv_s.buf_au8[5];

  // Convert RAW rotation measurement to physical [0.1 °/s]
  mpu6000_s.outp_s.rotX_p1degps_s16 = (int16_t)((double)rotX_s16 * mpu6000_s.prv_s.currRate_p1degps_s16 / 3276.70);
  mpu6000_s.outp_s.rotY_p1degps_s16 = (int16_t)((double)rotY_s16 * mpu6000_s.prv_s.currRate_p1degps_s16 / 3276.70);
  mpu6000_s.outp_s.rotZ_p1degps_s16 = (int16_t)((double)rotZ_s16 * mpu6000_s.prv_s.currRate_p1degps_s16 / 3276.70);
}

// Read register of MPU-6000 (polled)
static uint8_t mpu6000_readReg( uint8_t addr_u8 )
{
  // Set address
  mpu6000_s.prv_s.buf_au8[0] = addr_u8 & 0x7F; // 7-bit Register Address
  iic_mpu6000_send( mpu6000_s.prv_s.buf_au8, 1 );

  // Read 1-byte register data
  iic_mpu6000_recv( mpu6000_s.prv_s.buf_au8, 1 );

  // Return data
  return mpu6000_s.prv_s.buf_au8[0];
}

// Write register of MPU-6000 (polled)
static void mpu6000_writeReg( uint8_t addr_u8, uint8_t data_u8 )
{
  // Prepare buffer
  mpu6000_s.prv_s.buf_au8[0]  = addr_u8 & 0x7F; // 7-bit Register Address
  mpu6000_s.prv_s.buf_au8[1]  = data_u8;        // Value

  // Send data
  iic_mpu6000_send( mpu6000_s.prv_s.buf_au8, 2 );
}

// EOF
