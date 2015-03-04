[SECTION .text]
; 导出函数
global strcpy
global strlen
; ------------------------------------------------------------------------
; char* strcpy(char* p_dst, char* p_src);
; ------------------------------------------------------------------------
strcpy:
push ebp
mov ebp, esp
mov esi, [ebp + 12] ; Source
mov edi, [ebp + 8] ; Destination
.1:
mov al, [esi] ; ┓
inc esi ; ┃
; ┣ 逐字节移动
mov byte [edi], al ; ┃
inc edi ; ┛
cmp al, 0 ; 是否遇到 '\0'
jnz .1 ; 没遇到就继续循环，遇到就结束
mov eax, [ebp + 8] ; 返回值
pop ebp
ret ; 函数结束，返回
; strcpy 结束-------------------------------------------------------------
;; int strlen(char* p_str);
strlen:
push	ebp
mov	ebp, esp
mov eax, 0
mov esi, [ebp + 8]
.1:
cmp byte [esi], 0
jz .2
inc esi
inc eax
jmp .1
.2:
pop ebp
ret
