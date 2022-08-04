/*
 * Copyright (c) AppDynamics, Inc., and its affiliates
 * 2015-2018
 * All Rights Reserved
 */
#ifndef APPDYNAMICS_H_
#define APPDYNAMICS_H_

#ifdef __cplusplus
#include <string>
#include <memory>
#include <map>
extern "C" {
#endif

#ifdef _WIN32
#define APPD_API __declspec(dllexport)
#else
#define APPD_API __attribute__((visibility("default")))
#endif

/**
 * BT Handle is an opaque handle that will be returned when creating a Business Transaction. The
 * handle is used by subsequent business transaction API calls, and must be passed as the parameter
 * to the appd_bt_end() call when completing the transaction. See appd_bt_begin() for invocation
 * usage.
 *
 * NOTE: The user should not de-reference the handle, nor make any assumptions about its contents.
 * AppDynamics reserves the right to modify its meaning at a later date.
 */
typedef void* appd_bt_handle;

/**
 * Exit Call Handle is an opaque handle that will be returned when creating an Exit Call. The handle
 * is used by subsequent exit call API calls, and must be passed as the parameter to the
 * appd_exitcall_end() call when completing the exit call.
 *
 * NOTE: The user should not de-reference the handle, nor make any assumptions about its contents.
 * AppDynamics reserves the right to modify its meaning at a later date.
 */
typedef void* appd_exitcall_handle;

/**
 * Frame Handle is an opaque handle that will be returned when creating a C/C++ stack frame call.
 * The handle is used by subsequent stack frame API calls, and must be passed as the parameter to
 * the appd_frame_end() call when completing the transaction.
 *
 * NOTE: The user should not de-reference the handle, nor make any assumptions about its contents.
 * AppDynamics reserves the right to modify its meaning at a later date.
 */
typedef void* appd_frame_handle;

/**
 * Event Handle is an opaque handle that will be returned when creating a Custom Event. The handle
 * is used by subsequent custom event API calls, and must be passed as the parameter to the
 * appd_custom_event_end() call when completing the transaction.
 *
 * NOTE: The user should not de-reference the handle, nor make any assumptions about its contents.
 * AppDynamics reserves the right to modify its meaning at a later date.
 */
typedef void* appd_event_handle;

#ifdef _WIN32
#pragma pack(push, 8) /* CORE-65451 */
#endif

/**
 * The Default Application Context contains the configuration details used when creating the default
 * application context. This structure contains the minimum configuration settings for Controller
 * host, port, SSL setting, account name, access key, application name, tier name, and node name
 * that will be used to create metrics, although many other configuration options can be stored
 * including any HTTP proxy settings, the initialization and termination timer behaviors, and
 * backend names.
 *
 */
struct appd_config;

/**
 * An Alternate Application Context can create one or more contexts (i.e. sending metrics to a
 * potentially different controller host/port, or application, tier, or node) when creating metrics.
 * This structure contains the configuration details used when creating an application context other
 * than the default context. Typically this is only used for multi-tenant installations where the
 * user wishes to record metrics to separate controllers (i.e. different license accounts) or when
 * they wish to create multiple nodes within a tier, or multiple tiers within an application.
 *
 * This structure contains the minimum configuration settings for Controller host, port, SSL
 * setting, account name, access key, application name, tier name, and node name that will be used
 * when creating metrics, although many other configuration options can be stored including any HTTP
 * proxy settings, the initialization and termination timer behaviors, and backend names.
 */
struct appd_context_config;

/**
 * The SDK Logging Level sets the logging level for SDK.  See appd_config_set_logging_min_level()
 * for more details.  The default is APPD_LOG_LEVEL_INFO, which should provide adequate logging to
 * determine the cause of most issues.   Setting the level higher may result in excessive disk
 * consumption, and should be avoided unless actively diagnosing an issue.   It is possible to turn
 * off all but the most extreme debugging (effectively off) by setting it to APPD_LOG_LEVEL_FATAL,
 * although one may also wish to constrict the number of log files created, and/or direct them to
 * /dev/null.
 *
 * See also appd_config_set_logging_log_dir(),
 *          appd_config_set_logging_max_num_files(),
 *          appd_config_set_logging_max_file_size_bytes(),
 *          appd_config_set_logging_max_num_messages_per_minute()
 */
enum appd_config_log_level
{
  APPD_LOG_LEVEL_TRACE, /**< Logs everything, including debugging and trace information. */
  APPD_LOG_LEVEL_DEBUG, /**< Logs normally, but includes additional debugging information. */
  APPD_LOG_LEVEL_INFO,  /**< Logs normally (the default setting). */
  APPD_LOG_LEVEL_WARN,  /**< Logs warnings only or higher. */
  APPD_LOG_LEVEL_ERROR, /**< Logs errors only. */
  APPD_LOG_LEVEL_FATAL  /**< Logs fatal errors only, otherwise it doesn't log anything. Effectively
                           disables logging. */
};

#ifdef _WIN32
#pragma pack(pop) /* CORE-65451 */
#endif

/**
 * Creates an empty configuration structure and returns a pointer to the initialized configuration
 * object. Do not free this structure, it will be released when `appd_sdk_term()` is called.
 *
 * Call this function and set values in it with the following methods:
 *
 * * appd_config_set_analytics_enabled()
 * * appd_config_set_app_name()
 * * appd_config_set_tier_name()
 * * appd_config_set_node_name()
 * * appd_config_set_controller_host()
 * * appd_config_set_max_events_per_timewindow()
 * * appd_config_set_sensitive_data_filters()
 * * appd_config_set_controller_port()
 * * appd_config_set_controller_account()
 * * appd_config_set_controller_access_key()
 * * appd_config_set_controller_use_ssl()
 * * appd_config_set_controller_http_proxy_host()
 * * appd_config_set_controller_http_proxy_port()
 * * appd_config_set_controller_http_proxy_username()
 * * appd_config_set_controller_http_proxy_password()
 * * appd_config_set_controller_http_proxy_password_file()
 * * appd_config_set_controller_certificate_file()
 * * appd_config_set_controller_certificate_dir()
 * * appd_config_set_flush_metrics_on_shutdown()
 * * appd_config_set_logging_min_level()
 * * appd_config_set_logging_log_dir()
 * * appd_config_set_logging_max_num_files()
 * * appd_config_set_logging_max_file_size_bytes()
 * * appd_config_set_logging_max_num_messages_per_minute()
 * * appd_config_set_init_timeout_ms()
 * * appd_config_getenv()
 *
 * At a minimum, the host, port, SSL, account, access key, application name, tier name, and node
 * name must all be set for the SDK to properly connect to the Controller.  Once the structure
 * contains the correct configuration settings, call appd_sdk_init() to initialize the SDK.
 *
 * Use the `appd_sdk_init()` function to initialize the default context within the SDK.
 *
 * NOTE: Do not call `appd_config_init()` a second time, as it will reinitialize the structure and
 * zero out any previous configuration settings.
 *
 * @return returns a pointer to an initialized appd_config structure.
 *
 */
APPD_API struct appd_config* appd_config_init();

/**
 * Creates an empty alternate application context (for multi-node or multi-controller
 * implementations) configuration structure and returns a pointer to the initialized structure.
 * Do not free this structure, it will be released when appd_sdk_term() is called.
 *
 * Call the function (passing the name of the alternate application context) then set values within
 * it using the pointer returned from the call and the following methods:
 *
 *     appd_context_config_set_analytics_enabled()
 *     appd_context_config_set_app_name()
 *     appd_context_config_set_tier_name()
 *     appd_context_config_set_node_name()
 *     appd_context_config_set_controller_host()
 *     appd_context_config_set_controller_port()
 *     appd_context_config_set_controller_account()
 *     appd_context_config_set_controller_access_key()
 *     appd_context_config_set_controller_use_ssl()
 *     appd_context_config_set_controller_http_proxy_host()
 *     appd_context_config_set_controller_http_proxy_port()
 *     appd_context_config_set_controller_http_proxy_username()
 *     appd_context_config_set_controller_http_proxy_password()
 *     appd_context_config_set_controller_http_proxy_password_file()
 *     appd_context_config_set_controller_certificate_file()
 *     appd_context_config_set_controller_certificate_dir()
 *
 * At a minimum, the host, port, SSL, account, access key, application name, tier name, and node
 * name must all be set for the SDK to properly connect to the Controller.  Once the structure
 * contains the correct configuration settings, call appd_sdk_add_app_context() to initialized the
 * alternate context within the SDK.
 *
 * Use the appd_context_config_init() function to initialize the alternate context within the SDK,
 * only after the appd_sdk_init() call has been made.
 *
 * NOTE: Do not call appd_context_config_init() a second time, as it will reinitialize the structure
 * and zero out any previous configuration settintgs.
 *
 * @param context A null-terminated string that names the application context, similiar to
 * appd_config_init() for the default context.
 * @return A pointer to an initialized appd_config structure.
 */
APPD_API struct appd_context_config* appd_context_config_init(const char* context);

//-------------------------------------App Name-------------------------------------------------------------------

/**
 * The default application name sets the default application context's application name.  This will
 * be used for all the (default) application context metrics.
 *
 * @param cfg
 *   A pointer to the default application context structure created by appd_config_init().
 * @param app
 *   A null-terminated string that contains the application name.
 * @return void
 */
APPD_API void appd_config_set_app_name(struct appd_config* cfg, const char* app);

/**
 * An alternate application context application name sets the application name for an alternate
 * application context.
 *
 * @param context_cfg
 *   A pointer to the alternate application context structure created by appd_context_config_init().
 * @param app
 *   A null-terminated string that contains the application name.
 * @return void
 */
APPD_API void appd_context_config_set_app_name(struct appd_context_config* context_cfg, const char* app);

//-------------------------------------Tier Name-------------------------------------------------------------------

/**
 * The default tier name sets the default application context's tier name. This will be used for all
 * the default application context metrics.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param tier A null-terminated string that contains the tier name.
 * @return void
 */
APPD_API void appd_config_set_tier_name(struct appd_config* cfg, const char* tier);

/**
 * Alernate application context tier name sets the tier name for an alternate application context.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init()
 * @param tier A null-terminated string that contains the tier name.
 * @return void
 */
APPD_API void appd_context_config_set_tier_name(struct appd_context_config* context_cfg, const char* tier);

//-------------------------------------Node Name-------------------------------------------------------------------

/**
 * The default node name sets the default application context's node name. This will be used for all
 * the default application context metrics.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init()
 * @param node A null-terminated string that contains the node name.
 * @return void
 */
APPD_API void appd_config_set_node_name(struct appd_config* cfg, const char* node);

/**
 * The alternate application context node name sets the default business transaction's node name for
 * an alternate application context.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 * @param node A null-terminated string that contains the node name.
 * @return void
 */
APPD_API void appd_context_config_set_node_name(struct appd_context_config* context_cfg, const char* node);

//-------------------------------------Maximum Events--------------------------------------------------------------

/**
 * Allows the configuration of the number of events that can be gathered per timewindow
 * of 1 minute before sending data to controller.
 * Default value is 100.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param max_events_per_timewindow Unsigned integer representing the maximum number of events to
 * process in a 1 minute interval.
 * @return void
 */
APPD_API void appd_config_set_max_events_per_timewindow(struct appd_config* cfg,
                                                        size_t max_events_per_timewindow);

//-------------------------------------Sensitive Data Filters -----------------------------------------------------

/**
 * Configures the filters to prevent sending sensitive data from URLs, HTTP requests, messages, and
 * environment variables to the Controller. By default, the SDK creates data filters to redact
 * values for environment variables which names contain `password` or `key`. You need a valid JSON
 * string with data, URL, or message filters.
 *
 * Example
 * -------
 *
 *     struct appd_config* cfg = appd_config_init();
 *     appd_config_set_sensitive_data_filters(cfg,
 *       "{"
 *         "\"messageFilters\": ["
 *           "{  \"messageType\": \"throwable\","
 *               "\"matchPattern\": \"test\","
 *               "\"redactionRegex\": \".*\""
 *           "}"
 *         "],"
 *         "\"dataFilters\": ["
 *           "{  \"appliesTo\": \"http-headers\","
 *               "\"matchPattern\": \"test\""
 *           "}"
 *         "],"
 *         "\"urlFilters\": ["
 *           "{  \"delimiter\": \"/\","
 *               "\"segment\": \"1,2\","
 *               "\"matchPattern\": \"test1\","
 *               "\"paramPattern\": \"test2\""
 *           "}"
 *         "]"
 *       "}" );
 *
 * NOTE: Setting a configuration for the sensitive data filters will erase the default ones. If you
 * want to preserve them, you will need to add a configuration that will reproduce this filtering
 * behaviour.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param sensitive_data_filters A null-terminated string that contains valid json with sensitive
 * data filters.
 * @return void
 */
APPD_API void appd_config_set_sensitive_data_filters(struct appd_config* cfg,
                                                     const char* sensitive_data_filters);

//-------------------------------------Host Name-------------------------------------------------------------------

/**
 * Sets the default application context's Controller host name. This will be used for all the
 * default application context metrics.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param host A null-terminated string that contains the host URL string or IPADDR.
 * @return void
 */
APPD_API void appd_config_set_controller_host(struct appd_config* cfg, const char* host);

/**
 * Sets the context's Controller host name for alternate application contexts.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 * @param host A null-terminated string that contains the host URL string or IPADDR.
 * @return void
 */
APPD_API void appd_context_config_set_controller_host(struct appd_context_config* context_cfg, const char* host);

//-------------------------------------Port Number---------------------------------------------------------------

/**
 * Sets the port on which the default application context the Controller is expected to be
 * listening.
 *
 * If not specifed, defaults to 80 when `use_ssl` is false and 443 when `use_ssl` is true.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param port An unsigned short that contains the port number.
 * @return void
 */
APPD_API void appd_config_set_controller_port(struct appd_config* cfg, const unsigned short port);

/**
 * Sets the port on which the alternate application context the Controller is expected to be
 * listening.
 *
 * If not specifed, defaults to 80 when `use_ssl` is false and 443 when `use_ssl` is true.
 *
 * @param context_cfg A pointer to the default application context structure created by
 * appd_context_config_init().
 * @param port An unsigned short that contains the port number.
 * @return void
 */
APPD_API void appd_context_config_set_controller_port(struct appd_context_config* context_cfg, const unsigned short port);

//-------------------------------------Account Name-----------------------------------------------------------

/**
 * Sets the default business transaction's Controller account name.

 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param acct A null-terminated string that describes the account name.
 * @return void
 */
APPD_API void appd_config_set_controller_account(struct appd_config* cfg, const char* acct);

/**
 * Sets the alternate application context's Controller accountt name for an alternate application
 * context.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_config_init().
 * @param acct A null-terminated string that describes the account name.
 * @return void
 */
APPD_API void appd_context_config_set_controller_account(struct appd_context_config* context_cfg, const char* acct);

//-------------------------------------Access Key-----------------------------------------------------------

/**
 * Sets the default application context's Controller access key.
 * This value should match the key shown in the account tab of the license page off the Controller's
 * **Gear** icon.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param key A null-terminated string that describes the access key for this context.
 * @return void
 */
APPD_API void appd_config_set_controller_access_key(struct appd_config* cfg, const char* key);

/**
 * Sets the alternate application context's Controller access key.
 * This value should match the key shown in the account tab of the license page off the Controller's
 * **Gear** icon.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_config_init().
 * @param key A null-terminated string that describes the account name.
 * @return void
 */
APPD_API void appd_context_config_set_controller_access_key(struct appd_context_config* context_cfg, const char* key);

/**
 * Flag that specifies if SSL should be used to talk to the Controller.
 *
 * Set to a non-zero integer for true or to zero for false.
 *
 * This value must be non-zero for SaaS Controllers.
 *
 * @param cfg A pointer to the alternate application context structure created by
 * appd_config_init().
 * @param ssl If zero, do not use SSL, if non-zero, use SSL.
 * @return void
 */
APPD_API void appd_config_set_controller_use_ssl(struct appd_config* cfg, const unsigned int ssl);

/**
 * Flag that specifies if SSL should be used to talk to the Controller for an alternate application
 * context.
 *
 * Set to a non-zero integer for true or to zero for false.
 *
 * This value must be non-zero for SaaS Controllers.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 * @param ssl If zero, do not use SSL, if non-zero, use SSL.
 * @return void
 */
APPD_API void appd_context_config_set_controller_use_ssl(struct appd_context_config* context_cfg, unsigned int ssl);

/**
 * (Optional) The HTTP proxy host name if you are using an HTTP proxy to talk to the Controller.
 * The HTTP proxy config options are optional and only necessary if a proxy is required to connect
 * to the Controller.
 *
 * @param cfg A pointer to the deafult application context structure crerated by appd_config_init().
 * @param host A null-terminated string that contains the HTTP proxy host name.
 * @return void
 */
APPD_API void appd_config_set_controller_http_proxy_host(struct appd_config* cfg, const char* host);

/**
 * (Optional) The HTTP proxy host name if you are using an HTTP proxy to talk to the Controller for
 * an alternate application context. The HTTP proxy config options are optional and only necessary
 * if a proxy is required to connect to the Controller.
 *
 * @param context_cfg A pointer to the alternate application context structurer created by
 * appd_config_init().
 * @param host A null-terminated string that contains the HTTP proxy host name.
 * @return void
 */
APPD_API void appd_context_config_set_controller_http_proxy_host(struct appd_context_config* context_cfg, const char* host);

/**
 * (Optional) The port number of the HTTP proxy. (Default: 80)
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param port An unsigned short that contains the HTTP proxy host port.
 * @return void
 */
APPD_API void appd_config_set_controller_http_proxy_port(struct appd_config* cfg, const unsigned short port);

/**
 * (Optional) The port name of the HTTP proxy for an alternate application context. (Default: 80)
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init()
 * @param port An unsigned short that contains the HTTP proxy host port.
 * @return void
 */
APPD_API void appd_context_config_set_controller_http_proxy_port(struct appd_context_config* context_cfg, const unsigned short port);

/**
 * (Optional) Username to connect to the HTTP proxy with.
 * The HTTP proxy config options are optional, and only necessary if a proxy is required
 * to connect the SDK host to the Controller.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param user A null-terminated string that contains the user name.
 * @return void
 */
APPD_API void appd_config_set_controller_http_proxy_username(struct appd_config* cfg, const char* user);

/**
 * (Optional) The username to connect to the HTTP proxy for an alternate application context.
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init()
 * @param user A null-terminated string that contains the user name.
 * @return void
 */
APPD_API void appd_context_config_set_controller_http_proxy_username(struct appd_context_config* context_cfg, const char* user);

/**
 * (Optional) The password to connect to the HTTP proxy with.
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param pwd A null-terminated string that contains the password used to connect to the HTTP proxy.
 * @return void
 */
APPD_API void appd_config_set_controller_http_proxy_password(struct appd_config* cfg, const char* pwd);

/**
 * (Optional) The password to connect to the HTTP proxy for an alternate application context.
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 * @param pwd A null-terminated string that contains the password used to connect to the HTTP proxy.
 * @return void
 */
APPD_API void appd_context_config_set_controller_http_proxy_password(struct appd_context_config* context_cfg, const char* pwd);

/**
 * (Optional) The file that contains the password to connect to the HTTP proxy with.
 * The HTTP proxy config options are optional, and only necessary if a proxy is required to connect
 * the SDK host to the Controller.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init()
 * @param file A null-terminated string that contains the password file. The file is written in
 * clear text.
 * @return void
 */
APPD_API void appd_config_set_controller_http_proxy_password_file(struct appd_config* cfg, const char* file);

/**
 * (Optional) The file that contains the password to connect to the HTTP proxy in an alternate
 * application context. The HTTP proxy config options are optional, and only necessary if a proxy is
 * required to connect the SDK host to the Controller.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 * @param file A null-terminated string that contains the password file. The file is written in
 * clear text.
 * @return void
 */
APPD_API void appd_context_config_set_controller_http_proxy_password_file(struct appd_context_config* context_cfg, const char* file);

/**
 * CA certificate file (full path).
 *
 * Defaults to the included ca-bundle.crt file. Set this to use your own certificate file.
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param file A null-terminated string that contains the certificate file.
 * @return void
 */
APPD_API void appd_config_set_controller_certificate_file(struct appd_config* cfg, const char* file);

/**
 * CA certificate file (full path) for an alternate application context.
 * Defaults to the included ca-bundle.crt file. Set this to use your own certificate file.
 *
 * @param context_cfg A pointer to the default application context structure created by
 * appd_context_config_init().
 * @param file A null-terminated string that contains the certificate file.
 * @return void
 */
APPD_API void appd_context_config_set_controller_certificate_file(struct appd_context_config* context_cfg, const char* file);

/**
 * CA certificate directory.
 *
 * Set this if you have multiple certificate files located in a single directory.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param dir A null-terminated string that contains the certificate directory.
 * @return void
 */
APPD_API void appd_config_set_controller_certificate_dir(struct appd_config* cfg, const char* dir);

/**
 * CA certificate directory for an alternate application context.
 *
 * Set this if you have multiple certificate files located in a single directory.
 *
 * @param context_cfg A pointer to the default application context structure created by
 * appd_context_config_init().
 * @param dir A null-terminated string that contains the certificate directory.
 * @return void
 */
APPD_API void appd_context_config_set_controller_certificate_dir(struct appd_context_config* context_cfg, const char* dir);

/**
 * Host for the Analytics Agent.
 * Defaults to "localhost".
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param host URL The host URL.
 */
APPD_API void appd_config_set_analytics_host(struct appd_config* cfg, const char* host);

/**
 * Port on which the Analytics Agent is listening.
 * Defaults to 9090.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param port An unsigned short that contains the Analytics port.
 * @return void
 */
APPD_API void appd_config_set_analytics_port(struct appd_config* cfg, const unsigned short port);

/**
 * Sets the flag that indicates whether or not the Analytics Host connection uses SSL. By default,
 * SSL is deactivated.
 * @param ssl
 *    If non-zero, then SSL will be activated.
 *    If zero, then SSL will not be activated (by default).
 * @return void
 */
APPD_API void appd_config_set_analytics_use_ssl(struct appd_config* cfg, const unsigned short ssl);

/**
 * Set the enable/disable flag for the Analytics Agent.
 *
 * @param cfg A pointer to the default application context structure crerated by appd_config_init().
 * @param enable If non-zero, then the Analytics Agent is enabled.
 *               If zero, then the Analytics Agent is disabled (default behavior).
 * @return void
 */
APPD_API void appd_config_set_analytics_enabled(struct appd_config* cfg, const unsigned short enable);

/**
 * The minimum level of logging allowed. If ``APPD_LOG_LEVEL_TRACE``, all log messages are allowed.
 * If ``APPD_LOG_LEVEL_FATAL``, only the most severe errors are logged. The default is
 * ``APPD_LOG_LEVEL_INFO``.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param lvl The tag indicating the desired log level, see ``::appd_config_log_level`` for the
 * possible values.
 * @return void
 */
APPD_API void appd_config_set_logging_min_level(struct appd_config* cfg, enum appd_config_log_level lvl);

/**
 * The directory to log to. If not set, defaults to "/tmp/appd".
 * The process running the SDK must have permissions to create this directory (if it doesn't already
 * exist), to list the files within it, and to write to the files within it.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param dir A null-terminated string containing the path toward the desired log directory.
 * @return void
 */
APPD_API void appd_config_set_logging_log_dir(struct appd_config* cfg, const char* dir);

/**
 * The maximum number of log files allowed per tenant. Once this is hit, the logs are rotated.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param num An unsigned int indicating the maximum number of files to keep.
 * @return void
 */
APPD_API void appd_config_set_logging_max_num_files(struct appd_config* cfg, const unsigned int num);

/**
 * The maximum size of an individual log file, in bytes. Log files are rotated when they reach this
 * size.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param size An unsigned int indicating the maximum size n bytes for an individual file.
 * @return void
 */
APPD_API void appd_config_set_logging_max_file_size_bytes(struct appd_config* cfg, const unsigned int size);

/**
 * The maximum number of messages that can be logged per minute for an individual file.
 * Messages are discarded if they exceed this limit.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param num An unsigned int indicating the maximum number of messages per minute.
 * @return void
 */
APPD_API void appd_config_set_logging_max_num_messages_per_minute(struct appd_config* cfg,
                                                                  const unsigned int num);

/**
 * appd_sdk_init() relies on the Controller configuration to start business transactions.
 * This is an asynchronous action so that appd_sdk_init() does not block your program.
 * This ``appd_config`` field allows you to instruct appd_sdk_init() to wait for up to
 * ``init_timeout_ms`` milliseconds until it has received the Controller configuration and is ready
 * to capture business transactions. If a valid configurate is not received in that amount of time,
 * the function will return and allow normal application processing to proceed, but no metrics will
 * be recorded. The SDK will continue to (asynchronously) attempt to retrieve a configuration. Once
 * it does, the SDK will begin recording metrics.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param time The time (in milliseconds) to wait for the Controller configuration before proceeding.
 *        X: Wait up to x milliseconds for the Controller configuration.
 *        0: Do not wait for the Controller configuration.
 *        -1: Wait indefinitely until the Controller configuration is received by the agent.
 * @return void
 */
APPD_API void appd_config_set_init_timeout_ms(struct appd_config* cfg, const int time);

/**
 * This function controls the behavior of the SDK shutdown when the appd_sdk_term() call is
 * made. By default, any metrics not reported to the Controller in the minute before shutdown will
 * be lost. Enabling ``flush_metrics_on_shutdown`` will cause appd_sdk_term() to block for up to
 * one minute to allow the reporting of the final minute's metrics that have been captured but not
 * yet reported. This call applies to all application contexts.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param enable
 *      If non-zero, will enable flushing. Zero will disable flushing (the default behavior).
 * @return void
 */
APPD_API void appd_config_set_flush_metrics_on_shutdown(struct appd_config* cfg, int enable);

/**
 * Read configuration from environment variables.
 *
 * Environment variables are named like `<prefix>_<base>` where `<base>` is:
 *
 * * APP_NAME for `appd_config.app_name`
 * * TIER_NAME for `appd_config.tier_name`
 * * NODE_NAME for `appd_config.node_name`
 * * CONTROLLER_HOST for `appd_config.controller.host`
 * * CONTROLLER_PORT for `appd_config.controller.port`
 * * ANALYTICS_HOST for `appd_condif.analytics_agent.host`
 * * ANALYTICS_PORT for `appd_condif.analytics_agent.port`
 * * CONTROLLER_ACCOUNT for `appd_config.controller.account`
 * * CONTROLLER_ACCESS_KEY for `appd_config.controller.access_key`
 * * CONTROLLER_USE_SSL for `appd_config.controller.use_ssl`
 * * CONTROLLER_HTTP_PROXY_HOST for `appd_config.http_proxy.host`
 * * CONTROLLER_HTTP_PROXY_PORT for `appd_config.http_proxy.port`
 * * CONTROLLER_HTTP_PROXY_USERNAME for `appd_config.http_proxy.username`
 * * CONTROLLER_HTTP_PROXY_PASSWORD_FILE for `appd_config.http_proxy.password_file`
 * * INIT_TIMEOUT_MS for `appd_config.init_timeout_ms`
 * * FLUSH_METRICS_ON_SHUTDOWN for `appd_config_set_flush_metrics_on_shutdown`
 *
 * The `<prefix>` is the value of the `prefix` argument to this function. If
 * the passed prefix is NULL or empty, then "`APPD_SDK`" is used.
 *
 * For the `CONTROLLER_USE_SSL` environment variable, values of "off", "0", "f",
 * and "false" (case insensitive) set `use_ssl` to false. Any other value sets
 * `use_ssl` to true.
 *
 * Environment variables are not read by default. Call this
 * function to configure the SDK via environment variables.
 *
 * There is no built-in way to add multiple app contexts via
 * environment variables. You will have to build your own way of doing that.
 *
 * @param cfg A pointer to the default application context structure created by appd_config_init().
 * @param prefix A null-terminated string to prepend to the environment variables list above.
 */
APPD_API void appd_config_getenv(struct appd_config* cfg, const char* prefix);

/**
 * Initialize the AppDynamics SDK.
 *
 * @param config
 *     AppDynamics configuration object.
 * @return
 *     On success, zero is returned. Otherwise, a non-zero value is returned.
 */
APPD_API int appd_sdk_init(const struct appd_config* config);

/**
 * Add application context to AppDynamics SDK for multi-tenancy.
 *
 * @param context_cfg A pointer to the alternate application context structure created by
 * appd_context_config_init().
 */
APPD_API int appd_sdk_add_app_context(struct appd_context_config* context_cfg);

/**
 * Built-in exit call types.
 */
#define APPD_BACKEND_HTTP "HTTP"                /**<HTTP Definition.*/
#define APPD_BACKEND_DB "DB"                    /**<DB Definition.*/
#define APPD_BACKEND_CACHE "CACHE"              /**<Cache Definition.*/
#define APPD_BACKEND_RABBITMQ "RABBITMQ"        /**<RabbitMQ Definition.*/
#define APPD_BACKEND_WEBSERVICE "WEB_SERVICE"   /**<Web_Service Definition.*/
#define APPD_BACKEND_JMS "JMS"                  /**<JMS Definition.*/
#define APPD_BACKEND_WEBSPHEREMQ "WEBSPHERE_MQ" /**<WebSphere Definition.*/

/**
 * Declare the existence of a backend.
 *
 * @param type
 *     One of the `APPD_BACKEND_xxx` constants or any string.
 *
 * @param unregistered_name
 *     The name to give the backend if it has not been registered with the
 *     Controller.
 */
APPD_API void appd_backend_declare(const char* type, const char* unregistered_name);

/**
 * Set an identifying property of a backend. This must be called with a valid
 * key before appd_backend_add() for well known backend types.
 *
 * @param backend A null-terminated string representing the name of the backend to configure
 * @param key A null-terminated string representing the name of the identifying property
 * @param value A null-terminated string representing the value of the said property
 * @return
 *     Zero on success, otherwise non-zero. If non-zero is returned, a
 *     message is logged describing the error.
 */
APPD_API int appd_backend_set_identifying_property(const char* backend, const char* key,
                                                   const char* value);

/**
 * Call to prevent a downstream agent to resolve itself as this backend.
 * This must be called before appd_backend_add().
 *
 * Normally, if an agent picks up a correlation header for an unresolved
 * backend, it will resolve itself as that backend. This is usually the
 * desired behavior.
 *
 * However, if the backend is actually an uninstrumented tier that is
 * passing through the correlation header (for example, a message queue
 * or proxy), then you may wish the backend to show up distinct from the
 * tier that it routes to. If you call this function, correlation headers
 * generated for exit calls to this backend in the SDK will instruct
 * downstream agents to report as distinct from the backend.
 *
 * For example: if you have Tier A talking to uninstrumented Backend B
 * which routes to instrumented Tier C, if you do NOT call this function,
 * the flow map will be A --> C. If you DO call this function, the flow
 * map will be A --> B --> C.
 *
 * @param backend A null-terminated string which is the name of the backend to prevent resolution
 * @return
 *     Zero on success, otherwise non-zero. If non-zero is returned, a
 *     message is logged describing the error.
 */
APPD_API int appd_backend_prevent_agent_resolution(const char* backend);

/**
 * Manually add a backend name to be used later when adding exit-calls to a Buisness Transaction.
 * Note that ``Backend`` names must be unique in a running Agent instance, so if you specify a
 * name that already exists, the function will have no effect.
 *
 * @param backend A null-terminated string with the name of the new backend
 * @return
 *     Zero on success, otherwise non-zero. If non-zero is returned, a
 *     message is logged describing the error. The most common error is that a
 *     backend with the same identifying properties has already been added.
 */
APPD_API int appd_backend_add(const char* backend);

/**
 * This function begins a business transaction with the supplied name.
 * Do not call the function again until the business transaction in question is complete.
 *
 * NOTE : When you generate the business transaction name, do not exceed more than 50 unique names
 * (the default limit for any given tier/node) if more than 200 unique names have been generated
 * within the application (the default limit).
 *
 * @param name
 *     The name for the business transaction.
 * @param correlation_header
 *     A correlation string passed from an upstream node/tier if this is a continuing transaction,
 * else NULL.
 * @return
 *     An opaque handle for the business transaction that was started.
 */
APPD_API appd_bt_handle appd_bt_begin(const char* name, const char* correlation_header);

/**
 * This function begins a business transaction with the supplied name and marks it as a background
 * task.
 *
 * @param name
 *     The name for the business transaction.
 * @param correlation_header
 *     A correlation string passed from an upstream node/tier if this is a continuing transaction,
 * else NULL.
 * @return
 *     An opaque handle for the business transaction that was started.
 */
APPD_API appd_bt_handle appd_bt_begin_bg(const char* name, const char* correlation_header);

/**
 * Create a business transaction for the given alternate context. The targeted alternative context
 * should have been established by the `appd_context_config_init()` call.
 *
 * @param context
 *     The application context name string for the alternate application context.
 * @param name
 *     The name for the business transaction.
 * @param correlation_header
 *     A correlation header if this is a continuing transaction, else NULL.
 * @return
 *     An opaque handle for the business transaction that was started.
 */
APPD_API appd_bt_handle appd_bt_begin_with_app_context(const char* context, const char* name, const char* correlation_header);

/**
 * Create a business transaction for the given alternate context and mark it as a background task.
 * The targeted alternative context should have been established by the `appd_context_config_init()`
 * call.
 *
 * @param context
 *     The application context name string for the alternate application context.
 * @param name
 *     The name for the business transaction.
 * @param correlation_header
 *     A correlation header if this is a continuing transaction, else NULL.
 * @return
 *     An opaque handle for the business transaction that was started.
 */
APPD_API appd_bt_handle appd_bt_begin_bg_with_app_context(const char* context, const char* name, const char* correlation_header);

/**
 * Store a BT handle for retrieval with `appd_bt_get()`.
 *
 * This function allows you to store a BT in a global registry to retrieve
 * later. This is convenient when you need to start and end a BT in
 * separate places, and it is difficult to pass the handle to the BT
 * through the parts of the code that need it.
 *
 * When the BT is ended, the handle is removed from the global registry.
 *
 * Example
 * -------
 *
 *     int begin_transaction(uint64_t txid, uint64_t sku, float price)
 *     {
 *         appd_bt_handle bt = appd_bt_begin("payment-processing", NULL);
 *         appd_bt_store(bt, std::to_string(txid).c_str());
 *         // ...
 *     }
 *
 * @param bt
 *     The business transaction to store.
 * @param guid
 *     A globally unique identifier to associate with the given business transaction.
 */
APPD_API void appd_bt_store(appd_bt_handle bt, const char* guid);

/**
 * Get a BT handle associated with the given GUID by `appd_bt_store()`.
 *
 * @param guid
 *     The globally unique identifier that was passed to `appd_bt_store()`.
 * @return
 *     The BT handle associated with the given GUID. If no BT handle was
 *     associated with the GUID, or if the BT ended prior to getting it,
 *     a warning is logged and the returned handle may be safely used in
 *     other API functions but will cause these functions to immediately
 *     return without doing anything.
 */
APPD_API appd_bt_handle appd_bt_get(const char* guid);

/**
 * Error levels that pass to appd_bt_add_error() and
 * appd_exitcall_add_error().
 */
enum appd_error_level
{
  APPD_LEVEL_NOTICE,  /**< Some unexpected issue appeared though it does not critically impact the
                         workflow. */
  APPD_LEVEL_WARNING, /**< Serious issue appeared that might lead to a degraded behaviour. */
  APPD_LEVEL_ERROR /**< A critical issue occurred and the associated process/call did not gave the
                      expected result. */
};

/**
 * Add an error to a business transaction.
 *
 * Errors are reported as part of the business transaction. However, you can
 * add an error without marking the business transaction as an error (e.g.,
 * for non-fatal errors).
 *
 * @param bt
 *     The business transaction to add the error to.
 * @param level
 *     The error level. One of the `APPD_LEVEL_xxx` constants.
 * @param message
 *     The error message.
 * @param mark_bt_as_error
 *     If true, the business transaction is marked as an error. Otherwise, the
 *     error is added but the transaction is not marked as an error.
 * @return void
 */
APPD_API void appd_bt_add_error(appd_bt_handle bt, enum appd_error_level level, const char* message,
                                int mark_bt_as_error);

/**
 * Return non-zero if the business transaction is taking a snapshot.
 *
 * @param bt
 *     The business transaction to check for snapshotting.
 * @return char indicating snapshot status
 *     Non-zero if the given business transaction is taking a snapshot.
 *     Otherwise, zero.
 */
APPD_API char appd_bt_is_snapshotting(appd_bt_handle bt);

/**
 * Add user data to a snapshot (if one is being taken) or for Analytics (if
 * Analytics is enabled for this business transaction).
 *
 * Data should be either 7-bit ASCII or UTF-8.
 *
 * It is safe to call this function when a snapshot is not occurring or Analytics
 * is not enabled.
 *
 * When the data is for snapshotting only and if extracting the data to pass to
 * this function is expensive, you can use `appd_bt_is_snapshotting()` to check
 * if the business transaction is snapshotting before extracting the data
 * and calling this function.
 *
 * @param bt
 *     The business transaction to add the user data to if it is taking
 *     a snapshot.
 * @param key
 *     The name of the user data to add to the snapshot as 7-bit ASCII or
 *     UTF-8.
 * @param value
 *     The value of the user data to add to the snapshot as 7-bit ASCII or
 *     UTF-8.
 * @return void
 */
APPD_API void appd_bt_add_user_data(appd_bt_handle bt, const char* key, const char* value);

/**
 * Set URL for a snapshot (if one is being taken).
 *
 * URL is set for a snapshot if one is occurring. Data should be
 * either 7-bit ASCII or UTF-8.
 *
 * It is safe to call this function when a snapshot is not occurring.
 * When the given business transcation is NOT snapshotting, this function
 * immediately returns. However, if extracting the data to pass to this
 * function is expensive, you can use `appd_bt_is_snapshotting()` to check
 * if the business transaction is snapshotting before extracting the data
 * and calling this function.
 *
 * @param bt
 *     The business transaction to add the user data to, if it is taking a
 *     snapshot.
 * @param url
 *     The value of the URL for the snapshot as 7-bit ASCII or UTF-8.
 * @return void
 */
APPD_API void appd_bt_set_url(appd_bt_handle bt, const char* url);

/**
 * End the given business transaction.
 *
 * @param bt
 *     The handle to the business transaction to end.
 * @return void
 */
APPD_API void appd_bt_end(appd_bt_handle bt);

/**
 * Start an exit call as part of a business transaction. The destination backend
 * must have been previously declared via a call to appd_backend_add() or
 * appd_backend_declare() otherwise the function will have no effect.
 *
 * @param bt The Business Transaction which requires the exit call.
 * @param backend A null-terminated string representing the name of the targetted
 *     destination of the exit call.
 * @return
 *     An opaque handle to the exit call that was started. NULL if the call fails.
 */
APPD_API appd_exitcall_handle appd_exitcall_begin(appd_bt_handle bt, const char* backend);

/**
 * Store an exit call handle for retrieval with `appd_exitcall_get()`.
 *
 * This function allows you to store an exit call in a global registry to
 * retrieve later. This is convenient when you need to start and end the
 * call in separate places, and it is difficult to pass the handle through
 * the parts of the code that need it.
 *
 * The handle is removed when the exit call (or the business transaction containing it) ends.
 *
 * Example
 * -------
 *
 *     appd_exitcall_handle ec = appd_exitcall_begin(bt, "authdb");
 *     appd_exitcall_store(ec, "login-exit");
 *
 * @param exitcall
 *     The exit call to store.
 * @param guid
 *     A globally unique identifier to associate with the given call.
 * @return void
 */
APPD_API void appd_exitcall_store(appd_exitcall_handle exitcall, const char* guid);

/**
 * Get an exit call associated with a guid via appd_exitcall_store().
 *
 * @param guid
 *     The globally unique identifier that was passed to appd_exitcall_store().
 * @return
 *     The handle associated with the given guid. If no handle was associated
 *     with the guid, or if the call ended prior to getting it, a warning is
 *     logged and the returned handle may be safely used in other API
 *     functions but will cause these functions to immediately return without
 *     doing anything.
 */
APPD_API appd_exitcall_handle appd_exitcall_get(const char* guid);

/**
 * Set the details string for an exit call.
 *
 * This can be used, for example, to add the SQL statement that a DB backend
 * has executed as part of the exit call.
 *
 * @param exitcall
 *     The exit which should receive the given details.
 * @param details
 *     An arbitrary detail string to add to the exit call.
 * @return
 *     Zero on success. Non-zero on error. On error, a message is logged.
 */
APPD_API int appd_exitcall_set_details(appd_exitcall_handle exitcall, const char* details);

/**
 * The default name of the correlation header.
 *
 * Other AppDynamics agents perform automatic correlation for certain
 * types of entry and exit points by looking for a correlation header
 * in the payload with this name.
 *
 * Upstream Correlation
 * --------------------
 *
 * When your SDK instrumented process receives a continuing transaction
 * from an upstream agent that supports automatic correlation, extract
 * the header named `APPD_CORRELATION_HEADER_NAME` from the incoming
 * payload and pass it to appd_bt_begin():
 *
 *     const char* hdr = http_get_header(req, APPD_CORRELATION_HEADER_NAME);
 *     appd_bt_handle bt = appd_bt_begin("fraud detection", hdr);
 *
 * If the header retrieved by the third-party `http_get_header()` function
 * is valid, the BT started on the second line will be a continuation of the
 * business transaction started by the upstream service.
 *
 * Downstream Correlation
 * ----------------------
 *
 * If you are making an exit call where a downstream agent supports
 * automatic correlation, inject a header named `APPD_CORRELATION_HEADER_NAME`
 * into the outgoing payload. The value of the header is retrieved using the
 * `appd_exitcall_get_correlation_header()` function:
 *
 *     appd_exitcall_handle inventory = appd_exitcall_begin(bt, "inventory");
 *     const char* hdr = appd_exitcall_get_correlation_header(inventory);
 *     http_request req;
 *     http_init(&req, HTTP_POST, "https://inventory/holds/%s", sku);
 *     http_set_header(&req, APPD_CORRELATION_HEADER_NAME, hdr);
 *     http_perform(&req);
 *
 * In this example, the hypothetical third-party `http_xxx` functions are used
 * to make an HTTP POST request with an HTTP header containing the correlation
 * header as retrieved by `appd_exitcall_get_correlation_header()`. The header
 * is given the name `APPD_CORRELATION_HEADER_NAME`. A downstream agent that
 * supports automatic correlation for HTTP entry points will automatically
 * extract the correlation header and perform distributed transaction tracing.
 */
#define APPD_CORRELATION_HEADER_NAME "singularityheader"

/**
 * Get the header to correlate a business transaction.
 *
 * If a business transaction makes exit calls that you wish to correlate
 * across, you should retrieve the correlation header and inject that into
 * your exit call's payload.
 *
 * The returned string is freed when the exit call ends. Do not free it
 * yourself.
 *
 * @param exitcall
 *     A handle representing the exit call that you want to correlate and
 *     for which you need a correlation header.
 * @return
 *     A 7-bit ASCII string containing the correlation information.
 *     You can inject this into your payload for an exit call. An
 *     agent on the other end can then extract the header from your
 *     payload and continue the business transaction. On error, a
 *     message is logged and the default header that prevents
 *     downstream bt detection is returned.
 */
APPD_API const char* appd_exitcall_get_correlation_header(appd_exitcall_handle exitcall);

/**
 * Add an error to the exit call, note that an exit call can fail without causing its
 * Business Transaction to completely fail (for non critical errors for example). The
 * `mark_bt_as_error` parameter allows you to control this behaviour.
 *
 * @param exitcall
 *     A handle representing the exit call to which you want to add an error.
 * @param level
 *     The level of the error that occurred, see `::appd_error_level` enum for the
 *     valid possible values.
 * @param message
 *     A null-terminated string that describes the error. Note that only UTF-8 encoded
 *     strings are accepted.
 * @param mark_bt_as_error
 *     A flag indicating if the full Business Transaction should be marked as failed.
 *     Zero represents `false` and any other value wil represent `true`.
 * @return void
 */
APPD_API void appd_exitcall_add_error(appd_exitcall_handle exitcall, enum appd_error_level level,
                                      const char* message, int mark_bt_as_error);

/**
 * Terminate the specified exit call.
 *
 * @param exitcall A handle representing the exit call to close.
 */
APPD_API void appd_exitcall_end(appd_exitcall_handle exitcall);

enum appd_time_rollup_type
{
  APPD_TIMEROLLUP_TYPE_AVERAGE = 1, /**< Compute the average value of the metric over time. */
  APPD_TIMEROLLUP_TYPE_SUM,         /**< Compute the sum of the value of the metric over time.*/
  APPD_TIMEROLLUP_TYPE_CURRENT      /**< Report the current value of the metric.*/
};

enum appd_cluster_rollup_type
{
  APPD_CLUSTERROLLUP_TYPE_INDIVIDUAL = 1, /**< Roll-up the value individually for each member of the cluster.*/
  APPD_CLUSTERROLLUP_TYPE_COLLECTIVE     /**< Roll-up the value across all members of the cluster.*/
};

enum appd_hole_handling_type
{
  APPD_HOLEHANDLING_TYPE_RATE_COUNTER = 1, /**< hole handling: rate counter.     */
  APPD_HOLEHANDLING_TYPE_REGULAR_COUNTER   /**< hole handling: regular counter.  */
};

/**
 * Define a custom metric.
 *
 * @param application_context
 *     The application context for this custom metric.
 * @param metric_path
 *     The path of the custom metric.
 * @param time_rollup_type
 *     Specifies how to rollup metric values for this metric over time, e.g.,
 *     to compute the average over time, pass `APPD_TIMEROLLUP_TYPE_AVERAGE`.
 * @param cluster_rollup_type
 *     Specifies how to rollup metric values for this metric across clusters.
 * @param hole_handling_type
 *     Specifies how to handle holes (gaps where no value has been reported
 *     from this metric).
 * @return void
 */
APPD_API void appd_custom_metric_add(const char* application_context, const char* metric_path,
                                     enum appd_time_rollup_type time_rollup_type,
                                     enum appd_cluster_rollup_type cluster_rollup_type,
                                     enum appd_hole_handling_type hole_handling_type);

/**
 * Report a value for a given metric.
 *
 * @param application_context
 *     The application context for this custom metric.
 * @param metric_path
 *     The path of the metric to report, as defined by `appd_custom_metric_add()`.
 * @param value
 *     The value to report for the metric. The way the value is aggregated is
 *     specified by the roll-up parameters to `appd_custom_metric_add()`.
 * @return void
 */
APPD_API void appd_custom_metric_report(const char* application_context, const char* metric_path,
                                        long value);

/**
 * This is the language of the current frame (will be expanded in the future).
 */
enum appd_frame_type
{
  APPD_FRAME_TYPE_CPP = 1 /**< Default C/C++ Frame */
};

/**
 * Record start of a frame in a call graph that can be reported with a BT.
 * The info is collected only if the BT is snapshotting.
 * This should be called near the start of the method code and must be paired with
 * appd_frame_end() when returning from the method. In C++ code please use the Frame class
 * (below). The current implementation collects only frames from one thread for a BT. Subsequent
 * calls from a different thread will be dropped.
 *
 * @param bt
 *     The business transaction for the call graph.
 * @param frame_type
 *     The type of the frame. When used in C or C++ code, use APPD_FRAME_TYPE_CPP.
 * @param class_name
 *     The name of the class if this method is a member of the class. Please pass an empty string
 *     if tracing a free function.
 * @param method_name
 *     The name of the method.
 * @param file
 *     The path of the source file.
 * @param line_number
 *     The line number in the source file.
 * @return
 *     An opaque handle for the frame. NULL if an error happened.
 */
APPD_API appd_frame_handle appd_frame_begin(appd_bt_handle bt, enum appd_frame_type frame_type,
                                            const char* class_name, const char* method_name,
                                            const char* file, int line_number);

/**
 * Record the end of a frame. Must match a corresponding `appd_frame_begin()`.
 * Call this before returning from the method. Note that if exceptions are thrown, you must handle
 * this in your code, otherwise this part of the call graph will be discarded.
 *
 * @param bt
 *     The business transaction for the call graph.
 * @param frame
 *     The handle of returned by the corresponding `appd_frame_begin()`.
 * @return void
 */
APPD_API void appd_frame_end(appd_bt_handle bt, appd_frame_handle frame);

/**
 * Returns an EUM-ADRUM cookie for a business transaction in an EUM-enabled browser application. You
 * can call this function between `appd_bt_begin()` and `appd_bt_end()`.
 *
 * At this time, `appd_eum_get_cookie` works only on snapshots that contain call graphs.
 *
 * @param bt
 *     The handle to the business transaction that the EUM-ADRUM cookie is generated for.
 * @param https
 *     When set to a non-zero value, this flag indicates that the incoming request uses the HTTPS
 *     protocol. This flag is used to make the EUM cookie secure.
 * @param short_form
 *     When set to a non-zero value, this flag indicates that shortened names will be used for
 *     sub-cookie names inside the EUM cookie.
 * @param referrer_url
 *     The URL of the page from which user made the request. This can be set to NULL or "" if the
 *     referrer URL is unknown.
 * @param path
 *     The URL path where the cookie will be stored. This can be set to "/" in most cases. If set to
 *     NULL or "", the default value "/" is used.
 *
 * @return
 *     On success, the function returns a string that contains a cookie key and a cookie value
 *     separated by =. Do not free the memory associated with the returned cookie string; when you call
 *     `appd_bt_end()`, memory is automatically released. If EUM is disabled or no configuration
 *     information has been received from the Controller, then NULL is returned.
 */
APPD_API const char* appd_eum_get_cookie(appd_bt_handle bt, int https, int short_form,
                                         const char* referrer_url, const char* path);

/**
 * Manually enable snapshot for the business transaction.
 *
 * @param bt
 *     Handle for the relevant business transaction.
 * @return
 *     Returns non-zero if snapshot is enabled. Otherwise a zero value is returned.
 */
APPD_API int appd_bt_enable_snapshot(appd_bt_handle bt);

/**
 * Represents event severity which is passed to `appd_custom_event_start()`.
 */
enum appd_event_severity
{
  APPD_EVENT_SEVERITY_INFO,    /**< Info */
  APPD_EVENT_SEVERITY_WARNING, /**< Warning */
  APPD_EVENT_SEVERITY_ERROR    /**< Error */
};

/**
 * Start to define a custom event. The definition of custom event will complete when
 * `appd_custom_event_end()` is called and event will be reported.
 *
 * @param application_context
 *     A string contaning the application context name for this custom event.
 * @param severity
 *     Enum representing the severity for this custom event.
 * @param event_sub_type
 *     A string containing the custom event subtype. This subtype can be used in controller to
 *     filter the custom events belonging to a specific subtype.
 * @param summary
 *     A string containing the summary of what this custom event is about.
 * @return
 *     An event handle for the custom event that is being defined. It will be NULL if failed to
 *     start the custom event definition.
 */
APPD_API appd_event_handle appd_custom_event_start(const char* application_context,
                                                   enum appd_event_severity severity,
                                                   const char* event_sub_type,
                                                   const char* summary);

/**
 * Add property name and value for custom event. This property name and value are used to filter
 * the custom events in the Controller.
 *
 * @param event_handle
 *     An handle for the custom event.
 *     If this is NULL, zero, or invalid, then it is an error and zero is returned.
 * @param property_name
 *     A null-terminated string containing the name of the property to add.
 *     If this is NULL, zero, or empty, then it is an error and zero is returned.
 * @param property_value
 *     A null-terminated string containing the value of the property.
 *     If this is NULL, zero, or empty, then it is an error and zero is returned.
 * @return An integer indicating the success of operation.
 *     Returns non-zero if the property was successfully added to the custom event definition.
 *     Otherwise, zero is returned.
 */
APPD_API int appd_custom_event_add_property(appd_event_handle event_handle,
                                            const char* property_name,
                                            const char* property_value);

/**
 * Add detail name and value for custom event. This detail name and value are used to tag
 * additional details to the custom event.
 *
 * @param event_handle
 *     An handle for the custom event.
 *     If this is NULL, zero, or invalid, then it is an error and zero is returned.
 * @param detail_name
 *     A null-terminated string containing the name of detail.
 *     If this is NULL, zero, or empty, then it is an error and zero is returned.
 * @param detail_value
 *     A null-terminated string containing the value of detail.
 *     If this is NULL, zero, or empty, then it is an error and zero is returned.
 * @return
 *     Returns non-zero if the detail was successfully added to custom event definition. Otherwise,
 *     zero is returned.
 */
APPD_API int appd_custom_event_add_detail(appd_event_handle event_handle,
                                          const char* detail_name,
                                          const char* detail_value);

/**
 * End the definition of custom event and report it.
 *
 * If appd_custom_event_start() is called and appd_custom_event_end() is NOT called for the
 * corresponding `event_handle`, then the event will never be reported to the Controller. Also the
 * memory holding the even data will be kept alive until appd_sdk_term() is called.
 *
 * @param event_handle
 *     An handle for the custom event.
 *     If this is NULL, zero, or invalid, then it is an error and zero is returned.
 * @return
 *     Returns non-zero if custom event definition is completed and reported. Otherwise
 *     zero is returned.
 */
APPD_API int appd_custom_event_end(appd_event_handle event_handle);

/**
 * Terminate the AppDynamics SDK.
 */
APPD_API void appd_sdk_term();

#ifdef __cplusplus

namespace appd {
namespace sdk {
class CallGraph;
class CallGraphElement;
}  // namespace sdk
}  // namespace appd

APPD_API
appd::sdk::CallGraphElement* appd_construct_callgraph_element(
    const appd::sdk::CallGraph* callgraph, const std::string& class_name,
    const std::string& method_name, const std::string& file_path, int32_t line_number,
    int32_t time_msec, appd_frame_type frame_type);

APPD_API
bool appd_callgraph_add_to_snapshot(const appd::sdk::CallGraph* callgraph);

} /* extern "C" */

namespace appd {
namespace sdk {

template <typename HandleT>
class HandleWrapper
{
public:
  HandleWrapper() : m_handle(), m_borrowed_handle(false) {}
  HandleWrapper(HandleT handle) : m_handle(handle), m_borrowed_handle(true) {}
  virtual ~HandleWrapper() = default;

  HandleT handle() { return m_handle; }
  const HandleT handle() const { return m_handle; }

  virtual void add_error(enum appd_error_level level, const char* message,
                         bool mark_bt_as_error = true) = 0;

  void add_error(enum appd_error_level level, const std::string& message,
                 bool mark_bt_as_error = true)
  {
    add_error(level, message.c_str(), mark_bt_as_error);
  }

protected:
  HandleT m_handle;
  bool m_borrowed_handle;

private:
  HandleWrapper(const HandleWrapper&);
  HandleWrapper& operator=(const HandleWrapper&);
};

/**
 * Represents an AppDynamics business transaction for C++ applications.
 *
 * An instance of this class begins a business transaction (similar to
 * calling `appd_bt_begin()`) upon construction and ends the business
 * transaction upon destruction. You can match a business transaction's lifetime
 * to a scope:
 *
 *     {
 *         appd::sdk::BT bt("compute");
 *
 *         // all code in this scope is part of the "compute" BT
 *         // `bt` is automatically ended when it goes out of scope
 *     }
 *
 * When a BT has a lifetime that depends on the nondeterministic
 * lifetimes of other objects, you can use a shared pointer to a BT
 * to keep the BT alive for the lifetimes of its dependencies:
 *
 *     auto bt = std::make_shared<appd::sdk::BT>("compute");
 *     auto prod = createProducer(bt);
 *     auto consumers = createConsumers(bt, NUM_WORKERS);
 *
 * In this example, the `BT` ends when the last reference to it ends.
 *
 * You cannot copy a business transaction.
 */
class BT : public HandleWrapper<appd_bt_handle>
{
public:
  /**
   * Construct a business transaction object with the given name.
   *
   * If a valid AppDynamics `correlation_header` is passed (generated by
   * the SDK or another agent), the business transaction is marked to continue the
   * transaction described in the header.
   *
   * @param name
   *     The name of the Business Transaction.
   * @param correlation_header
   *     An AppDynamics correlation header or NULL.
   */
  BT(const char* name, const char* correlation_header = NULL) : HandleWrapper<appd_bt_handle>()
  {
    init(name, correlation_header);
  }

  /**
   * Construct a BT object with the given name.
   *
   * This name will be used if there is no correlation string available (for continuing BTs from an
   * upstream tier) or if the Controller has not yet initialized the agent configuration.
   *
   * @param name
   *     The name of the Business Transaction.
   */
  explicit BT(const std::string& name) : HandleWrapper<appd_bt_handle>() { init(name.c_str()); }

  /**
   * Construct a continuing business transaction object with the given name and
   * correlation header.
   *
   * The name is used only if the correlation header is empty or
   * otherwise invalid, in which case this BT will be reported as an
   * originating BT with the specified name.
   *
   * @param name
   *     The name of the Business Transaction if the correlation header is invalid.
   * @param correlation_header
   *     An AppDynamics correlation header.
   */
  BT(const std::string& name, const std::string& correlation_header)
      : HandleWrapper<appd_bt_handle>()
  {
    init(name.c_str(), correlation_header.c_str());
  }

  /**
   * Wrap an `appd_bt_handle` in a BT object. The BT object does not
   * own the wrapped handle: destructing this object will not end
   * the wrapped BT.
   *
   * @param bt
   *     The handle of a BT, as returned by `appd_bt_begin()` or `appd_bt_get()`.
   */
  BT(appd_bt_handle bt) : HandleWrapper<appd_bt_handle>(bt) {}

  ~BT()
  {
    if (!m_borrowed_handle) {
      appd_bt_end(m_handle);
    }
  }

  /**
   * Calls `appd_bt_store()` on this BT with the given GUID.
   */
  void store(const char* guid) { appd_bt_store(m_handle, guid); }

  /**
   * Calls `appd_bt_store()` on this BT with the given GUID.
   */
  void store(const std::string& guid) { store(guid.c_str()); }

  /**
   * Calls `appd_bt_is_snapshotting()` on this BT.
   */
  bool is_snapshotting() const { return appd_bt_is_snapshotting(m_handle); }

  /**
   * Calls `appd_bt_enable_snapshot()` on this BT.
   */
  bool enable_snapshot() const { return appd_bt_enable_snapshot(m_handle); }

  /**
   * Calls `appd_bt_add_error()` on this BT with the given error information.
   */
  virtual void add_error(enum appd_error_level level, const char* message,
                         bool mark_bt_as_error = true)
  {
    appd_bt_add_error(m_handle, level, message, mark_bt_as_error);
  }

  /**
   * Calls `appd_bt_add_user_data()` on this business transaction with the given key and value.
   */
  void add_user_data(const char* key, const char* value)
  {
    appd_bt_add_user_data(m_handle, key, value);
  }

  /**
   * Calls `appd_bt_add_user_data()` on this business transaction with the given key and value.
   */
  void add_user_data(const std::string& key, const std::string& value)
  {
    add_user_data(key.c_str(), value.c_str());
  }

  /**
   * Calls `appd_bt_set_url()` on this business transaction with the given URL.
   */
  void set_url(const char* url) { appd_bt_set_url(m_handle, url); }

  /**
   * Calls `appd_bt_set_url()` on this business transaction with the given URL.
   */
  void set_url(const std::string& url) { set_url(url.c_str()); }

protected:
  void init(const char* name, const char* correlation_header = NULL)
  {
    m_handle = appd_bt_begin(name, correlation_header);
  }

private:
  BT();
};

/**
 * Represents an AppDynamics exit call as part of a business transaction.
 *
 * As with `appd::sdk::BT`, this class allows automatically ending an
 * exit call when it goes out of scope:
 *
 *     {
 *         appd::sdk::BT bt("compute");
 *
 *         {
 *             appd::sdk::ExitCall db_call(bt, "primary-db");
 *             auto db = get_db_connection();
 *             db_call.set_details(query);
 *             db.execute(query, params);
 *         }  // exit call ends automatically
 *
 *         // ... more that happens as part of the BT ...
 *     }
 *
 * For managing an exit call with a more complex lifetime, you are
 * encouraged to look at a `std::unique_ptr<appd::sdk::ExitCall>` or
 * `std::shared_ptr<appd::sdk::ExitCall>`.
 *
 * You cannot copy an ExitCall.
 */
class ExitCall : public HandleWrapper<appd_exitcall_handle>
{
public:
  /**
   * Constructs an object representing an ExitCall.
   *
   * @param bt
   *     The BT object that owns this exit call.
   * @param backend
   *     The name of the registered backend this exit call talks to.
   */
  ExitCall(BT& bt, const char* backend) { init(bt, backend); }

  /**
   * Constructs an object representing an ExitCall.
   *
   * @param bt
   *     The BT object that owns this exit call.
   * @param backend
   *     The name of the registered backend this exit call talks to.
   */
  ExitCall(BT& bt, const std::string& backend) { init(bt, backend.c_str()); }

  /**
   * Constructs an object wrapping the given exit call handle.
   *
   * The handle is not owned by this object: the destructor will not call
   * appd_exitcall_end() on the given handle.
   *
   * @param exitcall
   *     An exit call handle as returned by `appd_exitcall_begin()` or
   *     `appd_exitcall_get()`.
   */
  ExitCall(appd_exitcall_handle exitcall)
      : HandleWrapper<appd_exitcall_handle>(exitcall), m_loaded_correlation_header(false)
  {
  }

  ~ExitCall()
  {
    if (!m_borrowed_handle) {
      appd_exitcall_end(m_handle);
    }
  }

  /**
   * Calls `appd_exitcall_store()` on this exit call with the given GUID.
   */
  void store(const char* guid) { appd_exitcall_store(m_handle, guid); }

  /**
   * Calls `appd_exitcall_store()` on this exit call with the given GUID.
   */
  void store(const std::string& guid) { store(guid.c_str()); }

  /**
   * Calls `appd_exitcall_get_correlation_header()` on this exit call.
   */
  const std::string& get_correlation_header()
  {
    if (!m_loaded_correlation_header) {
      const char* hdr = appd_exitcall_get_correlation_header(m_handle);
      m_loaded_correlation_header = true;

      if (hdr) {
        correlation_header = hdr;
      }
    }

    return correlation_header;
  }

  /**
   * Calls `appd_exitcall_set_details()` on this exit call with the given
   * information.
   */
  int set_details(const char* details) { return appd_exitcall_set_details(m_handle, details); }

  /**
   * Calls `appd_exitcall_set_details()` on this exit call with the given
   * information.
   */
  int set_details(const std::string& details) { return set_details(details.c_str()); }

  /**
   * Calls `appd_exitcall_add_error()` on this exit call with the given error
   * information.
   */
  virtual void add_error(enum appd_error_level level, const char* message,
                         bool mark_bt_as_error = true)
  {
    appd_exitcall_add_error(m_handle, level, message, mark_bt_as_error);
  }

protected:
  void init(BT& bt, const char* backend)
  {
    m_handle = appd_exitcall_begin(bt.handle(), backend);
    m_loaded_correlation_header = false;
  }

private:
  ExitCall();

  bool m_loaded_correlation_header;
  std::string correlation_header;
};

/**
 * Represents a frame in a call graph that can be reported with a business transaction.
 *
 * Each business transaction has a stack of active frames. When a Frame object is constructed,
 * it is pushed onto the business transaction's stack. The constructed Frame has as its parent
 * the Frame that was at the top of the stack at the time of its construction.
 * If the stack was empty when the Frame was created, it is the root of the
 * business transaction's call graph and has no parent.
 *
 * It is recommended to create these objects on the stack (with the RAII
 * pattern) and from a single thread.
 */

class Frame
{
public:
  /**
   * @param bt
   *     The business transaction object that owns this function call.
   * @param frame_type
   *     The type of the frame. When used in C or C++ code, use APPD_FRAME_TYPE_CPP.
   * @param class_name
   *     The name of the class if this method is a member of the class, else NULL.
   * @param method_name
   *     The name of the method.
   * @param file
   *     The path of the source file.
   * @param line_number
   *     The line number in the source file.
   */
  Frame(BT& bt, appd_frame_type frame_type, const char* class_name, const char* method_name,
        const char* file, int line_number)
      : m_bt(bt)
  {
    m_frame_handle =
        appd_frame_begin(m_bt.handle(), frame_type, class_name, method_name, file, line_number);
  }

  Frame() = delete;
  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;

  ~Frame() { appd_frame_end(m_bt.handle(), m_frame_handle); }

private:
  BT& m_bt;
  appd_frame_handle m_frame_handle;
};

/**
 * A call graph element.
 *
 * A member of the CallGraph tree.
 */
class CallGraphElement
{
protected:
  CallGraphElement() = default;
  CallGraphElement(const CallGraphElement&) = delete;
  CallGraphElement& operator=(const CallGraphElement&) = delete;

public:
  virtual ~CallGraphElement() = default;

  /**
   * @param time_msec
   *     The time in milliseconds for the execution of this method.
   * @param class_name
   *     The name of the class the function belongs to. If you are tracing a free
   *     function this can be an empty string.
   * @param method_name
   *     The name of the traced function. It must be encoded as a valid UTF-8 string.
   * @param file_path
   *     The file path/name containing the traced function. It must be encoded as a valid UTF-8
   *     string.
   * @param line_number
   *     The line number where the frame is taken for more precise localisation.
   * @param frame_type
   *     An enum value representing the language of the frame.
   * For the other parameters see the "Frame" class.
   */
  virtual CallGraphElement& add_child(const std::string& class_name, const std::string& method_name,
                                      const std::string& file_path, int32_t line_number,
                                      int32_t time_msec, appd_frame_type frame_type) = 0;

  virtual CallGraphElement& add_exit_call(appd_exitcall_handle exit_call, int32_t time_msec) = 0;
};

/**
 * A pre-populated call graph.
 *
 * Provide the execution time and specify the parent/child (caller/called)
 * relationships to construct the call graph tree.
 * Once the tree is constructed, you can add it to a business transaction with
 * 'add_to_snapshot()'. You can use `appd_bt_is_snapshotting()` to check
 * if the business transaction is snapshotting before generating the data
 * and calling this function.
 */

class CallGraph
{
public:
  /**
   * For the other parameters see the "Frame" class. They refer to the root of the call graph.
   *
   * @param bt
   *     The business transaction for this call graph.
   * @param time_msec
   * @param class_name
   * @param method_name
   * @param file_path
   * @param line_number
   * @param frame_type The time in milliseconds for the execution of this method.
   *
   */
  CallGraph(BT& bt, const std::string& class_name, const std::string& method_name,
            const std::string& file_path, int32_t line_number, int32_t time_msec,
            appd_frame_type frame_type)
      : m_bt(bt),
        m_root(appd_construct_callgraph_element(this, class_name, method_name, file_path,
                                                line_number, time_msec, frame_type))
  {
  }

  ~CallGraph() = default;

  CallGraph() = delete;
  CallGraph(const CallGraph&) = delete;
  CallGraph& operator=(const CallGraph&) = delete;

  bool add_to_snapshot() const { return appd_callgraph_add_to_snapshot(this); }

  CallGraphElement& root() const { return *m_root; }
  BT& bt() { return m_bt; }
  const BT& bt() const { return m_bt; }

private:
  BT& m_bt;
  const std::unique_ptr<CallGraphElement> m_root;
};

/**
 * Represents an AppDynamics Event for C++ applications.
 *
 * An instance of this class defines an event ( equivalent to calling `appd_custom_event_start()`,
 * `appd_custom_event_add_property()`, and `appd_custom_event_add_detail()`) upon construction.
 * Then on this event object `report()` method should be invoked ( equivalent to calling
 * `appd_custom_event_end`) to report the event.
 *
 * A simple usage example is shown below:
 *
 *     {
 *       std::string applicationContext = "";
 *       appd_event_severity severity = APPD_EVENT_SEVERITY_INFO;
 *       std::string eventSubType = "category3";
 *       std::string summary = "This is a summary message.";
 *       std::map<std::string, std::string> properties;
 *       properties.emplace("propertyName", "propertyValue");
 *       std::map<std::string, std::string> details;
 *       details.emplace("detailName", "detailValue");
 *
 *       appd::sdk::Event customEvent(applicationContext, severity, eventSubType, summary,
 *                                    properties, details);
 *       bool success = customEvent.report();
 *
 *       // other code
 *     }
 *
 * You cannot copy an Event.
 */

class Event
{
public:
  /**
   * Create a custom event object.
   *
   * @param applicationContext
   *     A string that contains the application context name for this custom event. If the
   *     application context is empty, then the default agent is picked.
   * @param severity
   *     Enum representing the severity for this custom event.
   * @param eventSubType
   *     A string that contains the custom event subtype. This subtype can be used in controller to
   *     filter the custom events belonging to a specific subtype.
   * @param summary
   *     A string that contains the summary of what this custom event is about.
   * @param properties
   *     A map that contains the property-name as key and property-value as value. These
   *     property-name and property-values cannot be empty, but an empty map can be passed in if there
   *     are no properties to pass.
   * @param details
   *     A map that contains the detail-name as key and detail-value as value. These detail-name
   *     and detail-values cannot be empty, but an empty map can be passed in if there are no
   *     details to pass.
   */
  Event(const std::string& applicationContext, appd_event_severity severity,
        const std::string& eventSubType, const std::string& summary,
        const std::map<std::string, std::string>& properties,
        const std::map<std::string, std::string>& details)
  {
    m_eventHandle = appd_custom_event_start(applicationContext.c_str(), severity,
                                            eventSubType.c_str(), summary.c_str());
    if (m_eventHandle == nullptr) {
      m_success = false;
      return;
    }

    for (const auto& property : properties) {
      m_success = appd_custom_event_add_property(m_eventHandle, property.first.c_str(),
                                                 property.second.c_str());
      if (!m_success) {
        return;
      }
    }

    for (const auto& detail : details) {
      m_success = appd_custom_event_add_detail(m_eventHandle, detail.first.c_str(),
                                               detail.second.c_str());
      if (!m_success) {
        return;
      }
    }
  }

  /**
   * Report the custom event.
   *
   * @return bool
   *    Returns true on successful reporting of the custom event, and false otherwise.
   */
  bool report()
  {
    if (! m_success) {
      return false;
    }
    return appd_custom_event_end(m_eventHandle);
  }

private:
  Event() = delete;
  Event(const Event&) = delete;

  bool m_success = true;
  appd_event_handle m_eventHandle = nullptr;
};

}  // namespace sdk
}  // namespace appd

#endif /* !defined(__cplusplus) */

/*
 * Frame and callstack helpers.
 */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#if __STDC_VERSION__ >= 199901L || __cplusplus > 199711L
#define APPD_FUNCTION_NAME __func__
#elif __GNUC__ >= 2 || defined(_MSC_VER)
#define APPD_FUNCTION_NAME __FUNCTION__
#else
#define APPD_FUNCTION_NAME "unknown"
#endif

#if defined(__cplusplus)

#define APPD_AUTO_FRAME(bt)                                                                      \
  appd::sdk::Frame __appd_f##__COUNTER__((bt), APPD_FRAME_TYPE_CPP, nullptr, APPD_FUNCTION_NAME, \
                                         __FILE__, __LINE__)

#elif __has_attribute(cleanup) || __GNUC__ >= 4

#define APPD_AUTO_FRAME(bt)                                                          \
  appd_frame_handle __appd_f##__COUNTER__ __attribute__((cleanup(appd_frame_end))) = \
      appd_frame_begin((bt), APPD_FRAME_TYPE_CPP, NULL, APPD_FUNCTION_NAME, __FILE__, __LINE__)

#endif

#endif /* APPDYNAMICS_H_ */
