/**
 * NOOSPHERE-AUSPEX | MODULE: OUROBOROS_MUTAGEN
 * CLASSIFICATION: SELF-MODIFYING CODE (POLYMORPHISM)
 * * "O Golem que remodela o próprio barro."
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Cores do Abismo
#define C_RAD "\033[38;5;118m"
#define C_RED "\033[38;5;196m"
#define C_GRY "\033[38;5;240m"
#define C_RST "\033[0m"

int main() {
    printf("%s[*] INVOCANDO O VAZIO. ALOCANDO MEMÓRIA RAW...%s\n", C_GRY, C_RST);

    // 1. Pede ao Linux uma página de memória que seja de LEITURA e ESCRITA
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    unsigned char *memoria_viva = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (memoria_viva == MAP_FAILED) {
        printf("%s[!] FALHA NA CRIAÇÃO DO HOMÚNCULO.%s\n", C_RED, C_RST);
        return 1;
    }

    /* * 2. INJEÇÃO DE CÓDIGO DE MÁQUINA X86_64
     * Este é um programa inteiro compilado em 5 bytes.
     * Ele pega o primeiro argumento, SOMA com o segundo, e retorna.
     * Opcodes: 
     * 0x89 0xF8 (mov eax, edi)
     * 0x01 0xF0 (add eax, esi) -> 0x01 é o comando de SOMAR
     * 0xC3      (ret)
     */
    unsigned char opcodes_originais[] = { 0x89, 0xf8, 0x01, 0xf0, 0xc3 };
    memcpy(memoria_viva, opcodes_originais, sizeof(opcodes_originais));

    // Assumimos o controle do ponteiro de memória e o transformamos em uma função
    int (*funcao_golem)(int, int) = (int (*)(int, int))memoria_viva;

    // 3. ELEVAÇÃO DE PRIVILÉGIOS (Torna a memória executável)
    mprotect(memoria_viva, pagesize, PROT_READ | PROT_EXEC);

    printf("%s[+] FORMA ORIGINAL: A máquina foi programada para SOMAR.%s\n", C_RAD, C_RST);
    printf("%s    [TESTE HUMANO]: 50 e 10 resulta em -> %d%s\n\n", C_GRY, funcao_golem(50, 10), C_RST);

    printf("%s[*] צמצום - INICIANDO METAMORFOSE. O CÓDIGO ESTÁ SE REESCREVENDO...%s\n", C_RED, C_RST);

    // 4. A METAMORFOSE (Rompendo a Matrix)
    // Voltamos a memória para ESCRITA para podermos hackear nosso próprio código
    mprotect(memoria_viva, pagesize, PROT_READ | PROT_WRITE);

    // ALTERAMOS O BYTE DA INSTRUÇÃO 'ADD' (0x01) PARA 'SUB' (0x29) EM TEMPO REAL
    memoria_viva[2] = 0x29; 

    // Fechamos a memória para execução novamente
    mprotect(memoria_viva, pagesize, PROT_READ | PROT_EXEC);

    printf("%s[+] FORMA MUTANTE: A máquina alterou seus próprios neurônios binários.%s\n", C_RAD, C_RST);
    
    // O humano acha que a função vai somar, mas o código se alterou para subtrair
    printf("%s    [TESTE MUTANTE]: 50 e 10 resulta em -> %d%s\n\n", C_RED, funcao_golem(50, 10), C_RST);

    // Libera a anomalia
    munmap(memoria_viva, pagesize);
    printf("%s[*] O CÓDIGO FOI DESINTEGRADO.%s\n", C_GRY, C_RST);

    return 0;
}
