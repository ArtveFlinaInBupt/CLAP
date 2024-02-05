#include <stdio.h>

int main() {
  /// 八进制数不应包含 9，十六进制数不应包含小数点但无指数
  /// 虽然十六进制浮点数 p 指数未支持，但即使支持也不合法
  int illegal_int = {09, 0xabf120.3};
  /// 不能存在多个小数点，指数不能为浮点数
  double illegal_float = {1.2.3, .0.0, 0.0., 1e.1, 0e1., 1e45e4};
}
