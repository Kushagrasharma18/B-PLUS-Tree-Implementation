// Searching on a B+ Tree in C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default order
#define ORDER 8

typedef struct data
{
  char *name;
  int employeeID;
  char *city;
  int salary;
} data;

// Node
typedef struct node//defining structure of b+ tree with its attributes
{
  void **child;
  int *id;
  struct node *parent;
  bool is_leaf;
  int num_id;
  struct node *next;
} node;

int order = ORDER;//defining order of b+ tree
node *queue = NULL;
bool flag1 = false;

// Enqueue
void enqueue(node *new_node);

// Dequeue
node *dequeue(void);
//declaration of all the functions used in code
int node_height(node *const root);
int pathToLeaves(node *const root, node *child);
void printLeaves(node *const root);
void print_BTree(node *const root);
void Print_data(node *const root, int key, bool flag);
node *find_leafnode(node *const root, int key, bool flag);
data *find(node *root, int key, bool flag, node **leaf_out);
int cut(int length);

data *makedata(char *name, int id, char *city, int a);
node *makeNode(void);
node *makeLeaf(void);
int getLeftIndex(node *parent, node *left);
node *insertIntoLeaf(node *leaf, int key, data *child);
node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key,
                                   data *child);
node *insertIntoNode(node *root, node *parent,
                     int left_index, int key, node *right);
node *insertIntoNodeAfterSplitting(node *root, node *parent,
                                   int left_index,
                                   int key, node *right);
node *insertIntoParent(node *root, node *left, int key, node *right);
node *insertIntoNewRoot(node *left, int key, node *right);
node *startNewTree(int key, data *child);
node *insert(node *root, int key, char *name, int id, char *city, int a);

// Enqueue
void enqueue(node *new_node)//function for enqueuing the data into new node
{
  node *c;
  if (queue == NULL)
  {
    queue = new_node;
    queue->next = NULL;
  }
  else
  {
    c = queue;
    while (c->next != NULL)
    {
      c = c->next;
    }
    c->next = new_node;
    new_node->next = NULL;
  }
}

// Dequeue
node *dequeue(void)//function for dequeuing
{
  node *n = queue;
  queue = queue->next;
  n->next = NULL;
  return n;
}

// Print the leaves
void printLeaves(node *const root)//function for printing the data stored into leaf node
{
  if (root == NULL)
  {
    printf("Empty tree.\n");
    return;
  }
  int i;
  node *c = root;
  while (!c->is_leaf)
    c = c->child[0];
  while (true)
  {
    for (i = 0; i < c->num_id; i++)
    {


      Print_data(root, c->id[i], false);
    }

    if (c->child[order - 1] != NULL)
    {
      c = c->child[order - 1];
    }
    else
      break;
  }
  printf("\n");
}

// Calculate node_height
int node_height(node *const root)
{
  int h = 0;
  node *c = root;
  while (!c->is_leaf)
  {
    c = c->child[0];
    h++;
  }
  return h;
}

// Get path to root
int pathToLeaves(node *const root, node *child)
{
  int length = 0;
  node *c = child;
  while (c != root)
  {
    c = c->parent;
    length++;
  }
  return length;
}

// Print the tree
void print_BTree(node *const root)
{
  node *n = NULL;
  int i = 0;
  int rank = 0;
  int new_rank = 0;

  if (root == NULL)
  {
    printf("Empty tree.\n");
    return;
  }
  queue = NULL;
  enqueue(root);
  while (queue != NULL)
  {
    n = dequeue();
    if (n->parent != NULL && n == n->parent->child[0])
    {
      new_rank = pathToLeaves(root, n);
      if (new_rank != rank)
      {
        rank = new_rank;
        printf("\n");
      }
    }
    if (flag1)
      printf("(%p)", n);
    for (i = 0; i < n->num_id; i++)
    {
      if (flag1)
        printf("%p ", n->child[i]);
      
      Print_data(root, n->id[i], false);
    }
    if (!n->is_leaf)
      for (i = 0; i <= n->num_id; i++)
        enqueue(n->child[i]);
    if (flag1)
    {
      if (n->is_leaf)
        printf("%p ", n->child[order - 1]);
      else
        printf("%p ", n->child[n->num_id]);
    }
    
  }
  printf("\n");
}

// Find the node and print it
void Print_data(node *const root, int key, bool flag)
{
  node *leaf = NULL;
  data *r = find(root, key, flag, NULL);
  if (r == NULL)
    printf("data not found under Employee id %d.\n", key);
  else
    printf("\nEmployeeID: %d\tName: %s\tcity: %s\tsalary: %d\n",
           r->employeeID, r->name, r->city, r->salary);
}

// Find the range

// Find the leaf
node *find_leafnode(node *const root, int key, bool flag)
{
  if (root == NULL)
  {
    if (flag)
      printf("Empty tree.\n");
    return root;
  }
  int i = 0;
  node *c = root;
  while (!c->is_leaf)
  {
    if (flag)
    {
      printf("[");
      for (i = 0; i < c->num_id - 1; i++)
        printf("%d ", c->id[i]);
      printf("%d] ", c->id[i]);
    }
    i = 0;
    while (i < c->num_id)
    {
      if (key >= c->id[i])
        i++;
      else
        break;
    }
    if (flag)
      printf("%d ->\n", i);
    c = (node *)c->child[i];
  }
  if (flag)
  {
    printf("Leaf [");
    for (i = 0; i < c->num_id - 1; i++)
      printf("%d ", c->id[i]);
    printf("%d] ->\n", c->id[i]);
  }
  return c;
}

data *find(node *root, int key, bool flag, node **leaf_out)//find the data corresponding to particular key in B+ tree
{
  if (root == NULL)
  {
    if (leaf_out != NULL)
    {
      *leaf_out = NULL;
    }
    return NULL;
  }

  int i = 0;
  node *leaf = NULL;
  leaf = find_leafnode(root, key, flag);

  for (i = 0; i < leaf->num_id; i++)
    if (leaf->id[i] == key)
      break;
  if (leaf_out != NULL)
  {
    *leaf_out = leaf;
  }
  if (i == leaf->num_id)
    return NULL;
  else
    return (data *)leaf->child[i];
}

int cut(int length)
{
  if (length % 2 == 0)
    return length / 2;
  else
    return length / 2 + 1;
}

data *makedata(char *name, int id, char *city, int a)//function for storing the data of employee
{
  data *new_data = (data *)malloc(sizeof(data));
  if (new_data == NULL)
  {
    perror("data creation.");
    exit(EXIT_FAILURE);
  }
  else
  {
    new_data->name = name;
    new_data->employeeID = id;
    new_data->city = city;
    new_data->salary = a;
  }
  return new_data;
}

node *makeNode(void)//function for making new node
{
  node *new_node;
  new_node = malloc(sizeof(node));
  if (new_node == NULL)
  {
    perror("Node creation.");
    exit(EXIT_FAILURE);
  }
  new_node->id = malloc((order - 1) * sizeof(int));
  if (new_node->id == NULL)
  {
    perror("New node id array.");
    exit(EXIT_FAILURE);
  }
  new_node->child = malloc(order * sizeof(void *));
  if (new_node->child == NULL)
  {
    perror("New node child array.");
    exit(EXIT_FAILURE);
  }
  new_node->is_leaf = false;
  new_node->num_id = 0;
  new_node->parent = NULL;
  new_node->next = NULL;
  return new_node;
}

node *makeLeaf(void)//function for making new node to leaf
{
  node *leaf = makeNode();
  leaf->is_leaf = true;
  return leaf;
}

int getLeftIndex(node *parent, node *left)//function for getting left index 
{
  int left_index = 0;
  while (left_index <= parent->num_id &&
         parent->child[left_index] != left)
    left_index++;
  return left_index;
}

node *insertIntoLeaf(node *leaf, int key, data *child)//function for inserting into leaf
{
  int i, insertion_point;

  insertion_point = 0;
  while (insertion_point < leaf->num_id && leaf->id[insertion_point] < key)
    insertion_point++;

  for (i = leaf->num_id; i > insertion_point; i--)
  {
    leaf->id[i] = leaf->id[i - 1];
    leaf->child[i] = leaf->child[i - 1];
  }
  leaf->id[insertion_point] = key;
  leaf->child[insertion_point] = child;
  leaf->num_id++;
  return leaf;
}

node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key, data *child)//function for inserting into leaf after splitting
{
  node *new_leaf;
  int *temp_id;
  void **temp_child;
  int insertion_index, split, new_key, i, j;

  new_leaf = makeLeaf();

  temp_id = malloc(order * sizeof(int));
  if (temp_id == NULL)
  {
    perror("Temporary id array.");
    exit(EXIT_FAILURE);
  }

  temp_child = malloc(order * sizeof(void *));
  if (temp_child == NULL)
  {
    perror("Temporary child array.");
    exit(EXIT_FAILURE);
  }

  insertion_index = 0;
  while (insertion_index < order - 1 && leaf->id[insertion_index] < key)
    insertion_index++;

  for (i = 0, j = 0; i < leaf->num_id; i++, j++)
  {
    if (j == insertion_index)
      j++;
    temp_id[j] = leaf->id[i];
    temp_child[j] = leaf->child[i];
  }

  temp_id[insertion_index] = key;
  temp_child[insertion_index] = child;

  leaf->num_id = 0;

  split = cut(order - 1);

  for (i = 0; i < split; i++)
  {
    leaf->child[i] = temp_child[i];
    leaf->id[i] = temp_id[i];
    leaf->num_id++;
  }

  for (i = split, j = 0; i < order; i++, j++)
  {
    new_leaf->child[j] = temp_child[i];
    new_leaf->id[j] = temp_id[i];
    new_leaf->num_id++;
  }

  free(temp_child);
  free(temp_id);

  new_leaf->child[order - 1] = leaf->child[order - 1];
  leaf->child[order - 1] = new_leaf;

  for (i = leaf->num_id; i < order - 1; i++)
    leaf->child[i] = NULL;
  for (i = new_leaf->num_id; i < order - 1; i++)
    new_leaf->child[i] = NULL;

  new_leaf->parent = leaf->parent;
  new_key = new_leaf->id[0];

  return insertIntoParent(root, leaf, new_key, new_leaf);
}

node *insertIntoNode(node *root, node *n,
                     int left_index, int key, node *right)//function for inseting into node
{
  int i;

  for (i = n->num_id; i > left_index; i--)
  {
    n->child[i + 1] = n->child[i];
    n->id[i] = n->id[i - 1];
  }
  n->child[left_index + 1] = right;
  n->id[left_index] = key;
  n->num_id++;
  return root;
}

node *insertIntoNodeAfterSplitting(node *root, node *old_node, int left_index,
                                   int key, node *right)//function for inserting into node after splitting
{
  int i, j, split, k_prime;
  node *new_node, *child;
  int *temp_id;
  node **temp_child;

  temp_child = malloc((order + 1) * sizeof(node *));
  if (temp_child == NULL)
  {
    exit(EXIT_FAILURE);
  }
  temp_id = malloc(order * sizeof(int));
  if (temp_id == NULL)
  {
    exit(EXIT_FAILURE);
  }

  for (i = 0, j = 0; i < old_node->num_id + 1; i++, j++)
  {
    if (j == left_index + 1)
      j++;
    temp_child[j] = old_node->child[i];
  }

  for (i = 0, j = 0; i < old_node->num_id; i++, j++)
  {
    if (j == left_index)
      j++;
    temp_id[j] = old_node->id[i];
  }

  temp_child[left_index + 1] = right;
  temp_id[left_index] = key;

  split = cut(order);
  new_node = makeNode();
  old_node->num_id = 0;
  for (i = 0; i < split - 1; i++)
  {
    old_node->child[i] = temp_child[i];
    old_node->id[i] = temp_id[i];
    old_node->num_id++;
  }
  old_node->child[i] = temp_child[i];
  k_prime = temp_id[split - 1];
  for (++i, j = 0; i < order; i++, j++)
  {
    new_node->child[j] = temp_child[i];
    new_node->id[j] = temp_id[i];
    new_node->num_id++;
  }
  new_node->child[j] = temp_child[i];
  free(temp_child);
  free(temp_id);
  new_node->parent = old_node->parent;
  for (i = 0; i <= new_node->num_id; i++)
  {
    child = new_node->child[i];
    child->parent = new_node;
  }

  return insertIntoParent(root, old_node, k_prime, new_node);
}

node *insertIntoParent(node *root, node *left, int key, node *right)//function for inserting key into parent node
{
  int left_index;
  node *parent;

  parent = left->parent;

  if (parent == NULL)
    return insertIntoNewRoot(left, key, right);

  left_index = getLeftIndex(parent, left);

  if (parent->num_id < order - 1)
    return insertIntoNode(root, parent, left_index, key, right);

  return insertIntoNodeAfterSplitting(root, parent, left_index, key, right);
}

node *insertIntoNewRoot(node *left, int key, node *right)//function for inserting key  into new root
{
  node *root = makeNode();
  root->id[0] = key;
  root->child[0] = left;
  root->child[1] = right;
  root->num_id++;
  root->parent = NULL;
  left->parent = root;
  right->parent = root;
  return root;
}

node *startNewTree(int key, data *child)//function for inserting tke key into empty tree
{
  node *root = makeLeaf();
  root->id[0] = key;
  root->child[0] = child;
  root->child[order - 1] = NULL;
  root->parent = NULL;
  root->num_id++;
  return root;
}

node *insert(node *root, int key, char *name, int id, char *city, int a)//function for inserting the data of employee implementing b+ tree
{
  data *data_child = NULL;
  node *leaf = NULL;

  data_child = find(root, key, false, NULL);
  if (data_child != NULL)
  {
    data_child->name = name;
    return root;
  }

  data_child = makedata(name, id, city, a);

  if (root == NULL)
    return startNewTree(key, data_child);

  leaf = find_leafnode(root, key, false);

  if (leaf->num_id < order - 1)
  {
    leaf = insertIntoLeaf(leaf, key, data_child);
    return root;
  }

  return insertIntoLeafAfterSplitting(root, leaf, key, data_child);
}

int main()
{
  node *root;
  char instruction;
  root = NULL;//inserting data of employees to server
  root = insert(root, 1122, "Kush", 1122, "Alwar", 500500);
  root = insert(root, 1123, "Deeksha", 1123, "Jaipur", 445000);
  root = insert(root, 1124, "Rupa", 1124, "Delhi", 400003);
  root = insert(root, 1125, "Gopal", 1125, "Noida", 35000);
  root = insert(root, 1126, "Ritu", 1126, "Bharatpur", 30000);
  root = insert(root, 1127, "Kushi", 1127, " Tigri", 90000);
  root = insert(root, 1128, "Deepak", 1128, "Saket", 415000);
  root = insert(root, 1129, "Ruhi", 1129, "Ropar", 400020);
  root = insert(root, 1130, "Govind", 1130, "Khanpur", 315000);
  root = insert(root, 1131, "Reena", 1131, "Pune", 300000);
  root = insert(root, 1132, "Karishma", 1132, "Bombay", 510000);
  root = insert(root, 1133, "Seema", 1133, "Hyderabad", 452000);
  root = insert(root, 1134, "Ritika", 1134, "Sikar", 400003);
  root = insert(root, 1135, "Taniya", 1135, "Kota", 350009);
  root = insert(root, 1136, "Radha", 1136, "Ahmbdabad", 309000);
  root = insert(root, 1137, "Disha", 1137, "Bikaner", 500007);
  root = insert(root, 1138, "Nikhil", 1138, "Jodhpur", 450090);
  root = insert(root, 1222, "Rohan", 1222, "Indore", 940000);
  root = insert(root, 1223, "Gourhari", 1223, "Pune", 1235000);
  root = insert(root, 1224, "Rishi", 1224, "Bharatpur", 1230000);
  root = insert(root, 1225, "Himanshu", 1225, "Alwar", 1150000);
  root = insert(root, 1226, "Diva", 1226, "Jaipur", 405000);
  root = insert(root, 1227, "Sita", 1227, "Delhi", 140000);
  root = insert(root, 1228, "Sheetal", 1228, "Noida", 1135000);
  root = insert(root, 1229, "Anushka", 1229, "Bharatpur", 1230000);
  root = insert(root, 1230, "Roshani", 1230, "Kashmir", 500000);
  root = insert(root, 1231, "Ayushi", 1231, "Poulara", 450000);
  root = insert(root, 1232, "Kirtdeep", 1232, "Srinagar", 1140000);
  root = insert(root, 1233, "Harshita", 1233, "Goa", 1235000);
  root = insert(root, 1234, "Ridhi", 1234, "Bantalab", 1230000);
  root = insert(root, 1235, "Siddhi", 1235, "Bhopal", 5002200);
  root = insert(root, 1236, "Sneha", 1236, "Vrindavan", 4502200);
  root = insert(root, 1237, "Tanishk", 1237, "Banaras", 1400001);
  root = insert(root, 1238, "Gouri", 1238, "Patna", 3500011);
  root = insert(root, 1239, "Priya", 1239, "Kharagpur", 1130000);

  printf("\n\t\t**Welcome to our site**\n\n");

  printf("\nPress 1: Inserting the data of a new Employee to server");
  // printf("\n2: Delete");
  printf("\nPress 2: Searching the data of a Employee\n");
  // printf("\n4: Display");
  printf("Press 3: Display of Employee details");
  printf("\npress 4: Exit the site");
  // printf("\n\nEnter your choice: ");
  printf("\n\nEnter your choice: ");
  int c;

  scanf("%d", &c);
  while (1) // performing the action as per user's choice
  {
    if (c == 1)
    {
      printf("Please enter the 4 digit Employee id: ");
      int i;
      scanf("%d", &i);
      printf("Please enter name of employee: ");
      char n[100];
      scanf("%s", n);
      printf("Please enter city of employee: ");
      char c[100];
      scanf("%s", c);
      printf("Please enter the salary of employee: ");
      int g;
      scanf("%d", &g);
      root = insert(root, i, n, i, c, g);
    }
    else if (c == 2)
    {
      printf("Please enter the 4 digit Employee id: ");
      int f;
      scanf("%d", &f);
      Print_data(root, f, false);
    }
    else if (c == 3)
    {
      printLeaves(root);
    }
    else if (c == 4)
    {

      break;
    }

    else
      printf("Invalid choice !!Please choose a valid option!!\n");
    printf("\nPress 1: Inserting the data of a new Employee to server");
  
    printf("\nPress 2: Searching the data of a Employee\n");
    printf("Press 3: Display of Employee details");

    printf("\npress 4: Exit the site");
    printf("\n\nEnter your choice: ");
    scanf("%d", &c);
  }
  printf("\nProgram terminated!!\n");
}
