#include "ulib.h"
#include "sys_call_entry.h"
#include "string.h"

int printf(const char *fmt, ...)
{
	int rc;
	char buf[256];
	va_list arg; 

	arg = (va_list)((char *)(&fmt) + sizeof(fmt));
	rc = vsprintf(buf, fmt, arg);
	write(buf, rc);
	return rc;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	char *p;
	char tmp[256];
	va_list p_next_arg = args;

	for (p=buf; *fmt; fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}
		fmt++;
		switch (*fmt) {
			case 'x':
				itoa(tmp, *((int *)p_next_arg));
				strcmp(p, tmp);
				p_next_arg += 4;
				p += strlen(tmp);
				break;
			case 's':
				break;
			default:
				break;
		}
	}
	return (p - buf);
}

char * itoa(char * str, int num)
{
	char *	p = str;
	char	ch;
	int	i;
	int	flag = 0;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = 1;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}
