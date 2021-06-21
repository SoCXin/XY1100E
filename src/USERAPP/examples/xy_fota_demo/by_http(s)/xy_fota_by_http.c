#if  HTTP_VER
#include "xy_http_client.h"

#include "at_http.h"
#include "http_api.h"
#if XY_FOTA
#include "xy_fota.h"
#endif
#include "xy_fota_by_http.h"

#define XYDELTA_URL          "/fota/xyDelta"                    //"/users/f"
#define HTTP_CONTENT_LENGTH  "Content-Length: "
#define SERVER_HOST          "https://www.gking.site:444" //"http://139.224.131.190:3000"

#define DOWNLOAD_UNIT_BYTES    1024

//下载差分文件的信息
download_info_struct g_download_info;
ota_context_t *g_http_fota_context = NULL;


//HTTP操作的相关数组
static fota_http_req_t g_http_req[2] = {
    {HTTP_METHOD_HEAD , XYDELTA_URL},
    {HTTP_METHOD_GET , XYDELTA_URL}
};

//HTTPS服务器证书，用于认证
static char *sever_cert ="2D2D2D2D2D424547494E2043455254494649434154452D2D2D2D2D0D0A4D494945716A434341354B674177494241674951416E6D7352597642736B57722B5942547A5379627354414E42676B71686B69473977304241517346414442680D0A4D517377435159445651514745774A56557A45564D424D474131554543684D4D52476C6E61554E6C636E51675357356A4D526B77467759445651514C457842330D0A643363755A476C6E61574E6C636E5175593239744D5341774867594456515144457864456157647051325679644342486247396959577767556D3976644342440D0A51544165467730784E7A45784D6A63784D6A51324D544261467730794E7A45784D6A63784D6A51324D5442614D473478437A414A42674E5642415954416C56540D0A4D525577457759445651514B4577784561576470513256796443424A626D4D784754415842674E5642417354454864336479356B61576470593256796443356A0D0A623230784C54417242674E5642414D544A45567559334A35634852706232346752585A6C636E6C33614756795A5342455669425554464D67513045674C5342480D0A4D544343415349774451594A4B6F5A496876634E4151454242514144676745504144434341516F4367674542414C50655036776B6162343164795168366D4B630D0A6F487174336A52497857354D447666395179694F5237566646774B363536657330554669496237344E3970526E747A46315567597A4447753370705A564D646F0D0A6C6278686D36645753394F4B2F6C4665684B4E54304F59493961716B36462B55376341366A7853432B6944425058776446347273334B5279703361516E36706A0D0A70703179723749423659347A76373245652F506C5A2F36724B36496E433657704B306E50564F5952376E3969447550653145344978554D42482F5433332B33680D0A79754833647666676957554F556B6A64704D627978582B584E6C6535754549697942736934497662635443683872756966434969356D44586B5A726E4D54386E0D0A77665943563676366B4464586B626747524C4B735234707563624A74624B71496B554778755A49327437706665774B5263356E5765637644425A66332B70314D0D0A704138434177454141614F43415538776767464C4D42304741315564446751574242525664452B79636B2F31594C70513064666D5556796141596361317A41660D0A42674E5648534D454744415767425144336C41315674464D753262776F2B496247384F58736A33525654414F42674E56485138424166384542414D43415959770D0A485159445652306C42425977464159494B7759424251554841774547434373474151554642774D434D42494741315564457745422F7751494D415942416638430D0A415141774E4159494B77594242515548415145454B44416D4D4351474343734741515546427A41426868686F644852774F69387662324E7A6343356B615764700D0A593256796443356A623230775167594456523066424473774F5441336F4457674D3459786148523063446F764C324E7962444D755A476C6E61574E6C636E51750D0A593239744C3052705A326C445A584A3052327876596D4673556D397664454E424C6D4E796244424D42674E5648534145525442444D44634743574347534147470D0A2F577742416A41714D4367474343734741515546427749424668786F64485277637A6F764C3364336479356B61576470593256796443356A62323076513142540D0A4D416747426D6542444145434154414E42676B71686B6947397730424151734641414F43415145414B334770362F6147713761425A7378662F6F512B54442F420D0A5377573341553445544B2B475166326B467A595A6B6279355346724864506F6D756E783248427A5669556368476F6F66476767376748573057334D6C514158570D0A4D3072354C5576537463723832514457594E5061557934746143516D79614A2B56422B3677784873745369674F6C534E463261367667347267657869786569560D0A34595342303359717032743354655A484D394553666B757337346E51795737705247657A6A2B54433434784361674351514F7A7A4E6D7A45415032536E43724A0D0A734E4532447052564D6E4C384A36784252646A6D4F7343334E366351754B755258627A427956426A437141413874314C30492B3977584A65724C507945726A790D0A724D4B576142464C6D664B2F41484E46345A69687750474F633777365548637A425A58483552467A4A4E6E77772B576E4B755450493048666E5648386C673D3D0D0A2D2D2D2D2D454E442043455254494649434154452D2D2D2D2D";
//HTTPS客户端证书，可以为空
static char *client_cert=NULL;
static char *client_pk = NULL;
//HTTP报文的header和content
static char *header = "6170692D6B65793A4A4459694B794B6669344934734F4657654A734934533343626C303D0d0a";
static char *content = "7b2252504d223a32327d";;

static void get_http_header(char *out_buf, int len)
{
    unsigned char *signptr = 0;

    if (out_buf == NULL)
    {
        xy_printf("[XY_FOTA]get_http_header:out buffer should not null!");
        return;
    }

    memset(out_buf, 0x00, len);

    xy_printf("[XY_FOTA]download index: %d, total: %d.", g_download_info.download_unit_index, g_download_info.download_unit_num);
    sprintf((char *)out_buf, "Range: bytes=%d-%d\r\n", 
            g_download_info.download_unit_index * DOWNLOAD_UNIT_BYTES,
            (g_download_info.download_unit_index+1) * DOWNLOAD_UNIT_BYTES - 1);

}

static int http_header_parse(char *data)
{
    char *signptr = 0;
    int size_str_len = 0;
    char size_str[8];
    if (data == NULL)
    {
        xy_printf("[XY_FOTA]http_header_parse:out buffer should not null!");
        return;
    }

    signptr = strstr(data, "\r\n");
    size_str_len = signptr - data - strlen(HTTP_CONTENT_LENGTH);
    memset(size_str, 0x00,8);
    memcpy(size_str, data + strlen(HTTP_CONTENT_LENGTH), size_str_len);
    
    return atoi(size_str);
}

int get_xyDelta_size(xy_http_method_e protocol)
{
    int http_id = 0;//http的id
    char header[128];
    char *recv_header_buf = NULL;
    unsigned int recv_header_len = 0;
    int delta_size = 0;//http的id
    
    /* host填写时必须带前缀http:// 或者https:// */
    /* 用户名 密码 选填*/
    http_id = xy_http_create(SERVER_HOST, NULL, NULL);
    if(http_id == XY_ERR)
        xy_assert(0);
    
    if(protocol == BY_HTTPS)
    {
        /* HTTPS：服务器证书必填 */
        if(xy_http_cfg(http_id, SEVER_CERT,sever_cert, HEX_CHARACTER))
            xy_assert(0);

        /* 建议 客户端证书可以不填：此时作单向验证  */
        /*if(xy_http_cfg(http_id, CLIENT_CERT,client_cert, HEX_CHARACTER))
            xy_assert(0);*/

        /* 建议 客户端证书可以不填：此时作单向验证  */
        /*if(xy_http_cfg(http_id, CLIENT_PK,client_pk, HEX_CHARACTER))
            xy_assert(0);*/
    }

    
    /* 增加用户header， HEX_CHARACTER模式：16进制传输， 也可选择ESCAPE_MECHANISM： 字符串传输*/
    /* 若新增多条header：应在header中输入\r\n 表示回车换行*/
    if(xy_http_header(http_id, header, HEX_CHARACTER))
        xy_assert(0);

    /* 增加content， HEX_CHARACTER模式：16进制传输， 也可选择ESCAPE_MECHANISM： 字符串传输*/
    if(xy_http_content(http_id, content, HEX_CHARACTER))
        xy_assert(0);
    
    /* 等待驻网成功*/
    while(xy_wait_tcpip_ok(10) != XY_OK);

    /* 建立socket连接，每次请求前都应先调用此接口*/
    if(xy_http_connect(http_id))
        xy_assert(0);

    /* 发起request请求， method可选*/
    if(xy_http_request(http_id, g_http_req[0].method, g_http_req[0].path))
        xy_assert(0);

more_header:
    recv_header_buf = xy_zalloc(1024);
    /* 接受header，HTTP_FOREVER 表示永久等待，可选择其他时间（单位ms） */
    recv_header_len = xy_http_recv_header(http_id , recv_header_buf, 1024, HTTP_FOREVER);
    if(recv_header_len > 0)
    {
        /* 此处处理接收到的header，查找差分文件的大小*/
        delta_size =  http_header_parse(strstr(recv_header_buf, HTTP_CONTENT_LENGTH));
    }
    xy_free(recv_header_buf);

    /* 查询是否还有header待接收*/
    if( header_recvlist_empty(http_id) != XY_TRUE )
    {
        goto more_header;
    }
    xy_http_close(http_id);
    
    return delta_size;
}


int get_fota_data(xy_http_method_e protocol, char *out_buf, int len)
{
    int http_id = 0;//http的id
    char send_header[128];
    char *recv_buf = NULL;
    int recv_len = 0;
    int total_len = 0;
    /* host填写时必须带前缀http:// 或者https:// */
    /* 用户名 密码 选填*/
    http_id = xy_http_create(SERVER_HOST, NULL, NULL);
    if(http_id == XY_ERR)
        xy_assert(0);
    
    if(protocol == BY_HTTPS)
    {
        /* HTTPS：服务器证书必填 */
        if(xy_http_cfg(http_id, SEVER_CERT,sever_cert, HEX_CHARACTER))
            xy_assert(0);

        /* 建议 客户端证书可以不填：此时作单向验证  */
        /*if(xy_http_cfg(http_id, CLIENT_CERT,client_cert, HEX_CHARACTER))
            xy_assert(0);*/

        /* 建议 客户端证书可以不填：此时作单向验证  */
        /*if(xy_http_cfg(http_id, CLIENT_PK,client_pk, HEX_CHARACTER))
            xy_assert(0);*/
    }

    /* 增加用户header， HEX_CHARACTER模式：16进制传输， 也可选择ESCAPE_MECHANISM： 字符串传输*/
    /* 若新增多条header：应在header中输入\r\n 表示回车换行*/
    get_http_header(send_header, 128);
    if(xy_http_header(http_id, send_header, ESCAPE_MECHANISM))
        xy_assert(0);

    /* 增加content， HEX_CHARACTER模式：16进制传输， 也可选择ESCAPE_MECHANISM： 字符串传输*/
    if(xy_http_content(http_id, content, HEX_CHARACTER))
        xy_assert(0);
    
    /* 等待驻网成功*/
    while(xy_wait_tcpip_ok(10) != XY_OK);

    /* 建立socket连接，每次请求前都应先调用此接口*/
    if(xy_http_connect(http_id))
        xy_assert(0);

    /* 发起request请求， method可选*/
    if(xy_http_request(http_id, g_http_req[1].method, g_http_req[1].path))
        xy_assert(0);


    if(HTTP_METHOD_HEAD != g_http_req[1].method)
    {
more_body:
        recv_buf = xy_zalloc(DOWNLOAD_UNIT_BYTES);
        /* 接受body，HTTP_FOREVER 表示永久等待，可选择其他时间（单位ms） */
        recv_len = xy_http_recv_body(http_id , recv_buf, DOWNLOAD_UNIT_BYTES, 10000);		// delay 100ms for data collection
        if(recv_len > 0 )
        {
            memcpy(out_buf + total_len, recv_buf, recv_len);
            total_len += recv_len;
        }

        xy_free(recv_buf); 

        /* 查询是否还有body待接收*/
        if( body_recvlist_empty(http_id) != XY_TRUE )
        {
        goto more_body;
        }
    }
    xy_http_close(http_id);

    return total_len;
}

static void  xy_fota_http_demo(void *args)
{
    int recv_len = 0;
    int total_len = 0;
    char data_buf[DOWNLOAD_UNIT_BYTES];
    //FOTA任务初始化
    g_http_fota_context = ota_update_Init();

    //获取差分包信息
    g_download_info.download_delta_size = get_xyDelta_size(BY_HTTPS);
    if(g_download_info.download_delta_size > 0)
    {
        if(g_download_info.download_delta_size % DOWNLOAD_UNIT_BYTES)
            g_download_info.download_unit_num = (g_download_info.download_delta_size/DOWNLOAD_UNIT_BYTES) + 1;
        else
            g_download_info.download_unit_num = (g_download_info.download_delta_size/DOWNLOAD_UNIT_BYTES);
    }

    //获取差分包
    for(; g_download_info.download_unit_index < g_download_info.download_unit_num; g_download_info.download_unit_index++)
    {
        recv_len = get_fota_data(BY_HTTPS, data_buf, DOWNLOAD_UNIT_BYTES);
        if(recv_len)
        {
            if(ota_downlink_packet_proc(g_http_fota_context, data_buf, recv_len))
                goto error;
            
            total_len += recv_len;
            xy_printf("[XY_FOTA]xy_fota_http_demo:get delta size %d %d", recv_len, total_len);
        }
        else
            g_download_info.download_unit_index--;
    }

    if(total_len != g_download_info.download_delta_size)
    {
        xy_printf("[XY_FOTA]xy_fota_http_demo:get delta size error ");
        goto error;
    }

    if(ota_delta_check_sync(g_http_fota_context))
        goto error;
    ota_update_start(g_http_fota_context);

error:
    memset(&g_download_info, 0x00, sizeof(g_download_info));
    ota_update_deInit(g_http_fota_context);
    osThreadExit();
    return;
}



void xy_fota_http_demo_init()
{
    osThreadNew((osThreadFunc_t)(xy_fota_http_demo),NULL,"fota_https_task_demo",0x3000,osPriorityNormal);
}
#endif
