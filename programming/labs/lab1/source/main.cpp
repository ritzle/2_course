#include <iostream>
#include <string>

#include "../headers/CompleteBinaryTree.h"
#include "../headers/DList.h"
#include "../headers/OList.h"
#include "../headers/array.h"
#include "../headers/queue.h"
#include "../headers/stack.h"

using namespace std;

int main() {
  CompleteBinaryTree tree{7};

  tree.insert(1);
  tree.insert(2);
  tree.insert(3);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);
  tree.insert(7);

  tree.printLevelOrder();
}