/**
* @file        xy_coap_api.c
* @brief       This file is the COAP API that users can call.
* @attention   请参考coap_opencpu_demo.c进行二次开发。
* @par
*/
#include "softap_macro.h"
#if LIBCOAP
/****************************************************************************************************
 *                           Include header files                                                    *
 ***************************************************************************************************/
#include "xy_utils.h"
#if XY_FOTA
#include "xy_fota.h"
#endif
#include "libcoap.h"
#include "pdu.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "coap_session.h"
#include "option.h"
#include "block.h"
#include "net.h"
#include "coap_debug.h"

/****************************************************************************************************
 *                         Macro definitions                                                         *
 ***************************************************************************************************/
#define hexchar_to_dec(c) ((c) & 0x40 ? ((c) & 0x0F) + 9 : ((c) & 0x0F))
#define FLAGS_BLOCK 0x01
#define coap_api_loglevel LIBCOAP_LOG_DEBUG

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef __GNUC__
#define UNUSED_PARAM __attribute__ ((unused))
#else /* not a GCC */
#define UNUSED_PARAM
#endif /* GCC */

#define BUFSIZE 100/* Maximum length of URI in bytes. */
#define NI_MAXSERV 64
#define NI_MAXHOST 32

#ifndef TRUE
#define  TRUE (1)
#endif

#ifndef FALSE
#define  FALSE (0)
#endif

/****************************************************************************************************
 *                           Structure definitions                                                   *
 ***************************************************************************************************/
typedef enum
{
    REGISTER = 4,
    UNREGISTER,
    IDLE = 0,
    DOWNLOADING,
    DOWNLOADED,
    UPDATING
}cmd_type;

typedef struct
{
    unsigned char code;
    const char *media_type;
} content_type_t;


typedef struct st_recvdata_msg  
{
    coap_pdu_t *pdu;
}recvdata_msg_t;


/****************************************************************************************************
 *                        Global variable definitions                                                *
 ***************************************************************************************************/
static osMessageQueueId_t xy_coap_recvdata_q = NULL;

static int ready = 0;          /* reading is done when this flag is set */

static int g_flags = 0;
static int ping_timeout = 0;
static int g_quit = 0;
static u16_t g_msgid = 0;
static coap_optlist_t *g_optlist = NULL;
static unsigned char g_token_data[8];
static coap_binary_t g_token = { 0, g_token_data };

static unsigned int g_wait_seconds = 90;                /* default timeout in seconds */
static unsigned int g_wait_ms = 0;
static int g_wait_ms_reset = 0;
static int g_obs_started = 0;
static unsigned int g_obs_seconds = 30;          /* default observe time */
static unsigned int g_obs_ms = 0;                /* timeout for current subscription */
static int g_obs_ms_reset = 0;
static int g_doing_getting_block = 0;

static coap_string_t g_payload = { 0, NULL };       /* optional payload to send */
static unsigned char g_msgtype = COAP_MESSAGE_CON;  /* usually, requests are sent confirmable */
typedef unsigned char method_t;
static method_t g_method = 1;                    /* the fota_method we are using in our requests */
static coap_block_t g_block = { .num = 0, .m = 0, .szx = 6 };
static uint16_t g_last_block1_tid = 0;
osSemaphoreId_t coap_pkt_sem = NULL;
static int8_t coap_msgid_random_flag = 1;        /* configure message_id randomly */
static int8_t coap_token_random_flag = 1;        /* configure token randomly */

extern void xy_fota_by_coap_hook(const uint8_t *data, size_t len);
void xy_coap_recvqueue_delete(osMessageQueueId_t pMsgQueueId);
/*****************************************************************************************************
Function    : append_to_output
Description : print string
Input       :
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
static int append_to_output(coap_pdu_t *received ,const uint8_t *data, size_t len)
{
    char *coap_rsp = (char*)xy_zalloc(40 + len*2);
    char *str_coap_rsp =NULL;
#if FOTA_DEMO
   // xy_fota_by_coap_hook(data, len);
#endif
    if(len <= 0 )
        snprintf(coap_rsp,40, "\r\n+COAPURC:%s, %d.%02d,%d\r\n",COAP_PDU_IS_REQUEST(received) ==TRUE ? "request":"response",\
                                (received->code >> 5), (received->code & 0x1F),received->tid);
    else
    {
        str_coap_rsp = (char*)xy_zalloc(len * 2 +1);
        bytes2hexstr(data, len,  str_coap_rsp, len * 2+1);
        snprintf(coap_rsp,40 + len*2, "\r\n+COAPURC:%s, %d.%02d,%d,%d,%s\r\n",COAP_PDU_IS_REQUEST(received) ==TRUE ? "request":"response",\
                                       (received->code >> 5), (received->code & 0x1F),received->tid,len,str_coap_rsp);
    }
    send_rsp_str_to_ext(coap_rsp);

    if(coap_rsp)
        xy_free(coap_rsp);
    if(str_coap_rsp)
        xy_free(str_coap_rsp);
    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_clear_option
Description : clear coap global option list
Input       :
Output      : None
Return      : None
*****************************************************************************************************/
void xy_coap_clear_option()
{
    coap_delete_optlist(g_optlist);
    g_optlist = NULL;
}
/*****************************************************************************************************
Function    : xy_coap_new_request
Description : config COAP header
Input       :
Output      : None
Return      : coap_pdu_t:structure for CoAP PDUs
*****************************************************************************************************/
static coap_pdu_t * xy_coap_new_request(coap_context_t *ctx,
                 coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length) 
{
    coap_pdu_t *pdu;
    (void)ctx;

    if (!(pdu = coap_new_pdu(session)))
        return NULL;

    pdu->type = g_msgtype;
    pdu->tid = coap_new_message_id(session);
    pdu->code = m;

    if ( !coap_add_token(pdu, g_token.length, g_token.s)) 
    {
        coap_log(LIBCOAP_LOG_DEBUG, "cannot add token to request\n");
    }

    if (options)
        coap_add_optlist_pdu(pdu, options);

    if (length) 
    {
        if ((g_flags & FLAGS_BLOCK) == 0)
            coap_add_data(pdu, length, data);
        else 
        {
            unsigned char buf[4];
            coap_add_option(pdu,
                    LIBCOAP_COAP_OPTION_SIZE1,
                    coap_encode_var_safe(buf, sizeof(buf), length),
                    buf);

            coap_add_block(pdu, length, data, g_block.num, g_block.szx);
        }
    }

    return pdu;
}

/*****************************************************************************************************
Function    : xy_content_type_set
Description : insert COAP_OPTION_CONTENT to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_content_type_set(char *arg, uint16_t key) 
{
    static content_type_t content_types[] = {
        {  0, "plain" },
        {  0, "text/plain" },
        { 40, "link" },
        { 40, "link-format" },
        { 40, "application/link-format" },
        { 41, "xml" },
        { 41, "application/xml" },
        { 42, "binary" },
        { 42, "octet-stream" },
        { 42, "application/octet-stream" },
        { 47, "exi" },
        { 47, "application/exi" },
        { 50, "json" },
        { 50, "application/json" },
        { 60, "cbor" },
        { 60, "application/cbor" },
        { 255, NULL }
    };
    coap_optlist_t *node;
    unsigned char i;
    uint16_t value;
    uint8_t buf[2];

    if (isdigit(*arg)) 
    {
        value = atoi(arg);
    } 
    else 
    {
        for (i=0; content_types[i].media_type &&strncmp(arg, content_types[i].media_type, strlen(arg)) != 0;++i)
            ;

        if (content_types[i].media_type) 
        {
            value = content_types[i].code;
        } 
        else
        {
            coap_log(LIBCOAP_LOG_WARNING, "W: unknown content-format '%s'\n",arg);
            return;
        }
    }

    node = coap_new_optlist(key, coap_encode_var_safe(buf, sizeof(buf), value), buf);
    if (node) 
    {
        coap_insert_optlist(&g_optlist, node);
    }
}

/*****************************************************************************************************
Function    : xy_coap_method_get
Description : invert method string to method code
Input       :
Output      : None
Return      :1:get 2:post 3:put 4:delete 5:fetch 6:patch 7:ipatch 8:error
*****************************************************************************************************/
static method_t xy_coap_method_get(char *arg) 
{
    static const char *methods[] =
    { 0, "get", "post", "put", "delete", "fetch", "patch", "ipatch", 0};
    unsigned char i;

    for (i=1; methods[i] && strcasecmp(arg,methods[i]) != 0 ; ++i)
        ;

    return i;     /* note that we do not prevent illegal methods */
}
/*****************************************************************************************************
Function    : xy_coap_type_get
Description : invert type string to type code
Input       :
Output      : None
Return      : 0:COAP_MESSAGE_CON 1:COAP_MESSAGE_NON 2:COAP_MESSAGE_ACK 3:COAP_MESSAGE_RST 4:error
*****************************************************************************************************/
static unsigned char xy_coap_type_get(char *arg)
{
    static const char *types[] =
    {"con", "non", "ack", "rst",0};
    unsigned char i;

    for (i=0; types[i] && strcasecmp(arg,types[i]) != 0 ; ++i)
        ;

    return i;     /* note that we do not prevent illegal types */
}

/*****************************************************************************************************
Function    : xy_coap_uri_host_set
Description : insert COAP_OPTION_URI_PATH to g_optlist
Input       :
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
static int xy_coap_path_set(char *arg, int length)
{
    unsigned char _buf[BUFSIZE];
    unsigned char *buf = _buf;
    size_t buflen;
    int res;
    
    if (length) 
    {
        buflen = BUFSIZE;
        if (length > BUFSIZE)
            coap_log(LIBCOAP_LOG_WARNING, "URI path will be truncated (max buffer %d)\n", BUFSIZE);
        res = coap_split_path(arg, length, buf, &buflen);

        while (res--) 
        {
          coap_insert_optlist(&g_optlist,
                          coap_new_optlist(LIBCOAP_COAP_OPTION_URI_PATH,
                          coap_opt_length(buf),
                          coap_opt_value(buf)));

          buf += coap_opt_size(buf);
        }
    }
    
    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_uri_host_set
Description : insert OPTION_URI_QUERY to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_uri_query_set(char *arg, int length)
{
    unsigned char _buf[BUFSIZE];
    unsigned char *buf = _buf;
    size_t buflen;
    int res;

    if (length)
    {
        buflen = BUFSIZE;
        if (length > BUFSIZE)
            coap_log(LIBCOAP_LOG_WARNING, "URI query will be truncated (max buffer %d)\n", BUFSIZE);
        res = coap_split_query(arg, length, buf, &buflen);

        while (res--)
        {
          coap_insert_optlist(&g_optlist,
                          coap_new_optlist(LIBCOAP_COAP_OPTION_URI_QUERY,
                          coap_opt_length(buf),
                          coap_opt_value(buf)));

          buf += coap_opt_size(buf);
        }
    }

    return;
}

/*****************************************************************************************************
Function    : xy_coap_uri_host_set
Description : insert OPTION_URI_HOST to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_uri_host_set(char *arg, int length)
{
    coap_insert_optlist(&g_optlist,
                  coap_new_optlist(LIBCOAP_COAP_OPTION_URI_HOST,
                  length,
                  arg));

    return;
}

/*****************************************************************************************************
Function    : xy_coap_option_size1_set
Description : insert COAP_OPTION_SIZE1 to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_option_size1_set(char *arg, uint16_t key)
{
    coap_optlist_t *node;
    long long value;
    uint8_t buf[5];

    if (isdigit(*arg))
    {
        value = atoi(arg);
    }
    else
    {
        coap_log(LIBCOAP_LOG_WARNING, "xy: unknown option-size1 '%s'\n",arg);
        return;
    }

    node = coap_new_optlist(key, coap_encode_var_safe(buf, sizeof(buf), value), buf);
    if (node)
    {
        coap_insert_optlist(&g_optlist, node);
    }
    return;
}

/*****************************************************************************************************
Function    : xy_coap_proxy_uri_set
Description : insert COAP_OPTION_PROXY_URI to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_proxy_uri_set(char *arg, int length)
{
	(void) length;

    size_t len = strlen(arg);
    while (len > 270)
    {
    coap_insert_optlist(&g_optlist,
                coap_new_optlist(LIBCOAP_COAP_OPTION_PROXY_URI,
                270,
                arg));

    len -= 270;
    arg += 270;
    }

    coap_insert_optlist(&g_optlist,
              coap_new_optlist(LIBCOAP_COAP_OPTION_PROXY_URI,
              len,
              arg));

    return;
}

/*****************************************************************************************************
Function    : xy_coap_uri_port_set
Description : insert COAP_OPTION_URI_PORT to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_uri_port_set(char *arg, uint16_t key)
{
    coap_optlist_t *node;
    long long value;
    uint8_t buf[3];

    if (isdigit(*arg))
    {
        value = atoi(arg);
    }
    else
    {
        coap_log(LIBCOAP_LOG_WARNING, "xy: unknown uri-port '%s'\n",arg);
        return;
    }

    node = coap_new_optlist(key, coap_encode_var_safe(buf, sizeof(buf), value), buf);
    if (node)
    {
        coap_insert_optlist(&g_optlist, node);
    }
    return;

}

/*****************************************************************************************************
Function    : xy_coap_observe_set
Description : insert COAP_OPTION_OBSERVE to g_optlist
Input       :
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_observe_set(char *arg, uint16_t key)
{
    coap_optlist_t *node;
    long long value;
    uint8_t buf[5];

    if (isdigit(*arg))
    {
        value = atoi(arg);
    }
    else
    {
        coap_log(LIBCOAP_LOG_WARNING, "xy: unknown observe '%s'\n",arg);
        return;
    }

    node = coap_new_optlist(key, coap_encode_var_safe(buf, sizeof(buf), value), buf);
    if (node)
    {
        coap_insert_optlist(&g_optlist, node);
    }
    return;

}
static int xy_coap_blocksize(uint16_t num, uint16_t size) 
{
  g_block.num = num;
  if (size)
    g_block.szx = (coap_fls(size >> 4) - 1) & 0x07;

  g_flags |= FLAGS_BLOCK;
  return 1;
}


/* Called after processing the options from the commandline to set
 * Block1 or Block2 depending on fota_method. */
static void xy_coap_blocksize_set(void) 
{
  static unsigned char buf[4];        /* hack: temporarily take encoded bytes */
  uint16_t opt;
  unsigned int opt_length;

  if (g_method != COAP_REQUEST_DELETE)
  {
    opt = g_method == COAP_REQUEST_GET ? LIBCOAP_COAP_OPTION_BLOCK2 : LIBCOAP_COAP_OPTION_BLOCK1;

    g_block.m = (opt == LIBCOAP_COAP_OPTION_BLOCK1) &&
      ((1u << (g_block.szx + 4)) < g_payload.length);

    opt_length = coap_encode_var_safe(buf, sizeof(buf),
          (g_block.num << 4 | g_block.m << 3 | g_block.szx));

    coap_insert_optlist(&g_optlist, coap_new_optlist(opt, opt_length, buf));
  }
}

/**
 * Decodes percent-encoded characters while copying the string @p seg
 * of size @p length to @p buf. The caller of this function must
 * ensure that the percent-encodings are correct (i.e. the character
 * '%' is always followed by two hex digits. and that @p buf provides
 * sufficient space to hold the result. This function is supposed to
 * be called by make_decoded_option() only.
 *
 * @param seg     The segment to decode and copy.
 * @param length  Length of @p seg.
 * @param buf     The result buffer.
 */
static void
decode_segment(const uint8_t *seg, size_t length, unsigned char *buf) {

    while (length--) 
   {
        if (*seg == '%') 
        {
            *buf = (hexchar_to_dec(seg[1]) << 4) + hexchar_to_dec(seg[2]);

            seg += 2; length -= 2;
        } 
        else 
        {
            *buf = *seg;
        }

        ++buf; ++seg;
    }
}

/**
 * Runs through the given path (or query) segment and checks if
 * percent-encodings are correct. This function returns @c -1 on error
 * or the length of @p s when decoded.
 */
static int check_segment(const uint8_t *s, size_t length)
{
    size_t n = 0;

    while (length) 
    {
        if (*s == '%') 
        {
            if (length < 2 || !(isxdigit(s[1]) && isxdigit(s[2])))
                return -1;

            s += 2;
            length -= 2;
        }
        ++s; ++n; --length;
    }

    return n;
}

/*****************************************************************************************************
Function    : xy_payload_copy
Description : receive coap empty message handler
Input       : text: payload source data address
              buf : payload dst data address
Output      : None
Return      : 1 -TRUE
              0 -FALSE
*****************************************************************************************************/
static bool xy_payload_copy(char *text, int textlen ,coap_string_t *buf)
{
    int len;
    len = check_segment((unsigned char *)text, textlen);

    if (len < 0)
        return 0;

    buf->s = (unsigned char *)coap_malloc(len);
    if (!buf->s)
        return 0;

    buf->length = len;
    decode_segment((unsigned char *)text, textlen, buf->s);
    return 1;
}

/*****************************************************************************************************
Function    : xy_coap_messgae_process_can_quit
Description : Judge whether can quit COAP downlink message process
Input       : ctx:COAP context
Output      : None
Return      : 1 -TRUE
              0 -FALSE
*****************************************************************************************************/
bool xy_coap_messgae_process_can_quit(coap_context_t *ctx)
{
	(void) ctx;

    return (!g_doing_getting_block) ;
}
/*****************************************************************************************************
Function    : xy_coap_check_token
Description : receive coap empty message handler
Input       : received:receive coap pdu
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
static inline int xy_coap_check_token(coap_pdu_t *received) 
{
    return received->token_length == g_token.length &&
        memcmp(received->token, g_token.s,g_token.length) == 0;
}

/*****************************************************************************************************
Function    : xy_coap_ping_handler
Description : receive coap empty message handler
Input       : received:receive coap pdu
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_ping_handler(struct coap_context_t *ctx UNUSED_PARAM,
                coap_session_t *session UNUSED_PARAM,
                coap_pdu_t *received,
                const coap_tid_t id UNUSED_PARAM)
{
    if(COAP_PDU_IS_EMPTY(received))
        append_to_output(received,NULL, 0);
    return;
}

/*****************************************************************************************************
Function    : xy_coap_event_handler
Description : coap event handler
Input       : event:coap event type
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
static int xy_coap_event_handler(coap_context_t *ctx UNUSED_PARAM, coap_event_t event, struct coap_session_t *session UNUSED_PARAM)
{

    switch(event) 
    {
        case COAP_EVENT_DTLS_CLOSED:
        case COAP_EVENT_TCP_CLOSED:
        case COAP_EVENT_SESSION_CLOSED:
            g_quit = 1;
            break;
        default:
            break;
    }
    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_nack_handler
Description : coap abnormal message handler
Input       : reason: abnormal coap message type
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_nack_handler(coap_context_t *context UNUSED_PARAM,
             coap_session_t *session UNUSED_PARAM,
             coap_pdu_t *sent UNUSED_PARAM,
             coap_nack_reason_t reason,
             const coap_tid_t id UNUSED_PARAM) {

    switch(reason) 
    {
        case COAP_NACK_TOO_MANY_RETRIES:
        case COAP_NACK_NOT_DELIVERABLE:
        case COAP_NACK_RST:
        case COAP_NACK_TLS_FAILED:
            g_quit = 1;
            break;
        case COAP_NACK_ICMP_ISSUE:
        default:
            break;
    }
    return;
}

/*****************************************************************************************************
Function    : xy_coap_message_handler
Description : coap receive coap message handler
Input       : ctx: COAP context
              session: COAP session
              sent: last send coap pdu
              received:receive coap pdu
              id:CoAP transaction id
Output      : None
Return      : VOID
*****************************************************************************************************/
static void
xy_coap_message_handler(struct coap_context_t *ctx,
                coap_session_t *session,
                coap_pdu_t *sent,
                coap_pdu_t *received,
                const coap_tid_t id UNUSED_PARAM) {

    coap_pdu_t *pdu = NULL;
    coap_opt_t *block_opt;
    coap_opt_iterator_t opt_iter;
    unsigned char buf[4];
    coap_optlist_t *option;
    size_t len;
    unsigned char *databuf;
    coap_tid_t tid;

    coap_log(LIBCOAP_LOG_INFO, "** process incoming %d.%02d response:\n", (received->code >> 5), received->code & 0x1F);
    if (coap_get_log_level() < LIBCOAP_LOG_DEBUG)
        coap_show_pdu(LIBCOAP_LOG_INFO, received);

	if (coap_get_data(received, &len, &databuf))
		append_to_output(received,databuf, len);
    else
		append_to_output(received,NULL, 0);        

    /* check if this is a response to our original request */
    if (!xy_coap_check_token(received))
    {
        /* drop if this was just some message, or send RST in case of notification */
        if (!sent && (received->type == COAP_MESSAGE_CON ||
                        received->type == COAP_MESSAGE_NON))
            coap_send_rst(session, received);
        return;
    }

    if (received->type == COAP_MESSAGE_RST) 
    {
        coap_log(LIBCOAP_LOG_INFO, "got RST\n");
        return;
    }

  /* output the received data, if any */
    if (COAP_RESPONSE_CLASS(received->code) == 2) 
    {

        /* set obs timer if we have successfully subscribed a resource */
        if (!g_obs_started && coap_check_option(received, LIBCOAP_COAP_OPTION_OBSERVE, &opt_iter)) 
        {
          coap_log(LIBCOAP_LOG_DEBUG,
                   "observation relationship established, set timeout to %d\n",
                    g_obs_seconds);
          g_obs_started = 1;
          g_obs_ms = g_obs_seconds * 1000;
          g_obs_ms_reset = 1;
        }

        /* Got some data, check if fota_block option is set. Behavior is undefined if
         * both, Block1 and Block2 are present. */
        block_opt = coap_check_option(received, LIBCOAP_COAP_OPTION_BLOCK2, &opt_iter);
        if (block_opt) 
        { /* handle Block2 */
            uint16_t blktype = opt_iter.type;

            /* TODO: check if we are looking at the correct fota_block number */

            xy_printf("[king][coap_opt_block_num]%d\n", coap_opt_block_num(block_opt));
            if (coap_opt_block_num(block_opt) == 0) 
            {
                /* See if observe is set in first response */
                coap_check_option(received,
                        LIBCOAP_COAP_OPTION_OBSERVE, &opt_iter) == NULL;
            }
      
            if(COAP_OPT_BLOCK_MORE(block_opt)) 
            {
                /* more bit is set */
                coap_log(LIBCOAP_LOG_INFO, "found the M bit, fota_block size is %u, fota_block nr. %u\n",
                      COAP_OPT_BLOCK_SZX(block_opt),
                      coap_opt_block_num(block_opt));

                /* create pdu with request for next fota_block */
                pdu = xy_coap_new_request(ctx, session, g_method, NULL, NULL, 0); /* first, create bare PDU w/o any option  */
                if ( pdu )
                {
                /* add URI components from optlist */
                    for (option = g_optlist; option; option = option->next ) 
                    {
                        switch (option->number) 
                        {
                            case LIBCOAP_COAP_OPTION_URI_HOST :
                            case LIBCOAP_COAP_OPTION_URI_PORT :
                            case LIBCOAP_COAP_OPTION_URI_PATH :
                            case LIBCOAP_COAP_OPTION_URI_QUERY :
                            coap_add_option(pdu, option->number, option->length,
                                            option->data);
                            break;
                            default:
                            ;     /* skip other options */
                        }
                    }

                    /* finally add updated fota_block option from response, clear M bit */
                    /* blocknr = (blocknr & 0xfffffff7) + 0x10; */
                    coap_log(LIBCOAP_LOG_DEBUG, "query fota_block %d\n",
                           (coap_opt_block_num(block_opt) + 1));
                    coap_add_option(pdu,
                                  blktype,
                                  coap_encode_var_safe(buf, sizeof(buf),
                                         ((coap_opt_block_num(block_opt) + 1) << 4) |
                                          COAP_OPT_BLOCK_SZX(block_opt)), buf);

                    coap_add_data(pdu, g_payload.length, g_payload.s);
                    tid = coap_send(session, pdu);

                    if (tid == COAP_INVALID_TID) 
                    {
                        coap_log(LIBCOAP_LOG_DEBUG, "message_handler: error sending new request\n");
                    } 
                    else 
                    {
                        g_wait_ms = g_wait_seconds * 1000;
                        g_wait_ms_reset = 1;
                        g_doing_getting_block = 1;
                    }

                    return;
                }
            }
            else
            {
                if (g_method == 1)
                {
                    g_flags = 0;
                }
            
                /* Failure of some sort */
                g_doing_getting_block = 0;
                return;
            } 
        }
        else 
        { /* no Block2 option */
            block_opt = coap_check_option(received, LIBCOAP_COAP_OPTION_BLOCK1, &opt_iter);

            if (block_opt) 
            { /* handle Block1 */
                unsigned int szx = COAP_OPT_BLOCK_SZX(block_opt);
                unsigned int num = coap_opt_block_num(block_opt);
                coap_log(LIBCOAP_LOG_DEBUG,
                        "found Block1 option, fota_block size is %u, fota_block nr. %u\n",
                            szx, num);
                if (szx != g_block.szx) 
                {
                    unsigned int bytes_sent = ((g_block.num + 1) << (g_block.szx + 4));
                    if (bytes_sent % (1 << (szx + 4)) == 0) 
                    {
                        /* Recompute the fota_block number of the previous packet given the new fota_block size */
                        num = g_block.num = (bytes_sent >> (szx + 4)) - 1;
                        g_block.szx = szx;
                        coap_log(LIBCOAP_LOG_DEBUG,
                                "new Block1 size is %u, fota_block number %u completed\n",
                                    (1 << (g_block.szx + 4)), g_block.num);
                    } 
                    else 
                    {
                        coap_log(LIBCOAP_LOG_DEBUG, "ignoring request to increase Block1 size, "
                                    "next fota_block is not aligned on requested fota_block size boundary. "
                                    "(%u x %u mod %u = %u != 0)\n",
                                    g_block.num + 1, (1 << (g_block.szx + 4)), (1 << (szx + 4)),
                                    bytes_sent % (1 << (szx + 4)));
                    }
                }

                if (g_last_block1_tid == received->tid) 
                {
                    /*
                    * Duplicate BLOCK1 ACK
                    *
                    * RFCs not clear here, but on a lossy connection, there could
                    * be multiple BLOCK1 ACKs, causing the client to retransmit the
                    * same fota_block multiple times.
                    *
                    * Once a fota_block has been ACKd, there is no need to retransmit it.
                    */
                    return;
                }
                g_last_block1_tid = received->tid;

                if (g_payload.length <= (g_block.num+1) * (1 << (g_block.szx + 4))) 
                {
                    coap_log(LIBCOAP_LOG_DEBUG, "upload ready\n");
                    ready = 1;
                    return;
                }

                /* create pdu with request for next fota_block */
                pdu = xy_coap_new_request(ctx, session, g_method, NULL, NULL, 0); /* first, create bare PDU w/o any option  */
                if (pdu) 
                {
                    /* add URI components from optlist */
                    for (option = g_optlist; option; option = option->next ) 
                    {
                        switch (option->number) 
                        {
                            case LIBCOAP_COAP_OPTION_URI_HOST :
                            case LIBCOAP_COAP_OPTION_URI_PORT :
                            case LIBCOAP_COAP_OPTION_URI_PATH :
                            case LIBCOAP_COAP_OPTION_CONTENT_FORMAT :
                            case LIBCOAP_COAP_OPTION_URI_QUERY :
                                coap_add_option(pdu, option->number, option->length,
                                        option->data);
                                break;
                            default:
                                ;     /* skip other options */
                        }
                    }

                    /* finally add updated fota_block option from response, clear M bit */
                    /* blocknr = (blocknr & 0xfffffff7) + 0x10; */
                    g_block.num = num + 1;
                    g_block.m = ((g_block.num+1) * (1 << (g_block.szx + 4)) < g_payload.length);

                    coap_log(LIBCOAP_LOG_DEBUG, "send fota_block %d\n", g_block.num);
                    coap_add_option(pdu,
                    	    LIBCOAP_COAP_OPTION_BLOCK1,
                                  coap_encode_var_safe(buf, sizeof(buf),
                                  (g_block.num << 4) | (g_block.m << 3) | g_block.szx), buf);

                    coap_add_option(pdu,
                            LIBCOAP_COAP_OPTION_SIZE1,
                                coap_encode_var_safe(buf, sizeof(buf), g_payload.length),
                                buf);

                    coap_add_block(pdu,
                                 g_payload.length,
                                 g_payload.s,
                                 g_block.num,
                                 g_block.szx);
                    if (coap_get_log_level() < LIBCOAP_LOG_DEBUG)
                        coap_show_pdu(LIBCOAP_LOG_INFO, pdu);

                    tid = coap_send(session, pdu);

                    if (tid == COAP_INVALID_TID) 
                    {
                        coap_log(LIBCOAP_LOG_DEBUG, "message_handler: error sending new request\n");
                    } 
                    else 
                    {
                        g_wait_ms = g_wait_seconds * 1000;
                        g_wait_ms_reset = 1;
                    }

                    return;
                }
            } 
            else 
            {
                /* There is no fota_block option set, just read the data and we are done. */
            }
        }
    } 
    else 
    {      /* no 2.05 */

        /* check if an error was signaled and output payload if so */
        if (COAP_RESPONSE_CLASS(received->code) >= 4) 
        {
            xy_printf("%d.%02d", (received->code >> 5), received->code & 0x1F);
            if (coap_get_data(received, &len, &databuf)) 
            {
                while(len--)
                    xy_printf("%c", *databuf++);
            }

        }
        coap_log(LIBCOAP_LOG_DEBUG, "no 2.05");
    }

    /* any pdu that has been created in this function must be sent by now */
    xy_assert(pdu == NULL);

    /* our job is done, we can exit at any time */
    ready = coap_check_option(received, LIBCOAP_COAP_OPTION_OBSERVE, &opt_iter) == NULL;
}

/*****************************************************************************************************
Function    : xy_log_handler
Description : coap log handler
Input       : level: log level
              message: COAP message
Output      : None
Return      : VOID
*****************************************************************************************************/
void xy_log_handler (coap_log_t level, const char *message)
{
	(void) level;

    xy_printf(message);
}

/*****************************************************************************************************
Function    : xy_coap_recv
Description : receive coap callback function
Input       : arg: coap session
              upcb:
              p:    received datagram packet buffer
              addr: UDP datagram's source address
              port: UDP datagram's port
Output      : None
Return      : VOID
*****************************************************************************************************/
static void xy_coap_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    coap_pdu_t *pdu = NULL;
    coap_session_t *session = (coap_session_t*)arg;
    recvdata_msg_t *msg =NULL;

    (void) upcb;
    (void) addr;
    (void) port;

    pdu = coap_pdu_from_pbuf(p);
    if (!pdu)
    goto error;

    if (!coap_pdu_parse(session->proto, p->payload, p->len, pdu)) 
    {
        goto error;
    }

	if(xy_coap_recvdata_q != NULL)
	{
	    msg = xy_zalloc(sizeof(recvdata_msg_t));
	    msg->pdu = pdu;
        osMessageQueuePut(xy_coap_recvdata_q, (void *)(&msg), 0, osWaitForever);
	}
	else
		xy_printf("cdp_opencpu_demo_q not create!");

    return;

error:
    /* FIXME: send back RST? */
    if (pdu) coap_delete_pdu(pdu);
    return;
}

/*****************************************************************************************************
Function    : xy_coap_client_create
Description : create session and register callback func
Input       : ipaddr: destination address port
              port: destination port
Output      : None
Return      : !0 -success
               0 -failure
*****************************************************************************************************/
coap_context_t *xy_coap_client_create(char *ipaddr,unsigned short port)
{
    int rc = -1;
    coap_context_t  *ctx = NULL;
    coap_session_t  *session = NULL;
    coap_address_t  bind_addr;
    coap_address_t  dst;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_INET, SOCK_DGRAM, 0, 0, NULL, NULL, NULL};

    memset(&bind_addr,0,sizeof(coap_address_t));
    memset(&dst,0,sizeof(coap_address_t));

    // init demo queue
    if(NULL == xy_coap_recvdata_q)
        xy_coap_recvdata_q = osMessageQueueNew(16, sizeof(void *));
    else
        xy_queue_clear(xy_coap_recvdata_q);

    //coap init
    coap_startup();
    coap_set_log_handler(xy_log_handler);
    coap_set_log_level(coap_api_loglevel);

    if ((rc = getaddrinfo(ipaddr, NULL, &hints, &result)) == 0)
    {
        struct addrinfo* res = result;

        /* prefer ip4 addresses */
        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

        if (result->ai_family == AF_INET)
        {
			dst.addr.u_addr.ip4.addr=((struct sockaddr_in*)(result->ai_addr))->sin_addr.s_addr;
			dst.addr.type = IPADDR_TYPE_V4;
			dst.port = port;

        }
        else
            rc = -1;

        freeaddrinfo(result);
    }


	if(rc != 0)
		goto end;

    //creat coap context
    ctx = coap_new_context( NULL );
    if ( !ctx ) {
        coap_log( LOG_EMERG, "cannot create context\n" );
        goto end;
    }

    coap_context_set_keepalive(ctx, ping_timeout);

    xy_printf("[COAP]dst.addr:%d dst.port:%d", dst.addr, dst.port);

    session = coap_new_client_session(ctx, &bind_addr, &dst, COAP_PROTO_UDP);
    
    udp_recv(session->sock.pcb, xy_coap_recv, session);
    
    //set coap callback
    coap_register_option(ctx, LIBCOAP_COAP_OPTION_BLOCK2);
    coap_register_response_handler(ctx, xy_coap_message_handler);
    coap_register_event_handler(ctx, xy_coap_event_handler);
    coap_register_nack_handler(ctx, xy_coap_nack_handler);
    coap_register_ping_handler(ctx, xy_coap_ping_handler);
    
    coap_pkt_sem = osSemaphoreNew(0xFFFF, 0);//register semaphore
    return ctx;

end: 
    if(NULL != xy_coap_recvdata_q)
    {
        xy_coap_recvqueue_delete(xy_coap_recvdata_q);
        xy_coap_recvdata_q = NULL;
    }
    coap_session_release( session );
    coap_free_context( ctx );
    coap_cleanup();
    return 0;  
}

/*****************************************************************************************************
Function    : xy_coap_client_delete
Description : pack option information to COAP message
Input       : ctx: COAP context
Output      : None
Return      : void
*****************************************************************************************************/
void xy_coap_client_delete(coap_context_t *ctx)
{
    coap_session_release( ctx->sessions );
    coap_free_context( ctx );
    coap_cleanup();

    osSemaphoreDelete(coap_pkt_sem);
    coap_pkt_sem = NULL;
    return;
}

/*****************************************************************************************************
Function    : xy_config_coap_head
Description : config COAP message header
Input       : msgid: message id
              token: token in COAP message
              tkl: length of token
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_config_coap_head(int msgid, char *token, int tkl)
{
    g_msgid = msgid;

    g_token.length = min(sizeof(g_token_data), tkl);
    if (g_token.length > 0) 
    {
        memcpy((char *)g_token.s, token, g_token.length);
    }
    if(g_msgid == 0)
        coap_msgid_random_flag =1;
    else
        coap_msgid_random_flag =0;

    if(g_token.length == 0)
        coap_token_random_flag =1;
    else
        coap_token_random_flag =0;
    return 0;
}

/*****************************************************************************************************
Function    : xy_config_coap_option
Description : pack option information to COAP message
Input       : opt: option type
              opt_value: option value
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_config_coap_option(int opt, char *opt_value)
{   
    switch(opt)
    {
        case LIBCOAP_COAP_OPTION_IF_MATCH:
            break;
        case LIBCOAP_COAP_OPTION_URI_HOST:
            if(strlen(opt_value) < 1 || strlen(opt_value) > 255)
                return -1;
            xy_coap_uri_host_set(opt_value, strlen(opt_value));
            break;
        case LIBCOAP_COAP_OPTION_ETAG:
            break;
        case LIBCOAP_COAP_OPTION_IF_NONE_MATCH:
            break;
        case LIBCOAP_COAP_OPTION_OBSERVE:
            if(atoi(opt_value) != 1 && atoi(opt_value) != 0)
                return -1;
            xy_coap_observe_set(opt_value, LIBCOAP_COAP_OPTION_OBSERVE);
            break;
        case LIBCOAP_COAP_OPTION_URI_PORT:
            if(atoi(opt_value) > 0xFFFF)
                return -1;
            xy_coap_uri_port_set(opt_value, LIBCOAP_COAP_OPTION_URI_PORT);
            break;
        case LIBCOAP_COAP_OPTION_LOCATION_PATH:
            break;
        case LIBCOAP_COAP_OPTION_URI_PATH:
            if(strlen(opt_value) > 255)
                return -1;
            xy_coap_path_set(opt_value, strlen(opt_value));
            break;
        case LIBCOAP_COAP_OPTION_CONTENT_TYPE:
            xy_content_type_set(opt_value, LIBCOAP_COAP_OPTION_CONTENT_TYPE);
            break;
        case LIBCOAP_COAP_OPTION_MAXAGE:
            break;
        case LIBCOAP_COAP_OPTION_URI_QUERY:
            if(strlen(opt_value) > 255)
                return -1;
            xy_coap_uri_query_set(opt_value, strlen(opt_value));
            break;
        case LIBCOAP_COAP_OPTION_ACCEPT:
            break;
        case LIBCOAP_COAP_OPTION_PROXY_URI:
            if(strlen(opt_value) < 1 || strlen(opt_value) > 1034)
                return -1;
            xy_coap_proxy_uri_set(opt_value, strlen(opt_value));
            break;
        case LIBCOAP_COAP_OPTION_SIZE1:
            if(atoi(opt_value) > 0xFFFFFFFF)
                return -1;
            xy_coap_option_size1_set(opt_value, LIBCOAP_COAP_OPTION_SIZE1);
            break;
        case LIBCOAP_COAP_OPTION_BLOCK1:
            xy_coap_blocksize(0, 512);
        	break;
        default:
            break;
    }

    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_send
Description : send and receive COAP message
Input       : ctx: COAP context
              method: request method code
              payload: payload in COAP message
              payload_len:payload length
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_coap_send(coap_context_t *ctx, char *method, char* payload,int payload_len)
{
    static coap_pdu_t  *pdu = NULL;
    static coap_pdu_t  *recv_pdu = NULL;
    recvdata_msg_t *temp_msg =NULL;
    coap_tick_t now;

    //set msgid
    if((g_msgid != 0) && (ctx->sessions->tx_mid == 0))
        ctx->sessions->tx_mid = (u16_t)g_msgid;
    
    //set COAP method
    g_method = xy_coap_method_get(method);

    //set payload
    if (!xy_payload_copy(payload, &g_payload,payload_len))
        g_payload.length = 0;
    
    /* set block option if need*/
    if (g_flags & FLAGS_BLOCK)
        xy_coap_blocksize_set();

    //set coap header
    if (! (pdu = xy_coap_new_request(ctx, ctx->sessions, g_method, &g_optlist, g_payload.s, g_payload.length))) 
    {
        goto finish;
    }

    if (coap_get_log_level() < LIBCOAP_LOG_DEBUG)
        coap_show_pdu(LIBCOAP_LOG_INFO, pdu);

    
    xy_printf("[COAP]pdu->pbuf:%x", pdu->pbuf);
    coap_send(ctx->sessions, pdu);

    g_wait_ms = g_wait_seconds * 1000;
    while (!g_quit && !(!ready && !g_doing_getting_block && coap_can_exit(ctx)) ) 
    {  
        if(xy_coap_recvdata_q != NULL)
            osMessageQueueGet(xy_coap_recvdata_q, (void *)(&temp_msg), NULL, osWaitForever);

        recv_pdu = temp_msg->pdu;
        if (!recv_pdu)
            goto error;

        coap_ticks(&now);
        LWIP_ASSERT("Proto not supported for LWIP", COAP_PROTO_NOT_RELIABLE(ctx->sessions->proto));
        coap_dispatch(ctx, ctx->sessions, recv_pdu);

        if(temp_msg != NULL)
        {
            if(temp_msg->pdu)
            {
                xy_free(temp_msg->pdu);
                temp_msg->pdu = NULL;
            }
            xy_free(temp_msg);
            temp_msg = NULL;
        }

        coap_delete_pdu(pdu);
        g_quit = 1;

        error:
        /* FIXME: send back RST? */
        if (pdu) coap_delete_pdu(pdu);
    }
    xy_printf("[COAP]g_quit:%dready:%d %d", g_quit, ready, g_doing_getting_block);
    
finish:
    coap_delete_optlist(g_optlist);
    g_optlist = NULL;

    if(g_payload.length != 0)
        coap_free(g_payload.s);

    return 0;

}

/*****************************************************************************************************
Function    : xy_coap_pkt_send
Description : package and send COAP message
Input       : ctx: COAP context
              method: method field in COAP message
              payload:data in COAP message
              payload_len:payload length
              synflag:synchronization or asynchronization(0:asynchronization,1:synchronization)
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_coap_pkt_send(coap_context_t *ctx,char *type, char *method, char* payload,int payload_len,int synflag)
{
    static coap_pdu_t  *pdu = NULL;

    //set designated msgid
    if(coap_msgid_random_flag == 0)
        ctx->sessions->tx_mid = (u16_t)g_msgid -1 ;

    //set random token
    if(coap_token_random_flag == 1)
    {
        sprintf(g_token.s, "%x", ctx->sessions->tx_mid );
        g_token.length =strlen(g_token.s);
    }

    //set COAP method
    g_method = xy_coap_method_get(method);
    if(g_method == 8)
        return -1;

    //set COAP type
    g_msgtype = xy_coap_type_get(type);
    if(g_msgtype == 4)
        return -1;

    //set payload
    if (payload == NULL || !xy_payload_copy(payload,payload_len, &g_payload))
        g_payload.length = 0;

    /* set block option if need*/
    if (g_flags & FLAGS_BLOCK)
        xy_coap_blocksize_set();

    //set coap header
    if (! (pdu = xy_coap_new_request(ctx, ctx->sessions, g_method, &g_optlist, g_payload.s, g_payload.length)))
    {
        if(g_payload.length != 0)
            coap_free(g_payload.s);
        return -1;
    }

    if (coap_get_log_level() < LIBCOAP_LOG_DEBUG)
        coap_show_pdu(LIBCOAP_LOG_INFO, pdu);

    xy_printf("[COAP]pdu->pbuf:%x", pdu->pbuf);
    coap_send(ctx->sessions, pdu);

    if(g_payload.length != 0)
        coap_free(g_payload.s);

    if(synflag)
    {
        xy_printf("[COAP] Semaphore_Take");
        if (osSemaphoreAcquire(coap_pkt_sem, 100000) != osOK)
            return -1;
    }

    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_asy_send
Description : send and receive COAP message,synchronous mode
Input       : ctx: COAP context
              method: method field in COAP message
              payload:data in COAP message
              payload_len:payload length
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_coap_asy_send(coap_context_t *ctx,char *type, char *method, char* payload,int payload_len)
{
    return xy_coap_pkt_send(ctx,type,method,payload,payload_len,0);
}
/*****************************************************************************************************
Function    : xy_coap_only_receive
Description : receive and process COAP message
Input       : ctx: COAP context
              quitflag: quit receive message thread flag
              synflag:synchronization or asynchronization(0:asynchronization,1:synchronization)
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_coap_pkt_receive(coap_context_t *ctx,int *quitflag,int synflag)
{
    static coap_pdu_t  *recv_pdu = NULL;
    recvdata_msg_t *temp_msg =NULL;
    coap_tick_t now;
    xy_printf("[COAP] receive %d,%d,%d",g_quit,ready,g_doing_getting_block);
    while (*quitflag == 0)
    {
        xy_printf("[COAP] loop for receive");

        if(xy_coap_recvdata_q != NULL)
        {
           osMessageQueueGet(xy_coap_recvdata_q, (void *)(&temp_msg), NULL, 2000);
           if(temp_msg == NULL || temp_msg->pdu == NULL)
                continue;
        }
        else
            goto exit;

        recv_pdu = temp_msg->pdu;
        if (!recv_pdu)
            continue;

        xy_printf("[COAP] coap protol =%d",ctx->sessions->proto);

        if(synflag)
            osSemaphoreRelease(coap_pkt_sem);

        coap_ticks(&now);
        if(!COAP_PROTO_NOT_RELIABLE(ctx->sessions->proto))
            continue;

        coap_dispatch(ctx, ctx->sessions, recv_pdu);

        if(temp_msg != NULL)
        {
            if(temp_msg->pdu)
            {
                coap_delete_pdu(temp_msg->pdu);
                temp_msg->pdu = NULL;
            }
            xy_free(temp_msg);
            temp_msg = NULL;
        }
    }
    xy_printf("[COAP]g_quit:%dready:%d %d", g_quit, ready, g_doing_getting_block);

exit:
    if(NULL != xy_coap_recvdata_q)
    {
        xy_coap_recvqueue_delete(xy_coap_recvdata_q);
        xy_coap_recvdata_q = NULL;
    }
    return 0;
}

/*****************************************************************************************************
Function    : xy_coap_asy_receive
Description : receive COAP message,asynchronous mode
Input       : ctx: COAP context
              method: method field in COAP message
              payload:data in COAP message
Output      : None
Return      : 0 -success
             -1 -failure
*****************************************************************************************************/
int xy_coap_asy_receive(coap_context_t *ctx,int *quitflag)
{
    return xy_coap_pkt_receive(ctx,quitflag,0);
}

void xy_coap_recvqueue_delete(osMessageQueueId_t pMsgQueueId)
{
	recvdata_msg_t *temp_msg = NULL;
	
    xy_assert(pMsgQueueId != NULL);

    while(osMessageQueueGet(pMsgQueueId, (void *)(&temp_msg), NULL, 0) == osOK)
    {
    	if(temp_msg->pdu)
        {
            coap_delete_pdu(temp_msg->pdu);
            temp_msg->pdu = NULL;
        }
    	xy_free(temp_msg);
    }
	osMessageQueueDelete(pMsgQueueId);
}
#endif
