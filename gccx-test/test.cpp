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
  VNode *root = (GCCX /*
   <div>
    <h1>HelloWorld!</h1>
    <div style="width: 100%" id="box">
      <button onclick={[](int a) -> int { return a + 1; }}>Test button</button>
    </div>
  </div> */);
  scan_dom(root);
  return 0;
}