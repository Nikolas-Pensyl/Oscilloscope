Project = Osciliscope

License in file LICENSE.TXT

Required hardware
================
   Platform = STM32G071 Nucleo board
   
   Wiring 
   		Measure Speed Knob = Quadrature knob pins A & B;
   		A -> PB8 aka Quad_A_PB7_Pin (with pull-up)
   		B -> PB9 aka Quad_A_PB9_Pin (with pull-up)
   		Quadrature common is GROUNDED
   		
   		Threshhold Knob = Quadrature knob pins A & B;
   		A -> PC0 aka Quad_A_PB7_Pin (with pull-up)
   		B -> PC1 aka Quad_A_PB9_Pin (with pull-up)
   		Quadrature common is GROUNDED
   		
   		
Internal - 
	Uses Timer17 to count variable time based on the time the user desires through the Measure Speed Knob, periodically, and uses the ISR
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){...}
	
	Uses Timer16 to count 1ms to intervals to tell both knobs it is ready to read data
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){...}
	
	Uses hadc1 to measure voltage then triggers the conversion callback and reads value
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {...}