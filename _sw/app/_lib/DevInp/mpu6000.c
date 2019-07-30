#define MPU6000_C

/****************** Includes ********************/

#include "mpu6000.h"

#include <string.h>

/******************* Defines ********************/

#define REG_GYRO_CFG   0x1B // Register - GYRO_CONFIG   - Gyroscope Configuration
#define REG_ACC_CFG    0x1C // Register - ACCEL_CONFIG  - Accelerometer Configuration
#define REG_ACCEL_XOUT 0x3B // Register - ACCEL_XOUT    - Accelerometer Measurements X axis
#define REG_PWR_MGMT_1 0x6B // Register - PWR_MGMT_1    - Power Management 1
#define REG_WHO_AM_I   0x75 // Register - WHO_AM_I      - Device Identification

#define EARTH_G 9.81274     // Gravitational acceleration constant g [m/s²]

/******************** Types *********************/

typedef struct mpu6000_SensorRaw_s
{
  // 3-Axis acceleration
  int16_t accX_s16;
  int16_t accY_s16;
  int16_t accZ_s16;

  // 3-Axis rotation
  int16_t rotX_s16;
  int16_t rotY_s16;
  int16_t rotZ_s16;

  // Temperature
  int16_t temp_s16;

} mpu6000_SensorRaw_ts;

/***************** Private Data *****************/

/****************** Prototypes ******************/

extern void spi_mpu6000_transferData( uint8_t * bufOut_au8, uint8_t *bufIn_au8, uint8_t ByteCnt_u8 );

static void mpu6000_readSensorRaw( mpu6000_SensorRaw_ts *data_ps );
static uint8_t mpu6000_spiReadReg( uint8_t addr_u8 );
static void mpu6000_spiWriteReg( uint8_t addr_u8, uint8_t data_u8 );

/**************** Implementation ****************/

// Initialize MPU-6000 chip
uint8_t mpu6000_init( void )
{
  // Initialize privates
  memset( &mpu6000_s.prv_s, 0, sizeof(mpu6000_s.prv_s) );

  // Initialize outputs
  memset( &mpu6000_s.outp_s, 0, sizeof(mpu6000_s.outp_s) );

  // Test "who am I" register
  uint8_t reg_u8 = mpu6000_spiReadReg( 0x75 );
  if( reg_u8 != 0x68 )
  {
    // No connection to IMU
    return 1;
  }

  // Set clock source - PLL with X axis gyro reference - CLKSEL = 1
  // Wake-up device - SLEEP = 0
  mpu6000_spiWriteReg( REG_PWR_MGMT_1, 0x01 );

  // Configure to 250 °/s gyro resolution
  mpu6000_spiWriteReg( REG_GYRO_CFG, 0x00 );
  mpu6000_s.prv_s.currRate_degps_s16 = 250;

  // Done
  return 0;
}

// Get current measurement from sensor
void mpu6000( void )
{
  mpu6000_SensorRaw_ts raw_s;

  // Get sensor raw data
  mpu6000_readSensorRaw( &raw_s );

  // Convert RAW acceleration measurement to physical [cm/s²]
  mpu6000_s.outp_s.accX_cmps2_s16 = (int16_t)((double)raw_s.accX_s16 / 327.670 * 2.0 * EARTH_G);
  mpu6000_s.outp_s.accY_cmps2_s16 = (int16_t)((double)raw_s.accY_s16 / 327.670 * 2.0 * EARTH_G);
  mpu6000_s.outp_s.accZ_cmps2_s16 = (int16_t)((double)raw_s.accZ_s16 / 327.670 * 2.0 * EARTH_G);

  // Convert RAW rotation measurement to physical [°/s]
  mpu6000_s.outp_s.rotX_degps_s16 = (int16_t)((double)raw_s.rotX_s16 * mpu6000_s.prv_s.currRate_degps_s16 / 32767.0);
  mpu6000_s.outp_s.rotY_degps_s16 = (int16_t)((double)raw_s.rotY_s16 * mpu6000_s.prv_s.currRate_degps_s16 / 32767.0);
  mpu6000_s.outp_s.rotZ_degps_s16 = (int16_t)((double)raw_s.rotZ_s16 * mpu6000_s.prv_s.currRate_degps_s16 / 32767.0);

  // Convert RAW temperature measurement to physical [°C]
  mpu6000_s.outp_s.temp_Cgrd_s16 = (int16_t)((double)raw_s.temp_s16 / 340.0 + 36.53);

  // Decrease gyro resolution at 80 % and increase gyro resolution at 20 % of current rate range
  int16_t hiBound_degps_s16 = mpu6000_s.prv_s.currRate_degps_s16 * 0.8;
  int16_t loBound_degps_s16 = mpu6000_s.prv_s.currRate_degps_s16 * 0.2;
  if( (mpu6000_s.outp_s.rotX_degps_s16 >=  hiBound_degps_s16) ||
      (mpu6000_s.outp_s.rotX_degps_s16 <= -hiBound_degps_s16) ||
      (mpu6000_s.outp_s.rotY_degps_s16 >=  hiBound_degps_s16) ||
      (mpu6000_s.outp_s.rotY_degps_s16 <= -hiBound_degps_s16) ||
      (mpu6000_s.outp_s.rotZ_degps_s16 >=  hiBound_degps_s16) ||
      (mpu6000_s.outp_s.rotZ_degps_s16 <= -hiBound_degps_s16) )
  {
    switch( mpu6000_s.prv_s.currRate_degps_s16 )
    {
    case 250:
      // Configure to 500 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x08 );
      mpu6000_s.prv_s.currRate_degps_s16 = 500;
      break;

    case 500:
      // Configure to 1000 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x10 );
      mpu6000_s.prv_s.currRate_degps_s16 = 1000;
      break;

    case 1000:
      // Configure to 2000 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x18 );
      mpu6000_s.prv_s.currRate_degps_s16 = 2000;
      break;

    default:
      // already at limit
      break;
    }
  }
  else if( (mpu6000_s.outp_s.rotX_degps_s16 <=  loBound_degps_s16) &&
           (mpu6000_s.outp_s.rotX_degps_s16 >= -loBound_degps_s16) &&
           (mpu6000_s.outp_s.rotY_degps_s16 <=  loBound_degps_s16) &&
           (mpu6000_s.outp_s.rotY_degps_s16 >= -loBound_degps_s16) &&
           (mpu6000_s.outp_s.rotZ_degps_s16 <=  loBound_degps_s16) &&
           (mpu6000_s.outp_s.rotZ_degps_s16 >= -loBound_degps_s16) )
  {
    switch( mpu6000_s.prv_s.currRate_degps_s16 )
    {
    case 2000:
      // Configure to 1000 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x10 );
      mpu6000_s.prv_s.currRate_degps_s16 = 1000;
      break;

    case 1000:
      // Configure to 500 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x08 );
      mpu6000_s.prv_s.currRate_degps_s16 = 500;
      break;

    case 500:
      // Configure to 250 °/s resolution
      mpu6000_spiWriteReg( REG_GYRO_CFG, 0x00 );
      mpu6000_s.prv_s.currRate_degps_s16 = 250;
      break;

    default:
      // already at limit
      break;
    }
  }
}

// Obtain RAW sensor data
static void mpu6000_readSensorRaw( mpu6000_SensorRaw_ts *data_ps )
{
  uint8_t buf_au8[15];

  // Prepare read command in byte 0
  buf_au8[0]  = REG_ACCEL_XOUT; // 7-bit register start address
  buf_au8[0] |= 0x80;           // read command

  // Transfer data (polled)
  spi_mpu6000_transferData( buf_au8, buf_au8, sizeof(buf_au8) );

  // Decode values
  data_ps->accX_s16 = ((int16_t)buf_au8[ 1] << 8) | buf_au8[ 2];
  data_ps->accY_s16 = ((int16_t)buf_au8[ 3] << 8) | buf_au8[ 4];
  data_ps->accZ_s16 = ((int16_t)buf_au8[ 5] << 8) | buf_au8[ 6];
  data_ps->temp_s16 = ((int16_t)buf_au8[ 7] << 8) | buf_au8[ 8];
  data_ps->rotX_s16 = ((int16_t)buf_au8[ 9] << 8) | buf_au8[10];
  data_ps->rotY_s16 = ((int16_t)buf_au8[11] << 8) | buf_au8[12];
  data_ps->rotZ_s16 = ((int16_t)buf_au8[13] << 8) | buf_au8[14];
}

// Read register of MPU-6000 via SPI
static uint8_t mpu6000_spiReadReg( uint8_t addr_u8 )
{
  uint8_t buf_au8[2];

  // Prepare read command
  buf_au8[0]  = addr_u8 & 0x7F; // 7-bit register address
  buf_au8[0] |= 0x80;           // read command

  // Transfer data (polled)
  spi_mpu6000_transferData( buf_au8, buf_au8, 2 );

  // Return data
  return buf_au8[1];
}

// Write register of MPU-6000 via SPI
static void mpu6000_spiWriteReg( uint8_t addr_u8, uint8_t data_u8 )
{
  uint8_t buf_au8[2];

  // Prepare write command
  buf_au8[0]  = addr_u8 & 0x7F; // 7-bit Register Address
  buf_au8[1]  = data_u8;        // Value

  // Transfer data (polled)
  spi_mpu6000_transferData( buf_au8, buf_au8, 2 );
}
