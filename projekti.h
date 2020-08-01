struct student {
    char id[6];
    char* firstname;
    char* lastname;
    int scores[6];
    int total;
};

typedef struct student Student;

Student *add_student(Student* collection, const char* p_id, const char* p_lastname, const char* p_firstname);

Student *update_points(Student* collection, const char* p_id, int p_round, int p_score);

void print_points(Student* collection);

void save_results(Student* collection, const char* filename);

Student *load_results(const char* filename);

void freeup(Student* collection);

int sort_descending(const void *, const void *);
