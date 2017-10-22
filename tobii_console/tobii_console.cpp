#include "stdafx.h"	

#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

#define WITH_TOBII 0
#define WITH_SOCKETS 1


char buf[256];
SOCKET sock;
SOCKADDR_IN remote_addr;
int addr_len = sizeof(SOCKADDR_IN);


int main()
{

#if WITH_TOBII
	tobii_api_t* api;
	tobii_error_t error = tobii_api_create(&api, NULL, NULL);
	assert(error == TOBII_ERROR_NO_ERROR);

	tobii_device_t* device;
	error = tobii_device_create(api, NULL, &device);
	assert(error == TOBII_ERROR_NO_ERROR);

	tobii_device_info_t info;
	error = tobii_get_device_info(device, &info);
	assert(error == TOBII_ERROR_NO_ERROR);

	printf("Serial number: %s\n", info.serial_number);


	error = tobii_gaze_point_subscribe(device,
		[](tobii_gaze_point_t const* gaze_point, void* user_data)
	{

		int x = 0;
		int y = 0;

		(void)user_data; // Unused parameter
		if (gaze_point->validity == TOBII_VALIDITY_VALID) {
			printf("Gaze point: %g %g %g\n", gaze_point->timestamp_us, gaze_point->position_xy[0], gaze_point->position_xy[1]);
		}
		else {
			printf("Gaze point: %g INVALID", gaze_point->position_xy);
		}
		
		if (sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&remote_addr, addr_len) == SOCKET_ERROR) {
			printf("send failed\n");
		}

		sprintf(buf, "%d\n%d", x, y);
		int ret = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)remote_addr, addr_len);
		if (ret == SOCKET_ERROR) {
			printf("sock send error\n");
		}
	}, 0);

	if (error != TOBII_ERROR_NO_ERROR)
	{
		printf("Failed to subscribe to gaze stream.\n");
		tobii_device_destroy(device);
		tobii_api_destroy(api);
		return 1;
	}
#endif

#if WITH_SOCKETS
	


	WSADATA wsa;
	long rc = WSAStartup(MAKEWORD(2, 0), &wsa);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sock != INVALID_SOCKET);

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(8888);

	

	printf("socket ok\n");
#endif
		
		
		while (true) {
			
#if WITH_TOBII
			error = tobii_wait_for_callbacks(device);
			if (error != TOBII_ERROR_NO_ERROR) {
				printf("error wait for callbacks\n");
			}

			printf("wait ok\n");
			error = tobii_process_callbacks(device);
			if (error != TOBII_ERROR_NO_ERROR) {
				printf("error process callbacks\n");
			}

			int64_t timestamp;
			error = tobii_system_clock(api, &timestamp);
			printf("time: %d\n", timestamp);
#endif
		}



#if WITH_TOBI
		error = tobii_device_destroy(device);
		assert(error == TOBII_ERROR_NO_ERROR);

		error = tobii_api_destroy(api);
		assert(error == TOBII_ERROR_NO_ERROR);
#endif
		return 0;
	}