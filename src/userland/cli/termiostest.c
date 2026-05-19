#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main(void) {
    printf("=== BoredOS Termios & Blocking Read Test ===\n");

    struct termios tios;
    memset(&tios, 0, sizeof(tios));

    printf("Calling tcgetattr on stdin (fd 0)...\n");
    int rc = tcgetattr(0, &tios);
    if (rc != 0) {
        printf("FAILED: tcgetattr returned %d\n", rc);
        return 1;
    }
    printf("SUCCESS: tcgetattr returned 0!\n");
    printf("  c_iflag: 0x%08X (ICRNL=%d, IXON=%d)\n",
           tios.c_iflag, !!(tios.c_iflag & ICRNL), !!(tios.c_iflag & IXON));
    printf("  c_oflag: 0x%08X (OPOST=%d, ONLCR=%d)\n",
           tios.c_oflag, !!(tios.c_oflag & OPOST), !!(tios.c_oflag & ONLCR));
    printf("  c_lflag: 0x%08X (ICANON=%d, ECHO=%d)\n",
           tios.c_lflag, !!(tios.c_lflag & ICANON), !!(tios.c_lflag & ECHO));
    printf("  c_cc[VMIN]: %d\n", tios.c_cc[VMIN]);
    printf("  c_cc[VTIME]: %d\n", tios.c_cc[VTIME]);

    printf("\nCalling tcsetattr with same settings...\n");
    rc = tcsetattr(0, TCSANOW, &tios);
    if (rc != 0) {
        printf("FAILED: tcsetattr returned %d\n", rc);
        return 1;
    }
    printf("SUCCESS: tcsetattr returned 0!\n");

    printf("\nTesting blocking read on stdin.\n");
    printf("Please type some characters and press Enter:\n");

    char buf[64];
    memset(buf, 0, sizeof(buf));
    ssize_t n = read(0, buf, sizeof(buf) - 1);
    if (n < 0) {
        printf("FAILED: read returned error %ld\n", (long)n);
        return 1;
    }
    printf("SUCCESS: read returned %ld bytes!\n", (long)n);
    printf("Input read: \"%s\"\n", buf);

    return 0;
}
