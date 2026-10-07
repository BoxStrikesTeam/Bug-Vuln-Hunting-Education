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
    char username[32];
    char password[32];
    int is_authenticated = 0;
    int is_admin = 0;

    printf("Kullanıcı adı: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Şifre: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Basit doğrulama
    if (strcmp(username, "admin") == 0 && strcmp(password, "s3cr3t") == 0) {
        is_authenticated = 1;
        is_admin = 1;
    } else if (strcmp(username, "guest") == 0 && strcmp(password, "guest") == 0) {
        is_authenticated = 1;
    }

    // ZAFİYET: || kullanılmış, && olmalıydı
    if (is_authenticated || is_admin) {
        printf("Erişim verildi!\n");
        give_flag();
    } else {
        printf("Erişim reddedildi.\n");
    }

    return 0;
}
