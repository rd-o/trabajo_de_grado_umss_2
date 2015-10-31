#include "MPU6050_6Axis_MotionApps20.h"
#include "Giro.hpp"

MPU6050 mpu;
using namespace std;

Giro::Giro(){
    dmpReady = false;  
    setup();
}

void Giro::setup() {

    uint8_t devStatus;   
    uint8_t mpuIntStatus;

    printf("Initializing I2C devices...\n");
    mpu.initialize();

    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n"
                                : "MPU6050 connection failed\n");

    printf("Initializing DMP...\n");
    devStatus = mpu.dmpInitialize();
    
    if (devStatus == 0) {
        printf("Enabling DMP...\n");
        mpu.setDMPEnabled(true);

        mpuIntStatus = mpu.getIntStatus();

        printf("DMP ready!\n");
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}


bool Giro::getAngles(float *&angle_out) {

    uint8_t fifoBuffer[64]; 
    Quaternion q;           
    float angle[3] = {0, 0, 0};
    angle_out = angle;
    bool ret = false;
    VectorFloat gravity;

    if (!dmpReady) return false;
    uint16_t fifoCount = mpu.getFIFOCount();

    if (fifoCount == 1024) {
        mpu.resetFIFO();
        printf("FIFO overflow!\n");
        ret = false;

    } else if (fifoCount >= 42) {
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(angle, &q, &gravity);

        angle_out = angle;
        ret = true;
    }
    return ret;
}
