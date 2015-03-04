typedef char * va_list;

extern int printf(const char *fmt, ...);
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern char * itoa(char *str, int num);
