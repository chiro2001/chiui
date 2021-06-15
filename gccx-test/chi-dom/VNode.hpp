#ifndef asmdom_VNode_hpp
#define asmdom_VNode_hpp

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>
#include <unordered_map>
#include "event.h"

namespace chidom {

typedef std::function<void(const Event &)> Callback;
typedef std::unordered_map<std::string, std::string> Attrs;
typedef std::unordered_map<std::string, std::string> Props;
typedef std::unordered_map<std::string, Callback> Callbacks;

enum VNodeFlags {
  // NodeType
  isElement = 1,
  isText = 1 << 1,
  isComment = 1 << 2,
  isFragment = 1 << 3,

  // flags
  hasKey = 1 << 4,
  hasText = 1 << 5,
  hasAttrs = 1 << 6,
  hasProps = 1 << 7,
  hasCallbacks = 1 << 8,
  hasDirectChildren = 1 << 9,
  hasChildren = hasDirectChildren | hasText,
  hasRef = 1 << 10,
  hasNS = 1 << 11,
  isNormalized = 1 << 12,

  // masks
  isElementOrFragment = isElement | isFragment,
  nodeType = isElement | isText | isComment | isFragment,
  // removeNodeType = ~0 ^ nodeType,
  // extractSel = ~0 << 13,
  removeNodeType = 0xFFFFFFFF ^ nodeType,
  extractSel = 0xFFFFFFFF << 13,
  id = extractSel | hasKey | nodeType
};

struct Data {
  Data() = default;;

  explicit Data(
          Attrs dataAttrs,
          Props dataProps = Props(),
          Callbacks dataCallbacks = Callbacks()
  ) : attrs(std::move(dataAttrs)), props(std::move(dataProps)), callbacks(std::move(dataCallbacks)) {};

  Data(
          Attrs dataAttrs,
          Callbacks dataCallbacks
  ) : attrs(std::move(dataAttrs)), callbacks(std::move(dataCallbacks)) {};;

  explicit Data(
          Callbacks dataCallbacks
  ) : callbacks(std::move(dataCallbacks)) {};

  Attrs attrs;
  Props props;
  Callbacks callbacks;
};

struct VNode {
private:
  unsigned int currentHash = 0;
  std::unordered_map<std::string, unsigned int> hashes;

  void normalize(bool injectSvgNamespace) {
    if (!(hash & isNormalized)) {
      if (data.attrs.count("key")) {
        hash |= hasKey;
        key = data.attrs["key"];
        data.attrs.erase("key");
      }
      if (sel[0] == '!') {
        hash |= isComment;
        sel = "";
      } else {
        children.erase(std::remove(children.begin(), children.end(), (VNode *) nullptr), children.end());

        auto it = data.attrs.begin();
        while (it != data.attrs.end()) {
          if (it->first == "ns") {
            hash |= hasNS;
            ns = it->second;
            it = data.attrs.erase(it);
          } else if (it->second == "false") {
            it = data.attrs.erase(it);
          } else {
            if (it->second == "true") {
              it->second = "";
            }
            ++it;
          }
        }
        bool addNS = injectSvgNamespace || (sel[0] == 's' && sel[1] == 'v' && sel[2] == 'g');
        if (addNS) {
          hash |= hasNS;
          ns = "http://www.w3.org/2000/svg";
        }

        if (!data.attrs.empty()) hash |= hasAttrs;
        if (!data.props.empty()) hash |= hasProps;
        if (!data.callbacks.empty()) hash |= hasCallbacks;
        if (!children.empty()) {
          hash |= hasDirectChildren;

          Children::size_type i = children.size();
          while (i--) {
            children[i]->normalize(
                    addNS && sel != "foreignObject"
            );
          }
        }
        if (sel[0] == '\0') {
          hash |= isFragment;
        } else {
          if (hashes[sel] == 0) {
            hashes[sel] = ++currentHash;
          }
          hash |= (hashes[sel] << 13) | isElement;
          if ((hash & hasCallbacks) && data.callbacks.count("ref")) {
            hash |= hasRef;
          }
        }
      }
      hash |= isNormalized;
    }
  }

public:
  typedef std::vector<VNode *> Children;

  explicit VNode(
          std::string nodeSel
  ) : sel(std::move(nodeSel)) {};

  VNode(
          std::string nodeSel,
          const std::string &nodeText
  ) : sel(std::move(nodeSel)) {
    normalize();
    if (hash & isComment) {
      sel = nodeText;
    } else {
      children.push_back(new VNode(nodeText, true));
      hash |= hasText;
    }
  };

  VNode(
          const std::string &nodeText,
          const bool textNode
  ) {
    if (textNode) {
      normalize();
      sel = nodeText;
      // replace current type with text type
      hash = (hash & removeNodeType) | isText;
    } else {
      sel = nodeText;
      normalize();
    }
  };

  VNode(
          std::string nodeSel,
          Data nodeData
  ) : sel(std::move(nodeSel)), data(std::move(nodeData)) {};

  VNode(
          std::string nodeSel,
          std::vector<VNode *> nodeChildren
  ) : sel(std::move(nodeSel)), children(std::move(nodeChildren)) {};

  VNode(
          std::string nodeSel,
          VNode *child
  ) : sel(std::move(nodeSel)), children{child} {};

  VNode(
          std::string nodeSel,
          Data nodeData,
          const std::string &nodeText
  ) : sel(std::move(nodeSel)), data(std::move(nodeData)) {
    normalize();
    if (hash & isComment) {
      sel = nodeText;
    } else {
      children.push_back(new VNode(nodeText, true));
      hash |= hasText;
    }
  };

  VNode(
          std::string nodeSel,
          Data nodeData,
          std::vector<VNode *> nodeChildren
  ) : sel(std::move(nodeSel)), data(std::move(nodeData)), children(std::move(nodeChildren)) {};

  VNode(
          std::string nodeSel,
          Data nodeData,
          VNode *child
  ) : sel(std::move(nodeSel)), data(std::move(nodeData)), children{child} {};

  ~VNode() {
    if (hash & hasText) {
      Children::size_type i = children.size();
      while (i--) delete children[i];
    }
  }

  void normalize() { normalize(false); };

  // contains selector for elements and fragments, text for comments and textNodes
  std::string sel;
  std::string key;
  std::string ns;
  unsigned int hash = 0;
  Data data;
  int elm = 0;
  std::vector<VNode *> children;
};

typedef VNode::Children Children;
}

#endif
