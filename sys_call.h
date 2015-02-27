#define SYS_CALL_NUM	50
typedef void* sys_call_handler_t;
extern sys_call_handler_t sys_call_tbl[SYS_CALL_NUM];
extern void init_sys_call_tbl();
