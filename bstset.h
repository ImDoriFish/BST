#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename T>
class BSTSet {
 private:
  struct BSTNode {
    // These two members should never change after initialization!
    const T value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(T value, BSTNode* parent)
        : value(value), parent(parent), left(nullptr), right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // add private helper function definitions here

  void insert_helper(BSTNode*& curr, BSTNode* parent, T value) {
    if (curr == nullptr) {
      curr = new BSTNode(value, parent);
      sz++;
      return;
    }

    if (value < curr->value) {
      insert_helper(curr->left, curr, value);
    } else if (value > curr->value) {
      insert_helper(curr->right, curr, value);
    } else {
      return;
    }
  }

  bool contain_helper(BSTNode* node, T value) const {
    if (node == nullptr) {
      return false;
    }

    if (node->value == value) {
      return true;
    }

    if (value < node->value) {
      return contain_helper(node->left, value);
    } else {
      return contain_helper(node->right, value);
    }
  }

  void clear_helper(BSTNode*& node) {
    if (node == nullptr) {
      return;
    }

    clear_helper(node->left);
    clear_helper(node->right);

    delete node;
    node = nullptr;
  }

  void to_string_helper(BSTNode* node, stringstream& ss) const {
    if (node == nullptr) {
      return;
    }

    to_string_helper(node->left, ss);
    ss << node->value << ", ";
    to_string_helper(node->right, ss);
  }

  void copycon_helper(BSTNode*& node, BSTNode* otherNode, BSTNode* parent) {
    if (otherNode == nullptr) {
      node = nullptr;
      return;
    }

    node = new BSTNode(otherNode->value, parent);
    copycon_helper(node->left, otherNode->left, node);
    copycon_helper(node->right, otherNode->right, node);
  }

  BSTNode* traverseMin_helper(BSTNode* rootNode) {
    BSTNode* currNode = rootNode;
    while (currNode->left != nullptr) {
      currNode = currNode->left;
    }

    return currNode;
  }

  BSTNode* next_helper(BSTNode* currNode) {
    if (currNode->right != nullptr) {
      currNode = traverseMin_helper(currNode->right);

    } else {
      bool found = false;
      while (currNode->parent != nullptr) {
        if (currNode == currNode->parent->left) {
          currNode = currNode->parent;
          found = true;
          break;
        }
        currNode = currNode->parent;
      }
      if (!found) {
        currNode = nullptr;
      }
    }

    return currNode;
  }

  BSTNode* find_helper(BSTNode* node, const T& value) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->value == value) {
      return node;
    }
    if (value < node->value) {
      return find_helper(node->left, value);
    } else {
      return find_helper(node->right, value);
    }
  }

  void successor_linking(BSTNode*& successor, BSTNode* deleteNode) {
    // Checking if the sucessor right next to the deleteNode
    // If it does enter the block, if not do not enter because the successor
    // will replace the deleteNode and don't need to link the child on the right
    // because it already linked
    if (successor->parent != deleteNode) {
      successor->parent->left = successor->right;

      // Check if there is child on the right for the sucessor then link the
      // child with that successor parent.
      if (successor->right != nullptr) {
        successor->right->parent = successor->parent;
      }

      // Link it back
      successor->right = deleteNode->right;
      successor->right->parent = successor;
    }
  }

 public:
  /**
   * Creates an empty `BSTSet`. Runs in O(1).
   */
  BSTSet() {
    root = nullptr;
    curr = nullptr;
    sz = 0;
  }

  /**
   * Checks if the `BSTSet` is empty. Runs in O(1).
   */
  bool empty() const {
    if (sz == 0) {
      return true;
    }
    return false;
  }

  /**
   * Returns the number of elements in the `BSTSet`. Runs in O(1).
   */
  size_t size() const {
    return sz;
  }

  /**
   * Adds the `value` to the `BSTSet`. If the value already
   * exists in the set, does not do anything.
   *
   * Uses the `value` to determine the location in the underlying BST.
   * Creates exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(T value) {
    // TODO_STUDENT
    insert_helper(root, nullptr, value);
  }

  /**
   * Returns `true` if the `value` is present in the set,
   * and `false` otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const T& value) const {
    // TODO_STUDENT
    bool containFlag = contain_helper(root, value);
    return containFlag;
  }

  /**
   * Empties the `BSTSet`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  void clear() {
    clear_helper(root);
    sz = 0;
    curr = nullptr;
    root = nullptr;
  }

  /**
   * Destructor, cleans up the `BSTSet`.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  ~BSTSet() {
    clear();
  }

  /**
   * Converts the `BSTSet` to a string representation, with the values
   * in-order by value.
   *
   * Example:
   *
   * c++
   * BSTSet<string> names;
   * names.insert("Gwen");
   * names.insert("Jen");
   * names.insert("Ben");
   * names.insert("Sven");
   *
   * Calling `names.to_string()` would return the following
   * string: `{Ben, Gwen, Jen, Sven}`
   *
   * Runs in O(N), where N is the number of elements.
   */
  string to_string() const {
    if (root == nullptr) {
      return "{}";
    }

    stringstream ss;
    ss << "{";

    to_string_helper(root, ss);
    string resultString = ss.str();

    resultString.pop_back();
    resultString.pop_back();
    resultString += "}";
    return resultString;
  }

  /**
   * Copy constructor.
   *
   * Copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of elements in `other`.
   */
  BSTSet(const BSTSet& other) {
    // TODO_STUDENT
    copycon_helper(root, other.root, nullptr);
    sz = other.sz;
    curr = nullptr;
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of elements in `this`, and O is
   * the number of elements in `other`.
   */
  BSTSet& operator=(const BSTSet& other) {
    if (this == &other) {
      return *this;
    }
    this->clear();
    copycon_helper(root, other.root, nullptr);
    sz = other.sz;
    curr = nullptr;

    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest element in the BSTSet, and removes it
   * from the `BSTSet`. If the `BSTSet` is empty, throws a
   * `runtime_error`.
   *
   * Creates no new nodes, and does not update any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  T remove_min() {
    if (root == nullptr) {
      throw runtime_error("Set is empty");
    }
    BSTNode* deleteNode = traverseMin_helper(root);
    T value = deleteNode->value;

    // Link the parent to node
    BSTNode* replacementNode = deleteNode->right;
    if (deleteNode == root) {
      root = replacementNode;
    } else {
      deleteNode->parent->left = replacementNode;

      // replacementNode->parent->parent->left = replacementNode;
    }

    // Link the node to parent (the other way around)be
    if (replacementNode != nullptr) {
      replacementNode->parent = deleteNode->parent;
    }

    delete deleteNode;
    sz--;

    return value;
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    if (root == nullptr) {
      curr = nullptr;
      return;
    }
    this->curr = traverseMin_helper(root);
  }

  /**
   * Uses the internal state to return the next in-order element
   * by reference, and advances the internal state. Returns `true`
   * if the reference was set, and `false` otherwise.
   *
   * Example usage:
   *
   * c++
   * BSTSet<string> bst;
   * bst.begin();
   * string value;
   * while (bst.next(val)) {
   *   cout << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(T& value) {
    if (curr == nullptr) {
      return false;
    }

    value = curr->value;
    curr = next_helper(curr);

    return true;
  }

  // =========================================================================

  /**
   * Removes the given element from the `BSTSet`, and returns the
   * value.
   *
   * Throws `out_of_range` if the value is not present in the set.
   * Creates no new nodes, and does not update the value of any
   * existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  T erase(const T& value) {
    BSTNode* deleteNode = find_helper(root, value);
    if (deleteNode == nullptr) {
      throw out_of_range("Value not found");
    }

    T valueReturn = deleteNode->value;

    // case 0
    if (deleteNode->left == nullptr && deleteNode->right == nullptr) {
      if (deleteNode == root) {
        root = nullptr;
      } else if (deleteNode == deleteNode->parent->left) {
        deleteNode->parent->left = nullptr;
      } else {
        deleteNode->parent->right = nullptr;
      }
      delete deleteNode;
      // Case 1
    } else if (deleteNode->left == nullptr || deleteNode->right == nullptr) {
      BSTNode* child;
      if (deleteNode->left != nullptr) {
        child = deleteNode->left;
      } else {
        child = deleteNode->right;
      }

      if (deleteNode == root) {
        root = child;
        child->parent = nullptr;
      } else if (deleteNode == deleteNode->parent->left) {
        deleteNode->parent->left = child;
        child->parent = deleteNode->parent;
      } else {
        deleteNode->parent->right = child;
        child->parent = deleteNode->parent;
      }

      delete deleteNode;
      // Case 2, 2 children
    } else {
      BSTNode* successor = traverseMin_helper(deleteNode->right);
      // handle cases when the successor is deep in the tree, if it's deep then
      // it have to relink again.
      successor_linking(successor, deleteNode);

      /*
      void successor_linking(BSTNode*& successor, BSTNode* deleteNode) {
    // Checking if the sucessor right next to the deleteNode
    // If it does enter the block, if not do not enter because the successor
    // will replace the deleteNode and don't need to link the child on the right
    // because it already linked
      if (successor->parent != deleteNode) {
        successor->parent->left = successor->right;

      // Check if there is child on the right for the sucessor then link the
      // child with that successor parent.
      if (successor->right != nullptr) {
        successor->right->parent = successor->parent;
      }

      // Link it back
      successor->right = deleteNode->right;
      successor->right->parent = successor;
    }
  }

      */

      // Linking the left branch
      successor->left = deleteNode->left;
      successor->left->parent = successor;

      if (deleteNode == root) {
        root = successor;
        successor->parent = nullptr;
      } else if (deleteNode == deleteNode->parent->left) {
        deleteNode->parent->left = successor;
        successor->parent = deleteNode->parent;
      } else {
        deleteNode->parent->right = successor;
        successor->parent = deleteNode->parent;
      }

      delete deleteNode;
    }

    sz--;
    return valueReturn;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTSet`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
