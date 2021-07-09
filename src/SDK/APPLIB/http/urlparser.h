/*
 * urlparser.h
 *
 *  Created on: 2020��8��18��
 *      Author: Administrator
 */

#ifndef TCPIP_HTTP_URLPARSER_H_
#define TCPIP_HTTP_URLPARSER_H_

/*
	Represents an url
*/
struct parsed_url
{
	char *uri;					/* mandatory */
    char *scheme;               /* mandatory */
    char *host;                 /* mandatory */
//	char *ip; 					/* mandatory */
    char ip[16];
    char port[8];                 /* optional */
    char *path;                 /* optional */
    char *query;                /* optional */
    char *fragment;             /* optional */
    char *username;             /* optional */
    char *password;             /* optional */
};

/*
	Free memory of parsed url
*/
void parsed_url_free(struct parsed_url *purl);

/*
	Retrieves the IP adress of a hostname
*/
//char* hostname_to_ip(char *hostname);
int hostname_to_ip(char *hostname, char *ip, int ip_len);

/*
	Check whether the character is permitted in scheme string
*/
int is_scheme_char(int c);

/*
	Parses a specified URL and returns the structure named 'parsed_url'
	Implented according to:
	RFC 1738 - http://www.ietf.org/rfc/rfc1738.txt
	RFC 3986 -  http://www.ietf.org/rfc/rfc3986.txt
*/
struct parsed_url *parse_url(const char *url);

#endif /* TCPIP_HTTP_URLPARSER_H_ */
