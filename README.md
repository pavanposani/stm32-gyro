# STM32 - I3G4250D - Gyro

Measuring Linear velocity and distance using the ST- MEMS I3G4250D Sensor on STM32 Microcontroller

In order to measure the linear velocity and distance travelled using the gyro sensor, we can rely on the calculation of magnitude of the X,Y,Z values 

magnitude=sqrt(𝑥^2+𝑦^2+𝑧^2)

angular_velocity = magnitude * gyro_sensitivty 

linear_velocity = angular_velocity * radius

Distance = linear_velocity * time
