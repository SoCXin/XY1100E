/*
 * stringx.h
 *
 *  Created on: 2020��8��18��
 *      Author: Administrator
 */

#ifndef TCPIP_HTTP_STRINGX_H_
#define TCPIP_HTTP_STRINGX_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
	#include <locale>
#endif

/*
	Gets the offset of one string in another string
*/
int str_index_of(const char *a, char *b);

/*
	Checks if one string contains another string
*/
int str_contains(const char *haystack, const char *needle);

/*
	Removes last character from string
*/
char* trim_end(char *string, char to_trim);

/*
	Concecates two strings, a wrapper for strcat from string.h, handles the resizing and copying
*/
char* str_cat(char *a, char *b);

/*
	Converts an integer value to its hex character
*/
char to_hex(char code);

/*
	URL encodes a string
*/
char *urlencode(char *str);

/*
	Replacement for the string.h strndup, fixes a bug
*/
char *str_ndup (const char *str, size_t max);

/*
	Replacement for the string.h strdup, fixes a bug
*/
char *str_dup(const char *src);

/*
 	Search and replace a string with another string , in a string
*/
char *str_replace(char *search , char *replace , char *subject);

/*
	Get's all characters until '*until' has been found
*/
char* get_until(char *haystack, char *until);


/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */
void decodeblock(unsigned char in[], char *clrstr);

/*
	Decodes a Base64 string
*/
char* base64_decode(char *b64src);

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock( unsigned char in[], char b64str[], int len );

/*
	Encodes a string with Base64
*/
char* base64_encode(char *clrstr);

#endif /* TCPIP_HTTP_STRINGX_H_ */
