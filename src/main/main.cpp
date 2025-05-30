// SPDX-License-Identifier: BSD-3-Clause
// mrv2
// Copyright Contributors to the mrv2 Project. All rights reserved.

#ifdef _WIN32
#    include <windows.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <shellapi.h>
#endif

#include <string>
#include <iostream>

#include <FL/fl_utf8.h>

#ifdef MRV2_PYBIND11
#    include <pybind11/embed.h>
namespace py = pybind11;

#    include "mrvPy/Cmds.h"
#endif

#include "mrvCore/mrvHome.h"

#include "mrvApp/mrvApp.h"

#ifdef MRV2_PYBIND11
PYBIND11_EMBEDDED_MODULE(mrv2, m)
{
    mrv2_enums(m);
    mrv2_vectors(m);
    mrv2_otio(m);
    mrv2_filepath(m);
    mrv2_fileitem(m);
    mrv2_filesmodel(m);
    mrv2_image(m);
    mrv2_media(m);
    mrv2_settings(m);
    mrv2_timeline(m);
    mrv2_ui(m);
    mrv2_playlist(m);
    mrv2_io(m);
    mrv2_annotations(m);
#    ifdef TLRENDER_USD
    mrv2_usd(m);
#    endif
    mrv2_session(m);
    mrv2_commands(m);
    mrv2_python_plugins(m);
    mrv2_python_redirect(m);
}
#endif

int main(int argc, char* argv[])
{
    int r = 1;
#ifdef MRV2_PYBIND11
    // start the interpreter and keep it alive
    py::scoped_interpreter guard{};
#endif
    try
    {
        auto context = tl::system::Context::create();
        mrv::App app(argc, argv, context);
        r = app.getExit();
        if (0 == r)
        {
            r = app.run();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    return r;
}

#if defined(_WIN32) && defined(_MSC_VER)

#include <stdio.h>


#    include <FL/fl_utf8.h>
#    include <FL/fl_string_functions.h>

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int rc;
    int i;
    int argc;
    char** argv;

    /*
     * If we are compiling in debug mode, open a console window so
     * we can see any printf's, etc...
     *
     * While we can detect if the program was run from the command-line -
     * look at the CMDLINE environment variable, it will be "WIN" for
     * programs started from the GUI - the shell seems to run all Windows
     * applications in the background anyways...
     */

    /* Convert the command line arguments to UTF-8 */
    LPWSTR* wideArgv = CommandLineToArgvW(GetCommandLineW(), &argc);

    /* Allocate an array of 'argc + 1' string pointers */
    argv = (char **)malloc((argc + 1) * sizeof(char *));
  
    /* Convert the command line arguments to UTF-8 */
    for (i = 0; i < argc; i++) {
        /* find the required size of the buffer */
        int u8size = WideCharToMultiByte(CP_UTF8,     /* CodePage */
                                         0,           /* dwFlags */
                                         wideArgv[i], /* lpWideCharStr */
                                         -1,          /* cchWideChar */
                                         NULL,        /* lpMultiByteStr */
                                         0,           /* cbMultiByte */
                                         NULL,        /* lpDefaultChar */
                                         NULL);       /* lpUsedDefaultChar */
        if (u8size > 0) {
            char *strbuf = (char *)malloc(u8size);
            int ret = WideCharToMultiByte(CP_UTF8,     /* CodePage */
                                          0,           /* dwFlags */
                                          wideArgv[i], /* lpWideCharStr */
                                          -1,          /* cchWideChar */
                                          strbuf,      /* lpMultiByteStr */
                                          u8size,      /* cbMultiByte */
                                          NULL,        /* lpDefaultChar */
                                          NULL);       /* lpUsedDefaultChar */
            if (ret) {
                argv[i] = strbuf;
            } else {
                argv[i] = _strdup("");
                free(strbuf);
                fprintf(stderr, "Failed to convert arg %d\n", i);
            }
        } else {
            argv[i] = _strdup("");
        }
    }
    argv[argc] = NULL; /* required by C standard at end of list */

    /* Free the wide character string array */
    LocalFree(wideArgv);

    /* Call the program's entry point main() */
    rc = main(argc, argv);

    /* Cleanup allocated memory for argv */
    for (int i = 0; i < argc; ++i)
    {
        free((void*)argv[i]);
    }
    free((void*)argv);

    return rc;
}

#endif
