#ifndef SMARTFLEET_CLIENT_H_
#define SMARTFLEET_CLIENT_H_

#include <Arduino.h>
#include <stdlib.h>
#include <Stream.h>
#include <Client.h>
#include <Udp.h>

#include "wrapper/Smartfleet_Type.h"

#include "util/json/ArduinoJson.h"
#include "util/ntp/NTPClient.h"

#include "wrapper/MqttWrapper.h"
#include "wrapper/messageWrapper.h"

//Smartfleet Client RETURN CODE
typedef enum SF_ERROR
{
	RET_ERROR_PROCESS_OVERFLOW = -6,
	RET_ERROR_MQTT_CONNECTION_FAIL = 5,
	RET_ERROR_MQTT_SUBSCRIBE_FAIL = -4,
	RET_ERROR_TRIP_SEND_FAIL = -3,
	RET_ERROR_RESPONSE_SEND_FAIL = -2,
	RET_ERROR_RESULT_SEND_FAIL = -1,

	RET_NOT_CONNECTED = 0,
	RET_SUCCESS = 1,
};


//extern global function
extern RPCType _rpc_rsp_type;
extern SMARTFLEET_STATE _state;

extern char g_req_id[64];
extern DeviceActivation g_da;
extern FirmwareUpdate g_fu;
extern FirmwareUpdateChunk g_fuc;


class smartfleet : public NTPClient
{

public:
	smartfleet(Client& client, UDP& udp) : NTPClient(udp, NTP_SERVER, NTP_TIMEZONE_OFFSET, NTP_INTERVAL_MS)
	{
		_client = &client;
		_udp = &udp;	
	}
	~smartfleet(){}

	/**
	 * @brief SmartfleetClient�� MQTT, NTP�� �ֿ� ��ɵ��� non-blocking���� ����
	 *        �����Լ��� �ֱ������� �����ؾ� �Ѵ�.
	 * @param void
	 * @return void
	 */
	void superloop(void);

	/**
	 * @brief SmartfleetClient init �Լ� MQTT, NTP init
	 * @param void
	 * @return void
	 */
	void begin(void);

	/**
	 * @brief RPC �޽��� ���Ž�  user callback�� ����ϴ� �Լ�
	 * @param RPC user callback
	 * @return void
	 */
	void setRPCCallback(onDeviceActivation_cb da = NULL, onFirmwareUpdate_cb fu = NULL, onOBDReset_cb odbr = NULL, onDeviceSerialNumberCheck_cb dsnc = NULL, onClearDeviceData_cb cdd = NULL, onFirmwareUpdateChunk_cb fuc = NULL, onVendorSpecific_cb vs = NULL);

	/**
	 * @brief smartfleet���� MQTT ����, Topic �����ϴ� �Լ�
	 * @param $mqtt_host mqtt host address
	 *        $mqtt_port mqtt port
	 *        $mqtt_username mqtt username
	 *        $mqtt_password mqtt password
	 * @return RET_ERROR_MQTT_CONNECTION_FAIL
	 *         RET_ERROR_MQTT_SUBSCRIBE_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR connectSmartFleet(IPAddress mqtt_ip, const int mqtt_port, const char* mqtt_username, const char* mqtt_password = NULL);
	SF_ERROR connectSmartFleet(const char* mqtt_host, const int mqtt_port, const char* mqtt_username, const char* mqtt_password = NULL);

	/**
	 * @brief MQTT disconnect
	 * @param void
	 * @return RET_ERROR_MQTT_CONNECTION_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR disconnectSmartFleet(void);

	/**
	 * @brief smartfleet ���� ���� Ȯ�� �Լ�
	 * @param void
	 * @return RET_NOT_CONNECTED
	 *         RET_SUCCESS
	 */
	SF_ERROR isMqttConnectStatus(void);


	/**
	 * @brief Trip �޽��� ����
	 * @param �� �޽����� object
	 * @return RET_ERROR_PROCESS_OVERFLOW
	 * 		   RET_ERROR_TRIP_SEND_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR sendTrip(Trip Obj);
	SF_ERROR sendMicroTrip(MicroTrip Obj);
	SF_ERROR sendHFDCapabilityInfomation(HFDCapabilityInfomation Obj);
	SF_ERROR sendDiagnosticInfomation(DiagnosticInfomation Obj);
	SF_ERROR sendDrivingCollisionWarning(DrivingCollisionWarning Obj);
	SF_ERROR sendParkingCollisionWarning(ParkingCollisionWarning Obj);
	SF_ERROR sendBatteryWarning(BatteryWarning Obj);
	SF_ERROR sendUnpluggedWarning(UnpluggedWarning Obj);
	SF_ERROR sendTurnoffWarning(TurnoffWarning Obj);


	/**
	 * @brief Trip �޽��� ����
	 * @param �� �޽����� raw �Ķ����
	 * @return RET_ERROR_PROCESS_OVERFLOW
	 * 		   RET_ERROR_TRIP_SEND_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR sendTrip(int tid, long stt, long edt, int dis, int tdis, int fc, double stlat, double stlon, double edlat, double edlon, int ctp, double coe, int fct, int hsts, int mesp, int idt, double btv, double gnv, int wut, int usm, int est, char* fwv, int dtvt);
	SF_ERROR sendMicroTrip(int tid, int fc, double lat, double lon, int lc, long clt, int cdit, int rpm, int sp, int em, int el, char* xyz, double vv, int tpos);
	SF_ERROR sendHFDCapabilityInfomation(int cm);
	SF_ERROR sendDiagnosticInfomation(int tid, char* dtcc, int dtck, int dtcs);
	SF_ERROR sendDrivingCollisionWarning(int tid, double dclat, double dclon);
	SF_ERROR sendParkingCollisionWarning(double pclat, double pclon);
	SF_ERROR sendBatteryWarning(int wbv);
	SF_ERROR sendUnpluggedWarning(int unpt, int pt);
	SF_ERROR sendTurnoffWarning(char* rs);


	/**
	 * @brief RPC ��û ���Ž� ���� callback�� ����
	 * @param void
	 * @return true(RPC ���Ž�)
	 *         false(�� ���Ž�)
	 */
	int isRPCMessage(void);


	/**
	 * @brief RPC ��û�� ó���ϰ� ��û�� ���� result ����
	 * @param $topic result topic
	 * @return RET_ERROR_PROCESS_OVERFLOW
	 * 		   RET_ERROR_RESULT_SEND_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR resultDeviceActivation(const char* vid, const char* topic);
	SF_ERROR resultFirmwareUpdate(const char* topic);
	SF_ERROR resultOBDReset(const char* topic);
	SF_ERROR resultDeviceSerialNumberCheck(const char* sn, const char* topic);
	SF_ERROR resultClearDeviceData(const char* topic);
	SF_ERROR resultFirmwareUpdateChunk(const char* topic);

private:

	/**
	 * @brief RPC ��û�� MQTT Client�� ����  callback�Ǵ� �Լ�, MQTT �޽��� �Ľ�
	 * @param $topic mqtt topic
	 *        $payload mqtt payload
	 * @return void
	 */
	static void onRPCMessageArrived(const char* topic, const char* payload);

	/**
	 * @brief RPC ��û�� ����  response ����
	 * @param $topic response topic
	 * @return RET_ERROR_PROCESS_OVERFLOW
	 * 		   RET_ERROR_RESPONSE_SEND_FAIL
	 *         RET_SUCCESS
	 */
	SF_ERROR responseDeviceActivation(const char* topic);
	SF_ERROR responseFirmwareUpdate(const char* topic);
	SF_ERROR responseOBDReset(const char* topic);
	SF_ERROR responseDeviceSerialNumberCheck(const char* topic);
	SF_ERROR responseClearDeviceData(const char* topic);
	SF_ERROR responseFirmwareUpdateChunk(const char* topic);


	/**
	 * @brief smartfleetClient�� ���¸� ��ȸ
	 * @param void
	 * @return STATE_NOTCONNECTED
	 * 		   STATE_IDLE
	 * 		   STATE_DOWORK
	 */
	static SMARTFLEET_STATE getCurrentState(void);

	/**
	 * @brief smartfleetClient�� ���°� STATE_IDLE ���� �Ǵ�
	 * @param void
	 * @return false
	 * 		   true
	 */
	static bool isIdleState(void);

	/**
	 * @brief smartfleetClient�� ���°� STATE_IDLE ���� �Ǵ�
	 * @param void
	 * @return false
	 * 		   true
	 */
	static void setRPCType(RPCType type);

	/**
	 * @brief ������ RPC�� Ÿ���� ����
	 * @param void
	 * @return $RPCType
	 */
	static RPCType getRPCType(void);

	/**
	 * @brief smartfleetClient�� ���¸� STATE_DOWORK
	 * @param void
	 * @return false
	 * 		   true
	 */
	static bool startWork(void);

	/**
	 * @brief smartfleetClient�� ���¸� STATE_IDLE
	 * @param void
	 * @return false
	 * 		   true
	 */
	static bool endWork(void);
	
	Client* _client;
	UDP* _udp;
	mqttWrapper _mqttwrapper;
};

#endif /* SMARTFLEET_CLIENT_H_ */
