#ifndef BTREE_H
#define BTREE_H

#include <exception>
#include <iostream>
#include <string>

template <typename T>
class BTree {
 private:
  class Node {
   private:
    T* data;
    Node* left;
    Node* right;

   public:
    class Exception : public std::exception {
     private:
      std::string message;

     public:
      explicit Exception(const char* message) : message(message) {}

      explicit Exception(const std::string& message) : message(message) {}

      virtual ~Exception() throw() {}
      virtual const char* what() const throw() { return message.c_str(); }
    };

   public:
    Node();
    Node(T element);

    T& getData();
    T* getDataPtr();
    Node*& getLeft();
    Node*& getRight();

    void setData(T& dataForeign);
    void setDataPtr(T* data);
    void setLeft(Node& left);
    void setRigth(Node& rigth);

    ~Node();
  };

 public:
  typedef Node* Position;
  BTree();
  BTree(BTree& other);

  ~BTree();

  bool isEmpty();
  void insertData(T& data);

  void deleteData(T& data);

  T& retrieve(Position& pos);

  int getHeight(Position& pos);
  int getHeight();
  int getLeftHeight();
  int getRightHeight();

  Position& findData(T& data);

  Position& getLowest();
  Position& getHighest();

  bool isLeaf(Position& pos);

  void parsePreOrder();
  void parseInOrder();
  void parsePostOrder();
  void copyAll(BTree<T>& treeToCopy);

  void deleteAll();

  BTree& operator=(BTree& other);

  class Exception : public std::exception {
   private:
    std::string message;

   public:
    explicit Exception(const char* message) : message(message) {}

    explicit Exception(const std::string& message) : message(message) {}

    virtual ~Exception() throw() {}
    virtual const char* what() const throw() { return message.c_str(); }
  };

 private:
  Position root;
  void insertData(Position& pos, T& data);
  void deleteData(Position& pos, T& data);

  void copyInOrder(Position& original, Position& copy);

  Position& findData(Position& pos, T& data);

  Position& getLowest(Position& pos);
  Position& getHighest(Position& pos);

  void parsePreOrder(Position& pos);
  void parseInOrder(Position& pos);
  void parsePostOrder(Position& pos);
};

#endif

// CLASS Implementation
template <typename T>
BTree<T>::Node::Node() : right(nullptr), left(nullptr), data(nullptr) {}

template <typename T>
BTree<T>::Node::Node(T element) {
  left = nullptr;
  right = nullptr;
  data = new T(element);
  if (data == nullptr) throw Exception("Not enought memory");
}

template <typename T>
T& BTree<T>::Node::getData() {
  if (data == nullptr) throw Exception("Non existent data, getData");
  return *data;
}

template <typename T>
T* BTree<T>::Node::getDataPtr() {
  return data;
}

template <typename T>
typename BTree<T>::Node*& BTree<T>::Node::getLeft() {
  return left;
}

template <typename T>
typename BTree<T>::Node*& BTree<T>::Node::getRight() {
  return right;
}

template <typename T>
void BTree<T>::Node::setData(T& dataForeign) {
  if (data == nullptr) {
    // Tries to create a new data, if it isn't capable of creating it
    // then rises an exception
    if ((data = new T(dataForeign)) == nullptr)
      throw Exception("Non available memory, setData");
  } else
    *data = dataForeign;
}

template <typename T>
void BTree<T>::Node::setDataPtr(T* data) {
  this->data = data;
}

template <typename T>
void BTree<T>::Node::setLeft(Node& left) {
  this->left = left;
}

template <typename T>
void BTree<T>::Node::setRigth(Node& right) {
  this->right = right;
}

template <typename T>
BTree<T>::Node::~Node() {
  delete data;
}

template <typename T>
BTree<T>::BTree() {
  root = nullptr;
}

template <typename T>
void BTree<T>::copyAll(BTree<T>& treeToCopy) {
  copyInOrder(treeToCopy.root, root);
}

template <typename T>
void BTree<T>::copyInOrder(Position& original, Position& copy) {
  T data;
  data = original->getData();
  copy = new Node(data);
  if (original->getLeft() != nullptr)
    copyInOrder(original->getLeft(), copy->getLeft());
  if (original->getRight() != nullptr)
    copyInOrder(original->getRight(), copy->getRight());
}

template <typename T>
BTree<T>::BTree(BTree<T>& other) {
  root = nullptr;
  copyAll(other);
}

template <typename T>
bool BTree<T>::isEmpty() {
  return root == nullptr;
}

template <typename T>
T& BTree<T>::retrieve(Position& pos) {
  if (root == nullptr || pos == nullptr)
    throw Exception("Not enough memory, cannot retrieve");

  return pos->getData();
}

template <typename T>
void BTree<T>::insertData(T& data) {
  insertData(root, data);
}

template <typename T>
void BTree<T>::insertData(Position& pos, T& data) {
  if (pos == nullptr) {
    try {
      if ((pos = new Node(data)) == nullptr)
        throw Exception("Not enough memory, cannot insert");
    } catch (typename Node::Exception ex) {
      throw Exception(ex.what());
    }
  }

  else {
    if (data < pos->getData())
      insertData(pos->getLeft(), data);
    else
      insertData(pos->getRight(), data);
  }
}

template <typename T>
void BTree<T>::deleteData(T& data) {
  deleteData(root, data);
}

template <typename T>
void BTree<T>::deleteData(Position& pos, T& data) {
  T replacement;
  // Position& aux; /// Se tiene que inicializar en la declaracion
  if (pos == nullptr) return;

  if (pos->getData() == data) {
    if (isLeaf(pos)) {
      delete pos;
      pos = nullptr;
    } else if (pos->getLeft() == nullptr) {
      pos = pos->getRight();
    } else if (pos->getRight() == nullptr) {
      pos = pos->getLeft();
    } else {
      replacement = getHighest(pos->getLeft())->getData();
      deleteData(pos->getLeft(), replacement);
      pos->setData(replacement);
    }
  } else {
    if (data < pos->getData())
      deleteData(pos->getLeft(), data);
    else
      deleteData(pos->getRight(), data);
  }
}

template <typename T>
typename BTree<T>::Position& BTree<T>::findData(T& data) {
  return findData(root, data);
}

template <typename T>
typename BTree<T>::Position& BTree<T>::findData(Position& pos, T& data) {
  if (pos == nullptr || data == pos->getData()) return pos;

  if (data < pos->getData()) return findData(pos->getLeft(), data);

  return findData(pos->getRight(), data);
}

template <typename T>
typename BTree<T>::Position& BTree<T>::getLowest() {
  return getLowest(root);
}

template <typename T>
typename BTree<T>::Position& BTree<T>::getLowest(Position& pos) {
  if (pos == nullptr || pos->getLeft() == nullptr) return pos;

  return getLowest(pos->getLeft());
}

template <typename T>
typename BTree<T>::Position& BTree<T>::getHighest() {
  return getHighest(root);
}

template <typename T>
typename BTree<T>::Position& BTree<T>::getHighest(Position& pos) {
  if (pos == nullptr || pos->getRight() == nullptr) return pos;

  return getHighest(pos->getRight());
}

template <typename T>
bool BTree<T>::isLeaf(Position& pos) {
  return pos != nullptr && pos->getLeft() == pos->getRight();
}

template <typename T>
int BTree<T>::getHeight() {
  return getHeight(root);
}

template <typename T>
int BTree<T>::getLeftHeight() {
  return getHeight(root->getLeft());
}

template <typename T>
int BTree<T>::getRightHeight() {
  return getHeight(root->getRight());
}

template <typename T>
int BTree<T>::getHeight(Position& pos) {
  if (pos == nullptr) return 0;

  int leftHeight = getHeight(pos->getLeft());
  int rightHeight = getHeight(pos->getRight());

  if (leftHeight > rightHeight) return leftHeight + 1;
  return rightHeight + 1;
}

template <typename T>
void BTree<T>::parsePreOrder() {
  parsePreOrder(root);
}

template <typename T>
void BTree<T>::parsePreOrder(Position& pos) {
  if (pos == nullptr) return;

  std::cout << pos->getData() << ",";
  parsePreOrder(pos->getLeft());
  parsePreOrder(pos->getRight());
}

template <typename T>
void BTree<T>::parseInOrder() {
  parseInOrder(root);
}

template <typename T>
void BTree<T>::parseInOrder(Position& pos) {
  if (pos == nullptr) return;

  parseInOrder(pos->getLeft());
  std::cout << pos->getData() << ",";
  parseInOrder(pos->getRight());
}

template <typename T>
void BTree<T>::parsePostOrder() {
  parsePostOrder(root);
}

template <typename T>
void BTree<T>::parsePostOrder(Position& pos) {
  if (pos == nullptr) return;

  parsePostOrder(pos->getLeft());
  parsePostOrder(pos->getRight());
  std::cout << pos->getData() << ",";
}

template <typename T>
void BTree<T>::deleteAll() {
  while (root->getLeft() != nullptr) {
    deleteData(root->getLeft()->getData());
  }

  while (root->getRight() != nullptr) {
    deleteData(root->getRight()->getData());
  }

  delete root;
  root = nullptr;
}

template <typename T>
BTree<T>& BTree<T>::operator=(BTree<T>& other) {
  copyAll(other);
  return *this;
}

template <typename T>
BTree<T>::~BTree() {
  deleteAll();
}