/*
 * The program execution is started from here.
*/

#include <gtk/gtk.h>
#include "application.h"
#include "src/utils/logger.h"
#include "src/app.h"

int main(int argc, char* argv[]){

    log_info("NAME=%s", APP_NAME);
    log_info("VERSION=%s", APP_VERSION);
    log_info("ID=%s", APP_ID);
    
    /*
     * app() function starts application and once application is exited
     * a status code of type int is returned.
     * app_status=0 means app ran without issues.
    */
    int app_status = app(argc, argv);

    return app_status;
}