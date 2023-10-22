/*
 * Data_Store_Object.h
 *
 *  Created on: Oct 9, 2023
 *      Author: simon
 */

#ifndef OSCILLOSCOPE_DATA_STORE_OBJECT_H_
#define OSCILLOSCOPE_DATA_STORE_OBJECT_H_

#include "main.h"
#include "KnobFSM.h"
#include "Sample_clock.h"
#include "Singledigitcounter.h"
#include "Display.h"
#include "Sean_queue.h"
#include "Adc_to_Vert.h"


class DataStoreObject {
	private:

	    int16_t buffer1[100];
	    int16_t buffer2[100];

	    int16_t trigger_level;

	    int16_t* readable_buffer;
	    int16_t* writable_buffer;

	    bool readable;


	public:
	    DataStoreObject();
	    void updateDataStore(int16_t value);
	    void setTriggerLevel(int16_t level);
	    void bufferSwap();
	    int16_t* getReadBuffer();
	    enum storeState{CHECK_1,CHECK_2,CHECK_3,CHECK_4,RECORDING};
	    enum storeState store_state;

};


#endif /* OSCILLOSCOPE_DATA_STORE_OBJECT_H_ */