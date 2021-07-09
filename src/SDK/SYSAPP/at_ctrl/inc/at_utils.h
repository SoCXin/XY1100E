#pragma once

/*******************************************************************************
 *                             Macro definitions                               *
 ******************************************************************************/
#define DIFF_VALUE ('a'-'A')

/*******************************************************************************
 *                             Type definitions                                *
 ******************************************************************************/
typedef struct
{
	char ch;          //转义字符 如\a 中的a
	char esc_ch;      //转义字符对应的ASCII码
} esc_item_t;

/*******************************************************************************
 *                       Global function declarations                          *
 ******************************************************************************/
 /* convert HEX string (0XA358)  to hex value*/
int hexstr2int(char *hex, int *value);
int is_digit_str(char *str);
char *find_first_print_char(char *at_str, int len);
char *at_strstr2(char *source, char *substr);
char *at_strstr(char *source, char *substr);
char *at_strnchr(char *s, int c, int n);
char *at_strnstr(char *source, char *substr, unsigned int n);
int at_strcasecmp(const char *s1, const char *s2);
int at_strncasecmp(const char *s1, const char *s2, int n);

/**
 * @brief judge response is error info or not
 * @param str [IN] at cmd data
 * @param err_no [OUT] error number,see @AT_XY_ERR
 * @note
 */
int at_is_rsp_err(char *str, int *err_no);

/**
 * @brief judge response is OK info
 * @note
 */
int is_ok_at(char *str);

/**
 * @brief judge received info is result or not
 * @note
 */
int is_result_at(char *str);

/**
 * @brief create an OK info
 * @note
 */
char *at_ok_build();

/**
 * @brief 解析字符串中的转义字符，并转换为对应的ASCII码
 * @param input [IN]/[OUT]
 * @note
 */
void format_escape_char(char *input);

/**
 * @brief Determines if the input character is an AT character
 * @param input_char [IN] input character
 * @return return 1 if the character is an AT character, otherise return 0;
 * @note 
 */
int is_at_char(char input_char);

/**
 * @brief 获取字符串中下一个双引号位置，排除\"转义字符干扰
 * @param data [IN] 输入字符串
 * @return 返回下一个双引号位置，若未找到，返回NULL
 * @note
 */
char *find_next_double_quato(char *data);

/**
 * @brief 将AT命令中的小写字母转为大写字母
 * @param at_cmd [IN] input character
 * @return return 1 表示转换成功, 否则表示失败
 * @note 
 */
int vary_to_uppercase(char *at_cmd);
