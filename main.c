#include <windows.h>
#include <gl/gl.h>
#include <stb_easy_font.h>

#include "main.h"

#define MAX_COUNT 40
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

int numbers[MAX_COUNT]; // array to be sorted


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    float pxPerOne =  WINDOW_HEIGHT / 2 / MAX_COUNT; // 9.72 px

    srand(time(NULL));
    arrayInit(); // initial filling of the array by numbers: [1, 50]

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
                          WS_POPUP,
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
            glClearColor(0.88, 0.8, 0.74, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            glBegin(GL_QUADS);
            glColor3f(0.04, 0.47, 0.44);

                for (int i = 0; i < MAX_COUNT; i++)
                {
                    glVertex2f(-0.8 + i * 0.041, -0.9);
                    glVertex2f(-0.8 + (i + 1) * 0.041, -0.9);
                    glVertex2f(-0.8 + (i + 1) * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    glVertex2f(-0.8 + i * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                }

            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor3f(0.92, 0.80, 0.51);

                for (int i = 0; i < MAX_COUNT; i++)
                {
                    glVertex2f(-0.8 + i * 0.041, -0.9);
                    glVertex2f(-0.8 + i * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    glVertex2f(-0.8 + (i + 1) * 0.041, -0.9 + pxPerOne * numbers[i] / 540);
                    glVertex2f(-0.8 + (i + 1) * 0.041, -0.9);
                }

            glEnd();


            glColor3f(0.92, 0.80, 0.51);
            glScalef(0.0027, -0.0027, 1);

            int step = 0;

            for (int i = 0; i < MAX_COUNT; i++)
            {
                if (numbers[i] < 10)
                {
                    step += 3;
                    print_string(-294 + step, 340, toArray(numbers[i]), 0, 0, 0);
                    step += 13;
                }
                else
                {
                    step -= 1;
                    print_string(-294 + step, 340, toArray(numbers[i]), 0, 0, 0);
                    step += 16;
                }
            }

            glPopMatrix();
            SwapBuffers(hDC);

            Sleep (1);
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
