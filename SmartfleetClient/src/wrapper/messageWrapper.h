#ifndef SKT_SMARTFLEET_WRAPPER_MESSAGEWRAPPER_H_
#define SKT_SMARTFLEET_WRAPPER_MESSAGEWRAPPER_H_

#include "../util/json/ArduinoJson.h"
#include "../wrapper/Smartfleet_Type.h"
#include "../obj/TripMessage.h"
#include "../obj/RPCMessage.h"

class tripMessage : public tripMessage_Base
{
public:
	tripMessage(){};
	~tripMessage(){};

	/**
	 * @brief Trip JsonObject ����, $tripObj�� Json Object�� �����ؼ� $jsonObj�� �����Ѵ�
	 * @param $jsonObj JsonObject class
	 *        $ts timestamp
	 *        $ty Trip type
	 *        $tripObj Trip object
	 * @return JsonObject$
	 */
	JsonObject& messagePackage(JsonObject& jsonObj, long ts, TripType ty, void* tripObj);
};


class rpcMessage : public RPCMessage_Base
{
public:
	rpcMessage(){};
	~rpcMessage(){};

	/**
	 * @brief RPC Response, Result JsonObject ����, $ty�� ���� �޽��� ����, $addInfoObj�� �����Ͱ� payload�� �߰���
	 * @param $jsonObj JsonObject class
	 *        $code RPC ���� �ڵ�
	 *        $ty RPC type, 0�̸� response object ����
	 *        $addInfoObj RPC body
	 * @return JsonObject$
	 */
	JsonObject& messagePackage(JsonObject& jsonObj, int code, int ty, void* addInfoObj = NULL);
};

extern DeviceActivation g_da;
extern FirmwareUpdate g_fu;
extern FirmwareUpdateChunk g_fuc;
extern char g_buffer[DEFAULT_RPC_BUFFER_SIZE];

class arrivedRPCMessage : public RPCMessage_Base
{
public:
	arrivedRPCMessage(){};
	~arrivedRPCMessage(){};

	/**
	 * @brief RPC �޽����� ������ $jsonObj�� �Ľ� ��  $messageObj�� �� RPC class�� �����Ѵ�.
	 *        $messageObj�� NULL �Է½� extern �Ǿ��ִ� global class�� ���ǵȴ�
	 * @param $jsonObj JsonObject class
	 *        $ty RPC type
	 *        $messageObj RPC Object
	 * @return JsonObject$
	 */
	JsonObject& messageUnpackage(JsonObject& jsonObj, RPCType ty, void* messageObj = NULL);

	/**
	 * @brief RPC �޽����� ���Ž� �޽����� ���Ե� method�� �Ľ� �� ������ ����
	 * @param $jsonObj RPC �޽��� jsonObject
	 *        $mtd method ������
	 * @return request id ptr
	 */
	char* getMethod(JsonObject& jsonObj, char* mtd);

	/**
	 * @brief RPC �޽����� ���Ž� topic�� ���Ե� request id�� �Ľ� �� ������ ����
	 * @param $topic RPC request topic
	 *        $req_id Request ID
	 * @return request id ptr
	 */
	char* getReqId(const char* topic, char* req_id);

};

#endif /* SKT_SMARTFLEET_WRAPPER_MESSAGEWRAPPER_H_ */
