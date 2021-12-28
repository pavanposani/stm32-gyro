
#ifndef __GYRO_H__
#define __GYRO_H__


#include "mbed.h"
#include "FlashIAP.h"

/*=========================================================================
    PRE DEFINED CONSTANTS AND SETTINGS 
    -----------------------------------------------------------------------*/
#define SENSORS_DPS_TO_RADS (0.017453293F)
#define WRITE_BIT 0x00    //  00000000
#define READ_BIT 0x80     //  10000000 
#define DUMMY_BYTE 0x00   //  00000000                    
#define MS_BIT 0x40       //  01000000 - increments address in multiple reads
#define BASE_ADDRESS 0x080E0000 // Last Sector - sector 16 address - page size - 128K
#define BLOCK_SIZE 10240
#define SAMPLE_COUNT 200  // Number of x,y,z angular velocity to be collected from gyro
#define TIME_DURATION 20  // time duration in seconds for values to be considered 
#define RADIUS 80         // Average Leg Height - 80 cms 
/* Sesitivity values from the mechanical characteristics in the datasheet.  */
#define GYRO_SENSITIVITY_250DPS (0.00875F) // Sensitivity at 250 dps
#define GYRO_SENSITIVITY_500DPS (0.0175F)  // Sensitivity at 500 dps
#define GYRO_SENSITIVITY_2000DPS (0.070F)  // Sensitivity at 2000 dps

/*=========================================================================*/


/*! ==============================Registers================================*/
typedef enum {           // DEFAULT    TYPE
  WHO_AM_I = 0x0F,      // 11010100   r
  CTRL_REG1 = 0x20,     // 00000111   rw
  CTRL_REG2 = 0x21,     // 00000000   rw
  CTRL_REG3 = 0x22,     // 00000000   rw
  CTRL_REG4 = 0x23,     // 00000000   rw
  CTRL_REG5 = 0x24,     // 00000000   rw
  REFERENCE = 0x25,     // 00000000   rw
  OUT_TEMP = 0x26,      //            r
  STATUS_REG = 0x27,    //            r
  OUT_X_L = 0x28,       //            r
  OUT_X_H = 0x29,       //            r
  OUT_Y_L = 0x2A,       //            r
  OUT_Y_H = 0x2B,       //            r
  OUT_Z_L = 0x2C,       //            r
  OUT_Z_H = 0x2D,       //            r
  FIFO_CTRL_REG = 0x2E, // 00000000   rw
  FIFO_SRC_REG = 0x2F,  //            r
  INT1_CFG = 0x30,      // 00000000   rw
  INT1_SRC = 0x31,      //            r
  INT1_THS_XH = 0x32,   // 00000000   rw
  INT1_THS_XL = 0x33,   // 00000000   rw
  INT1_THS_YH = 0x34,   // 00000000   rw
  INT1_THS_YL = 0x35,   // 00000000   rw
  INT1_THS_ZH = 0x36,   // 00000000   rw
  INT1_THS_ZL = 0x37,   // 00000000   rw
  INT1_DURATION = 0x38  // 00000000   rw
} gyroRegisters_t;

typedef enum {
  GYRO_RANGE_250DPS = 250,
  GYRO_RANGE_500DPS = 500,
  GYRO_RANGE_2000DPS = 2000
} gyroRange_t;


/*=========================================================================
    RAW GYROSCOPE DATA 
    -----------------------------------------------------------------------*/
typedef struct gyroRawData_s {
  int16_t x;  /**  X axis data. */
  int16_t y;  /**  Y axis data. */
  int16_t z;  /**  Z axis data. */
} gyroRawData_t;
/*=========================================================================*/


extern SPI gyroscope;
extern DigitalOut chip_select;

gyroRawData_t read_gyro_values();
void initialize_gyro();
void compute_linear_velocity(gyroRawData_t []);
#endif

//#0x080E0000 - 0x080FFFFF