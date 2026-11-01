#ifndef KHOHANG_H
#define KHOHANG_H

#include <string>
#include "AVLTree.h"
#include "mathang.h"

// Dùng chung một bí danh KhoHang trong toàn bộ chương trình.
typedef AVLTree<std::string, MatHang> KhoHang;

#endif // KHOHANG_H
