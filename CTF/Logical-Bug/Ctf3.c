#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    char *path = "/tmp/safe.txt";

    // 1. KONTROL (Check): Dosya var mi ve okunabilir mi?
    if (access(path, R_OK) != 0) {
        printf("Dosya yok veya okunamaz.\n");
        return 1;
    }

    // 2. KULLANIM (Use): Dosyayi ac ve oku
    // HATA: access ile open arasinda zaman var.
    // Saldirgan bu sirada /tmp/safe.txt'yi silip,
    // yerine istedigi dosyaya symlink koyabilir.
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("Acma hatasi.\n");
        return 1;
    }

    char buf[256];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("Dosya icerigi:\n%s\n", buf);
        printf("FLAG{toctou_race_condition}\n");
    }

    close(fd);
    return 0;
}
