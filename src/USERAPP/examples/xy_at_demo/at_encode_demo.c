/**
 * @file at_encode_demo.c
 * @brief 
 * @version 1.0
 * @date 2021-02-24
 * @copyright Copyright (c) 2021  芯翼信息科技有限公司
 * 
 */

#include "xy_api.h"

#if XY_DTLS

osThreadId_t at_encode_thread_id = NULL;

void at_encode_create_thread(char *encode_buf)
{
	char *encoded_buf= NULL;
	char *temp_buf = NULL;
	int in_len;
	int out_len;
	int temp_len;

	in_len = strlen(encode_buf);
	temp_len = out_len = (in_len / 16 + 1) * 16; //encoding/decoding data len must be 16 multiple

	encoded_buf = xy_zalloc(out_len);
	temp_buf = xy_zalloc(temp_len);
	if (xy_encrypt(encode_buf, in_len, encoded_buf, &out_len) < 0) {
		xy_printf("encoding failed");
		goto encode_out;
	}
	xy_printf("encoding finished, encoded len: %d", out_len);
	if (xy_decrypt(encoded_buf, out_len, temp_buf, &temp_len) < 0) {
		xy_printf("decoding failed");
		goto encode_out;
	}
	xy_printf("decoding finished, decoded len: %d", temp_len);
	if (temp_len != in_len || memcmp(encode_buf, temp_buf, temp_len) != 0) {
		xy_printf("decoded buf is not same with original buf");
		goto encode_out;
	}
	xy_printf("encode and decode succeed");
encode_out:
	if (encode_buf != NULL) {
		xy_free(encode_buf);
	}
	if (encoded_buf != NULL) {
		xy_free(encoded_buf);
	}
	if (temp_buf != NULL) {
		xy_free(temp_buf);
	}
	xy_printf("exit decode/encode thread!!!");
	at_encode_thread_id = NULL;
	osThreadExit();
}

#endif //XY_DTLS