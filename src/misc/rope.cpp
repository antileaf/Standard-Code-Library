#include <ext/rope>
using namespace __gnu_cxx;

push_back(x); // 在末尾添加x
insert(pos, x); // 在pos插入x, 自然支持整个char数组的一次插入
erase(pos, x); // 从pos开始删除x个
copy(pos, len, x); // 从pos开始到pos + len为止的部分, 赋值给x
replace(pos, x); // 从pos开始换成x
substr(pos, x); // 提取pos开始x个
at(x) / [x]; // 访问第x个元素