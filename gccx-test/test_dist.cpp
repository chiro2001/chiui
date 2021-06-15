#include "chi-dom.h"
#include <cstdio>

using namespace asmdom;

void blanks(int width) {
  while (width-- > 0)
    printf("  ");
}

void scan_dom(VNode *vnode, int depth = 0) {
  if (vnode == nullptr) return;
  blanks(depth);
  if (!vnode->children.empty()) {
    printf("<%s>\n", vnode->sel.c_str());
    for (const auto child : vnode->children) {
      if (child == nullptr) continue;
      scan_dom(child, depth + 1);
    }
    blanks(depth);
    printf("</%s>\n", vnode->sel.c_str());
  } else {
    printf("%s\n", vnode->sel.c_str());
  }
}

int main() {
  VNode *root = (
   asmdom::h(u8"div", asmdom::Children {asmdom::h(u8"h1", std::string(u8"HelloWorld!")), asmdom::h(u8"div", asmdom::Data (asmdom::Attrs {{u8"style", u8"width: 100%"}, {u8"id", u8"box"}}), asmdom::h(u8"button", asmdom::Data (asmdom::Callbacks {{u8"onclick", [](int a) -> int { return a + 1; }}}), std::string(u8"Test button")))}) );
  scan_dom(root);
  return 0;
}