#include <stdio.h>

int main() {
  char quote = '\"';
  char backslash = '\\';
  char message[] = "Line1\nLine2";
  char multiline_message[] = "Line1\
Line2";

  printf("%c, %c, %s, %s\n", quote, backslash, message, multiline_message);
  return 0;
}
