#include <stdio.h>
#include <string.h>

int main() {
    char flag[32] = "FLAG{loop_off_by_one_leak}";
    int data[4] = {100, 200, 300, 400};

    // HATA: i <= 4
    for (int i = 0; i <= 4; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }

    return 0;
}
