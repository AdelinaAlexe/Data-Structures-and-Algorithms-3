#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura pentru o ruta
typedef struct route {
  char *city;
  int trons;
  float *degrad;
  int *change;
  struct route *next;
} route;

// structura pentru o lista de rute
typedef struct list {
  struct route *start;
} list;

// structura pentru un graf
typedef struct Graph {
  int n;
  struct list *adj_list;
} Graph;

// structura pentru a retine rutele in ordine
typedef struct structs {
  char *start_city;
  char *end_city;
  int trons;
  float *degrad;
} structs;

// structura pentru o lista de adiacenta
typedef struct adjList {
  int start, end, cost;
} adjList;

// structura pentru un graf
typedef struct Graph2 {
  int n, m;
  struct adjList *adj_list;
} Graph2;

// strcutura pentru a retine o ruta
typedef struct set {
  int parent;
  int rank;
} set;

// structura pentru a retine o ruta
typedef struct saveRoute {
  char start[100];
  char end[100];
  int cost;
} saveRoute;

// functie pentru a initializa o structura de tip structs
void initS(struct structs *vect) {
  if (vect == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  vect->start_city = malloc(100 * sizeof(char));

  if (vect->start_city == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  vect->end_city = malloc(100 * sizeof(char));

  if (vect->end_city == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  vect->degrad = NULL;
}

// functie pentru a initializa o lista de rute
void initL(struct list *adj_list, char *start_city) {
  adj_list->start = malloc(sizeof(struct route));

  if (adj_list->start == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  adj_list->start->city = malloc(100 * sizeof(char));

  if (adj_list->start->city == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  int len = strlen(start_city);
  int i;

  for (i = 0; i < len; i++) adj_list->start->city[i] = start_city[i];

  adj_list->start->city[len] = '\0';

  adj_list->start->trons = 0;
  adj_list->start->degrad = NULL;
  adj_list->start->change = NULL;

  adj_list->start->next = NULL;
}

// functie pentru a adauga o ruta in lista de rute
void addRoute(struct list *adj_list, char *end_city, int trons, float *degrad) {
  struct route *q = adj_list->start;

  while (q->next != NULL) q = q->next;

  struct route *t = malloc(sizeof(struct route));

  if (t == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  t->city = malloc(100 * sizeof(char));

  if (t->city == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  int len = strlen(end_city);
  int i;

  for (i = 0; i < len; i++) t->city[i] = end_city[i];

  t->city[len] = '\0';

  t->trons = trons;
  t->degrad = malloc(t->trons * sizeof(float));
  t->change = calloc(t->trons, sizeof(int));

  if (t->degrad == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  for (i = 0; i < t->trons; i++) t->degrad[i] = degrad[i];

  q->next = t;
  t->next = NULL;
}

// functie pentru a initializa un graf
void initG(struct Graph *graph, int n) {
  graph->n = n;
  graph->adj_list = malloc(100 * sizeof(struct list));

  if (graph->adj_list == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  int i;
  for (i = 0; i < n; i++) graph->adj_list[i].start = NULL;
}

// functie pentru a simula un an
void oneYear(struct Graph *graph) {
  int i;

  // parcurg fiecare ruta
  for (i = 0; i < graph->n; i++) {
    struct route *q = graph->adj_list[i].start;
    struct route *t = graph->adj_list[i].start->next;

    // parcurg fiecare oras
    while (t != NULL) {
      int j;

      // parcurg fiecare tronson
      for (j = 0; j < t->trons; j++) {
        // daca e 0 si nu a fost vizitat
        if (t->degrad[j] == 0 && t->change[j] == 0) {
          // daca e primul tronson
          if (j == 0) {
            float max = 0;
            struct route *p = q->next;

            // parcurg toate rutele si verific legaturile din margine
            while (p != NULL) {
              if (p->degrad[0] > max && p->change[0] == 0) max = p->degrad[0];

              p = p->next;
            }

            int k;

            // parcurg toate rutele si verific legaturile din margine
            for (k = 0; k < graph->n; k++) {
              if (k != i) {
                struct route *p = graph->adj_list[k].start->next;

                // parcurg toate orasele
                while (p != NULL) {
                  if (strcmp(p->city, q->city) == 0) {
                    if (p->degrad[p->trons - 1] > max &&
                        p->change[p->trons - 1] == 0)
                      max = p->degrad[p->trons - 1];
                  }
                  p = p->next;
                }
              }
            }

            // daca e un singur tronson
            if (t->trons == 1) {
              for (k = 0; k < graph->n; k++) {
                if (k != i) {
                  if (strcmp(t->city, graph->adj_list[k].start->city) == 0) {
                    struct route *p = graph->adj_list[k].start->next;

                    while (p != NULL) {
                      if (p->degrad[0] > max && p->change[0] == 0)
                        max = p->degrad[0];

                      p = p->next;
                    }
                  }
                }
              }

              // parcurg toate rutele si verific legaturile din margine
              for (k = 0; k < graph->n; k++) {
                if (k != i) {
                  struct route *p = graph->adj_list[k].start->next;
                  while (p != NULL) {
                    if (strcmp(t->city, p->city) == 0) {
                      if (p->degrad[p->trons - 1] > max &&
                          p->change[p->trons - 1] == 0)
                        max = p->degrad[p->trons - 1];
                    }
                    p = p->next;
                  }
                }
              }
            }

            // verific cu vecinii
            if (j + 1 < t->trons && t->degrad[j + 1] > max &&
                t->change[j + 1] == 0)
              max = t->degrad[j + 1];

            t->degrad[j] = max / 2;
            t->change[j] = 1;
          }

          else
            // daca e ultimul tronson
            if (j == t->trons - 1) {
              int k;
              float max = 0;

              for (k = 0; k < graph->n; k++) {
                if (k != i) {
                  if (strcmp(t->city, graph->adj_list[k].start->city) == 0) {
                    struct route *p = graph->adj_list[k].start->next;

                    while (p != NULL) {
                      if (p->degrad[0] > max && p->change[0] == 0)
                        max = p->degrad[0];

                      p = p->next;
                    }
                  }
                }
              }

              for (k = 0; k < graph->n; k++) {
                if (k != i) {
                  struct route *p = graph->adj_list[k].start->next;
                  while (p != NULL) {
                    if (strcmp(t->city, p->city) == 0) {
                      if (p->degrad[p->trons - 1] > max &&
                          p->change[p->trons - 1] == 0)
                        max = p->degrad[p->trons - 1];
                    }
                    p = p->next;
                  }
                }
              }

              if (t->degrad[j - 1] > max && t->change[j - 1] == 0)
                max = t->degrad[j - 1];

              t->degrad[j] = max / 2;
              t->change[j] = 1;
            }

            else {
              float max = 0;
              if (t->degrad[j - 1] > t->degrad[j + 1] && t->change[j - 1] == 0)
                max = t->degrad[j - 1];

              else if (t->degrad[j - 1] <= t->degrad[j + 1] &&
                       t->change[j + 1] == 0)
                max = t->degrad[j + 1];

              t->degrad[j] = max / 2;
              t->change[j] = 1;
            }
        }
      }

      t = t->next;
    }
  }

  // dublez pe cele diferit de 0
  for (i = 0; i < graph->n; i++) {
    struct route *t = graph->adj_list[i].start->next;

    while (t != NULL) {
      int j;

      for (j = 0; j < t->trons; j++) {
        if (t->degrad[j] != 0 && t->change[j] == 0) {
          t->degrad[j] = t->degrad[j] * 2;
          if (t->degrad[j] > 100) t->degrad[j] = 100;
          t->change[j] = 1;
        }
      }

      t = t->next;
    }
  }

  // resetez la 0 valorile din charge
  for (i = 0; i < graph->n; i++) {
    struct route *t = graph->adj_list[i].start->next;

    while (t != NULL) {
      int j;

      for (j = 0; j < t->trons; j++) {
        t->change[j] = 0;
      }

      t = t->next;
    }
  }
}

// functie pentru a initializa un graf
struct Graph2 *initG2(int n, int m) {
  struct Graph2 *graph = (struct Graph2 *)malloc(sizeof(struct Graph2));
  if (graph == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  graph->n = n;
  graph->m = m;
  graph->adj_list = (struct adjList *)malloc(m * sizeof(struct adjList));
  if (graph->adj_list == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  return graph;
}

// functie pentru a adauga o ruta in graf
void addG2(struct Graph2 *graph, int idx, int start, int end, int cost) {
  graph->adj_list[idx].start = start;
  graph->adj_list[idx].end = end;
  graph->adj_list[idx].cost = cost;
}

// functie pentru a compara costurile
int compareCosts(const void *a, const void *b) {
  struct adjList *x = (struct adjList *)a;
  struct adjList *y = (struct adjList *)b;
  return x->cost - y->cost;
}

// functie pentru a gasi un element
int find(struct set sets[], int i) {
  if (sets[i].parent != i) {
    sets[i].parent = find(sets, sets[i].parent);
  }
  return sets[i].parent;
}

// functie pentru a gasi rangul
void findRank(struct set sets[], int x, int y) {
  int a = find(sets, x);
  int b = find(sets, y);

  if (sets[a].rank < sets[b].rank) {
    sets[a].parent = b;
  } else if (sets[a].rank > sets[b].rank) {
    sets[b].parent = a;
  } else {
    sets[b].parent = a;
    sets[a].rank++;
  }
}

// functie pentru algoritmul lui Kruskal
void kruskal(struct Graph2 *graph, int max_routes, struct saveRoute routes[],
             int total_routes, char cities[][100], FILE *g) {
  int n = graph->n;
  struct adjList result[100];
  int num_routes = 0;
  int idx = 0, i, j;

  qsort(graph->adj_list, graph->m, sizeof(graph->adj_list[0]), compareCosts);

  struct set sets[100];

  for (i = 0; i < n; i++) {
    sets[i].parent = i;
    sets[i].rank = 0;
  }

  while (num_routes < max_routes && idx < graph->m) {
    struct adjList next_route = graph->adj_list[idx];
    idx++;

    int x = find(sets, next_route.start);
    int y = find(sets, next_route.end);

    if (x != y) {
      result[num_routes] = next_route;
      num_routes++;
      findRank(sets, x, y);
    }
  }

  fprintf(g, "%d\n", num_routes);
  for (i = 0; i < total_routes; i++) {
    for (j = 0; j < num_routes; j++) {
      if ((strcmp(routes[i].start, cities[result[j].start]) == 0 &&
           strcmp(routes[i].end, cities[result[j].end]) == 0) ||
          (strcmp(routes[i].start, cities[result[j].end]) == 0 &&
           strcmp(routes[i].end, cities[result[j].start]) == 0)) {
        fprintf(g, "%s %s\n", routes[i].start, routes[i].end);
        break;
      }
    }
  }
}

// functie pentru a gasi indexul unui oras
int getCityIndex(char *city, char cities[][100], int *num) {
  int i;
  int n = *num;
  for (i = 0; i < n; i++) {
    if (strcmp(city, cities[i]) == 0) {
      return i;
    }
  }
  strcpy(cities[n], city);
  return (*num)++;
}

// functie pentru a elibera memoria
void freeS(struct structs *vect) {
  free(vect->start_city);
  free(vect->end_city);
  free(vect->degrad);
}

// functie pentru a elibera memoria
void freeL(struct list *adj_list) {
  struct route *q = adj_list->start;

  while (q != NULL) {
    struct route *t = q;
    q = q->next;
    free(t->city);
    free(t->degrad);
    free(t->change);
    free(t);
  }
}

// functie pentru a calcula media
float grade(struct Graph *graph, char *start_city, char *end_city) {
  int i;
  float m_grade = 0;
  for (i = 0; i < graph->n; i++) {
    struct route *q = graph->adj_list[i].start;
    struct route *t = graph->adj_list[i].start->next;

    if (strcmp(q->city, start_city) == 0) {
      while (t != NULL) {
        if (strcmp(end_city, t->city) == 0) {
          int j;
          for (j = 0; j < t->trons; j++) m_grade = m_grade + t->degrad[j];

          m_grade = m_grade / t->trons;
          return m_grade;
        }
        t = t->next;
      }
    }
  }

  return 0;
}

// functie pentru a elibera memoria
void freeG(struct Graph *graph) {
  int i;
  for (i = 0; i < 100; i++) freeL(&graph->adj_list[i]);

  free(graph->adj_list);
}

// functie pentru a printa o ruta
void printG(struct Graph *graph, FILE *g, char *start_city, char *end_city) {
  int i;
  for (i = 0; i < graph->n; i++) {
    struct route *q = graph->adj_list[i].start;
    struct route *t = graph->adj_list[i].start->next;

    if (strcmp(start_city, q->city) == 0) {
      while (t != NULL) {
        if (strcmp(end_city, t->city) == 0) {
          fprintf(g, "%s ", graph->adj_list[i].start->city);
          fprintf(g, "%s %d ", t->city, t->trons);
          int j;
          for (j = 0; j < t->trons; j++)
            if (j < t->trons - 1)
              fprintf(g, "%.2f ", t->degrad[j]);

            else
              fprintf(g, "%.2f", t->degrad[j]);

          fprintf(g, "\n");
          return;
        }
        t = t->next;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argv[argc - 1][0] == '1') {
    FILE *f = fopen("tema3.in", "r");
    FILE *g = fopen("tema3.out", "w");

    int r, k, l, n, i, j, trons, len;
    float *degrad;
    char *start_city, *end_city;
    struct Graph graph;
    struct structs *vect;

    initG(&graph, 100);

    fscanf(f, "%d", &r);
    fscanf(f, "%d", &k);
    fscanf(f, "%d", &l);

    vect = malloc(r * sizeof(struct structs));
    for (i = 0; i < r; i++) initS(&vect[i]);

    // citesc datele
    n = 0;
    for (i = 0; i < r; i++) {
      start_city = malloc(100 * sizeof(char));
      end_city = malloc(100 * sizeof(char));

      fscanf(f, "%s", start_city);
      fscanf(f, "%s", end_city);
      fscanf(f, "%d", &trons);
      degrad = malloc(trons * sizeof(float));

      for (j = 0; j < trons; j++) fscanf(f, "%f", &degrad[j]);

      len = strlen(start_city);
      for (j = 0; j < len; j++) vect[i].start_city[j] = start_city[j];
      vect[i].start_city[len] = '\0';

      len = strlen(end_city);
      for (j = 0; j < len; j++) vect[i].end_city[j] = end_city[j];
      vect[i].end_city[len] = '\0';

      vect[i].trons = trons;
      vect[i].degrad = malloc(trons * sizeof(float));
      for (j = 0; j < trons; j++) vect[i].degrad[j] = degrad[j];

      int ok = 0;
      for (j = 0; j < n; j++) {
        if (strcmp(graph.adj_list[j].start->city, start_city) == 0) {
          ok = 1;
          addRoute(&graph.adj_list[j], end_city, trons, degrad);
        }
      }

      if (ok == 0) {
        initL(&graph.adj_list[n], start_city);
        addRoute(&graph.adj_list[n], end_city, trons, degrad);
        n++;
      }

      free(degrad);
      free(start_city);
      free(end_city);
    }
    graph.n = n;

    // apelez functia one year de un numar k de ani
    for (i = 0; i < k; i++) oneYear(&graph);

    for (i = 0; i < r; i++) {
      printG(&graph, g, vect[i].start_city, vect[i].end_city);
    }

    // calculez media si afisez indicii rutelor
    for (i = 0; i < r; i++) {
      float m_grade = grade(&graph, vect[i].start_city, vect[i].end_city);
      if (m_grade < l) fprintf(g, "%d ", i + 1);
    }

    // eliberez memoria

    for (i = 0; i < r; i++) freeS(&vect[i]);
    free(vect);
    freeG(&graph);
    fclose(f);
    fclose(g);

  }

  else {
    FILE *f = fopen("tema3.in", "r");
    FILE *g = fopen("tema3.out", "w");
    if (!f || !g) {
      printf("Error opening file\n");
      return 1;
    }

    char city[100];
    int k, m, i;
    char cities[100][100];
    int num_cities = 0;
    struct adjList routes[100];
    struct saveRoute save[100];

    fscanf(f, "%s", city);
    fscanf(f, "%d", &k);
    fscanf(f, "%d", &m);

    char start[100], end[100];
    int cost;

    // citesc datele
    for (i = 0; i < m; i++) {
      fscanf(f, "%s %s %d", start, end, &cost);
      int start_idx = getCityIndex(start, cities, &num_cities);
      int end_idx = getCityIndex(end, cities, &num_cities);

      routes[i].start = start_idx;
      routes[i].end = end_idx;
      routes[i].cost = cost;

      strcpy(save[i].start, start);
      strcpy(save[i].end, end);
      save[i].cost = cost;
    }

    // construiesc graful
    int n = num_cities;
    struct Graph2 *graph = initG2(n, m);

    for (i = 0; i < m; i++) {
      addG2(graph, i, routes[i].start, routes[i].end, routes[i].cost);
    }

    // apelez functia kruskal
    kruskal(graph, k, save, m, cities, g);

    free(graph->adj_list);
    free(graph);
    fclose(f);
    fclose(g);
  }

  return 0;
}
