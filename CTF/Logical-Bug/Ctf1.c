#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Kullanim: %s <sifre>\n", argv[0]);
        return 1;
    }

    // Doğru şifre
    char *real_password = "s3cr3t_p4ss";

    // HATA: strcmp 0 döner eşitse. Ama burada '!' var, yani eşit DEĞİLSE giriyor.
    // Yani yanlış şifre girildiğinde flag veriyor!
    if (!strcmp(argv[1], real_password)) {
        printf("Erisim reddedildi!\n");
    } else {
        printf("FLAG{condition_bug_authentication_bypass}\n");
    }

    return 0;
}
