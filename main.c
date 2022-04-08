#include <windows.h>
#include <string.h>
#include <gl/gl.h>
#include <stb_easy_font.h>

#include "main.h"

#define MAX_COUNT 40
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define SWAP(A, B) { int t = A; A = B; B = t; }

int curWidth, curHeight;
int numbers[MAX_COUNT]; // array to be sorted
int firstFlag = 1;
int speed = 50;
int indexOfCurElem = 0; // index of elem in numbers[] wich is considered
int sortedQ = 0; // bool variable which showing is array sorted or not
int sortingQ = 0;

/* bubble sort */
int bubbleSortFlag = 1; // bool variable which showing is bubbleSort turned on or not
int i, j;

/* shaker sort */
int shakerSortFlag = 0; // bool variable which showing is shakerSort turned on or not
int high, low;
int flag1;
int flag2;

TButton btn[] = // list of buttons on the screen
{
    /* sorts buttons */
    {"firstSort", {1024, 40, 1208, 40, 1208, 120, 1024, 120}, FALSE, TRUE, TRUE, 0.183, 0.183, 0.156},
    {"secondSort", {1248, 40, 1432, 40, 1432, 120, 1248, 120}, FALSE, TRUE, FALSE, 0.183, 0.183, 0.156},
    {"thirdSort", {1472, 40, 1656, 40, 1656, 120, 1472, 120}, FALSE, TRUE, FALSE, 0.183, 0.183, 0.156},
    {"fourthSort", {1696, 40, 1880, 40, 1880, 120, 1696, 120}, FALSE, TRUE, FALSE, 0.183, 0.183, 0.156},

    /* funcs buttons */
    {"initArray", {40, 40, 224, 40, 224, 101, 40, 101}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156},
    {"startSort", {40, 141, 224, 141, 224, 202, 40, 202}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156},
    {"stopSort", {40, 242, 224, 242, 224, 303, 40, 303}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156},
    {"exit", {40, 343, 224, 343, 224, 404, 40, 404}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156},
    {"speed+", {810, 343, 890, 343, 890, 404, 810, 404}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156},
    {"speed-", {690, 343, 770, 343, 770, 404, 690, 404}, FALSE, FALSE, FALSE, 0.183, 0.183, 0.156}
};
int btnCount = sizeof(btn) / sizeof(btn[0]); // count of buttons on the screen


void showButtons()
{
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, curWidth, curHeight, 0, -1, 1);
            for (int i = 0; i < btnCount; i++)
            {
                TButtonShow(btn[i]);
            }
    glPopMatrix();
}


void TButtonShow(TButton btn)
{
    glEnableClientState(GL_VERTEX_ARRAY);
        if (btn.active) glColor3f(0.191, 0.632, 0.671);
        else if (btn.hover) glColor3f(0.277, 0.277, 0.250);
        else glColor3f(btn.r, btn.g, btn.b);

        glVertexPointer(2, GL_FLOAT, 0, btn.vert);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
        glTranslatef(btn.vert[0], btn.vert[1], 0);
        if (!btn.activetivable)
        {
            glScalef(3.3, 3.3, 3.3);

            if (strcmp(btn.name, "startSort") == 0)
            {
                if (firstFlag)
                {
                    print_string(2, 6, "begin sort", 0.937, 0.933, 0.992);
                }
                else
                {
                    if (!sortingQ)
                    {
                        if (!sortedQ) print_string(7, 6, "continue", 0.937, 0.933, 0.992);
                        else print_string(2, 6, "begin sort", 0.937, 0.933, 0.992);
                    }
                    else print_string(2, 6, "begin sort", 0.937, 0.933, 0.992);
                }
            }
            else if (strcmp(btn.name, "speed+") == 0) print_string(0, 6, " ->", 0.937, 0.933, 0.992);
            else if (strcmp(btn.name, "speed-") == 0) print_string(3, 6, " <-", 0.937, 0.933, 0.992);

            else if (strcmp(btn.name, "initArray") == 0) print_string(2, 6, "new array", 0.937, 0.933, 0.992);

            else if (strcmp(btn.name, "stopSort") == 0) print_string(4, 6, "stop sort", 0.937, 0.933, 0.992);

            else if (strcmp(btn.name, "exit") == 0) print_string(18, 6, "exit", 0.937, 0.933, 0.992);
        }
        else
        {
            glScalef(2.5, 2.5, 2.5);
            print_string(3, 6, btn.name, 0.937, 0.933, 0.992);
        }
    glPopMatrix();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    float pxPerOne =  WINDOW_HEIGHT / 2 / MAX_COUNT; // 9.72 px


    srand(time(NULL));
    arrayInit(); // initial filling of the array by numbers: [1, 40]

    switchOnBubbleSort(); // on the start we need to turn on first type of sortings

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    if (!RegisterClassEx(&wcex))
        return 0;

    // Create main window
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Visualization of sorting algorithms", // Title
                          WS_POPUP ,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          WINDOW_WIDTH, // Main window width
                          WINDOW_HEIGHT + 1, // Main window height
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    EnableOpenGL(hwnd, &hDC, &hRC);


    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.937, 0.933, 0.992, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            glBegin(GL_QUADS);
                glColor3f(0.183, 0.183, 0.156);

                glVertex2f(-1, 0.175);
                glVertex2f(1, 0.175);
                glVertex2f(1, -1);
                glVertex2f(-1, -1);

            glEnd();

            glBegin(GL_QUADS);
                glColor3f(0.183, 0.183, 0.156);

                glVertex2f(-0.025, 1);
                glVertex2f(0.025, 1);
                glVertex2f(0.025, 0.175);
                glVertex2f(-0.025, 0.175);

            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.683, 0.109, 0.109);

                for (int i = 0; i < MAX_COUNT; i++)
                {
                    if (i == indexOfCurElem)
                    {
                        glColor3f(0.191, 0.632, 0.671);

                        glVertex2f(-0.83 + i * 0.041, -0.9);
                        glVertex2f(-0.83 + (i + 1) * 0.041, -0.9);
                        glVertex2f(-0.83 + (i + 1) * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                        glVertex2f(-0.83 + i * 0.041, -0.9 + pxPerOne * numbers[i] / 540);

                        glColor3f(0.683, 0.109, 0.109);
                    }
                    else
                    {
                        glVertex2f(-0.83 + i * 0.041, -0.9);
                        glVertex2f(-0.83 + (i + 1) * 0.041, -0.9);
                        glVertex2f(-0.83 + (i + 1) * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                        glVertex2f(-0.83 + i * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    }
                }

            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor3f(0.937, 0.933, 0.992);

                for (int i = 0; i < MAX_COUNT; i++)
                {
                    glVertex2f(-0.83 + i * 0.041, -0.9);
                    glVertex2f(-0.83 + i * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    glVertex2f(-0.83 + (i + 1) * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    glVertex2f(-0.83 + (i + 1) * 0.041, -0.9);
                }

            glEnd();


            glColor3f(0.92, 0.80, 0.51);
            glScalef(0.0027, -0.0027, 1);

            int step = 0;

            for (int i = 0; i < MAX_COUNT; i++)
            {
                if (numbers[i] < 10)
                {
                    step += 2;
                    print_string(-305 + step, 340, toArray(numbers[i]), 0.937, 0.933, 0.992);
                    step += 14;
                }
                else
                {
                    step -= 1;
                    print_string(-305 + step, 340, toArray(numbers[i]), 0.937, 0.933, 0.992);
                    step += 16;
                }
            }

            glScalef(2, 2, 1);
            print_string(-46, -95, "SPEED", 0.183, 0.183, 0.156);
            if (speed == 100) print_string(-37.5, -80, "10", 0.683, 0.109, 0.109);
            else if (speed == 0) print_string(-41.2, -80, "100", 0.683, 0.109, 0.109);
            else print_string(-37.5, -80, toArray(100 - speed), 0.683, 0.109, 0.109);

            glPopMatrix();

            showButtons(); // paint the buttons on the screen

            SwapBuffers(hDC);

            Sleep(speed); // step of sorting in mlSec

            if (sortingQ)
            {
                if (bubbleSortFlag)
                {
                    bubblesort();

                    if (sortedQ)
                    {
                        sortingQ = 0;
                        indexOfCurElem = 0;
                    }
                }
                else if (shakerSortFlag)
                {
                    shakerSort();

                    if (sortedQ)
                    {
                        sortingQ = 0;
                        indexOfCurElem = MAX_COUNT / 2;
                    }
                }
            }
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);

    return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_LBUTTONDOWN:
            for (int i = 0; i < btnCount; i++)
            {
                if (PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i]))
                {
                    /* painting the button */
                    if (btn[i].activetivable)
                    {
                        if (!btn[i].active)
                        {
                            for (int j = 0; j < btnCount; j++)
                            {
                                if (btn[j].activetivable) btn[j].active = FALSE;
                            }
                            btn[i].active = TRUE;

                            if (strcmp(btn[i].name, "firstSort") == 0)
                            {
                                if (!bubbleSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    sortedQ = 0;
                                    indexOfCurElem = 0;
                                    arrayInit();

                                    switchOfShakerSort();

                                    switchOnBubbleSort();
                                }
                            }
                            if (strcmp(btn[i].name, "secondSort") == 0)
                            {
                                if (!shakerSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    sortedQ = 0;
                                    indexOfCurElem = 0;
                                    arrayInit();

                                    switchOfBubbleSort();

                                    switchOnShakerSort();
                                }
                            }
                        }
                    }

                    /* do smth */
                    if (strcmp(btn[i].name, "startSort") == 0)
                    {
                        sortingQ = 1;
                        firstFlag = 0;
                    }
                    if (strcmp(btn[i].name, "stopSort") == 0)
                    {
                        sortingQ = 0;
                    }
                    if (strcmp(btn[i].name, "initArray") == 0)
                    {
                        firstFlag = 1;
                        sortingQ = 0;
                        sortedQ = 0;
                        indexOfCurElem = 0;
                        arrayInit();
                        if (bubbleSortFlag)
                        {
                            switchOnBubbleSort();
                        }
                        else if (shakerSortFlag)
                        {
                            switchOnShakerSort();
                        }
                    }
                    if (strcmp(btn[i].name, "speed+") == 0)
                    {
                        speed -= 10;
                        if (speed < 0) speed = 0;
                    }
                    if (strcmp(btn[i].name, "speed-") == 0)
                    {
                        speed += 10;
                        if (speed > 100) speed = 100;
                    }
                }
            }
        break;

        case WM_MOUSEMOVE:
            for (int i = 0; i < btnCount; i++)
            {
                btn[i].hover = PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i]);
            }
        break;

        case WM_SIZE:
            curWidth = LOWORD(lParam);
            curHeight = HIWORD(lParam);
            glViewport(0, 0, curWidth, curHeight);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}


void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    *hDC = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}


void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


void arrayInit()
{
    for (int i = 0; i < MAX_COUNT; i++)
    {
        numbers[i] = 1 + rand() % 40; // [1; 40]
    }
    sortedQ = 0;
}


void print_string(float x, float y, char *text, float r, float g, float b)
{
  static char buffer[99999]; // ~500 chars
  int num_quads;

  num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

  glColor3f(r,g,b);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 16, buffer);
  glDrawArrays(GL_QUADS, 0, num_quads*4);
  glDisableClientState(GL_VERTEX_ARRAY);
}


char * toArray(int number)
{
    int n = log10(number) + 1;
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10)
    {
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}



void switchOnBubbleSort()
{
    bubbleSortFlag = 1;
    i = MAX_COUNT - 1;
    j = 0;
}


void switchOfBubbleSort()
{
    bubbleSortFlag = 0;
    indexOfCurElem = 0;
}


void bubblesort()
{
    if (i > 0)
    {
        if (j < i)
        {
            indexOfCurElem = j + 1;
            if (numbers[j] > numbers[j + 1]) SWAP( numbers[j], numbers[j + 1] );
            j++;
        }
        else
        {
            j = 0;
            i--;
        }
    }
    else sortedQ = 1;
}


void switchOnShakerSort()
{
    shakerSortFlag = 1;

    high = 0;
    low = MAX_COUNT - 1;
    i = high;
    j = low;
    flag1 = 1;
    flag2 = 0;
}


void switchOfShakerSort()
{
    shakerSortFlag = 0;
    indexOfCurElem = 0;
}


void shakerSort()
{
    if (high < low)
    {
        if (flag1)
        {
            if (i < low)
            {
                indexOfCurElem = i + 1;
                if (numbers[i] > numbers[i + 1]) SWAP(numbers[i], numbers[i + 1]);
                i++;
            }
            else
            {
                flag1 = 0;
                low--;
                i = high + 1;
                flag2 = 1;
            }
        }
        else if (flag2)
        {
            if (j > high)
            {
                indexOfCurElem = j - 1;
                if (numbers[j] < numbers[j - 1]) SWAP(numbers[j], numbers[j - 1]);
                j--;
            }
            else
            {
                flag2 = 0;
                high++;
                j = low - 1;
                flag1 = 1;
            }
        }
    }
    else sortedQ = 1;
}


BOOL PointInButton(int x, int y, TButton btn)
{
    return (x > btn.vert[0]) && (x < btn.vert[4]) &&
            (y > btn.vert[1]) && (y < btn.vert[5]);
}



