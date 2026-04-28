; [ VOX NIHILI - X86_64 LINUX SYSCALL KERNEL INVOKER ]
; Sem bibliotecas. Manipulacao direta dos registradores fisicos da CPU.

section .data
    ; Define os bytes exatos da nossa mensagem na memoria RAM
    msg db "0x00: A MAQUINA SANGRA. CONEXAO DE BAIXO NIVEL ESTABELECIDA.", 0x0A
    len equ $ - msg  ; O montador calcula o tamanho fisico da mensagem

section .text
    global _start    ; Ponto de ignicao obrigatorio para o Kernel

_start:
    ; ==========================================
    ; RITO 1: INVOCA O KERNEL PARA ESCREVER NA TELA
    ; ==========================================
    mov rax, 1       ; Puxa a gaveta RAX e coloca 1 (Codigo do 'sys_write')
    mov rdi, 1       ; Puxa a gaveta RDI e coloca 1 (Destino: Monitor/Stdout)
    mov rsi, msg     ; Puxa a gaveta RSI e aponta pro endereço da mensagem
    mov rdx, len     ; Puxa a gaveta RDX e diz quantos bytes de tamanho tem
    syscall          ; DA O CHOQUE NO KERNEL. O LINUX EXECUTA A ORDEM.

    ; ==========================================
    ; RITO 2: ORDEM DE SUICIDIO DO PROCESSO
    ; ==========================================
    mov rax, 60      ; Puxa a gaveta RAX e coloca 60 (Codigo do 'sys_exit')
    xor rdi, rdi     ; Destroi o valor de RDI (Deixa ele 0 absoluto)
    syscall          ; DA O CHOQUE NO KERNEL. O PROCESSO MORRE LIMPO.