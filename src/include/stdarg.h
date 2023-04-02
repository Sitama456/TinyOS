/*** 
 * @Author: mzm
 * @Date: 2023-04-02 00:04:37
 * @LastEditTime: 2023-04-02 16:22:35
 * @LastEditors: mzm
 * @Description: 
 * @FilePath: /TinyOS/src/include/stdarg.h
 * @
 */
#ifndef _STDARG_H_
#define _STDARG_H_

typedef char *va_list;

#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *))
#define va_arg(ap, t) (*(t *)((ap += sizeof(char *)) - sizeof(char *)))
#define va_end(ap) (ap = (va_list)0)


#endif // _STDARG_H_