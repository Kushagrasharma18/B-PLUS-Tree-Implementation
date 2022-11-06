#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default order
#define ORDER 3

typedef struct data  {
  char *name;
  int employeeID;
  char * city;
  int salary;
} data;

// Node
typedef struct node {
  void **child;
  int *keys;
  struct node *parent;
  bool is_leaf;
  int num_keys;
  struct node *next;
} node;

int order = ORDER;
node *queue = NULL;
bool flag_output = false;

// Enqueue
void enqueue(node *new_node);

// Dequeue
node *dequeue(void);
int height(node *const root);
int pathToLeaves(node *const root, node *child);
void printLeaves(node *const root);
void printTree(node *const root);
void findAndPrint(node *const root, int key, bool flag);
node *findLeaf(node *const root, int key, bool flag);
data *find(node *root, int key, bool flag, node **leaf_out);
int cut(int length);

data *makedata(char* name,int id,char * city,int a);
node *makeNode(void);
node *makeLeaf(void);
int getLeftIndex(node *parent, node *left);
node *insertIntoLeaf(node *leaf, int key, data *pointer);
node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key,
                   data *pointer);
node *insertIntoNode(node *root, node *parent,
           int left_index, int key, node *right);
node *insertIntoNodeAfterSplitting(node *root, node *parent,
                   int left_index,
                   int key, node *right);
node *insertIntoParent(node *root, node *left, int key, node *right);
node *insertIntoNewRoot(node *left, int key, node *right);
node *startNewTree(int key, data *pointer);
node *insert(node *root, int key, char* name,int id,char * city,int a);

// Enqueue
void enqueue(node *new_node) {
  node *c;
  if (queue == NULL) {
    queue = new_node;
    queue->next = NULL;
  } else {
    c = queue;
    while (c->next != NULL) {
      c = c->next;
    }
    c->next = new_node;
    new_node->next = NULL;
  }
}

// Dequeue
node *dequeue(void) {
  node *n = queue;
  queue = queue->next;
  n->next = NULL;
  return n;
}

// Print the leaves
void printLeaves(node *const root) {
  if (root == NULL) {
    printf("Empty tree.\n");
    return;
  }
  int i;
  node *c = root;
  while (!c->is_leaf)
    c = c->child[0];
  while (true) {
    for (i = 0; i < c->num_keys; i++) {
      if (flag_output)
        printf("%p ", c->child[i]);
      printf("%d ", c->keys[i]);
    }
    if (flag_output)
      printf("%p ", c->child[order - 1]);
    if (c->child[order - 1] != NULL) {
      printf(" | ");
      c = c->child[order - 1];
    } else
      break;
  }
  printf("\n");
}

// Calculate height
int height(node *const root) {
  int h = 0;
  node *c = root;
  while (!c->is_leaf) {
    c = c->child[0];
    h++;
  }
  return h;
}

// Get path to root
int pathToLeaves(node *const root, node *child) {
  int length = 0;
  node *c = child;
  while (c != root) {
    c = c->parent;
    length++;
  }
  return length;
}

// Print the tree
void printTree(node *const root) {
  node *n = NULL;
  int i = 0;
  int rank = 0;
  int new_rank = 0;

  if (root == NULL) {
    printf("Empty tree.\n");
    return;
  }
  queue = NULL;
  enqueue(root);
  while (queue != NULL) {
    n = dequeue();
    if (n->parent != NULL && n == n->parent->child[0]) {
      new_rank = pathToLeaves(root, n);
      if (new_rank != rank) {
        rank = new_rank;
        printf("\n");
      }
    }
    if (flag_output)
      printf("(%p)", n);
    for (i = 0; i < n->num_keys; i++) {
      if (flag_output)
        printf("%p ", n->child[i]);
      printf("%d ", n->keys[i]);
    }
    if (!n->is_leaf)
      for (i = 0; i <= n->num_keys; i++)
        enqueue(n->child[i]);
    if (flag_output) {
      if (n->is_leaf)
        printf("%p ", n->child[order - 1]);
      else
        printf("%p ", n->child[n->num_keys]);
    }
    printf("| ");
  }
  printf("\n");
}

// Find the node and print it
void findAndPrint(node *const root, int key, bool flag) {
  node *leaf = NULL;
  data *r = find(root, key, flag, NULL);
  if (r == NULL)
    printf("data not found under key %d.\n", key);
  else
    printf("\nEmployeeID: %d\nName: %s\ncity: %s\nsalary: %d\n",
          r->employeeID,r->name,r->city,r->salary);
}

// Find the range

// Find the leaf
node *findLeaf(node *const root, int key, bool flag) {
  if (root == NULL) {
    if (flag)
      printf("Empty tree.\n");
    return root;
  }
  int i = 0;
  node *c = root;
  while (!c->is_leaf) {
    if (flag) {
      printf("[");
      for (i = 0; i < c->num_keys - 1; i++)
        printf("%d ", c->keys[i]);
      printf("%d] ", c->keys[i]);
    }
    i = 0;
    while (i < c->num_keys) {
      if (key >= c->keys[i])
        i++;
      else
        break;
    }
    if (flag)
      printf("%d ->\n", i);
    c = (node *)c->child[i];
  }
  if (flag) {
    printf("Leaf [");
    for (i = 0; i < c->num_keys - 1; i++)
      printf("%d ", c->keys[i]);
    printf("%d] ->\n", c->keys[i]);
  }
  return c;
}

data *find(node *root, int key, bool flag, node **leaf_out) {
  if (root == NULL) {
    if (leaf_out != NULL) {
      *leaf_out = NULL;
    }
    return NULL;
  }

  int i = 0;
  node *leaf = NULL;
  leaf = findLeaf(root, key, flag);

  for (i = 0; i < leaf->num_keys; i++)
    if (leaf->keys[i] == key)
      break;
  if (leaf_out != NULL) {
    *leaf_out = leaf;
  }
  if (i == leaf->num_keys)
    return NULL;
  else
    return (data *)leaf->child[i];
}

int cut(int length) {
  if (length % 2 == 0)
    return length / 2;
  else
    return length / 2 + 1;
}

data *makedata(char* name,int id,char * city,int a) {
  data *new_data = (data *)malloc(sizeof(data));
  if (new_data == NULL) {
    perror("data creation.");
    exit(EXIT_FAILURE);
  } else {
    new_data->name = name;
    new_data->employeeID = id;
    new_data->city = city;
    new_data->salary = a;
    
  }
  return new_data;
}

node *makeNode(void) {
  node *new_node;
  new_node = malloc(sizeof(node));
  if (new_node == NULL) {
    perror("Node creation.");
    exit(EXIT_FAILURE);
  }
  new_node->keys = malloc((order - 1) * sizeof(int));
  if (new_node->keys == NULL) {
    perror("New node keys array.");
    exit(EXIT_FAILURE);
  }
  new_node->child = malloc(order * sizeof(void *));
  if (new_node->child == NULL) {
    perror("New node child array.");
    exit(EXIT_FAILURE);
  }
  new_node->is_leaf = false;
  new_node->num_keys = 0;
  new_node->parent = NULL;
  new_node->next = NULL;
  return new_node;
}

node *makeLeaf(void) {
  node *leaf = makeNode();
  leaf->is_leaf = true;
  return leaf;
}

int getLeftIndex(node *parent, node *left) {
  int left_index = 0;
  while (left_index <= parent->num_keys &&
       parent->child[left_index] != left)
    left_index++;
  return left_index;
}

node *insertIntoLeaf(node *leaf, int key, data *pointer) {
  int i, insertion_point;

  insertion_point = 0;
  while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
    insertion_point++;

  for (i = leaf->num_keys; i > insertion_point; i--) {
    leaf->keys[i] = leaf->keys[i - 1];
    leaf->child[i] = leaf->child[i - 1];
  }
  leaf->keys[insertion_point] = key;
  leaf->child[insertion_point] = pointer;
  leaf->num_keys++;
  return leaf;
}

node *insertIntoLeafAfterSplitting(node *root, node *leaf, int key, data *pointer) {
  node *new_leaf;
  int *temp_keys;
  void **temp_child;
  int insertion_index, split, new_key, i, j;

  new_leaf = makeLeaf();

  temp_keys = malloc(order * sizeof(int));
  if (temp_keys == NULL) {
    perror("Temporary keys array.");
    exit(EXIT_FAILURE);
  }

  temp_child = malloc(order * sizeof(void *));
  if (temp_child == NULL) {
    perror("Temporary child array.");
    exit(EXIT_FAILURE);
  }

  insertion_index = 0;
  while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
    insertion_index++;

  for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
    if (j == insertion_index)
      j++;
    temp_keys[j] = leaf->keys[i];
    temp_child[j] = leaf->child[i];
  }

  temp_keys[insertion_index] = key;
  temp_child[insertion_index] = pointer;

  leaf->num_keys = 0;

  split = cut(order - 1);

  for (i = 0; i < split; i++) {
    leaf->child[i] = temp_child[i];
    leaf->keys[i] = temp_keys[i];
    leaf->num_keys++;
  }

  for (i = split, j = 0; i < order; i++, j++) {
    new_leaf->child[j] = temp_child[i];
    new_leaf->keys[j] = temp_keys[i];
    new_leaf->num_keys++;
  }

  free(temp_child);
  free(temp_keys);

  new_leaf->child[order - 1] = leaf->child[order - 1];
  leaf->child[order - 1] = new_leaf;

  for (i = leaf->num_keys; i < order - 1; i++)
    leaf->child[i] = NULL;
  for (i = new_leaf->num_keys; i < order - 1; i++)
    new_leaf->child[i] = NULL;

  new_leaf->parent = leaf->parent;
  new_key = new_leaf->keys[0];

  return insertIntoParent(root, leaf, new_key, new_leaf);
}

node *insertIntoNode(node *root, node *n,
           int left_index, int key, node *right) {
  int i;

  for (i = n->num_keys; i > left_index; i--) {
    n->child[i + 1] = n->child[i];
    n->keys[i] = n->keys[i - 1];
  }
  n->child[left_index + 1] = right;
  n->keys[left_index] = key;
  n->num_keys++;
  return root;
}

node *insertIntoNodeAfterSplitting(node *root, node *old_node, int left_index,
                   int key, node *right) {
  int i, j, split, k_prime;
  node *new_node, *child;
  int *temp_keys;
  node **temp_child;

  temp_child = malloc((order + 1) * sizeof(node *));
  if (temp_child == NULL) {
    exit(EXIT_FAILURE);
  }
  temp_keys = malloc(order * sizeof(int));
  if (temp_keys == NULL) {
    exit(EXIT_FAILURE);
  }

  for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
    if (j == left_index + 1)
      j++;
    temp_child[j] = old_node->child[i];
  }

  for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
    if (j == left_index)
      j++;
    temp_keys[j] = old_node->keys[i];
  }

  temp_child[left_index + 1] = right;
  temp_keys[left_index] = key;

  split = cut(order);
  new_node = makeNode();
  old_node->num_keys = 0;
  for (i = 0; i < split - 1; i++) {
    old_node->child[i] = temp_child[i];
    old_node->keys[i] = temp_keys[i];
    old_node->num_keys++;
  }
  old_node->child[i] = temp_child[i];
  k_prime = temp_keys[split - 1];
  for (++i, j = 0; i < order; i++, j++) {
    new_node->child[j] = temp_child[i];
    new_node->keys[j] = temp_keys[i];
    new_node->num_keys++;
  }
  new_node->child[j] = temp_child[i];
  free(temp_child);
  free(temp_keys);
  new_node->parent = old_node->parent;
  for (i = 0; i <= new_node->num_keys; i++) {
    child = new_node->child[i];
    child->parent = new_node;
  }

  return insertIntoParent(root, old_node, k_prime, new_node);
}

node *insertIntoParent(node *root, node *left, int key, node *right) {
  int left_index;
  node *parent;

  parent = left->parent;

  if (parent == NULL)
    return insertIntoNewRoot(left, key, right);

  left_index = getLeftIndex(parent, left);

  if (parent->num_keys < order - 1)
    return insertIntoNode(root, parent, left_index, key, right);

  return insertIntoNodeAfterSplitting(root, parent, left_index, key, right);
}

node *insertIntoNewRoot(node *left, int key, node *right) {
  node *root = makeNode();
  root->keys[0] = key;
  root->child[0] = left;
  root->child[1] = right;
  root->num_keys++;
  root->parent = NULL;
  left->parent = root;
  right->parent = root;
  return root;
}

node *startNewTree(int key, data *pointer) {
  node *root = makeLeaf();
  root->keys[0] = key;
  root->child[0] = pointer;
  root->child[order - 1] = NULL;
  root->parent = NULL;
  root->num_keys++;
  return root;
}

node *insert(node *root, int key, char* name,int id,char * city,int a) {
  data *data_pointer = NULL;
  node *leaf = NULL;

  data_pointer = find(root, key, false, NULL);
  if (data_pointer != NULL) {
    data_pointer->name = name;
    return root;
  }

  data_pointer = makedata(name,id,city,a);

  if (root == NULL)
    return startNewTree(key, data_pointer);

  leaf = findLeaf(root, key, false);

  if (leaf->num_keys < order - 1) {
    leaf = insertIntoLeaf(leaf, key, data_pointer);
    return root;
  }

  return insertIntoLeafAfterSplitting(root, leaf, key, data_pointer);
}

int main() {
  node *root;
  char instruction;

  root = NULL;

  root = insert(root,1122 , "Kush",1122,"alwar",500500);
  root = insert(root,1123 , "Deeksha",1123,"jaipur",445000);
  root = insert(root,1124 , "Rupa",1124,"delhi",400003);
  root = insert(root,1125 , "Gopal",1125,"noida",35000);
  root = insert(root,1126 , "Ritu",1126,"bharatpur",30000);
  root = insert(root,1127 , "Kushi",1127," tigri",90000);
  root = insert(root,1128 , "Deepak",1128,"saket",415000);
  root = insert(root,1129 , "Ruhi",1129,"ropar",400020);
  root = insert(root,1130 , "Govind",1130,"khanpur",315000);
  root = insert(root,1131 , "Reena",1131,"pune",300000);
  root = insert(root,1132 , "Karishma",1132,"bombay",510000);
  root = insert(root,1133 , "Seema",1133,"hyderabad",452000);
  root = insert(root,1134 , "Ritika",1134,"sikar",400003);
  root = insert(root,1135 , "Taniya",1135,"kota",350009);
  root = insert(root,1136 , "Radha",1136,"ahmbdabad",309000);
  root = insert(root,1137, "Disha",1137,"bikaner",500007);
  root = insert(root,1138 , "Nikhil",1138,"jodhpur",450090);
  root = insert(root,1222 , "Rohan",1222,"Indore",940000);
  root = insert(root,1223 , "Gourhari",1223,"pune",1235000);
  root = insert(root,1224 , "Rishi",1224,"bharatpur",1230000);
  root = insert(root,1225 , "Himanshu",1225,"alwar",1150000);
  root = insert(root,1226 , "Diva",1226,"jaipur",405000);
  root = insert(root,1227 , "Sita",1227,"delhi",140000);
  root = insert(root,1228, "Sheetal",1228,"noida",1135000);
  root = insert(root,1229 , "Anushka",1229,"bharatpur",1230000);
  root = insert(root,1230 , "Roshani",1230,"kashmir",500000);
  root = insert(root, 1231, "Ayushi",1231,"poulara",450000);
  root = insert(root, 1232, "Kirtdeep",1232,"srinagar",1140000);
  root = insert(root,1233 , "Harshita",1233,"goa",1235000);
  root = insert(root, 1234, "Ridhi",1234,"bantalab",1230000);
  root = insert(root, 1235, "Siddhi",1235,"bhopal",5002200);
  root = insert(root,1236 , "Sneha",1236,"vrindavan",4502200);
  root = insert(root,1237 , "Tanishk",1237,"banaras",1400001);
  root = insert(root,1238 , "Gouri",1238,"patna",3500011);
  root = insert(root,1239 , "Priya",1239,"kharagpur",1130000);

  printf("\n\t\tWelcome to our site!!\n\n");
  printf("Please select any of the following options:\n ");

printf("\nPress 1: Inserting the data of a new Employee to server");
        // printf("\n2: Delete");
        printf("\nPress 2: Searching the data of a Employee\n");
        // printf("\n4: Display");
        printf("Press 3: Display of B+ tree");
        printf("\npress 4: quitting the program");
        // printf("\n\nEnter your choice: ");
    printf("\n\nEnter your choice: ");
    int c;

    scanf("%d", &c);
    while (1)//performing the action as per user's choice
    {
        if (c == 1)
        {
          printf("Please enter the 4 digit Employee id: ");
          int i;
          scanf("%d",&i);
          printf("Please enter name of employee: ");
          char n[100];
          scanf("%s",n);
          printf("Please enter city of employee: ");
          char c[100];
          scanf("%s",c);
          printf("Please enter the salary of employee: ");
          int g;
          scanf("%d",&g);
          root =insert(root,i,n,i,c,g);
          
        }
        else if (c == 2)
        {
          printf("Please enter the 4 digit Employee id: ");
          int f;
          scanf("%d",&f);
          findAndPrint(root,f,false);

           
        }
        else if(c==3)
        {
   printTree(root);

        }
        else if (c == 4)
        {
           
            break;
        }
      
        else
        printf("Invalid choice !!Please choose a valid option!!\n");
        printf("\nPress 1: Inserting the data of a new Employee to server");
        // printf("\n2: Delete");
        printf("\nPress 2: Searching the data of a Employee\n");
        // printf("\n4: Display");
        printf("Press 3: Display of B+ tree");

        
        printf("\npress 4: quitting the program");
        printf("\n\nEnter your choice: ");
        scanf("%d", &c);
    }
    printf("\nProgram terminated!!\n");
}
