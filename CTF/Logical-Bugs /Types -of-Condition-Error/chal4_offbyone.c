#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void give_flag() {
    FILE *f = fopen("flag.txt", "r");
    if (!f) return;
    char buf[128];
    fgets(buf, sizeof(buf), f);
    printf("TEBRİKLER! Flag: %s\n", buf);
    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Kullanım: %s <sayi>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int secret[10];
    int is_admin = 0;

    // secret dizisini doldur
    for (int i = 0; i < 10; i++) secret[i] = i * 10;

    // ZAFİYET: <= kullanılmış, < olmalıydı
    for (int i = 0; i <= n; i++) {
        if (i < 10) secret[i] = 0;
    }

    // Eğer is_admin bozulursa...
    if (is_admin != 0) {
        printf("Admin oldun!\n");
        give_flag();
    } else {
        printf("Normal kullanıcı. is_admin = %d\n", is_admin);
    }

    return 0;
}
