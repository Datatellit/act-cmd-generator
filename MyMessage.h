/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */


#ifndef MyMessage_h
#define MyMessage_h

//#include "application.h"
#include "xliCommon.h"

#define PROTOCOL_VERSION 1
#define MAX_MESSAGE_LENGTH 32
#define HEADER_SIZE 7
#define MAX_PAYLOAD (MAX_MESSAGE_LENGTH - HEADER_SIZE)

// Message types
typedef enum {
	C_PRESENTATION = 0,
	C_SET = 1,
	C_REQ = 2,
	C_INTERNAL = 3,
	C_STREAM = 4 // For Firmware and other larger chunks of data that need to be divided into pieces.
} mysensor_command;

// Type of sensor (used when presenting sensors)
typedef enum {
	S_DOOR, // Door sensor, V_TRIPPED, V_ARMED
	S_MOTION,  // Motion sensor, V_TRIPPED, V_ARMED
	S_SMOKE,  // Smoke sensor, V_TRIPPED, V_ARMED
	S_LIGHT, // Binary light or relay, V_STATUS (or V_LIGHT), V_WATT
	S_BINARY=3, // Binary light or relay, V_STATUS (or V_LIGHT), V_WATT (same as S_LIGHT)
	S_DIMMER, // Dimmable light or fan device, V_STATUS (on/off), V_DIMMER (dimmer level 0-100), V_WATT
	S_COVER, // Blinds or window cover, V_UP, V_DOWN, V_STOP, V_DIMMER (open/close to a percentage)
	S_TEMP, // Temperature sensor, V_TEMP
	S_HUM, // Humidity sensor, V_HUM
	S_BARO, // Barometer sensor, V_PRESSURE, V_FORECAST
	S_WIND, // Wind sensor, V_WIND, V_GUST
	S_RAIN, // Rain sensor, V_RAIN, V_RAINRATE
	S_UV, // Uv sensor, V_UV
	S_WEIGHT, // Personal scale sensor, V_WEIGHT, V_IMPEDANCE
	S_POWER, // Power meter, V_WATT, V_KWH
	S_HEATER, // Header device, V_HVAC_SETPOINT_HEAT, V_HVAC_FLOW_STATE, V_TEMP
	S_DISTANCE, // Distance sensor, V_DISTANCE
	S_LIGHT_LEVEL, // Light level sensor, V_LIGHT_LEVEL (uncalibrated in percentage),  V_LEVEL (light level in lux)
	S_ARDUINO_NODE, // Used (internally) for presenting a non-repeating Arduino node
	S_ARDUINO_REPEATER_NODE, // Used (internally) for presenting a repeating Arduino node
	S_LOCK, // Lock device, V_LOCK_STATUS
	S_IR, // Ir device, V_IR_SEND, V_IR_RECEIVE
	S_WATER, // Water meter, V_FLOW, V_VOLUME
	S_AIR_QUALITY, // Air quality sensor, V_LEVEL
	S_CUSTOM, // Custom sensor
	S_DUST, // Dust sensor, V_LEVEL
	S_SCENE_CONTROLLER, // Scene controller device, V_SCENE_ON, V_SCENE_OFF.
	S_RGB_LIGHT, // RGB light. Send color component data using V_RGB. Also supports V_WATT
	S_RGBW_LIGHT, // RGB light with an additional White component. Send data using V_RGBW. Also supports V_WATT
	S_COLOR_SENSOR,  // Color sensor, send color information using V_RGB
	S_HVAC, // Thermostat/HVAC device. V_HVAC_SETPOINT_HEAT, V_HVAC_SETPOINT_COLD, V_HVAC_FLOW_STATE, V_HVAC_FLOW_MODE, V_TEMP
	S_MULTIMETER, // Multimeter device, V_VOLTAGE, V_CURRENT, V_IMPEDANCE
	S_SPRINKLER,  // Sprinkler, V_STATUS (turn on/off), V_TRIPPED (if fire detecting device)
	S_WATER_LEAK, // Water leak sensor, V_TRIPPED, V_ARMED
	S_SOUND, // Sound sensor, V_TRIPPED, V_ARMED, V_LEVEL (sound level in dB)
	S_VIBRATION, // Vibration sensor, V_TRIPPED, V_ARMED, V_LEVEL (vibration in Hz)
	S_MOISTURE, // Moisture sensor, V_TRIPPED, V_ARMED, V_LEVEL (water content or moisture in percentage?)
	S_GAS,
	S_ALCOHOL,
	S_ZENSENSOR = 65, // Xlight Super Sensor
	S_ZENREMOTE = 66, // Xlight Super Remote
	S_ZENSICK,
} mysensor_sensor;

// Type of sensor data (for set/req/ack messages)
typedef enum {
	V_TEMP, // S_TEMP. Temperature S_TEMP, S_HEATER, S_HVAC
	V_HUM, // S_HUM. Humidity
	V_STATUS, //  S_LIGHT, S_DIMMER, S_SPRINKLER, S_HVAC, S_HEATER. Used for setting/reporting binary (on/off) status. 1=on, 0=off
	V_LIGHT=2, // Same as V_STATUS
	V_PERCENTAGE, // S_DIMMER. Used for sending a percentage value 0-100 (%).
	V_DIMMER=3, // S_DIMMER. Same as V_PERCENTAGE.
	V_PRESSURE, // S_BARO. Atmospheric Pressure
	V_FORECAST, // S_BARO. Whether forecast. string of "stable", "sunny", "cloudy", "unstable", "thunderstorm" or "unknown"
	V_RAIN, // S_RAIN. Amount of rain
	V_RAINRATE, // S_RAIN. Rate of rain
	V_WIND, // S_WIND. Wind speed
	V_GUST,  // S_WIND. Gust
	V_DIRECTION, // S_WIND. Wind direction 0-360 (degrees)
	V_UV, // S_UV. UV light level
	V_WEIGHT, // S_WEIGHT. Weight(for scales etc)
	V_DISTANCE, // S_DISTANCE. Distance
	V_IMPEDANCE, // S_MULTIMETER, S_WEIGHT. Impedance value
	V_ARMED, // S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER. Armed status of a security sensor. 1 = Armed, 0 = Bypassed
	V_TRIPPED, // S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER, S_WATER_LEAK, S_SOUND, S_VIBRATION, S_MOISTURE. Tripped status of a security sensor. 1 = Tripped, 0
	V_WATT, // S_POWER, S_LIGHT, S_DIMMER, S_RGB, S_RGBW. Watt value for power meters
	V_KWH, // S_POWER. Accumulated number of KWH for a power meter
	V_SCENE_ON, // S_SCENE_CONTROLLER. Turn on a scene
	V_SCENE_OFF, // S_SCENE_CONTROLLER. Turn off a scene
	V_HEATER, // Deprecated. Use V_HVAC_FLOW_STATE instead.
	V_HVAC_FLOW_STATE=21,  // S_HEATER, S_HVAC. HVAC flow state ("Off", "HeatOn", "CoolOn", or "AutoChangeOver")
	V_HVAC_SPEED, // S_HVAC, S_HEATER. HVAC/Heater fan speed ("Min", "Normal", "Max", "Auto")
	V_LIGHT_LEVEL, // S_LIGHT_LEVEL. Uncalibrated light level. 0-100%. Use V_LEVEL for light level in lux
	V_VAR1, V_VAR2, V_VAR3, V_VAR4, V_VAR5,
	V_UP, // S_COVER. Window covering. Up
	V_DOWN, // S_COVER. Window covering. Down
	V_STOP, // S_COVER. Window covering. Stop
	V_IR_SEND, // S_IR. Send out an IR-command
	V_IR_RECEIVE, // S_IR. This message contains a received IR-command
	V_FLOW, // S_WATER. Flow of water (in meter)
	V_VOLUME, // S_WATER. Water volume
	V_LOCK_STATUS, // S_LOCK. Set or get lock status. 1=Locked, 0=Unlocked
	V_LEVEL, // S_DUST, S_AIR_QUALITY, S_SOUND (dB), S_VIBRATION (hz), S_LIGHT_LEVEL (lux)
	V_VOLTAGE, // S_MULTIMETER
	V_CURRENT, // S_MULTIMETER
	V_RGB, 	// S_RGB_LIGHT, S_COLOR_SENSOR.
					// Used for sending color information for multi color LED lighting or color sensors.
					// Sent as ASCII hex: RRGGBB (RR=red, GG=green, BB=blue component)
	V_RGBW, // S_RGBW_LIGHT
					// Used for sending color information to multi color LED lighting.
					// Sent as ASCII hex: RRGGBBWW (WW=white component)
	V_ID,   // S_TEMP
					// Used for sending in sensors hardware ids (i.e. OneWire DS1820b).
	V_UNIT_PREFIX, 	// S_DUST, S_AIR_QUALITY
					// Allows sensors to send in a string representing the
					// unit prefix to be displayed in GUI, not parsed by controller! E.g. cm, m, km, inch.
					// Can be used for S_DISTANCE or gas concentration
	V_HVAC_SETPOINT_COOL, // S_HVAC. HVAC cool setpoint (Integer between 0-100)
	V_HVAC_SETPOINT_HEAT, // S_HEATER, S_HVAC. HVAC/Heater setpoint (Integer between 0-100)
	V_HVAC_FLOW_MODE, // S_HVAC. Flow mode for HVAC ("Auto", "ContinuousOn", "PeriodicOn")

	V_RELAY_ON = 65,        // Xlight relay on
	V_RELAY_OFF,            // Xlight relay off
	V_RELAY_MAP,			// Xlight relay keymap
    V_REMOTE_KEY,			// Xlight remote button trigger
    V_REMOTE_SCENARIO = 68,	// Xlight remote scenario
	V_FAN=69,               // FAN

	V_MODBUS_READ = 80,     // Read ModBus Register
	V_MODBUS_WRITE = 81     // Write ModBus Register

} mysensor_data;


// Type of internal messages (for internal messages)
typedef enum {
	I_BATTERY_LEVEL, I_TIME, I_VERSION, I_ID_REQUEST, I_ID_RESPONSE,
	I_INCLUSION_MODE, I_CONFIG, I_FIND_PARENT, I_FIND_PARENT_RESPONSE,
	I_LOG_MESSAGE, I_CHILDREN, I_SKETCH_NAME, I_SKETCH_VERSION,
	I_REBOOT, I_GATEWAY_READY, I_REQUEST_SIGNING, I_GET_NONCE, I_GET_NONCE_RESPONSE
} mysensor_internal;


// Type of data stream  (for streamed message)
typedef enum {
	ST_FIRMWARE_CONFIG_REQUEST, ST_FIRMWARE_CONFIG_RESPONSE, ST_FIRMWARE_REQUEST, ST_FIRMWARE_RESPONSE,
	ST_SOUND, ST_IMAGE
} mysensor_stream;

typedef enum {
	P_STRING, P_BYTE, P_INT16, P_UINT16, P_LONG32, P_ULONG32, P_CUSTOM, P_FLOAT32
} mysensor_payload;

// Getters/setters for special bit fields in header
#define mSetVersion(_msg,_version) BF_SET(_msg.header.version_length, _version, 0, 2)
#define mGetVersion(_msg) BF_GET(_msg.header.version_length, 0, 2)

#define mSetSigned(_msg,_signed) BF_SET(_msg.header.version_length, _signed, 2, 1)
#define mGetSigned(_msg) BF_GET(_msg.header.version_length, 2, 1)

#define mSetLength(_msg,_length) BF_SET(_msg.header.version_length, _length, 3, 5)
#define mGetLength(_msg) BF_GET(_msg.header.version_length, 3, 5)

#define mSetCommand(_msg,_command) BF_SET(_msg.header.command_ack_payload, _command, 0, 3)
#define mGetCommand(_msg) BF_GET(_msg.header.command_ack_payload, 0, 3)

#define mSetRequestAck(_msg,_rack) BF_SET(_msg.header.command_ack_payload, _rack, 3, 1)
#define mGetRequestAck(_msg) BF_GET(_msg.header.command_ack_payload, 3, 1)

#define mSetAck(_msg,_ackMsg) BF_SET(_msg.header.command_ack_payload, _ackMsg, 4, 1)
#define mGetAck(_msg) BF_GET(_msg.header.command_ack_payload, 4, 1)

#define mSetPayloadType(_msg, _pt) BF_SET(_msg.header.command_ack_payload, _pt, 5, 3)
#define mGetPayloadType(_msg) BF_GET(_msg.header.command_ack_payload, 5, 3)


// internal access for special fields
#define miSetCommand(_command) BF_SET(msg.header.command_ack_payload, _command, 0, 3)
#define miGetCommand() BF_GET(msg.header.command_ack_payload, 0, 3)

#define miSetLength(_length) BF_SET(msg.header.version_length, _length, 3, 5)
#define miGetLength() BF_GET(msg.header.version_length, 3, 5)

#define miSetRequestAck(_rack) BF_SET(msg.header.command_ack_payload, _rack, 3, 1)
#define miGetRequestAck() BF_GET(msg.header.command_ack_payload, 3, 1)

#define miSetAck(_ack) BF_SET(msg.header.command_ack_payload, _ack, 4, 1)
#define miGetAck() BF_GET(msg.header.command_ack_payload, 4, 1)

#define miSetPayloadType(_pt) BF_SET(msg.header.command_ack_payload, _pt, 5, 3)
#define miGetPayloadType() BF_GET(msg.header.command_ack_payload, 5, 3)

#define miSetVersion(_version) BF_SET(msg.header.version_length, _version, 0, 2)
#define miGetVersion() BF_GET(msg.header.version_length, 0, 2)

#define miSetSigned(_signed) BF_SET(msg.header.version_length, _signed, 2, 1)
#define miGetSigned() BF_GET(msg.header.version_length, 2, 1)

#endif
