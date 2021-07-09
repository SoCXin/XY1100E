#ifndef ATINY_LOG_H
#define ATINY_LOG_H

typedef struct
{
    char method;
    char path[50];
}fota_http_req_t;

typedef enum 
{
    BY_HTTP = 0,
    BY_HTTPS = 1
} xy_http_method_e;

typedef struct
{  
    unsigned int download_delta_size;
    unsigned short download_unit_index;
    unsigned short download_unit_num;
}download_info_struct;

#endif

