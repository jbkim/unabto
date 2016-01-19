#include "unabto_config.h"

#if NABTO_ENABLE_PROVISIONING

#include "unabto_provision_gopt.h"
#include "unabto_provision_http.h"
#include "modules/cli/gopt/gopt.h"
#include "modules/provision/unabto_provision.h"

#ifndef NABTO_DEVICE_KEY_FILE
#  define NABTO_DEVICE_KEY_FILE "~/.nabto-key.txt"
#endif

static bool set_default_prov_host(provision_context_t* context) {
#ifdef NABTO_PROVISION_HOST
    context->host_ = NABTO_PROVISION_HOST;
    return true;
#else
    return false;
#endif
}

static bool set_default_prov_apikey(provision_context_t* context) {
#ifdef NABTO_PROVISION_API_KEY
    context->api_key_ = NABTO_PROVISION_API_KEY
    return true;
#else
    return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////

void unabto_provision_gopt_help(const char* progname) {
    printf("Provisioning options:\n");
    printf("    --%s=<host>: The hostname of the provisioning service\n", UNABTO_PROVISION_GOPT_NAME_HOST);
    printf("    --%s=<key>: The provisioning service api key, matching the hostname pattern used\n", UNABTO_PROVISION_GOPT_NAME_API_KEY);
    printf("    --%s=<id>: The input id for the provisioning server (mac address or nabto id)\n", UNABTO_PROVISION_GOPT_NAME_ID_INPUT);
    printf("    --%s=<token>: Authorization token for the provisioning service\n", UNABTO_PROVISION_GOPT_NAME_USER_TOKEN);
    printf("Provisoning examples:\n");
    printf("    %s -P --%s=customer-b.provision.nabto.com --%s=0e:a1:c3:ed:eb:3f   # service issues id and key for mac\n", progname, UNABTO_PROVISION_GOPT_NAME_HOST, UNABTO_PROVISION_GOPT_NAME_ID_INPUT);
    printf("    %s -P --%s=customer-c.provision.nabto.com   # service issues id and key\n", progname, UNABTO_PROVISION_GOPT_NAME_HOST);
    printf("    %s -P --%s=customer-d.provision.nabto.com --%s=e5a4-0c1e-99ed-f01a  # service issues id and key, checks one-time token\n", progname, UNABTO_PROVISION_GOPT_NAME_HOST, UNABTO_PROVISION_GOPT_NAME_ID_INPUT);
    printf("    %s -P   # use results from earlier provisioning \n", progname);
}

bool unabto_provision_gopt_apply(const char* progname, void* options) {
    provision_context_t context;
    memset(&context, 0, sizeof(context));

    if (!gopt_arg(options, UNABTO_PROVISION_GOPT_HOST, (const char**)(&(context.host_))) && !set_default_prov_host(&context)) {
        NABTO_LOG_ERROR(("Provision host not specified on cmd line or in source"));
        return false;
    }
    
    if (!gopt_arg(options, UNABTO_PROVISION_GOPT_API_KEY, (const char**)(&(context.api_key_))) && !set_default_prov_apikey(&context)) {
        NABTO_LOG_ERROR(("Provision API key not specified on cmd line or in source"));
        return false;
    }

    gopt_arg(options, UNABTO_PROVISION_GOPT_ID_INPUT, (const char**)&(context.id_));
    
    gopt_arg(options, UNABTO_PROVISION_GOPT_USER_TOKEN, (const char**)(&(context.token_)));

    NABTO_LOG_TRACE(("Provisioning context: host_=%s, api_key_=%s, token_=%s, id_=%s",
                     context.host_, context.api_key_, context.token_, context.id_));
    return false;  // work in progress
//    return true;
}

#endif
