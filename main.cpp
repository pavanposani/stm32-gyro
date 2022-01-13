/* 
Author: Pavan Posani

*/

#include "gyro.h"

Ticker read_data;         //  ticker to sample data for every 0.5 seconds
bool capture_value;       //  to capture gyro readings based on the flag using ticker
FlashIAP flash_storage;   //


void capture_data(){
  capture_value = !capture_value;
}


int main() {
  capture_value = true;
  bool compute_speed = false;                // Make this flag false to read gyro values from sensor
  int read_counter=0;
  
  gyroRawData_t gyro_vals[SAMPLE_COUNT];    // Array to read and store raw gyro scope values
  flash_storage.init();                     // Intialize flash storage
  
  initialize_gyro();                        // initialize the gyro sensor to 250 dps and enable x,y,z
  read_data.attach(&capture_data, 0.5f);    // interval (0.5 seconds)
  
  while(1) {
    if(compute_speed){
      flash_storage.read(gyro_vals, BASE_ADDRESS, sizeof(gyro_vals)); // retrieve data from flash and copy to array gyro_vals
      compute_linear_velocity(gyro_vals);                             // compute linear veloctuy from the retrieved data from flash
      break;
    }

    if(capture_value){                                  // capture data only for every 0.5 second intervals
      if(read_counter < SAMPLE_COUNT){                  // For 20 seconds of data we consider around 50 samples
        gyro_vals[read_counter] = read_gyro_values();   // read gyro values and store it to array
        capture_value=false;
        read_counter++;
      }
      else {                                                                            // after completion of 50 samples
        flash_storage.erase(BASE_ADDRESS, flash_storage.get_sector_size(BASE_ADDRESS)); // erase flash sector
        flash_storage.program(gyro_vals, BASE_ADDRESS, sizeof(gyro_vals));              // write data to flash
        break;
      }  
    }
  }
  flash_storage.deinit();                   // De Intialize flash storage
  return 0; 
}
