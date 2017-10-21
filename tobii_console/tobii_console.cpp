#include "stdafx.h"	

#include <tobii/tobii.h>
#include <assert.h>
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

	error = tobii_device_destroy(device);
	assert(error == TOBII_ERROR_NO_ERROR);

	error = tobii_api_destroy(api);
	assert(error == TOBII_ERROR_NO_ERROR);
	return 0;
}

