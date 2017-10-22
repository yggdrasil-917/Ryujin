#include "Platform.hpp"


namespace Ryujin
{
#if defined(_WIN32)

    void ErrorMsg(const char *string)
    {
        MessageBoxA(NULL, string, "Error", MB_OK | MB_ICONERROR);
    }

    void WarningMsg(const char *string)
    {
        MessageBoxA(NULL, string, "Warning", MB_OK | MB_ICONWARNING);
    }

    void InfoMsg(const char *string)
    {
        MessageBoxA(NULL, string, "Information", MB_OK | MB_ICONINFORMATION);
    }

#else

    #if defined(LINUX)


    //#include <gtk/gtk.h>

    // This is such a hack, but at least it works.
//    gboolean idle(gpointer data)
//    {
//        gtk_main_quit();
//        return FALSE;
//    }

//    void MessageBox(const char *string, const GtkMessageType msgType)
//    {
//        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, msgType, GTK_BUTTONS_OK, string);
//        gtk_dialog_run(GTK_DIALOG(dialog));
//        gtk_widget_destroy(dialog);
//        g_idle_add(idle, NULL);
//        gtk_main();
//    }

    void ErrorMsg(const char *string)
    {
//        MessageBox(string, GTK_MESSAGE_ERROR);
    }

    void WarningMsg(const char *string)
    {
//        MessageBox(string, GTK_MESSAGE_WARNING);
    }

    void InfoMsg(const char *string)
    {
//        MessageBox(string, GTK_MESSAGE_INFO);
    }

    #elif defined(__APPLE__)

    void ErrorMsg(const char *string)
    {
        BlockingAlert("Error", string, "OK");
    }

    void WarningMsg(const char *string)
    {
        BlockingAlert("Warning", string, "OK");
    }

    void InfoMsg(const char *string)
    {
        BlockingAlert("Message", string, "OK");
    }
    #endif

#endif
}
