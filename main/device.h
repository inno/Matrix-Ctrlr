#ifndef device_h
#define device_h

#define MATRIX_DEVICE_A 0 //0x0a // matrix connected to Serial1
#define MATRIX_DEVICE_B 1 //0x0b // matrix connected to Serial2
#define MATRIX_DEVICE_C 2 //0x0c // matrix connected to Serial4
#define MATRIX_DEVICE_D 3 //0x0d // matrix connected to Serial5

extern bool matrix_model_A;
extern bool matrix_model_B;
extern bool matrix_model_C;
extern bool matrix_model_D;

#define MATRIX_6 1
#define MATRIX_1000 0

//device var used in chaosmatrix.c and else
extern bool matrix_modele; // indicate if matrix6 or 1000
extern unsigned char device; // set the Matrix Device A, B, C or D regarding i2c output
extern unsigned char router_device; // define IIC port

extern unsigned char device_arp; // set the Matrix Device A, B, C or D regarding i2c output
extern unsigned char device_seq;

extern void Matrix_Modele_Init(void);
extern void Device_Select(unsigned char pin);
extern void Show_Selected_Device(unsigned char device);
extern void Device_Init(unsigned char device);
extern void Router(unsigned char evnt0, unsigned char evnt1, unsigned char evnt2);
extern void Router_Arp(unsigned char evnt0, unsigned char evnt1, unsigned char evnt2);

#endif
