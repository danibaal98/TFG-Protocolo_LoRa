/* Automatically generated nanopb header */
/* Generated by 0.4.1-dev */

#ifndef PB_MTPPLATINO_MTPPLATINO_PB_H_INCLUDED
#define PB_MTPPLATINO_MTPPLATINO_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _mtpPlatino_MSG_PHASE1_DRONE_ID {
    int32_t id;
} mtpPlatino_MSG_PHASE1_DRONE_ID;

typedef struct _mtpPlatino_MSG_PHASE1_ENDDEVICE_ID {
    int32_t id;
    int32_t numPackets;
} mtpPlatino_MSG_PHASE1_ENDDEVICE_ID;

typedef struct _mtpPlatino_MSG_PHASE2_DRONE_NACK {
    int32_t id;
    int32_t idPacket;
} mtpPlatino_MSG_PHASE2_DRONE_NACK;

typedef struct _mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA {
    int32_t idDev;
    int32_t idPacket;
    float nodeLoad;
    float batteryLoad;
    float panelLoad;
    float windSpeed;
    float temperature;
    float humidity;
    float batteryVolt;
    int32_t samplingTime;
} mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA;


/* Initializer values for message structs */
#define mtpPlatino_MSG_PHASE1_DRONE_ID_init_default {0}
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_default {0, 0}
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_default {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_init_default {0, 0}
#define mtpPlatino_MSG_PHASE1_DRONE_ID_init_zero {0}
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_zero {0, 0}
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_zero {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_init_zero {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define mtpPlatino_MSG_PHASE1_DRONE_ID_id_tag    1
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_id_tag 1
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_numPackets_tag 2
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_id_tag  1
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_idPacket_tag 2
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_idDev_tag 1
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_idPacket_tag 2
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_nodeLoad_tag 3
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_batteryLoad_tag 4
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_panelLoad_tag 5
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_windSpeed_tag 6
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_temperature_tag 7
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_humidity_tag 8
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_batteryVolt_tag 9
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_samplingTime_tag 10

/* Struct field encoding specification for nanopb */
#define mtpPlatino_MSG_PHASE1_DRONE_ID_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    id,                1)
#define mtpPlatino_MSG_PHASE1_DRONE_ID_CALLBACK NULL
#define mtpPlatino_MSG_PHASE1_DRONE_ID_DEFAULT NULL

#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    id,                1) \
X(a, STATIC,   SINGULAR, INT32,    numPackets,        2)
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_CALLBACK NULL
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_DEFAULT NULL

#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    idDev,             1) \
X(a, STATIC,   SINGULAR, INT32,    idPacket,          2) \
X(a, STATIC,   SINGULAR, FLOAT,    nodeLoad,          3) \
X(a, STATIC,   SINGULAR, FLOAT,    batteryLoad,       4) \
X(a, STATIC,   SINGULAR, FLOAT,    panelLoad,         5) \
X(a, STATIC,   SINGULAR, FLOAT,    windSpeed,         6) \
X(a, STATIC,   SINGULAR, FLOAT,    temperature,       7) \
X(a, STATIC,   SINGULAR, FLOAT,    humidity,          8) \
X(a, STATIC,   SINGULAR, FLOAT,    batteryVolt,       9) \
X(a, STATIC,   SINGULAR, INT32,    samplingTime,     10)
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_CALLBACK NULL
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_DEFAULT NULL

#define mtpPlatino_MSG_PHASE2_DRONE_NACK_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    id,                1) \
X(a, STATIC,   SINGULAR, INT32,    idPacket,          2)
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_CALLBACK NULL
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_DEFAULT NULL

extern const pb_msgdesc_t mtpPlatino_MSG_PHASE1_DRONE_ID_msg;
extern const pb_msgdesc_t mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_msg;
extern const pb_msgdesc_t mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_msg;
extern const pb_msgdesc_t mtpPlatino_MSG_PHASE2_DRONE_NACK_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define mtpPlatino_MSG_PHASE1_DRONE_ID_fields &mtpPlatino_MSG_PHASE1_DRONE_ID_msg
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_fields &mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_msg
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_fields &mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_msg
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_fields &mtpPlatino_MSG_PHASE2_DRONE_NACK_msg

/* Maximum encoded size of messages (where known) */
#define mtpPlatino_MSG_PHASE1_DRONE_ID_size      11
#define mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_size  22
#define mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_size 68
#define mtpPlatino_MSG_PHASE2_DRONE_NACK_size    22

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif