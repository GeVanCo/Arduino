// NvmComm.h

#ifndef NVM_COMM_H
#define NVM_COMM_H

void callNvmWrite(int device, int addressHigh, int addressLow, int rndNumber);
int callNvmRead(int device, int addressHigh, int addressLow, int nrOfBytes);

#endif
