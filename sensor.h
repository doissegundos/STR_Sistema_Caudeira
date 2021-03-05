#ifndef SENSOR_H
#define SENSOR_H

void sensor_putH(double lido);
double sensor_getH(void);
void sensor_alarmeH(double limite);

void sensor_putT(double lido);
double sensor_getT(void);
void sensor_alarmeT(double limite);

void sensor_putTi(double lido);
double sensor_getTi(void);
void sensor_alarmeTi(double limite);

void sensor_putTa(double lido);
double sensor_getTa(void);
void sensor_alarmeTa(double limite);

void sensor_putNo(double lido);
double sensor_getNo(void);
void sensor_alarmeNo(double limite);

#endif
