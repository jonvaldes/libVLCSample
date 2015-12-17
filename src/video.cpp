#include "vlc/libvlc.h"
#include "vlc/libvlc_media.h"
#include "vlc/libvlc_media_player.h"

// Global media player
libvlc_media_player_t *g_mediaPlayer;

// ----------------------------
// VLC lib function definitions
// ----------------------------
typedef libvlc_instance_t *__stdcall libvlc_new_func(int, const char *const *);
libvlc_new_func *_libvlc_new;

typedef libvlc_media_t *__stdcall libvlc_media_new_path_func(libvlc_instance_t *, const char *);
libvlc_media_new_path_func *_libvlc_media_new_path;

typedef libvlc_media_player_t *__stdcall libvlc_media_player_new_from_media_func(libvlc_media_t *p_md);
libvlc_media_player_new_from_media_func *_libvlc_media_player_new_from_media;

typedef void __stdcall libvlc_media_release_func(libvlc_media_t *p_md);
libvlc_media_release_func *_libvlc_media_release;

typedef void libvlc_media_player_set_hwnd_func(libvlc_media_player_t *p_mi, void *drawable);
libvlc_media_player_set_hwnd_func *_libvlc_media_player_set_hwnd;

typedef int __stdcall libvlc_media_player_play_func(libvlc_media_player_t *p_mi);
libvlc_media_player_play_func *_libvlc_media_player_play;

typedef libvlc_time_t __stdcall libvlc_media_player_get_length_func(libvlc_media_player_t *p_mi);
libvlc_media_player_get_length_func *_libvlc_media_player_get_length;

typedef libvlc_time_t __stdcall libvlc_media_player_get_time_func(libvlc_media_player_t *p_mi);
libvlc_media_player_get_time_func *_libvlc_media_player_get_time;

typedef void __stdcall libvlc_media_player_set_time_func(libvlc_media_player_t *p_mi, libvlc_time_t i_time);
libvlc_media_player_set_time_func *_libvlc_media_player_set_time;

typedef int libvlc_media_player_set_rate_func(libvlc_media_player_t *p_mi, float rate);
libvlc_media_player_set_rate_func *_libvlc_media_player_set_rate;

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
    LOAD_FUNCTION(libvlc_media_player_get_length);
    LOAD_FUNCTION(libvlc_media_player_get_time);
    LOAD_FUNCTION(libvlc_media_player_set_time);
    LOAD_FUNCTION(libvlc_media_player_set_rate);
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

    // Play and wait for video player to start
    _libvlc_media_player_play(g_mediaPlayer);
    while(_libvlc_media_player_get_time(g_mediaPlayer) == 0) {
        Sleep(5);
    }
    //_libvlc_media_player_set_rate(g_mediaPlayer, 0.2f);
}
