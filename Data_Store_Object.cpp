/*
 * Data_Store_Object.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: simon
 */
#include "Data_Store_Object.h"




	DataStoreObject::DataStoreObject(){
	   	trigger_level = 0;
	   	readable_buffer = buffer1;
	   	writable_buffer = buffer2;
	   	readable = false;
	   	store_state = CHECK_1;
	}

    void DataStoreObject::setTriggerLevel(int16_t level) {
        trigger_level = level;
    }

    void DataStoreObject::updateDataStore(int16_t value){
    	static int16_t index = 0;
    	switch(store_state){

    	case CHECK_1:
    		if(value < trigger_level){
    			writable_buffer[index] = value;
    			store_state = CHECK_2;
    			index = 1;
    		}
    		break;
    	case CHECK_2:
    		if(value < trigger_level){
    			writable_buffer[index] = value;
    			store_state = CHECK_3;
    			index = 2;
    		}
    		break;
    	case CHECK_3:
    		if(value > trigger_level){
    			writable_buffer[index] = value;
    			store_state = CHECK_3;
    			index = 3;
    		}
    		break;
    	case CHECK_4:
    	    if(value > trigger_level){
    	    	writable_buffer[index] = value;
    	    	store_state = RECORDING;
    	    	index = 4;
    	    }
    	    break;
    	case RECORDING:

    		writable_buffer[index] = value;
    		index++;
    		if(index == 100){
    			index=0;
    			store_state = CHECK_1;
    			bufferSwap();
    		}

    	}


    }

    void DataStoreObject::bufferSwap(){
    	if(writable_buffer == buffer1){
    		writable_buffer = buffer2;
    		readable_buffer = buffer1;
    	}
    	else{
    		writable_buffer = buffer1;
    		readable_buffer = buffer2;
    	}
    }

    int16_t* DataStoreObject::getReadBuffer(){
    	 return readable_buffer;
    }






