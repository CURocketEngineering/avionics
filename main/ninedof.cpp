#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "ninedof.h"

LSM9DS1 imu;

void ninedof_init() {
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;

    gyro_prev.x = 0.0;
    gyro_prev.y = 0.0;
    gyro_prev.z = 0.0;

    imu.begin();
}

void ninedof_read(bool filter) {
    if (imu.gyroAvailable()) {
        imu.readGyro();

        gyro_prev = gyro;

        gyro.x = imu.calcGyro(imu.gx);
        gyro.y = imu.calcGyro(imu.gy);
        gyro.z = imu.calcGyro(imu.gz);

        if (filter) {
            gyro.x = GYRO_GAIN*gyro.x + (1.0 - GYRO_GAIN)*gyro_prev.x;
            gyro.y = GYRO_GAIN*gyro.y + (1.0 - GYRO_GAIN)*gyro_prev.y;
            gyro.z = GYRO_GAIN*gyro.z + (1.0 - GYRO_GAIN)*gyro_prev.z;
        }
    }

    if (imu.accelAvailable()) {
        imu.readAccel();

        acc_prev = acc;

        acc.x = imu.calcAccel(imu.ax);
        acc.y = imu.calcAccel(imu.ay);
        acc.z = imu.calcAccel(imu.az);

        if (filter) {
            acc.x = ACCEL_GAIN*acc.x + (1.0 - ACCEL_GAIN)*acc_prev.x;
            acc.y = ACCEL_GAIN*acc.y + (1.0 - ACCEL_GAIN)*acc_prev.y;
            acc.z = ACCEL_GAIN*acc.z + (1.0 - ACCEL_GAIN)*acc_prev.z;
        }

        att.r = atan2(acc.y, acc.z);
        att.p = atan2(-acc.x, sqrt(acc.y*acc.y + acc.z*acc.z));
    }

    if (imu.magAvailable()) {
        imu.readMag();

        mag.x = imu.calcMag(imu.mx);
        mag.y = imu.calcMag(imu.my);
        mag.z = imu.calcMag(imu.mz);

        if (filter) {
            mag.x = MAG_GAIN*mag.x + (1.0 - MAG_GAIN)*mag_prev.x;
            mag.y = MAG_GAIN*mag.y + (1.0 - MAG_GAIN)*mag_prev.y;
            mag.z = MAG_GAIN*mag.z + (1.0 - MAG_GAIN)*mag_prev.z;
        }

        if (fabs(mag.y) < EPSILON) {
          att.y = (mag.x < 0) ? PI : 0;
        }
        else {
          att.y = atan2(mag.x, mag.y);
        }

        att.y -= DECLINATION*PI/180.0;

        if (heading > PI) {
            att.y -= 2*PI;
        }
        else if (heading < -PI) {
            att.y += 2*PI;
        }
    }
}
