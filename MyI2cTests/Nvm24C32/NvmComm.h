// NvmComm.h

#ifndef NVM_COMM_H
#define NVM_COMM_H

void nvmWrite(int device, int addressHigh, int addressLow, int rndNumber);
int nvmRead(int device, int addressHigh, int addressLow, int nrOfBytes);
int nvmReadCurrentAddress(int device);

#endif
