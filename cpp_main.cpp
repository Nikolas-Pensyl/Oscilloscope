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
///////////////// Debugging code depository //////////////
// int16_t debug_mailbox = -1;
//////////////////////////////////////////////////////////

/*** Declare a queue, globally since it is IPC ...                                                      ***/
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

Sean_queue buffer_finished;

Sean_queue adc_raw_queue;
Sean_queue pixel_vertical_queue;

Sean_queue adc_speed_queue_in;
Sean_queue adc_threshhold_queue_in;

Sean_queue adc_speed_queue_out;
Sean_queue adc_threshhold_queue_out;


Knob_FSM adc_speed_knob(&adc_speed_queue_out, &adc_speed_queue_in, GPIOB, ADC_Speed_Knob_A_Pin, GPIOB, ADC_Speed_Knob_B_Pin);
DoubleDigitCounter speed_counter(&adc_speed_queue_out);

/*****************************************************************************/

/* USER CODE BEGIN 0 */
/******************************************************************************/
/**************** Really by Sean **********************************************/
/*** The only ISR I need -- a periodic timer handler.                       ***/
/*** Prototype HAL_TIM_ (etc) is fixed in the HAL framework.                ***/
/*** See the file Drivers/STM32G0xx_HAL_Driver/stm32g0xx_hal_tim.c          ***/
/*** and in particular, search for void TIM_ResetCallback(, which lists     ***/
/*** the HAL ISR "callback" functions.                                      ***/
/*** You can instead use avoid using HAL (an "advanced" option in the       ***/
/*** CubeMX tab = Project Manager ... choose TIM17 = LL instead of HAL),    ***/
/*** and then you must define void TIM17_IRQHandler (void) as declared in   ***/
/*** the auto-generated file stm32g0xx_it.c -- trickier, and quicker!       ***/
extern ADC_HandleTypeDef hadc1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		if(htim->Instance == TIM16) {
			adc_speed_queue_in.enqueue(1);
			adc_threshhold_queue_in.enqueue(1);
		}
		else if(htim->Instance == TIM17) {
			HAL_TIM_Base_Stop(&htim17);
			__HAL_TIM_SET_AUTORELOAD(&htim17, (speed_counter.count()+1)*500); //Count 0-99 so timer period is 1-100 us
			HAL_TIM_Base_Start_IT(&htim17);
			HAL_ADC_Start_IT(&hadc1);
		}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adc_raw_queue.enqueue(HAL_ADC_GetValue(hadc));
	HAL_ADC_Stop(&hadc1);
}

/********************* Keep everything in C++-language from this pt. ***/
void do_cpp_loop()
{
	// INITIALIZE OBJECTS OF INTEREST
	// Students - this is the "Dramatis Personae" section, before the loop


	// THE QUADRATURE-ENCODED KNOB - knob moving CW or CCW, or still.
	// Needs to know the input pins.
	// Needs an input queue to hold a sampling command, to be served ASAP.
	// Needs an output  queue to hold its interpretation. WHY? The input is
	// controlled by real-time sampling, not by function calls -- so it is an
	// async function. The UPDATE method here will often do nothing, because
	// no action will be taken until its input queue holds a TICK from the
	// sample-clock.
	// ** Educational note: For SWE. In JavaScript for web development,
	// ** you may see this idea called ASYNC. A function in Javascript can be
	// ** declared ASYNC, which means that the function might need to wait
	// ** before it can return. The JavaScript interpreter should run try to
	// ** run this function to see if it can return, but if this function
	// ** cannot return, then the interpreter should move on to other
	// ** threads/tasks.
	// ** An ASYNC function usually has a lot of normal code, and one (or a
	// ** few) line(s) marked with AWAIT, which means that the line can "block."
	// ** These are the places that the JavaScript interpreter should be ready to
	// ** bookmark and leave if need be -- moving on to come back to later.
	// ** The quad-knob update() function does not have an interpreter, so it
	// ** uses a QUEUE in place of AWAIT. The get-TICK-from-sample-clock is
	// ** AWAITED = is a DEQUEUE MESSAGE line. If the input queue is EMPTY,
	// ** then the "promise" from the ISR has not yet been fulfilled, and the
	// ** C++ program should move on to let other functions run; of course we use
	// ** an endless while loop to come back to catch up on the needed work when
	// ** the promise will be fulfilled, later. If the promise is fulfilled, then
	// ** the sample-clock queue holds a TICK message, so the quad_knob update
	// ** will complete and finish its job.
	// ** As a second example -- the completed quad_knob update enqueues a CW/CCW
	// ** message in an output queue -- so an AYSNC function can call AWAIT quad-knob,
	// ** just as quad-knob awaited the sample clock.
	// ** The ASYNC function in this case is the COUNTER update... more on that below.
	// ** It will await the decision CW, CCW, No-Movement.
	// **
	// Educational note 2: Students - this object has sub-objects --  a Debouncer for
	// ** each pin. This is normal C++ -- nothing weird.
	// **
	// INITIALIZATION -- GPIO port pointer and GPIO pin IDs for the quadrature
	// logic signals, and a pointer to the queue for output.
	//Knob_FSM knob1(&q_user_command, &q_get_data_asap, GPIOB, Quad_A_PB7_Pin, GPIOB, Quad_B_PB9_Pin);

	DataStoreObject adc_data(&buffer_finished);


	Display DOG(&hspi1, &adc_data, &buffer_finished);
	DOG.init();
	DOG.clearScreen();



	Knob_FSM adc_threshhold_knob(&adc_threshhold_queue_out, &adc_threshhold_queue_in, GPIOC, Threshhold_Knob_A_Pin, GPIOC, Threshhold_Knob_B_Pin);


	DoubleDigitCounter threshhold_counter(&adc_threshhold_queue_out);


	int16_t adc_data_int;

	__HAL_RCC_ADC_CLK_ENABLE();
	HAL_TIM_Base_Start_IT(&htim16);
	__HAL_TIM_SET_AUTORELOAD(&htim17, (speed_counter.count()+1)*500); //Count 0-99 so timer period is 1-100 us
	HAL_TIM_Base_Start_IT(&htim17);

	init_mem_barrier();


	while(1){

		//For Release
		//if(!getRamHealth()) { reboot(); }


		//For Debugging purposes
		if(!getRamHealth()) { while(1) {} }

		adc_data.setTriggerLevel(threshhold_counter.count()*THRESHHOLD_MULTIPLIER);

		// Second - run the input driver. This awaits the sample-clock TICK.
		// Often calling this accomplishes nothing, but at the chosen
		// times, it cause a debouncer to read the knob pins and decide
		// if there is a twist in progress.
		//knob1.update();
		if(adc_raw_queue.dequeue(&adc_data_int)) {
			adc_data.updateDataStore(adc_data_int);
		}
		// Third - run the counter. This awaits the knob's sampled
		// and decoded input. This call often does nothing - since
		// the user rarely turns the knob, it REALLY rarely does
		// anything. It acts like a butler that waits in case you
		// choose to issue any orders. If there is a CW or CCW
		// command, then we increase or decrease the count.

		adc_speed_knob.update();
		adc_threshhold_knob.update();


		threshhold_counter.update();
		speed_counter.update();


		DOG.update();
		//DOG.drawDiag();
		/*
		user_count.update();
		if(q_ms.getUseCount()>0) {
			user_count.update();
		}*/

		////////////////////////////////////////////////////////////////////
		// DONE: Let the counter work by OMITTING THIS SECTION
		// PURPOSE: overriding the display to show an error-code

		//user_count.debug_setter(debug_mailbox);
		////////////////////////////////////////////////////////////////////

		// Fourth - run the display. For a seven-seg, we'll just assert all segments.
		// This object was constructed to serve the count, so it has a pointer
		// to that as an attribute, and will use that to collect a value.
		// You CAN use a queue for this transfer of data, but I wanted to
		// illustrate a SYNCHRONOUS transfer, and this is such a demo: the display
		// demands an immediate reply from the counter using the normal C flow-
		// of-control via a function call.
		//display.update();
	}
}
