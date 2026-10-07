#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// flag.txt dosyasını okur
void give_flag() {
    FILE *f = fopen("flag.txt", "r");
    if (!f) { printf("flag.txt yok!\n"); return; }
    char buf[128];
    fgets(buf, sizeof(buf), f);
    printf("TEBRİKLER! Flag: %s\n", buf);
    fclose(f);
}

int main() {
    char input[64];
    int is_admin = 0;

    printf("Kullanıcı adı: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    // ZAFİYET: = kullanılmış, == olmalıydı
    if (is_admin = 1) {
        printf("Admin olarak giriş yapıldı!\n");
        give_flag();
    } else {
        printf("Normal kullanıcı.\n");
    }

    return 0;
}
