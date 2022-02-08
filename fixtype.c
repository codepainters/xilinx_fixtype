#define _GNU_SOURCE

#include <freetype/freetype.h>
#include <dlfcn.h>

/* Note:
 * - loading NotoColorEmoji.ttf crashes ISE GUI for some reason
 * - intercepting open() to hide this TTF file worked, but GUI crashed
 *   a moment later with SIGSEGV for some unknown reason
 * - hence the idea to only intercept FT_New_Face
 */

#define FONT_TO_HIDE "/usr/share/fonts/truetype/noto/NotoColorEmoji.ttf"

static FT_Error (*orig_call)(FT_Library library, const char *path, FT_Long face_index, FT_Face *aface);

void __attribute__((constructor)) __init_ft_new_face_hijack() {
    orig_call = dlsym(RTLD_NEXT, "FT_New_Face");
}


FT_EXPORT(FT_Error) FT_New_Face(FT_Library library, const char *path, FT_Long face_index, FT_Face *aface) {
    if (strcmp(path, FONT_TO_HIDE) == 0) {
        return FT_Err_Cannot_Open_Resource;
    }

    return (*orig_call)(library, path, face_index, aface);
}
