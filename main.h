typedef struct { // struct of button
    char name[20];
    float vert[8];
    BOOL hover;
} TButton;


BOOL PointInButton(int x, int y, TButton btn); // check mouse on button or not


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
