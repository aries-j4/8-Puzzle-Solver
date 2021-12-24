#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define N 3

int row[] = {1, 0, -1, 0};
int col[] = {0, -1, 0, 1};

struct Node
{
    struct Node * parent;
    int matrix[N][N];
    int x, y; // blank tile co-ordinates
    int cost; // no. of misplaced tiles
    int level; // no. of moves(depth)
};

void printMatrix(int matrix[N][N], int level)
{
    printf("\n\n");
    if (level == 0)
    {
        printf("\t\t\t\t\t\t  ");
        printf("Initial State");
    }
    else
    {
        printf("\t\t\t\t\t\t     ");
        printf("Step : %d", level);
    }
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t");
    printf("_____|_____|_____\n");
    printf("\t\t\t\t\t\t");
    printf("     |     |     \n");
    for (int i = 0; i < N; ++i)
    {
        printf("\t\t\t\t\t\t");
        for (int j = 0; j < N; ++j)
        {
            if (j == N-1)
            {
                if (matrix[i][j] == 0)
                {
                    printf("      ");
                }

                else
                {
                    printf("  %d   ", matrix[i][j]);
                }
            }
            else
            {
                if (matrix[i][j] == 0)
                {
                    printf("     |");
                }

                else
                {
                    printf("  %d  |", matrix[i][j]);
                }
            }
        }
        printf("\n");
        printf("\t\t\t\t\t\t");
        printf("_____|_____|_____\n");
        printf("\t\t\t\t\t\t");
        printf("     |     |     \n");
    }
    printf("\n");
}

struct Node * newNode(int matrix[N][N], int x, int y, int newX, int newY, int level, struct Node * parent)
{
    int temp;
    struct Node * n = (struct Node *) malloc(sizeof(struct Node));
    n->parent = parent;     
    memcpy(n->matrix, matrix, sizeof(n->matrix));
    temp = n->matrix[x][y];
    n->matrix[x][y] = n->matrix[newX][newY];
    n->matrix[newX][newY] = temp;
    n->cost = INT_MAX;
    n->level = level;
    n->x = newX;
    n->y = newY;
    return n;
}

int calCost(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (initial[i][j] && initial[i][j] != final[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

int isValid(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

void printPath(struct Node * root)
{
    if(root == NULL)
    {
        return;
    }
    printPath(root->parent);
    printMatrix(root->matrix, root->level);
    printf("\n");
}

struct pq_struct
{
    struct Node * og_node;
    int priority;
    struct pq_struct * next;
};

struct pq_struct * pq_newNode(struct Node * root)
{
    struct pq_struct * temp = (struct pq_struct *) malloc(sizeof(struct pq_struct));
    temp->og_node = root;
    temp->priority = root->cost + root->level;
    temp->next = NULL;
    return temp;
}

struct Node * pq_peek(struct pq_struct ** head)
{
    return (*head)->og_node;
}

void pq_pop(struct pq_struct ** head)
{
    struct pq_struct * temp = (*head);
    (*head) = (*head)->next;
    free(temp);
}

void pq_push(struct pq_struct ** head, struct Node * root)
{
    struct pq_struct * start = (struct pq_struct *) malloc(sizeof(struct pq_struct));
    start = (*head);
    struct pq_struct * temp = (struct pq_struct *) malloc(sizeof(struct pq_struct));
    temp = pq_newNode(root);
    if ((*head) == NULL)
    {
        (*head) = pq_newNode(root);
    }
    else if ((*head)->priority > temp->priority)
    {
        temp->next = (*head);
        (*head) = temp;
    }
    else
    {
        while(start->next != NULL && start->next->priority < temp->priority)
        {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}

int pq_isEmpty(struct pq_struct ** head)
{
    return ((*head) == NULL);
}

void solve(int initial[N][N], int x, int y, int final[N][N])
{
    struct Node * root = (struct Node *) malloc(sizeof(struct Node));
    root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calCost(initial, final);

    struct pq_struct * pq = (struct pq_struct *) malloc(sizeof(struct pq_struct));
    pq = pq_newNode(root);

    while(!pq_isEmpty(&pq))
    {
        struct Node * min = pq_peek(&pq);
        pq_pop(&pq);
        if (min->cost == 0)
        {
            printPath(min);
            return;
        }
        for (int i = 0; i < 4; ++i) // max 4 children of every node
        {
            if (isValid(min->x + row[i], min->y +col[i]))
            {
                struct Node * child = (struct Node *) malloc(sizeof(struct Node));
                child = newNode(min->matrix, min->x, min->y, min->x + row[i], min->y + col[i], min->level + 1, min);
                child->cost = calCost(child->matrix, final);
                pq_push(&pq ,child);
            }
        }
    }
}

int main()
{
    //style from: http://patorjk.com/software/taag/#p=display&f=Standard&t=8-Puzzle%20Solver
    printf("\n\
   ___        ____                _        ____        _\n\
  ( _ )      |  _ \\ _   _ _______| | ___  / ___|  ___ | |_   _____ _ __\n\
  / _ \\ _____| |_) | | | |_  /_  / |/ _ \\ \\___ \\ / _ \\| \\ \\ / / _ \\ '__|\n\
 | (_) |_____|  __/| |_| |/ / / /| |  __/  ___) | (_) | |\\ V /  __/ |\n\
  \\___/      |_|    \\__,_/___/___|_|\\___| |____/ \\___/|_| \\_/ \\___|_|\n");
    printf("\n\n\n");

    int initial[N][N] =
    {
        {1, 2, 3},
        {5, 6, 0},
        {7, 8, 4}
    };

    int final[N][N] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    int x = 1, y = 2;
    solve(initial, x, y, final);

    printf("\n");
    printf("\t\t\t\t\t\t");
    printf("Solution Achieved!\n\n\n");

    system("pause");
  
  return 0;
}