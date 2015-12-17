#include "vlc/libvlc.h"
#include "vlc/libvlc_media.h"
#include "vlc/libvlc_media_player.h"

// Global media player
libvlc_media_player_t *g_mediaPlayer;

// ----------------------------
// VLC lib function definitions
// ----------------------------

#define DLL_FUNC(name)                  \
    typedef decltype(name) name##_func; \
    name##_func *_##name

DLL_FUNC(libvlc_new);
DLL_FUNC(libvlc_media_new_path);
DLL_FUNC(libvlc_media_player_new_from_media);
DLL_FUNC(libvlc_media_release);
DLL_FUNC(libvlc_media_player_set_hwnd);
DLL_FUNC(libvlc_media_player_play);
DLL_FUNC(libvlc_media_player_get_time);
DLL_FUNC(libvlc_media_player_set_time);

#undef DLL_FUNC

// -------------------------
// Library functions loading
// -------------------------
void startVideo(HWND Window, const char *Filename) {
    HMODULE Lib = LoadLibraryA("libvlc.dll");
    if(Lib == nullptr) {
        DebugBreak();
    }

#define LOAD_FUNCTION(name) _##name = (name##_func *)GetProcAddress(Lib, #name)
    LOAD_FUNCTION(libvlc_new);
    LOAD_FUNCTION(libvlc_media_new_path);
    LOAD_FUNCTION(libvlc_media_player_new_from_media);
    LOAD_FUNCTION(libvlc_media_player_play);
    LOAD_FUNCTION(libvlc_media_player_set_hwnd);
    LOAD_FUNCTION(libvlc_media_release);
    LOAD_FUNCTION(libvlc_media_player_get_time);
    LOAD_FUNCTION(libvlc_media_player_set_time);
#undef LOAD_FUNCTION

    // Initialize libvlc
    char const *vlc_argv[] = {"-vv"};
    libvlc_instance_t *libvlc = _libvlc_new(1, vlc_argv);

    // Load video
    libvlc_media_t *m = _libvlc_media_new_path(libvlc, Filename);
    g_mediaPlayer = _libvlc_media_player_new_from_media(m);
    _libvlc_media_release(m);

    // Set output window
    _libvlc_media_player_set_hwnd(g_mediaPlayer, Window);

    // Play video
    _libvlc_media_player_play(g_mediaPlayer);
}
