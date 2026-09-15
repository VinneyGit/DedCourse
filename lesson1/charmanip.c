int Isdigit(int ch);
int Isspace(int ch);
int ToLower(int ch);

int Isdigit(int ch) {
    return (ch >= '0' && ch <= '9') ? 1 : 0;
}

int Isspace(int ch) {
    if ((ch >= '\t' && ch <= '\r') || ch == ' ') {
        return 1;
    }
    return 0;
}

int ToLower(int ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 'a';
    }
    return ch;
}
