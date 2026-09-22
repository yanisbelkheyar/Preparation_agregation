struct objet {
    int p;
    int v;
};

struct donnees {
    int n;
    int pmax;
    struct objet *tab;
};

struct donnees *creer_tab_objets(char *filename);