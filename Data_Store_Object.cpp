/*
 * Data_Store_Object.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: simon
 */

class DataStoreObject {
	private:
		// Buffer arrays with 100 16-bit signed integers each
	    int16_t buffer1[100];
	    int16_t buffer2[100];

	    // Trigger level attribute
	    int16_t trigger_level;

	    // Readable and writable buffer pointers
	    int16_t* readable_buffer;
	    int16_t* writable_buffer;



	public:
		DataStore() {

		}

};



