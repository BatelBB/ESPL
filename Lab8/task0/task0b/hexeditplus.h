typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc {
  char *name;
  void (*fun)(state*);
}; 



void fun(state* s); // Getting the state as a pointer allows the functions to change it.
void toggleDebugMode(state *s);
void setFileName(state *s);
void setUnitSize(state *s);
void loadIntoMemory(state *s);
void toggleDisplayMode(state *s);
void memoryDisplay(state *s);
void saveIntoFile(state *s);
void memoryModify(state *s);
void quit(state *s);
int getInput();
void checkDebug(state *s);