#include "gyro.h"

// SPI Interface and CHIP SELECT
SPI gyroscope(PF_9, PF_8, PF_7);    //mosi miso sclk
DigitalOut chip_select(PC_1);       //chip select pin

char add_rw_ms_bit(char reg_addr,bool write_bit=false, bool ms_bit=false){
    // set write and ms bit based on flags passed to the register
    if (write_bit)
        reg_addr = WRITE_BIT|reg_addr;
    else
        reg_addr = READ_BIT|reg_addr;
    if(ms_bit)
        reg_addr = MS_BIT|reg_addr;

    return reg_addr;
}

void compute_linear_velocity(gyroRawData_t gyro_vals[]){
    float x_linv, y_linv, z_linv;
    double avg_v =0;    
    printf("\nIndependent Linear Velocities\n");
    for(int i=0;i< SAMPLE_COUNT; i++){
        x_linv = gyro_vals[i].x * GYRO_SENSITIVITY_250DPS;       //  Logic for computing independent linear velocity  
        y_linv = gyro_vals[i].y * GYRO_SENSITIVITY_250DPS;       //  Gyro Value * sensitivity  = angular velocity in degrees per second 
        z_linv = gyro_vals[i].z * GYRO_SENSITIVITY_250DPS;       //  convert angular velocity to radians by multiplying with constant(3.14/180) 

        avg_v += sqrt((x_linv * x_linv) + (y_linv * y_linv) + (z_linv * z_linv));       // magnitude of gyro 
        printf("X:%0.2f\tY:%0.2f\tZ:%0.2f \n", x_linv, y_linv, z_linv);
        // printf("%0.2f, %0.2f, %0.2f, %0.2f\n", x_linv, y_linv, z_linv, sqrt((x_linv * x_linv) + (y_linv * y_linv) + (z_linv * z_linv)));
    }
    float lvelocity = (avg_v * SENSORS_DPS_TO_RADS * RADIUS/100)/SAMPLE_COUNT;    // divide by 100 to convert from centimetres to metres
    printf("\n***************************************\n");
    printf("Average Velocity of Person in %d seconds :%0.2f m/s(metres per second)\n", TIME_DURATION, lvelocity);
    printf("Total Distance travelled in %d Seconds: %d metres\n", TIME_DURATION, (int)(lvelocity * TIME_DURATION));
    printf("\n***************************************\n");
}

void initialize_gyro(){
    
    chip_select = 1;                                        // set CS Line High during initialization
    gyroscope.format(8, 3);                                 // Setup the SPI for 8 bit data, high steady state clock,
    gyroscope.frequency(1000000);                           // set to 1 MHz -> Max Value - 10 MHz

    chip_select = 0;                                        // set CS Line to Low before transaction
    gyroscope.write(add_rw_ms_bit(WHO_AM_I));               // Send the command to read the adress by using WHOAMI register
    int whoami = gyroscope.write(DUMMY_BYTE);               // Write Dummy byte to read data through SPI
    printf("Register Intialized with Address:0x%X\n", whoami);
    chip_select = 1;                                        // set CS Line to High After transaction

    chip_select = 0;                                        // set CS Line to Low before transaction
    gyroscope.write(add_rw_ms_bit(CTRL_REG1, true));        // address to write data CTRL_REG1
    gyroscope.write(add_rw_ms_bit(0x0F, true));             // Set transfer rate and Enable the Gyroscope to read X,Y,Z value - 0b00001111
    chip_select = 1;                                        // set CS Line to High After transaction

}

gyroRawData_t read_gyro_values(){
    chip_select = 0;
    gyroscope.write(add_rw_ms_bit(OUT_X_L, false, true));   // Write X_l register address with auto incrementing bit set
  
    uint8_t x_l = gyroscope.write(DUMMY_BYTE);              // Read data from register X_L   
    uint8_t x_h = gyroscope.write(DUMMY_BYTE);              // Read data from register X_H   
    uint8_t y_l = gyroscope.write(DUMMY_BYTE);              // Read data from register Y_L   
    uint8_t y_h = gyroscope.write(DUMMY_BYTE);              // Read data from register Y_H   
    uint8_t z_l = gyroscope.write(DUMMY_BYTE);              // Read data from register Z_L   
    uint8_t z_h = gyroscope.write(DUMMY_BYTE);              // Read data from register Z_H   

	int16_t x = (x_h<<8|x_l);
    int16_t y = (y_h<<8|y_l);
    int16_t z = (z_h<<8|z_l);
    chip_select = 1;
    printf("\nRaw Gyroscope Data: X:%d\tY:%d\tZ:%d\n",x,y,z);
    gyroRawData_t gyro_vals = {x,y,z};
    return gyro_vals;
}
