#ifndef SMARTCARD_PROXY_H
#define SMARTCARD_PROXY_H


typedef unsigned short uint16_t;


struct sc_request_msg {
	uint16_t req_len;
	uint8_t  data[0];

};

struct sc_response_msg {
	uint16_t rsp_len;
	uint8_t  data[0];

};


int send_msg_to_smartcardproxy(void *buf,unsigned int len);


int smartcard_write_to_cp(struct sc_response_msg * p_response_msg, int msglen);



void proxy_smartcard_task(void);

void proxy_smartcard_init();


#endif
