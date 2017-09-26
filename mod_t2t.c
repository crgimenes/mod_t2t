#include <Python.h>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"

static int t2t_handler(request_rec *r) {
    PyObject * obj;
    char *out=NULL;
    char pyCode[BUFSIZ];

    sprintf(pyCode,
            "import mod_t2t\n"
            "out = mod_t2t.t2t('%s')\n",
            r->filename);

    Py_Initialize();

    PyRun_SimpleString(pyCode);

    obj = PyMapping_GetItemString(PyModule_GetDict(PyImport_AddModule("__main__")), "out");
    if (obj != NULL) {
        out = PyString_AsString ( obj );
    } else {
        PyErr_Print();
    }

    Py_Finalize();

    r->content_type = "text/html";
    ap_send_http_header(r);
    ap_rprintf(r, "%s", out);

    return OK;
}

/* Dispatch list of content handlers */
static const handler_rec t2t_handlers[] = {
    { "t2t", t2t_handler },
    { NULL, NULL }
};

/* Dispatch list for API hooks */
module MODULE_VAR_EXPORT t2t_module = {
    STANDARD_MODULE_STUFF,
    NULL,                  /* module initializer                  */
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    t2t_handlers,          /* [#8] MIME-typed-dispatched handlers */
    NULL,                  /* [#1] URI to filename translation    */
    NULL,                  /* [#4] validate user id from request  */
    NULL,                  /* [#5] check if the user is ok _here_ */
    NULL,                  /* [#3] check access by host address   */
    NULL,                  /* [#6] determine MIME type            */
    NULL,                  /* [#7] pre-run fixups                 */
    NULL,                  /* [#9] log a transaction              */
    NULL,                  /* [#2] header parser                  */
    NULL,                  /* child_init                          */
    NULL,                  /* child_exit                          */
    NULL                   /* [#0] post read-request              */
#ifdef EAPI
   ,NULL,                  /* EAPI: add_module                    */
    NULL,                  /* EAPI: remove_module                 */
    NULL,                  /* EAPI: rewrite_command               */
    NULL                   /* EAPI: new_connection                */
#endif
};

