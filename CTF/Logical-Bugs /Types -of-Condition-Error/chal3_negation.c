#include <stdio.h>
#include <string.h>

void give_flag() {
    FILE *f = fopen("flag.txt", "r");
    if (!f) return;
    char buf[128];
    fgets(buf, sizeof(buf), f);
    printf("TEBRİKLER! Flag: %s\n", buf);
    fclose(f);
}

int main() {
    char password[64];

    printf("Şifre: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // ZAFİYET: ! unutulmuş
    if (strcmp(password, "gizli_sifre")) {
        printf("Hoş geldin!\n");
        give_flag();
    } else {
        printf("Yanlış şifre!\n");
    }

    return 0;
}
