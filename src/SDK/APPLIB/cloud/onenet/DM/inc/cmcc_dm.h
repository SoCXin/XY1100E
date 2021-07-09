#ifndef _XY_CMCCDM_H
#define _XY_CMCCDM_H

void cmccdm_getDevinfo(char* devInfo, unsigned int len);
void cmccdm_getAppinfo(char* appInfo, unsigned int len);
void cmccdm_getMacinfo(char* macInfo, unsigned int len);
void cmccdm_getRominfo(char* romInfo, unsigned int len);
void cmccdm_getRaminfo(char* ramInfo, unsigned int len);
void cmccdm_getCpuinfo(char* CpuInfo, unsigned int len);
void cmccdm_getSysinfo(char* sysInfo, unsigned int len);
void cmccdm_getSoftVer(char* softInfo, unsigned int len);
void cmccdm_getSoftName(char* softname, unsigned int len);
void cmccdm_getVolteinfo(char* volInfo, unsigned int len);
void cmccdm_getNetType(char* netType, unsigned int len);
void cmccdm_getNetAccount(char* netInfo, unsigned int len);
void cmccdm_getPNumber(char* pNumber, unsigned int len);
void cmccdm_getLocinfo(char* locInfo, unsigned int len);
void cmccdm_getRouteMac(char* routeMac, unsigned int len);
void cmccdm_getBrandinfo(char* brandInfo, unsigned int len);
void cmccdm_getGPUinfo(char* GPUInfo, unsigned int len);
void cmccdm_getBoardinfo(char* boardInfo, unsigned int len);
void cmccdm_getModelinfo(char* modelInfo, unsigned int len);
void cmccdm_getResinfo(char* resInfo, unsigned int len);
void cmcc_dm_run(void);

#endif

