// Status -- 8/18/2023 -- Builds
//  Hard Fault at >> display.update()

/***************** From scratch by Sean ****************************************/
#include "main.h"
#include "../Inc/cpp_main.h"
#include "KnobFSM.h"
#include "Sample_clock.h"
#include "DoubleDigitCounter.h"
#include "Sean_queue.h"
#include "Adc_to_Vert.h"
#include "RamHealth.h"
#include "Data_Store_Object.h"
#include "Display.h"

#define THRESHHOLD_MULTIPLIER 41
#define SPEED_MULTIPLIER 5
///////////////// Debugging code depository //////////////
// int16_t debug_mailbox = -1;
//////////////////////////////////////////////////////////

/*** Declare a queue, globally since it is IPC ...                                                      ***/
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

//IPCs
Sean_queue buffer_finished;

Sean_queue adc_raw_queue;

Sean_queue adc_speed_queue_in;
Sean_queue adc_threshhold_queue_in;

Sean_queue adc_speed_queue_out;
Sean_queue adc_threshhold_queue_out;

bool start_adc = false;




/*****************************************************************************/

/* USER CODE BEGIN 0 */
extern ADC_HandleTypeDef hadc1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

		if(htim->Instance == TIM16) {
			//Time to read the knob data
			adc_speed_queue_in.enqueue(1);
			adc_threshhold_queue_in.enqueue(1);
		}
		else if(htim->Instance == TIM17) {
			//time to start an adc conversion
			start_adc = true;
		}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	//ADC conversion complete enqueue value read
	adc_raw_queue.enqueue(HAL_ADC_GetValue(hadc));
}

/********************* Keep everything in C++-language from this pt. ***/
void do_cpp_loop()
{
	// INITIALIZE OBJECTS OF INTEREST

	//Init Buffer
	DataStoreObject adc_data(&buffer_finished);

	//Init Display
	Display DOG(&hspi1, &adc_data);
	DOG.init();
	DOG.clearScreen();


	//Initialize Knobs
	Knob_FSM adc_threshhold_knob(&adc_threshhold_queue_out, &adc_threshhold_queue_in, GPIOC, Threshhold_Knob_A_Pin, GPIOC, Threshhold_Knob_B_Pin);
	Knob_FSM adc_speed_knob(&adc_speed_queue_out, &adc_speed_queue_in, GPIOB, ADC_Speed_Knob_A_Pin, GPIOB, ADC_Speed_Knob_B_Pin);

	//Initialize counters
	DoubleDigitCounter threshhold_counter(&adc_threshhold_queue_out);
	DoubleDigitCounter speed_counter(&adc_speed_queue_out);

	//
	int16_t adc_data_int;
	int16_t adc_period = speed_counter.count();
	int16_t buffer_finished_data;

	//Start/enable hardware timers
	__HAL_RCC_ADC_CLK_ENABLE();
	HAL_TIM_Base_Start_IT(&htim16);
	__HAL_TIM_SET_AUTORELOAD(&htim17, (speed_counter.count()+1)*SPEED_MULTIPLIER); //Count 0-99 so timer period is 1-100 us
	HAL_TIM_Base_Start_IT(&htim17);


	init_mem_barrier();


	while(1){

		//For Release
		//if(!getRamHealth()) { reboot(); }


		//For Debugging purposes
		if(!getRamHealth()) { while(1) {} }

		//If start_ADC flag start the adc conversion then the adc_callback will run once complete
		if(start_adc) {
			start_adc=false;
			HAL_ADC_Start_IT(&hadc1);
		}

		//Set trigger threshold for the adc reader to start reading data
		adc_data.setTriggerLevel(threshhold_counter.count()*THRESHHOLD_MULTIPLIER);

		//Check if there is data from the adc to process
		if(adc_raw_queue.dequeue(&adc_data_int)) {
			//process the new data accordingly
			adc_data.updateDataStore(adc_data_int);
		}


		//Knob check to see if either knob was turned in any direction
		adc_speed_knob.update();
		adc_threshhold_knob.update();

		//If a knob was turned data was enqueued for the counters to dequeue
		//Then increment or decrement accordingly
		threshhold_counter.update();
		speed_counter.update();

		//If message from buffer saying data read complete update display
		if(buffer_finished.dequeue(&buffer_finished_data))  {
			//Display function is too slow on its own so stop reading from adc while displaying
			HAL_TIM_Base_Stop(&htim17);

			//Update Display
			DOG.update();

			//Once Display is done start the timer to start triggering adc conversions.
			HAL_TIM_Base_Start_IT(&htim17);
		}


		//If speed_counter is not up to date
		if(adc_period!=speed_counter.count()) {
			//Stop the timer so we can update the value of top
			HAL_TIM_Base_Stop(&htim17);

			//update the value to our variable
			adc_period = speed_counter.count();

			//Reload top into our timer with new value
			__HAL_TIM_SET_AUTORELOAD(&htim17, (adc_period+1)*SPEED_MULTIPLIER); //Count 0-99 so timer period is 1-100 us

			//Restart the timer once complete
			HAL_TIM_Base_Start_IT(&htim17);
		}
	}
}
