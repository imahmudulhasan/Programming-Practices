#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INF 1e9

typedef struct { int r, c; } Point;

/* ===================== Linked List for Path ===================== */
typedef struct PathNode {
    Point p;
    struct PathNode* next;
} PathNode;

PathNode* path_push_front(PathNode* head, Point p) {
    PathNode* n = (PathNode*)malloc(sizeof(PathNode));
    n->p = p; n->next = head; return n;
}

PathNode* path_reverse(PathNode* head) {
    PathNode* prev = NULL; PathNode* cur = head;
    while (cur) { PathNode* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    return prev;
}

void path_free(PathNode* head) {
    while (head) { PathNode* t = head; head = head->next; free(t); }
}

/* ===================== Stack (for DFS) ===================== */
typedef struct {
    int *a; int top; int cap;
} Stack;

Stack stack_new(int cap){ Stack s; s.a=(int*)malloc(sizeof(int)*cap); s.top=-1; s.cap=cap; return s; }
int stack_empty(Stack* s){ return s->top<0; }
void stack_push(Stack* s, int v){ s->a[++s->top]=v; }
int stack_pop(Stack* s){ return s->a[s->top--]; }
void stack_free(Stack* s){ free(s->a); }

/* ===================== Queue (for BFS) ===================== */
typedef struct {
    int *a; int head, tail, size, cap;
} Queue;

Queue queue_new(int cap){ Queue q; q.a=(int*)malloc(sizeof(int)*cap); q.head=q.tail=q.size=0; q.cap=cap; return q; }
int queue_empty(Queue* q){ return q->size==0; }
void enqueue(Queue* q, int v){ q->a[q->tail]=v; q->tail=(q->tail+1)%q->cap; q->size++; }
int dequeue(Queue* q){ int v=q->a[q->head]; q->head=(q->head+1)%q->cap; q->size--; return v; }
void queue_free(Queue* q){ free(q->a); }

/* ===================== Min-Heap (for A*) ===================== */
typedef struct {
    int *node; double *prio; int n, cap;
} MinHeap;

MinHeap heap_new(int cap){
    MinHeap h; h.node=(int*)malloc(sizeof(int)*cap);
    h.prio=(double*)malloc(sizeof(double)*cap);
    h.n=0; h.cap=cap; return h;
}

void heap_swap(MinHeap* h, int i, int j){
    int tn=h->node[i]; h->node[i]=h->node[j]; h->node[j]=tn;
    double tp=h->prio[i]; h->prio[i]=h->prio[j]; h->prio[j]=tp;
}

void heap_up(MinHeap* h, int i){
    while(i>0){
        int p=(i-1)/2;
        if(h->prio[p] <= h->prio[i]) break;
        heap_swap(h,p,i); i=p;
    }
}
void heap_down(MinHeap* h, int i){
    for(;;){
        int l=2*i+1, r=2*i+2, m=i;
        if(l<h->n && h->prio[l]<h->prio[m]) m=l;
        if(r<h->n && h->prio[r]<h->prio[m]) m=r;
        if(m==i) break;
        heap_swap(h,i,m); i=m;
    }
}
void heap_push(MinHeap* h, int node, double pr){
    h->node[h->n]=node; h->prio[h->n]=pr; heap_up(h,h->n); h->n++;
}
int heap_pop(MinHeap* h, double* outPrio){
    int v=h->node[0]; if(outPrio) *outPrio=h->prio[0];
    h->n--; h->node[0]=h->node[h->n]; h->prio[0]=h->prio[h->n]; heap_down(h,0);
    return v;
}
int heap_empty(MinHeap* h){ return h->n==0; }
void heap_free(MinHeap* h){ free(h->node); free(h->prio); }

/* ===================== Maze / Grid Helpers ===================== */
typedef struct {
    int R, C;
    char **g;
    Point S, E;
} Maze;

int in_bounds(Maze* M, int r, int c){ return (r>=0 && r<M->R && c>=0 && c<M->C); }
int to_id(Maze* M, int r, int c){ return r*M->C + c; }
Point from_id(Maze* M, int id){ Point p = { id / M->C, id % M->C }; return p; }

int passable(Maze* M, int r, int c){ return M->g[r][c] != '#'; }

/* Manhattan heuristic for A* */
int manhattan(Point a, Point b){ int dr=a.r-b.r; if(dr<0) dr=-dr; int dc=a.c-b.c; if(dc<0) dc=-dc; return dr+dc; }

/* Reconstruct Path to Linked List from parent[] */
PathNode* reconstruct(Maze* M, int* parent, int startId, int goalId){
    if(parent[goalId]==-2) return NULL; /* never set */
    PathNode* head=NULL;
    for(int cur=goalId; cur!=-1; cur=parent[cur]){
        head = path_push_front(head, from_id(M, cur));
    }
    return head; /* start..goal order */
}

/* Overlay path (*) and print */
void print_with_path(Maze* M, PathNode* path){
    /* Copy grid */
    char **cpy = (char**)malloc(sizeof(char*)*M->R);
    for(int r=0;r<M->R;r++){ cpy[r]=(char*)malloc(M->C+1); memcpy(cpy[r], M->g[r], M->C); cpy[r][M->C]='\0'; }
    for(PathNode* it=path; it; it=it->next){
        int r=it->p.r, c=it->p.c;
        if(cpy[r][c]=='.') cpy[r][c]='*';
    }
    cpy[M->S.r][M->S.c]='S';
    cpy[M->E.r][M->E.c]='E';
    printf("\nSolved Maze (path = *):\n");
    for(int r=0;r<M->R;r++) printf("%s\n", cpy[r]);
    for(int r=0;r<M->R;r++) free(cpy[r]); free(cpy);
}

/* ===================== BFS (Queue) ===================== */
PathNode* solve_bfs(Maze* M){
    int N = M->R * M->C;
    int *vis = (int*)calloc(N, sizeof(int));
    int *parent = (int*)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++) parent[i]=-2; /* -2 means unseen */
    int s = to_id(M, M->S.r, M->S.c);
    int t = to_id(M, M->E.r, M->E.c);

    Queue q = queue_new(N);
    enqueue(&q, s); vis[s]=1; parent[s]=-1;
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    while(!queue_empty(&q)){
        int u = dequeue(&q);
        if(u==t) break;
        Point pu = from_id(M,u);
        for(int k=0;k<4;k++){
            int nr=pu.r+dirs[k][0], nc=pu.c+dirs[k][1];
            if(in_bounds(M,nr,nc) && passable(M,nr,nc)){
                int v=to_id(M,nr,nc);
                if(!vis[v]){
                    vis[v]=1; parent[v]=u; enqueue(&q,v);
                }
            }
        }
    }
    PathNode* path = reconstruct(M,parent,s,t);
    queue_free(&q); free(vis); free(parent);
    return path;
}

/* ===================== DFS (Stack) ===================== */
PathNode* solve_dfs(Maze* M){
    int N = M->R * M->C;
    int *vis = (int*)calloc(N, sizeof(int));
    int *parent = (int*)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++) parent[i]=-2;
    int s = to_id(M, M->S.r, M->S.c);
    int t = to_id(M, M->E.r, M->E.c);

    Stack st = stack_new(N);
    stack_push(&st, s); parent[s]=-1;
    int found=0;
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};

    while(!stack_empty(&st)){
        int u = stack_pop(&st);
        if(vis[u]) continue;
        vis[u]=1;
        if(u==t){ found=1; break; }
        Point pu=from_id(M,u);
        for(int k=0;k<4;k++){
            int nr=pu.r+dirs[k][0], nc=pu.c+dirs[k][1];
            if(in_bounds(M,nr,nc) && passable(M,nr,nc)){
                int v=to_id(M,nr,nc);
                if(!vis[v]){ parent[v]=u; stack_push(&st,v); }
            }
        }
    }
    PathNode* path = found ? reconstruct(M,parent,s,t) : NULL;
    stack_free(&st); free(vis); free(parent);
    return path;
}

/* ===================== A* (Min-Heap) ===================== */
PathNode* solve_astar(Maze* M){
    int N = M->R * M->C;
    double *g = (double*)malloc(sizeof(double)*N);
    int *parent = (int*)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++){ g[i]=INF; parent[i]=-2; }
    int s = to_id(M, M->S.r, M->S.c);
    int t = to_id(M, M->E.r, M->E.c);

    MinHeap h = heap_new(N);
    g[s]=0; parent[s]=-1;
    heap_push(&h, s, 0 + manhattan(M->S, M->E));

    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};

    while(!heap_empty(&h)){
        double fp; int u = heap_pop(&h, &fp);
        if(u==t) break;
        Point pu=from_id(M,u);
        for(int k=0;k<4;k++){
            int nr=pu.r+dirs[k][0], nc=pu.c+dirs[k][1];
            if(in_bounds(M,nr,nc) && passable(M,nr,nc)){
                int v=to_id(M,nr,nc);
                double ng = g[u] + 1.0; /* cost 1 per move */
                if(ng < g[v]){
                    g[v]=ng; parent[v]=u;
                    Point pv = { nr, nc };
                    double f = ng + manhattan(pv, M->E);
                    heap_push(&h, v, f); /* no decrease-key, allow duplicates */
                }
            }
        }
    }
    PathNode* path = reconstruct(M,parent,s,t);
    heap_free(&h); free(g); free(parent);
    return path;
}

/* ===================== I/O ===================== */
int load_maze(const char* filename, Maze* M){
    FILE* fp = fopen(filename, "r");
    if(!fp){ perror("open"); return 0; }
    if(fscanf(fp, "%d %d\n", &M->R, &M->C)!=2){ fclose(fp); return 0; }
    M->g = (char**)malloc(sizeof(char*)*M->R);
    int hasS=0, hasE=0;
    for(int r=0;r<M->R;r++){
        M->g[r] = (char*)malloc(M->C+1);
        int got = 0;
        while(got < M->C){
            int ch = fgetc(fp);
            if(ch==EOF){ fclose(fp); return 0; }
            if(ch=='\n' || ch=='\r') continue;
            M->g[r][got++] = (char)ch;
        }
        M->g[r][M->C]='\0';
        for(int c=0;c<M->C;c++){
            if(M->g[r][c]=='S'){ M->S=(Point){r,c}; hasS=1; }
            if(M->g[r][c]=='E'){ M->E=(Point){r,c}; hasE=1; }
        }
    }
    fclose(fp);
    if(!hasS || !hasE){ fprintf(stderr,"Maze must contain S and E.\n"); return 0; }
    return 1;
}

void free_maze(Maze* M){
    for(int r=0;r<M->R;r++) free(M->g[r]);
    free(M->g);
}

void print_maze(Maze* M){
    printf("Maze %dx%d:\n", M->R, M->C);
    for(int r=0;r<M->R;r++) puts(M->g[r]);
}

/* ===================== Main ===================== */
   int main(int argc, char** argv){
    Maze M;
    const char* file = NULL;
    char buf[512];

    if (argc >= 2) {
        file = argv[1];
    } else {
        printf("Enter maze file path (or press Enter for 'maze.txt'): ");
        if (fgets(buf, sizeof(buf), stdin)) {
            size_t n = strlen(buf);
            while (n && (buf[n-1] == '\n' || buf[n-1] == '\r')) buf[--n] = '\0';
            file = (n > 0) ? buf : "maze.txt";
        } else {
            file = "maze.txt";
        }
    }

    if (!load_maze(file, &M)) {
        fprintf(stderr, "Failed to load '%s'\n", file);
        return 1;
    }

    print_maze(&M);
    printf("\nChoose algorithm:\n");
    printf("1) BFS\n2) DFS\n3) A*\nEnter 1/2/3: ");
    int choice=0; if (scanf("%d",&choice)!=1){ fprintf(stderr,"Invalid input\n"); free_maze(&M); return 1; }

    PathNode* path=NULL;
    if (choice==1) path = solve_bfs(&M);
    else if (choice==2) path = solve_dfs(&M);
    else if (choice==3) path = solve_astar(&M);
    else { fprintf(stderr,"Unknown choice\n"); free_maze(&M); return 1; }

    if (!path){ printf("\nNo path found!\n"); free_maze(&M); return 0; }

    int steps=0; for(PathNode* it=path; it; it=it->next) steps++;
    printf("\nPath found! Length (nodes) = %d, moves = %d\n", steps, steps-1);
    print_with_path(&M, path);

    path_free(path);
    free_maze(&M);
    return 0;
}
