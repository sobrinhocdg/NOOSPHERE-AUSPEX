#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Paleta NOOSPHERE-AUSPEX (Grimdark)
#define C_RAD "\033[38;5;118m"   // Verde Fósforo
#define C_RED "\033[38;5;196m"   // Vermelho Sangue
#define C_GRY "\033[38;5;240m"   // Cinza Chumbo
#define C_WHT "\033[1;37m"       // Branco Puro
#define C_RST "\033[0m"

#define HEADER_SIZE 54
#define DELIMITER "||AUSPEX||"

void limpar_tela() {
    printf("\033[2J\033[1;1H");
}

void noosphere_boot() {
    limpar_tela();
    printf("%s", C_RAD);
    printf("    [=================================================]\n");
    printf("    [  NOOSPHERE-AUSPEX :: COGITATOR LINK ESTABLISHED ]\n");
    printf("    [  TECH-PRIEST PROTOCOL : LOW-LEVEL MEMORY ACCESS ]\n");
    printf("    [=================================================]%s\n\n", C_RST);
}

// Injeta o bit no byte do pixel
void inject_bit(uint8_t *pixel_byte, int bit) {
    if (bit) {
        *pixel_byte |= 1; // Força LSB para 1
    } else {
        *pixel_byte &= ~1; // Força LSB para 0
    }
}

// Extrai o bit do byte do pixel
int extract_bit(uint8_t pixel_byte) {
    return pixel_byte & 1;
}

void forjar_artefato(const char *input_bmp, const char *output_bmp, const char *secret) {
    FILE *fin = fopen(input_bmp, "rb");
    if (!fin) {
        printf("%s[!] FALHA NO COGITADOR: O arquivo base '%s' nao existe no plano material.%s\n", C_RED, input_bmp, C_RST);
        return;
    }

    FILE *fout = fopen(output_bmp, "wb");
    if (!fout) {
        printf("%s[!] FALHA NA FORJA: Sem permissao para gravar a anomalia.%s\n", C_RED, C_RST);
        fclose(fin);
        return;
    }

    // Aloca memoria e copia o cabecalho de 54 bytes do BMP
    uint8_t header[HEADER_SIZE];
    fread(header, 1, HEADER_SIZE, fin);
    fwrite(header, 1, HEADER_SIZE, fout);

    // Concatena o delimitador ao segredo
    size_t secret_len = strlen(secret);
    size_t delim_len = strlen(DELIMITER);
    size_t total_len = secret_len + delim_len;
    
    char *full_secret = (char *)malloc(total_len + 1);
    strcpy(full_secret, secret);
    strcat(full_secret, DELIMITER);

    printf("%s[*] INICIANDO OPERACAO DE PONTEIROS. INJETANDO BITS NA MATRIZ...%s\n", C_GRY, C_RST);

    uint8_t pixel;
    size_t bit_count = 0;
    size_t total_bits = total_len * 8;

    // Le a imagem byte a byte e injeta os bits
    while (fread(&pixel, 1, 1, fin) == 1) {
        if (bit_count < total_bits) {
            int char_index = bit_count / 8;
            int bit_index = bit_count % 8;
            int bit_val = (full_secret[char_index] >> (7 - bit_index)) & 1;
            
            inject_bit(&pixel, bit_val);
            bit_count++;
        }
        fwrite(&pixel, 1, 1, fout);
    }

    free(full_secret);
    fclose(fin);
    fclose(fout);

    printf("%s[+] FORJA CONCLUIDA. O ARTEFATO '%s' FOI GRAVADO COM SUCESSO.%s\n", C_RAD, output_bmp, C_RST);
}

void extrair_anomalia(const char *target_bmp) {
    FILE *fin = fopen(target_bmp, "rb");
    if (!fin) {
        printf("%s[!] ERRO DE MEMORIA: O alvo '%s' e inacessivel.%s\n", C_RED, target_bmp, C_RST);
        return;
    }

    // Pula o cabecalho
    fseek(fin, HEADER_SIZE, SEEK_SET);

    printf("%s[*] LENDO RADICAO DE FUNDO. DECODIFICANDO PONTEIROS LSB...%s\n", C_GRY, C_RST);

    uint8_t pixel;
    char current_char = 0;
    int bit_count = 0;
    
    // Aloca um buffer dinamico na heap
    size_t buffer_size = 1024;
    char *secret = (char *)malloc(buffer_size);
    size_t secret_len = 0;

    while (fread(&pixel, 1, 1, fin) == 1) {
        int bit = extract_bit(pixel);
        current_char = (current_char << 1) | bit;
        bit_count++;

        if (bit_count == 8) {
            secret[secret_len++] = current_char;
            
            // Realoca memoria se a mensagem for gigante
            if (secret_len >= buffer_size - 1) {
                buffer_size *= 2;
                secret = (char *)realloc(secret, buffer_size);
            }
            
            secret[secret_len] = '\0';
            current_char = 0;
            bit_count = 0;

            // Verifica se achou o delimitador
            if (secret_len >= strlen(DELIMITER)) {
                if (strcmp(&secret[secret_len - strlen(DELIMITER)], DELIMITER) == 0) {
                    secret[secret_len - strlen(DELIMITER)] = '\0';
                    printf("\n%s[+] ANOMALIA EXTRAIDA DA NOOSFERA:%s\n", C_RAD, C_RST);
                    printf("%s====================================================%s\n", C_GRY, C_RST);
                    printf("%s%s%s\n", C_WHT, secret, C_RST);
                    printf("%s====================================================%s\n", C_GRY, C_RST);
                    free(secret);
                    fclose(fin);
                    return;
                }
            }
        }
    }

    printf("\n%s[!] AVISO: Nenhum artefato encontrado. Apenas ruido estatico.%s\n", C_RED, C_RST);
    free(secret);
    fclose(fin);
}

int main(int argc, char *argv[]) {
    noosphere_boot();

    if (argc < 2) {
        printf("%sUso da Ferramenta de Baixo Nivel:%s\n", C_WHT, C_RST);
        printf("  Forjar:  %s./forge -i <arquivo_base.bmp> <arquivo_saida.bmp> \"Seu Segredo\"%s\n", C_GRY, C_RST);
        printf("  Extrair: %s./forge -x <anomalia.bmp>%s\n\n", C_GRY, C_RST);
        return 1;
    }

    if (strcmp(argv[1], "-i") == 0 && argc == 5) {
        forjar_artefato(argv[2], argv[3], argv[4]);
    } 
    else if (strcmp(argv[1], "-x") == 0 && argc == 3) {
        extrair_anomalia(argv[2]);
    } 
    else {
        printf("%s[!] Sintaxe Invalida. Leia as instrucoes acima.%s\n", C_RED, C_RST);
    }

    return 0;
}