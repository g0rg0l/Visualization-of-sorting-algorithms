#include <windows.h>
#include <string.h>
#include <gl/gl.h>
#include "stb_easy_font.h"

#include "main.h"

#define MAX_COUNT 40
#define WINDOW_HEIGHT 1081
#define WINDOW_WIDTH 1921
#define SWAP(A, B) { int t = A; A = B; B = t; }
#define RFACTOR 1.24733

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

/* comb sort */
int combSortFlag = 0; // bool variable which showing is combSort turned on or not
int jump;
int swapped;
int count;
int indexOfCurElem1;

/* insert sort */
int insertSortFlag = 0; // bool variable which showing is insertSort turned on or not
int newElement;
int location;


TButton btn[] = // list of buttons on the screen
{
    /* sorts buttons */
    {"firstSort", {1024, 40, 1208, 40, 1208, 120, 1024, 120}, FALSE, TRUE, TRUE, 0.183f, 0.183f, 0.156f},
    {"secondSort", {1248, 40, 1432, 40, 1432, 120, 1248, 120}, FALSE, TRUE, FALSE, 0.183f, 0.183f, 0.156f},
    {"thirdSort", {1472, 40, 1656, 40, 1656, 120, 1472, 120}, FALSE, TRUE, FALSE, 0.183f, 0.183f, 0.156f},
    {"fourthSort", {1696, 40, 1880, 40, 1880, 120, 1696, 120}, FALSE, TRUE, FALSE, 0.183f, 0.183f, 0.156f},

    /* funcs buttons */
    {"initArray", {40, 40, 224, 40, 224, 101, 40, 101}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f},
    {"startSort", {40, 141, 224, 141, 224, 202, 40, 202}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f},
    {"stopSort", {40, 242, 224, 242, 224, 303, 40, 303}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f},
    {"exit", {40, 343, 224, 343, 224, 404, 40, 404}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f},
    {"speed+", {810, 343, 890, 343, 890, 404, 810, 404}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f},
    {"speed-", {690, 343, 770, 343, 770, 404, 690, 404}, FALSE, FALSE, FALSE, 0.183f, 0.183f, 0.156f}
};
int btnCount = sizeof(btn) / sizeof(btn[0]); // count of buttons on the screen

void TButtonShow(TButton cur_btn)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    if (cur_btn.active) glColor3f(0.191f, 0.632f, 0.671f);
    else if (cur_btn.hover) glColor3f(0.277f, 0.277f, 0.250f);
    else glColor3f(cur_btn.r, cur_btn.g, cur_btn.b);

    glVertexPointer(2, GL_FLOAT, 0, cur_btn.vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
    glTranslatef(cur_btn.vert[0], cur_btn.vert[1], 0);
    if (!cur_btn.activetivable)
    {
        glScalef(3.3f, 3.3f, 3.3f);

        if (strcmp(cur_btn.name, "startSort") == 0)
        {
            if (firstFlag)
            {
                print_string(2, 6, "begin sort", 0.937f, 0.933f, 0.992f);
            }
            else
            {
                if (!sortingQ)
                {
                    if (!sortedQ) print_string(7, 6, "continue", 0.937f, 0.933f, 0.992f);
                    else print_string(2, 6, "begin sort", 0.937f, 0.933f, 0.992f);
                }
                else print_string(2, 6, "begin sort", 0.937f, 0.933f, 0.992f);
            }
        }
        else if (strcmp(cur_btn.name, "speed+") == 0) print_string(0, 6, " ->", 0.937f, 0.933f, 0.992f);
        else if (strcmp(cur_btn.name, "speed-") == 0) print_string(3, 6, " <-", 0.937f, 0.933f, 0.992f);

        else if (strcmp(cur_btn.name, "initArray") == 0) print_string(2, 6, "new array", 0.937f, 0.933f, 0.992f);

        else if (strcmp(cur_btn.name, "stopSort") == 0) print_string(4, 6, "stop sort", 0.937f, 0.933f, 0.992f);

        else if (strcmp(cur_btn.name, "exit") == 0) print_string(18, 6, "exit", 0.937f, 0.933f, 0.992f);
    }
    else
    {
        glScalef(4, 4, 4);

        if (strcmp(cur_btn.name, "firstSort") == 0) print_string(7, 6, "Bubble", 0.937f, 0.933f, 0.992f);
        if (strcmp(cur_btn.name, "secondSort") == 0) print_string(7, 6, "Shaker", 0.937f, 0.933f, 0.992f);
        if (strcmp(cur_btn.name, "thirdSort") == 0) print_string(11, 6, "Comb", 0.937f, 0.933f, 0.992f);
        if (strcmp(cur_btn.name, "fourthSort") == 0) print_string(7, 6, "Insert", 0.937f, 0.933f, 0.992f);
    }
    glPopMatrix();
}


void showButtons()
{
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, curWidth, curHeight, 0, -1, 1);
            for (int x = 0; x < btnCount; x++)
            {
                TButtonShow(btn[x]);
            }
    glPopMatrix();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    float pxPerOne =  (float) WINDOW_HEIGHT / 2 / MAX_COUNT; // 9.72 px

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
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    // Create main window
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Visualization of sorting algorithms", // Title
                          WS_POPUP,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          WINDOW_WIDTH, // Main window width
                          WINDOW_HEIGHT, // Main window height
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
            glClearColor(0.937f, 0.933f, 0.992f, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            glBegin(GL_QUADS);
                glColor3f(0.183f, 0.183f, 0.156f);

                glVertex2f(-1, 0.175f);
                glVertex2f(1, 0.175f);
                glVertex2f(1, -1);
                glVertex2f(-1, -1);

            glEnd();

            glBegin(GL_QUADS);
                glColor3f(0.183f, 0.183f, 0.156f);

                glVertex2f(-0.025f, 1);
                glVertex2f(0.025f, 1);
                glVertex2f(0.025f, 0.175f);
                glVertex2f(-0.025f, 0.175f);

            glEnd();

            glBegin(GL_QUADS);
            glColor3f(0.683f, 0.109f, 0.109f);

                for (int x = 0; x < MAX_COUNT; x++)
                {
                    if (x == indexOfCurElem || (x == indexOfCurElem1 && (combSortFlag || insertSortFlag)))
                    {
                        glColor3f(0.191f, 0.632f, 0.671f);

                        glVertex2f((float) (-0.83 + x * 0.041), -0.9f);
                        glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f);
                        glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);
                        glVertex2f((float) (-0.83 + x * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);

                        glColor3f(0.683f, 0.109f, 0.109f);
                    }
                    else
                    {
                        glVertex2f((float) (-0.83 + x * 0.041), -0.9f);
                        glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f);
                        glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);
                        glVertex2f((float) (-0.83 + x * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);
                    }
                }

            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor3f(0.937f, 0.933f, 0.992f);

                for (int x = 0; x < MAX_COUNT; x++)
                {
                    glVertex2f((float) (-0.83 + x * 0.041), -0.9f);
                    glVertex2f((float) (-0.83 + x * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);
                    glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f + pxPerOne * (float) numbers[x] / 540);
                    glVertex2f((float) (-0.83 + (x + 1) * 0.041), -0.9f);
                }

            glEnd();


            glColor3f(0.92f, 0.80f, 0.51f);
            glScalef(0.0027f, -0.0027f, 1);

            int step = 0;

            for (int x = 0; x < MAX_COUNT; x++)
            {
                if (numbers[x] < 10)
                {
                    step += 2;
                    print_string((float) (-305 + step), 340, toArray(numbers[x]), 0.937f, 0.933f, 0.992f);
                    step += 14;
                }
                else
                {
                    step -= 1;
                    print_string((float) (-305 + step), 340, toArray(numbers[x]), 0.937f, 0.933f, 0.992f);
                    step += 16;
                }
            }

            /* speed btns */
            glScalef(2, 2, 1.0f);
            print_string(-46.6f, -95, "SPEED", 0.183f, 0.183f, 0.156f);
            if (speed == 100) print_string(-37.5f, -80, "10", 0.683f, 0.109f, 0.109f);
            else if (speed == 0) print_string(-41.2f, -80, "100", 0.683f, 0.109f, 0.109f);
            else print_string(-37.5f, -80, toArray(100 - speed), 0.683f, 0.109f, 0.109f);

            /* sorts info text */
            glScalef(1.1f, 1.1f, 1.0f);
            print_string(12, -100, "Asymptotics:", 0.183f, 0.183f, 0.156f);
            print_string(80, -80, "<-worse", 0.183f, 0.183f, 0.156f);
            print_string(80, -60, "<-best", 0.183f, 0.183f, 0.156f);

            if (bubbleSortFlag || insertSortFlag)
            {
                print_string(32.0f, -80.0f, "O(n)", 0.683f, 0.109f, 0.109f);
                glScalef(0.6f, 0.6f, 1.0f);
                print_string(75.3f, -140.0f, "2", 0.683f, 0.109f, 0.109f);
                glScalef(1.66f, 1.66f, 1.0f);
                print_string(32.0f, -60.0f, "O(n)", 0.683f, 0.109f, 0.109f);
            }
            else if (shakerSortFlag || combSortFlag)
            {
                print_string(32.0f, -80.0f, "O(n)", 0.683f, 0.109f, 0.109f);
                glScalef(0.6f, 0.6f, 1.0f);
                print_string(75.3f, -140.0f, "2", 0.683f, 0.109f, 0.109f);
                glScalef(1.66f, 1.66f, 1.0f);
                print_string(32.0f, -60.0f, "O(nlogn)", 0.683f, 0.109f, 0.109f);
            }

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
                else if (combSortFlag)
                {
                    combSort();

                    if (sortedQ)
                    {
                        sortingQ = 0;
                    }
                }
                else if (insertSortFlag)
                {
                    insertSort();

                    if (sortedQ)
                    {
                        sortingQ = 0;
                    }
                }
            }
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);

    return (signed int) msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_LBUTTONDOWN:
            for (int x = 0; x < btnCount; x++)
            {
                if (PointInButton(LOWORD(lParam), HIWORD(lParam), btn[x]))
                {
                    /* painting the button */
                    if (btn[x].activetivable)
                    {
                        if (!btn[x].active)
                        {
                            for (int y = 0; y < btnCount; y++)
                            {
                                if (btn[y].activetivable) btn[y].active = FALSE;
                            }
                            btn[x].active = TRUE;

                            if (strcmp(btn[x].name, "firstSort") == 0)
                            {
                                if (!bubbleSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    indexOfCurElem = 0;
                                    arrayInit();

                                    switchOfShakerSort();
                                    switchOfCombSort();
                                    switchOfInsertSort();

                                    switchOnBubbleSort();
                                }
                            }
                            if (strcmp(btn[x].name, "secondSort") == 0)
                            {
                                if (!shakerSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    indexOfCurElem = 0;
                                    arrayInit();

                                    switchOfBubbleSort();
                                    switchOfCombSort();
                                    switchOfInsertSort();

                                    switchOnShakerSort();
                                }
                            }
                            if (strcmp(btn[x].name, "thirdSort") == 0)
                            {
                                if (!combSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    indexOfCurElem = 0;
                                    indexOfCurElem1 = 0;
                                    arrayInit();

                                    switchOfBubbleSort();
                                    switchOfShakerSort();
                                    switchOfInsertSort();

                                    switchOnCombSort();
                                }
                            }
                            if (strcmp(btn[x].name, "fourthSort") == 0)
                            {
                                if (!insertSortFlag)
                                {
                                    firstFlag = 1;
                                    sortingQ = 0;
                                    indexOfCurElem = 0;
                                    indexOfCurElem1 = 0;
                                    arrayInit();

                                    switchOfBubbleSort();
                                    switchOfShakerSort();
                                    switchOfCombSort();

                                    switchOnInsertSort();
                                }
                            }
                        }
                    }

                    /* do smth */
                    if (strcmp(btn[x].name, "startSort") == 0)
                    {
                        sortingQ = 1;
                        firstFlag = 0;
                    }
                    if (strcmp(btn[x].name, "stopSort") == 0)
                    {
                        sortingQ = 0;
                    }
                    if (strcmp(btn[x].name, "initArray") == 0)
                    {
                        firstFlag = 1;
                        sortingQ = 0;
                        sortedQ = 0;
                        indexOfCurElem = 0;
                        indexOfCurElem1 = 0;
                        arrayInit();
                        if (bubbleSortFlag)
                        {
                            switchOnBubbleSort();
                        }
                        else if (shakerSortFlag)
                        {
                            switchOnShakerSort();
                        }
                        else if (combSortFlag)
                        {
                            switchOnCombSort();
                        }
                        else if (insertSortFlag)
                        {
                            switchOnInsertSort();
                        }
                    }
                    if (strcmp(btn[x].name, "speed+") == 0)
                    {
                        speed -= 10;
                        if (speed < 0) speed = 0;
                    }
                    if (strcmp(btn[x].name, "speed-") == 0)
                    {
                        speed += 10;
                        if (speed > 100) speed = 100;
                    }
                    if (strcmp(btn[x].name, "exit") == 0) PostQuitMessage(0);
                }
            }
        break;

        case WM_MOUSEMOVE:
            for (int x = 0; x < btnCount; x++)
            {
                btn[x].hover = PointInButton(LOWORD(lParam), HIWORD(lParam), btn[x]);
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
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
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
    SYSTEMTIME st;
    GetLocalTime(&st);

    srand(st.wMilliseconds * st.wSecond);

    for (int x = 0; x < MAX_COUNT; x++)
    {
        numbers[x] = 1 + rand() % 40; // [1; 40]
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
    int n = (int) log10(number) + 1;

    char *numberArray = calloc(n, sizeof(char));
    for (int x = n - 1; x >= 0; --x, number /= 10)
    {
        numberArray[x] = (char) ((number % 10) + '0');
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
            if (numbers[j] > numbers[j + 1]) SWAP( numbers[j], numbers[j + 1] )
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
                if (numbers[i] > numbers[i + 1]) SWAP(numbers[i], numbers[i + 1])
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
                if (numbers[j] < numbers[j - 1]) SWAP(numbers[j], numbers[j - 1])
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



void combSort()
{
    if ((jump > 0 || swapped) && count != 2)
    {
        if (flag1)
        {
            if (jump > 1) jump = (int) (jump / RFACTOR);
            swapped = 0;
            i = 0;
            flag1 = 0;
        }
        if (!flag1)
        {
            if (i + jump < MAX_COUNT)
            {
                indexOfCurElem = i;
                indexOfCurElem1 = i + jump;
                if (numbers[i + jump] < numbers[i])
                {
                    SWAP(numbers[i], numbers[i + jump])
                    swapped = 1;
                }
                i++;
            }
            else
            {
                flag1 = 1;
                if (jump == 1) count++;
            }
        }
    }
    else sortedQ = 1;
}


void switchOnCombSort()
{
    combSortFlag = 1;

    flag1 = 1;
    jump = MAX_COUNT;
    i = 0;
    swapped = 1;
    count = 0;

}


void switchOfCombSort()
{
    combSortFlag = 0;
}


void switchOnInsertSort()
{
    insertSortFlag = 1;

    flag1 = 1;
    i = 1;
}


void switchOfInsertSort()
{
    insertSortFlag = 0;
}


void insertSort()
{
    if (i < MAX_COUNT)
    {
        indexOfCurElem = i;
        if (flag1)
        {
            newElement = numbers[i];
            location = i - 1;

            flag1 = 0;
        }
        if (!flag1)
        {
            if (location >= 0 && numbers[location] > newElement)
            {
                indexOfCurElem1 = location;
                numbers[location + 1] = numbers[location];
                location -= 1;
            }
            else
            {
                numbers[location + 1] = newElement;
                i++;
                flag1 = 1;
            }
        }
    }
    else sortedQ = 1;
}


BOOL PointInButton(float x, float y, TButton cur_btn)
{
    return (x > cur_btn.vert[0]) && (x < cur_btn.vert[4]) &&
           (y > cur_btn.vert[1]) && (y < cur_btn.vert[5]);
}



