#include <windows.h>
#include <dlfcn.h>
#include <stdint.h>

static void* so = NULL;

int(*init)() = NULL;
int(*poll)() = NULL;
void(*get_opbtns)(uint8_t*) = NULL;
void(*get_gamebtns)(uint8_t*, uint8_t*) = NULL;
void(*get_lever)(int16_t*) = NULL;
//void(*led_init)() = NULL;
//int (*led_set_colors)(uint8_T board, uint8_t* rgb) = NULL;

BOOL WINAPI DllMain(HINSTANCE p1, DWORD p2, LPVOID p3)
{
    UNREFERENCED_PARAMETER(p1);
    UNREFERENCED_PARAMETER(p2);
    UNREFERENCED_PARAMETER(p3);

    so = dlopen("./mu3io_bsod.so", RTLD_NOW);
    if(so) {
        init = dlsym(so, "init");
        poll = dlsym(so, "poll");
        get_opbtns = dlsym(so, "get_opbtns");
        get_gamebtns = dlsym(so, "get_gamebtns");
        get_lever = dlsym(so, "get_lever");
        //led_init = dlsym(so, "led_init");
        //led_set_colors = dlsym(so, "led_set_colors");

        return
            init &&
            poll &&
            get_gamebtns &&
            get_opbtns &&
            get_lever; // &&
            // led_init &&
            // led_set_colors;
    } else {
        return FALSE;
    }
}

uint16_t WINAPI mu3_io_get_api_version(void)
{
    return 0x0101;
}

HRESULT WINAPI mu3_io_init(void)
{
    return init() == 0 ? S_OK : E_FAIL;
}

HRESULT WINAPI mu3_io_poll(void)
{
    return poll() == 0 ? S_OK : E_FAIL;
}

void WINAPI mu3_io_get_opbtns(uint8_t* opbtn)
{
    get_opbtns(opbtn);
}

void WINAPI WINAPI mu3_io_get_gamebtns(uint8_t* left, uint8_t* right)
{
    get_gamebtns(left, right);
}

void WINAPI mu3_io_get_lever(int16_t* pos) {
    get_lever(pos);
}

HRESULT WINAPI mu3_io_led_init(void) {
    // STUB
    // led_init();
    return S_OK;
}

void WINAPI mu3_io_led_set_colors(uint8_t board, uint8_t* rgb) {
    // STUB
    UNREFERENCED_PARAMETER(board);
    UNREFERENCED_PARAMETER(rgb);
    // led_set_colors(board, rgb);
}