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
  CompleteBinaryTree tree(10);
  tree.fillTreeFromFile("input.txt");
  tree.printLevelOrder();
  return 0;
}