typedef struct { // struct of button
    char name[20];
    float vert[8];
    BOOL hover;
    BOOL activetivable;
    BOOL active;
    float r;
    float g;
    float b;
} TButton;


BOOL PointInButton(float x, float y, TButton cur_btn); // check mouse on button or not


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // main func


void EnableOpenGL(HWND hwnd, HDC*, HGLRC*); // work with openGL


void DisableOpenGL(HWND, HDC, HGLRC); // work with openGL


void arrayInit(); // func that fills an array with numbers [1; 40]


void print_string(float x, float y, char *text, float r, float g, float b); // func to print the text


char * toArray(int number); // func that makes int -> char* []


/* Bubble Sort fucntions */
void bubblesort();
void switchOnBubbleSort();
void switchOfBubbleSort();


/* Shaker Sort functions */
void shakerSort();
void switchOnShakerSort();
void switchOfShakerSort();


/* Comb sort functions */
void combSort();
void switchOnCombSort();
void switchOfCombSort();


/* Insert sort functions */
void insertSort();
void switchOnInsertSort();
void switchOfInsertSort();
