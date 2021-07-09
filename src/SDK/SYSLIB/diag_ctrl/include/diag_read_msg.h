#ifndef _DIAG_READ_MSG_H_
#define _DIAG_READ_MSG_H_

void xy_diag_read(void);

void PC_CMD(char bDumpFile);

void ResetMem();

void ParseData(unsigned char cspATCdata, char bDumpFile);
#endif



