/*
 * adcFile.h
 *
 *  Created on: May 2, 2024
 *      Author: PDS-User
 */

#ifndef SRC_ADCFILE_H_
#define SRC_ADCFILE_H_

#include "main.h"

void ConfigAdc(void);
void ChannelConfigurationReadyFlag(void);
int ReadAdc(int channel);

#endif /* SRC_ADCFILE_H_ */
