Project = Quad_knob_counter

License in file LICENSE.TXT

Required hardware
================
   Platform = STM32G071 Nucleo board
   
   Wiring 
   		Input device 1 = Quadrature knob pins A & B;
   		A -> PB7 aka Quad_A_PB7_Pin (with pull-up)
   		B -> PB9 aka Quad_A_PB9_Pin (with pull-up)
   		Quadrature common is GROUNDED
   		
   		Output Device 1 = Seven segment display
   		Segments
   		A -> UserPC0_Pin
   		B -> UserPC1_Pin
   		C -> UserPC2_Pin
   		D -> UserPC3_Pin
   		E -> UserPC4_Pin
   		F -> UserPC5_Pin
   		G -> UserPC6_Pin
Internal - 
	Uses Timer17 to count 1 ms, periodically, and uses the ISR
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim17){...}