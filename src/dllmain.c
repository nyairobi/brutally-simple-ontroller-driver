#include <windows.h>
#include <dlfcn.h>
#include <stdint.h>

static void* so = NULL;

int(*init_fn)() = NULL;
int(*poll_fn)() = NULL;
void(*get_opbtns_fn)(uint8_t*) = NULL;
void(*get_gamebtns_fn)(uint8_t*, uint8_t*) = NULL;
void(*get_lever_fn)(int16_t*) = NULL;

BOOL WINAPI DllMain(HINSTANCE p1, DWORD p2, LPVOID p3)
{
    UNREFERENCED_PARAMETER(p1);
    UNREFERENCED_PARAMETER(p2);
    UNREFERENCED_PARAMETER(p3);

    so = dlopen("./mu3io_bsod.so", RTLD_NOW | RTLD_GLOBAL);
    if(so) {
        init_fn = dlsym(so, "init");
        poll_fn = dlsym(so, "poll");
        get_opbtns_fn = dlsym(so, "get_opbtns");
        get_gamebtns_fn = dlsym(so, "get_gamebtns");
        get_lever_fn = dlsym(so, "get_lever");

        return
            init_fn &&
            poll_fn &&
            get_gamebtns_fn &&
            get_opbtns_fn &&
            get_lever_fn;
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
    return init_fn() == 0 ? S_OK : E_FAIL;
}

HRESULT WINAPI mu3_io_poll(void)
{
    return poll_fn() == 0 ? S_OK : E_FAIL;
}

void WINAPI mu3_io_get_opbtns(uint8_t *opbtn)
{
    get_opbtns_fn(opbtn);
}

void WINAPI WINAPI mu3_io_get_gamebtns(uint8_t *left, uint8_t *right)
{
    get_gamebtns_fn(left, right);
}

void WINAPI mu3_io_get_lever(int16_t* pos) {
    get_lever_fn(pos);
}

HRESULT WINAPI mu3_io_led_init(void) {
    // STUB
    return 0;
}

void WINAPI mu3_io_led_set_colors(uint8_t board, uint8_t *rgb) {
    // STUB
    UNREFERENCED_PARAMETER(board);
    UNREFERENCED_PARAMETER(rgb);
}