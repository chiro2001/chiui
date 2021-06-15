#include "chi-dom.hpp"
#include <string>
#include <cstdio>

using namespace chidom;

void blanks(int width) {
  while (width-- > 0)
    printf("  ");
}

void scan_dom(VNode *vnode, int depth = 0) {
  if (vnode == nullptr) return;
  blanks(depth);
  if (!vnode->children.empty()) {
    printf("<%s>\n", vnode->sel.c_str());
    if (!vnode->data.callbacks.empty()) {
      for (const auto &f : vnode->data.callbacks) {
        blanks(depth + 1);
        printf("callback: %s\n", f.first.c_str());
        Event event{.target = vnode, .type = f.first};
        f.second(event);
      }
    }
    for (const auto child : vnode->children) {
      if (child == nullptr) continue;
      scan_dom(child, depth + 1);
    }
    blanks(depth);
    printf("</%s>\n", vnode->sel.c_str());
  } else {
    printf("[%s]\n", vnode->sel.c_str());
  }
}

VNode *app() {
  static const State<std::string> value = useState<std::string>("Value TEXT");
  VNode *root = (
    chidom::h(u8"div", chidom::Children {chidom::h(u8"h1", std::string(u8"Title")), chidom::h(u8"p", (*value).get()), chidom::h(u8"div", chidom::h(u8"button", chidom::Data (chidom::Callbacks {{u8"onclick", [=](const Event& e) -> void {
            printf("onClick: 0x%06X\n", &e);
            (*value).set("Changed Data");
            printf("target: %s, type: %s\n", e.target->sel.c_str(), e.type.c_str());
            // printf("value: %s\n", (*value).get().c_str());
          }}, {u8"onkeydown", [=](const Event& e) -> void {
            printf("onKeyDown: 0x%06X\n", &e);
          }}}), std::string(u8"Button")))})
  );
  return root;
}

int main() {
  VNode *dom = nullptr;
  for (int i = 0; i < 2; i++) {
    dom = app();
    scan_dom(dom);
    delete dom;
  }
  return 0;
}