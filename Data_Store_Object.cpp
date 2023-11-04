/*
 * Data_Store_Object.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: simon
 */
#include "Data_Store_Object.h"




	DataStoreObject::DataStoreObject(Sean_queue *buff_finished){
	   	trigger_level = 0;
	   	readable_buffer = buffer1;
	   	writable_buffer = buffer2;
	   	store_state = CHECK_1;
	   	this->buffer_finished = buff_finished;
	}

    void DataStoreObject::setTriggerLevel(int16_t level) {
        trigger_level = level;
    }

    void DataStoreObject::updateDataStore(int16_t value){
    	static int16_t index = 0;

    	switch(store_state){
    	//raw_To_Vert(int16_t raw, int16_t* pixel)
    	case CHECK_1:
    		if(value <= trigger_level){
    			raw_To_Vert(value, &writable_buffer[0]);
    			store_state = CHECK_2;
    		}
    		break;
    	case CHECK_2:
    		if(value <= trigger_level){
    			raw_To_Vert(value, &writable_buffer[1]);
    			store_state = CHECK_3;
    		}
    		else{
    			store_state = CHECK_1;
    		}
    		break;
    	case CHECK_3:
    		if(value >= trigger_level){
    			raw_To_Vert(value, &writable_buffer[2]);
    			store_state = CHECK_4;
    		}
    		else{
    			writable_buffer[0] = writable_buffer[1];
    		    raw_To_Vert(value, &writable_buffer[1]);
    		}

    		break;
    	case CHECK_4:
    	    if(value >= trigger_level){
    	    	raw_To_Vert(value, &writable_buffer[3]);
    	    	store_state = RECORDING;
    	    	index = 4;
    	    }
    	    else{
    	    	raw_To_Vert(value, &writable_buffer[0]);
    	    	store_state = CHECK_2;
    	    }
    	    break;
    	case RECORDING:

    		raw_To_Vert(value, &writable_buffer[index]);
    		index++;
    		if(index == 100){
    			index=0;
    			store_state = CHECK_1;
    			bufferSwap();
    		}

    	}


    }

    void DataStoreObject::bufferSwap(){
    	if (display_is_reading){
    		return;
    	}

    	if(writable_buffer == buffer1){
    		writable_buffer = buffer2;
    		readable_buffer = buffer1;
    	}
    	else{
    		writable_buffer = buffer1;
    		readable_buffer = buffer2;
    	}
    	buffer_finished->enqueue(1);
    }

    int16_t* DataStoreObject::getReadBuffer(){
    	 return readable_buffer;
    }






