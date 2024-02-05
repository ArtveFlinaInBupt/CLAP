%:include <stdio.h>

int main() {
  int data<:5:> = <%1, 2, 3, 4, 5%>;
  int sum = 0;

  for (int i = 0; i < 5; i++) <% // 混合使用替用记号和标准符号
    sum += data<:i:>;
  %>

  return 0;
}
