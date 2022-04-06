LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);


void DisableOpenGL(HWND, HDC, HGLRC);


void arrayInit();


void print_string(float x, float y, char *text, float r, float g, float b);


char * toArray(int number);
