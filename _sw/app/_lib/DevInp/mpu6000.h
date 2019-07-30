#ifndef MPU6000_H_
#define MPU6000_H_

#ifdef MPU6000_C
#define MPU6000_SCOPE
#else
#define MPU6000_SCOPE extern
#endif

/****************** Includes ********************/

#include <stdint.h>

/******************* Defines ********************/

/******************** Types *********************/

typedef struct mpu6000_prv_s
{
  int16_t currRate_degps_s16;

} mpu6000_prv_ts;

typedef struct mpu6000_outp_s
{
  // 3-Axis acceleration [cm/s²]
  int16_t accX_cmps2_s16;
  int16_t accY_cmps2_s16;
  int16_t accZ_cmps2_s16;

  // 3-Axis rotation [°/sec]
  int16_t rotX_degps_s16;
  int16_t rotY_degps_s16;
  int16_t rotZ_degps_s16;

  // Temperature [°C]
  int16_t temp_Cgrd_s16;

} mpu6000_outp_ts;

typedef struct mpu6000_s
{
  mpu6000_prv_ts  prv_s;
  mpu6000_outp_ts outp_s;

} mpu6000_ts;

/****************** Public Data *****************/

MPU6000_SCOPE mpu6000_ts mpu6000_s;

/******************* Interface ******************/

MPU6000_SCOPE uint8_t mpu6000_init( void );
MPU6000_SCOPE void mpu6000( void );


#endif /* MPU6000_H_ */
