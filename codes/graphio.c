/*
**   Celso Antonio Uliana Junior,
**   April 2019
*/
#include "graphio.h"

TGraph * readGraphDimacs(TGraph * g, FILE * handler){
    int n, m, dir, flag = 0;
    char d, * p = (char *) malloc (255 * sizeof(char));;
    
    fscanf(handler, "%c %c %d %d %s\n", &d, &d, &n, &m, p);
    
    dir = d == 'U' ? 0 : 1;
    
    if(g == NULL)
        g = createGraph(n, p, dir); 
    else
        flag = initGraph(g, n, p, dir);
    
    for(int i = 0; i < n; i++){
        int aux = 0;
        double weight = 0;
        char c, * label = (char *) malloc(255 * sizeof(char));;
        
        fscanf(handler, "%c %d %lf %s\n", &c, &aux, &weight, label);
        g -> vertices[i].weight = weight;
        g -> vertices[i].label = label;
    }
    
    for(int i = 0; i < m; i++){
        int u, v;
        char c, * label = (char *) malloc(255 * sizeof(char));
        double weight = 0;
        
        fscanf(handler, "%c %d %d %f %s\n", &c, &u, &v, &weight, label);
        connectWeight(g, u, v, weight, label);
    }
    
    return g;
}

TGraph * debugReadGraphDimacs(TGraph * g){
    int n, m, dir;
    char d, * p = (char *) malloc (255 * sizeof(char));;
    
    scanf("%c %c %d %d %s\n", &d, &d, &n, &m, p);
    
    dir = d == 'U' ? 0 : 1;
    
    if(g == NULL)
        g = createGraph(n, p, dir);    
    else
        initGraph(g, n, p, dir);
    
    for(int i = 0; i < n; i++){
        int aux = 0;
        double weight = 0;
        char c, * label = (char *) malloc(255 * sizeof(char));;
        
        scanf("%c %d %lf %s\n", &c, &aux, &weight, label);
        g -> vertices[i].weight = weight;
        g -> vertices[i].label = label;
    }
    
    for(int i = 0; i < m; i++){
        int u, v;
        char c, * label = (char *) malloc(255 * sizeof(char));
        double weight = 0;
        
        scanf("%c %d %d %f %s\n", &c, &u, &v, &weight, label);
        connectWeight(g, u, v, weight, label);
    }
    
    return g;
}

int saveGraphDimacs(const TGraph * g, FILE * handler){  
    char dir = g -> directed == 1 ? 'D' : 'U' ;
    int i;
    
    fprintf(handler, "G %c %d %d %s\n", dir, g -> n, g -> m, g -> name);
    
    for(i = 0; i < g -> n; i++){
        fprintf(handler, "N %d %lf %s\n", i, g -> vertices[i].weight, g -> vertices[i].label);
    }
    
    if(g -> vertices != NULL){
        
        int vis[g -> n][g -> n];
        
        memset(vis, 0, sizeof vis); 
        
        for(i = 0; i < g -> n; i++){
            TNodeList * cur = g -> vertices[i].direct;

            while(cur != NULL){

                if(g -> directed){
                    if(!vis[i][cur -> edge.destination]){
                        fprintf(handler, "E %d %d %lf %s\n", 
                        i, cur -> edge.destination, cur -> edge.weight, cur -> edge.label);
                        vis[i][cur -> edge.destination] = 1;
                    }
                }
                
                else{
                    if(!vis[i][cur -> edge.destination] || !vis[cur -> edge.destination][i]){
                        fprintf(handler, "E %d %d %lf %s\n", 
                        i, cur -> edge.destination, cur -> edge.weight, cur -> edge.label);
                        vis[i][cur -> edge.destination] = vis[cur -> edge.destination][i] = 1;
                    }
                }
                
                cur = cur -> next;
            }
        }
    }
    
    return 1;
}

int debugSaveGraphDimacs(const TGraph * g){  
    char dir = g -> directed == 1 ? 'D' : 'U' ;
    int i;
    
    printf("G %c %d %d %s\n", dir, g -> n, g -> m, g -> name);
    
    for(i = 0; i < g -> n; i++){
        printf("N %d %lf %s\n", i, g -> vertices[i].weight, g -> vertices[i].label);
    }
    
    if(g -> vertices != NULL){
        
        int vis[g -> n][g -> n];
        
        memset(vis, 0, sizeof vis); 
        
        for(i = 0; i < g -> n; i++){
            TNodeList * cur = g -> vertices[i].direct;
            
            while(cur != NULL){

                if(g -> directed){
                    if(!vis[i][cur -> edge.destination]){
                        printf("E %d %d %lf %s\n", i, cur -> edge.destination, 
                        cur -> edge.weight, cur -> edge.label);
                        vis[i][cur -> edge.destination] = 1;
                    }
                }
                
                else{
                    if(!vis[i][cur -> edge.destination] || !vis[cur -> edge.destination][i]){
                        printf("E %d %d %lf %s\n", i, cur -> edge.destination, 
                        cur -> edge.weight, cur -> edge.label);
                        vis[i][cur -> edge.destination] = vis[cur -> edge.destination][i] = 1;
                    }
                }
                
                cur = cur -> next;
            }
            
        }
    }
    
    return 1;
}

int saveGraphDot(const TGraph * g, FILE * handler){
    
    if(g -> directed)
        fprintf(handler, "digraph %s {\n", g -> name);
    else
        fprintf(handler, "graph %s {\n", g -> name);
        
    
    fprintf(handler, " graph [fontsize = 36,\n\tlabel=\"\\n\\n%s - " 
    "Autor: Celso A. Uliana Junior -- Sep 2018\",\n \tlabelloc=t]\n", g -> name);
    
    for(int i = 0; i < g -> n; i++){
        fprintf(handler, " V%d [label=\"%s\\n%lf\"]\n", 
        i, g -> vertices[i].label, g -> vertices[i].weight);
    }
    
    int vis[g -> n][g -> n];
        
    memset(vis, 0, sizeof vis); 
        
    for(int i = 0; i < g -> n; i++){
        TNodeList * cur = g -> vertices[i].direct;
            
        while(cur != NULL){
            
            if(g -> directed){
                if(!vis[i][cur -> edge.destination]){
                    fprintf(handler, " V%d -> V%d [label=\"%s\\n%lf\"]\n",
                     i, cur -> edge.destination, cur -> edge.label, cur -> edge.weight);
                    vis[i][cur -> edge.destination] = 1;
                }
            }
            
            else{
                if(!vis[i][cur -> edge.destination] || !vis[cur -> edge.destination][i]){
                    fprintf(handler, " V%d -- V%d [label=\"%s\\n%lf\"]\n",
                     i, cur -> edge.destination, cur -> edge.label, cur -> edge.weight);
                    vis[i][cur -> edge.destination] = vis[cur -> edge.destination][i] = 1;
                }
            }
                
            cur = cur -> next;
        }     
    }
    
    fprintf(handler, "}\n");
    
    return 1;
}

int debugSaveGraphDot(const TGraph * g){
    
    if(g -> directed)
        printf("digraph %s {\n", g -> name);
    else
        printf("graph %s {\n", g -> name);
        
    printf(" graph [fontsize = 36,\n\tlabel=\"\\n\\n%s - "
    "Autor: Celso A. Uliana Junior -- Apri 2019\",\n \tlabelloc=t]\n", g -> name);
    
    for(int i = 0; i < g -> n; i++){
        printf(" V%d [label=\"%s\\n%lf\"]\n", 
        i, g -> vertices[i].label, g -> vertices[i].weight);
    }
    
    int vis[g -> n][g -> n];
        
    memset(vis, 0, sizeof vis); 
        
    for(int i = 0; i < g -> n; i++){
        TNodeList * cur = g -> vertices[i].direct;
            
        while(cur != NULL){
            
            if(g -> directed){
                if(!vis[i][cur -> edge.destination]){
                    printf(" V%d -> V%d [label=\"%s\\n%lf\"]\n",
                    i, cur -> edge.destination, cur -> edge.label, cur -> edge.weight);
                    vis[i][cur -> edge.destination] = 1;
                }
            }
            
            else{
                if(!vis[i][cur -> edge.destination] || !vis[cur -> edge.destination][i]){
                    printf(" V%d -- V%d [label=\"%s\\n%lf\"]\n",
                    i, cur -> edge.destination, cur -> edge.label, cur -> edge.weight);
                    vis[i][cur -> edge.destination] = vis[cur -> edge.destination][i] = 1;
                }
            }
                
            cur = cur -> next;
        }    
    }
    
    printf("}\n");
        
    return 1;
}
