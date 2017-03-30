void run_shell() {
    system("/bin/sh");
}

void vul() {
    char s[0x10];
    scanf("%s", s);
}

int main() {
    vul();
    return 0;
}