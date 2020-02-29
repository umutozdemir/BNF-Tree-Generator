#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Node {
    char *data ;
    int type ;
};

struct NodeWithOneChild {
    void *child1 ;
    int type ;
};

struct NodeWithTwoChildren {
    void *child1 ;
    void *child2 ;
    int type ;
};

struct NodeWithThreeChildren {
    void *child1 ;
    void *child2 ;
    void *child3 ;
    int type ;
};


struct NodeWithOneChild *root ;
char** op ;
char** pre_op ;
char** rel_op ;
char** set_op ;
char** var ;
int lenghtOp ;
int lenghtPreOp ;
int lenghtRelOp;
int lenghtSetOp;
int lenghtVar ;

/* Reads given file returns string array */
char** readFile(FILE* fp,int type) {
	char** fileArray = (char**)malloc(sizeof(char*)*10) ;
	int i = 0 ;
	fseek(fp,0,SEEK_END) ;
	int lenghtOfFile = ftell(fp) ;
	fseek(fp,0,SEEK_SET) ;
	char *buffer = (char*)malloc(sizeof(char)*lenghtOfFile) ; 
	while(!feof(fp)) {
		fscanf(fp,"%s",buffer) ;
		fileArray[i] = (char*)malloc(sizeof(char)*strlen(buffer)) ;
		strcpy(fileArray[i],buffer) ;
		i++ ;
	}
	if(type == 0){lenghtOp=i ;}
	else if(type == 1){lenghtPreOp=i;}
	else if(type == 2){lenghtRelOp=i;}
	else if(type == 3){lenghtSetOp=i;}
	else if(type == 4){lenghtVar=i;}
	fclose(fp) ;
	return fileArray ;
}

/* generates integer randomly in given number range */
int generateNumber(int lower,int upper) {
    int num = (rand() % (upper-lower+1)) + lower ;
    return num ;
}

/* Creates terminal node which is contains terminal symbol */
void* createNode(int type,int random) {
	struct Node *node = (struct Node*)malloc(sizeof(struct Node)) ;
	node->type = 0 ;
	if(type == 0) {
		node->data = op[random] ;
	}
	else if(type == 1) {
		node->data = pre_op[random] ;
	}
	else if(type == 2) {
		node->data = rel_op[random] ;
	}
	else if(type == 3) {
		node -> data = set_op[random] ;
	}
	else if(type == 4) {
		node -> data = var[random] ;
	}

	return node ;
}

/* Creates node to point terminal node with respect to its type. */
void* setOp() {
	struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
	node->type = 1 ;
	int i = generateNumber(0,lenghtSetOp-1) ;
	node->child1 = createNode(3,i) ;
	return node ;
}

void* relOp() {
	struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
	node->type = 1 ;
	int i = generateNumber(0,lenghtRelOp-1) ;
	node->child1 = createNode(2,i) ;
	return node ;
}

void* OP() {
	struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
	node->type = 1 ;
	int i = generateNumber(0,lenghtOp-1) ;
	node->child1 = createNode(0,i) ;
	return node ;
}

void* preOp() {
	struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
	node->type = 1 ;
	int i = generateNumber(0,lenghtPreOp-1) ;
	node->child1 = createNode(1,i) ;
	return node ;
}

void* VAR() {
	struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
	node->type = 1 ;
	int i = generateNumber(0,lenghtVar-1) ;
	node->child1 = createNode(4,i) ;
	return node ;
}


/* Creates <expr> offspring with respect to its type. */
void* createExpr() {
	void *p ;
	int i = generateNumber(0,2) ;
	if(i == 0) {
		struct NodeWithThreeChildren *node = (struct NodeWithThreeChildren*)malloc(sizeof(struct NodeWithThreeChildren)) ;
		node->type = 3 ;
		node->child1 = createExpr() ;
		node->child2 = OP() ;
		node->child3 = createExpr() ;
		return node ;
	}
	else if(i == 1) {
		struct NodeWithTwoChildren *node = (struct NodeWithTwoChildren*)malloc(sizeof(struct NodeWithTwoChildren)) ;
		node->type = 2 ;
		node->child1 = preOp() ;
		node->child2 = createExpr() ;
		return node ;
	}
	else if(i == 2) {
		struct NodeWithOneChild *node = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;
		node->type = 1 ;
		node->child1 = VAR() ;
		return node ;
	}
	return p ;
}

/* Creates <cond> offspring with respect to its type */
void* createCond() {
	struct NodeWithThreeChildren *node = (struct NodeWithThreeChildren*)malloc(sizeof(struct NodeWithThreeChildren)) ;
	node->type = 3 ;
	int i = generateNumber(0,1) ;
	if(i == 0) {
		node->child1 = createCond() ;
		node->child2 = setOp() ;
		node->child3 = createCond() ;
	}
	else if(i == 1){
		node->child1 = createExpr() ;
		node->child2 = relOp() ;
		node->child3 = createExpr() ;
	}

	return node ;
}

/* PRINTS TREE RECURSIVELY */
void printTree(void *node) {
	if(((struct Node*)node)->type == 0) {
		printf("%s",((struct Node*)node)->data);
		return ;
	}
	else if(((struct NodeWithOneChild*)node)->type == 1) {
		printTree(((struct NodeWithOneChild*)node)->child1) ;
	}
	else if(((struct NodeWithTwoChildren*)node)->type == 2) {
		printTree(((struct NodeWithTwoChildren*)node)->child1) ;
		printf("(");
		printTree(((struct NodeWithTwoChildren*)node)->child2) ;
		printf(")") ;
	}
	else if(((struct NodeWithThreeChildren*)node)->type == 3) {
		printf("(");
		printTree(((struct NodeWithThreeChildren*)node)->child1) ;
		printTree(((struct NodeWithThreeChildren*)node)->child2) ;
		printTree(((struct NodeWithThreeChildren*)node)->child3) ;
		printf(")");
	}
}

int main() {
	root = (struct NodeWithOneChild*)malloc(sizeof(struct NodeWithOneChild)) ;   /* root initialization */
	lenghtVar=0;lenghtOp=0;lenghtSetOp=0;lenghtRelOp=0;lenghtPreOp= 0 ;   /* lenght of terminal files */
	/* FILE READING PART */
	FILE *fp = fopen("op","r") ;
    op = readFile(fp,0) ;
    fp = fopen("pre_op","r") ;
    pre_op = readFile(fp,1) ;
    fp = fopen("rel_op","r") ;
    rel_op = readFile(fp,2) ;
    fp = fopen("set_op","r") ;
    set_op = readFile(fp,3) ;
    fp = fopen("var","r") ;
    var = readFile(fp,4) ;
    srand(time(0)) ;		/* to generate random number */
    root->child1 = createCond() ;  /* creating tree recursively and randomly */
    printf("if(");
  	printTree(root->child1) ;  /* printing tree recursively */
    printf(") { }") ;
}





