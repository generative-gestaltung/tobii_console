#include "stdafx.h"	

#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <assert.h>
#include <stdio.h>
#include <stdio.h>

	int main()
	{
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
			(void)user_data; // Unused parameter
			if (gaze_point->validity == TOBII_VALIDITY_VALID)
				printf ("Gaze point: %g %g %g\n", gaze_point->timestamp_us, gaze_point->position_xy[0], gaze_point->position_xy[1]);
			else
				printf ("Gaze point: %g INVALID", gaze_point->position_xy);
		}, 0);

		if (error != TOBII_ERROR_NO_ERROR)
		{
			printf("Failed to subscribe to gaze stream.\n");
			tobii_device_destroy(device);
			tobii_api_destroy(api);
			return 1;
		}

		
		error = tobii_wait_for_callbacks(device);
		if (error != TOBII_ERROR_NO_ERROR) {
			printf("error wait for callbacks\n");
		}

		printf("wait ok\n");
		error = tobii_process_callbacks(device);
		if (error != TOBII_ERROR_NO_ERROR) {
			printf("error process callbacks\n");
		}

		printf("process ok\n");

		while (true) {

		}




		error = tobii_device_destroy(device);
		assert(error == TOBII_ERROR_NO_ERROR);

		error = tobii_api_destroy(api);
		assert(error == TOBII_ERROR_NO_ERROR);
		return 0;
	}
