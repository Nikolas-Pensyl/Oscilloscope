/*
 * Data_Store_Object.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: simon
 */


class DataStoreObject{


	DataStoreObject(){
	   	trigger_level = 0;
	   	readable_buffer = buffer1;
	   	writable_buffer = buffer2;
	   	readable = false;
	   	store_state = CHECK_1;
	}

    void setTriggerLevel(int16_t level) {
        trigger_level = level;
    }

    void updateDataStore(int16_t value){
    	static int16_t index = 0;
    	switch(store_state){

    	case CHECK_1:
    		if(value < trigger_level){
    			*writeable_buffer[index] = value;
    			store_state = CHECK_2;
    			index = 1;
    		}
    		break;
    	case CHECK_2:
    		if(value < trigger_level){
    			*writeable_buffer[index] = value;
    			store_state = CHECK_3;
    			index = 2;
    		}
    		break;
    	case CHECK_3:
    		if(value > trigger_level){
    			*writeable_buffer[index] = value;
    			store_state = CHECK_3;
    			index = 3;
    		}
    		break;
    	case CHECK_4:
    	    if(value > trigger_level){
    	    	*writeable_buffer[index] = value;
    	    	store_state = RECORDING;
    	    	index = 4;
    	    }
    	    break;
    	case RECORDING:

    		*writeable_buffer[index] = value;
    		index++;
    		if(index == 100){
    			index=0;
    			store_state = CHECK_1;
    			bufferSwap();
    		}

    	}


    }

    void bufferSwap(){
    	if(writeable_buffer == buffer1){
    		writeable_buffer = buffer2;
    		readable_buffer = buffer1;
    	}
    	else{
    		writeable_buffer = buffer1;
    		readable_buffer = buffer2;
    	}
    }

    int16_t* getReadBuffer(){
    	 return readable_buffer;
    }




};



