/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <math.h>
#include <math.h>
#include <stdlib.h>

/* Ultrasonic Sensors Variables */
int timer1_count = 0; float dist_measured_1 = 0;
int timer2_count = 0; float dist_measured_2 = 0;
int uss_1_trig = 1; int uss_2_trig = 0;
int print_count = 0; int measure_timer_flag = 0;

/* State Machine Variables */
int stop_robot = 0;   char msg[20]; int next_state = 1;
int zone[4] = {0}; int zones_visited = 0;

/* Navigation Variables */
int counter_master = 0; int counter_slave = 0; 
int counter_turn = 0;
int robot_stop = 0;

/* Color Sensor Variables */
int compare_ready = 0; int color_result = 0;
int counter_rgb[4] = {0}; int idle_state = 0;



/******* --------- Functions --------- *******/
/* --- Sub-system: Distance Measurement --- */
void Ultrasonic_Sensor_1() 
{
    Trig_Control_Write(0);
    Echo_Control_Write(0);
    
    Trigger_1_Write(1);
    CyDelayUs(10);
    Trigger_1_Write(0);
    
}

void Ultrasonic_Sensor_2() 
{
    Trig_Control_Write(1);
    Echo_Control_Write(1);
    
    Trigger_2_Write(1);
    CyDelayUs(10);
    Trigger_2_Write(0);
}

void Ultrasonic_Sensor_Start() 
{
    Trigger_Timer_Start(); Measure_Timer_Start();
    
}

void Ultrasonic_Sensor_Stop()
{
    Trigger_Timer_Stop(); Measure_Timer_Stop();
    print_count = 0;
}

int Ultrasonic_Measure(int dist, int scan) 
{
    int stop_measurement = 0; int zone = 0;
    Measure_Timer_Start();
    
    while (stop_measurement == 0) {
        Ultrasonic_Sensor_1();
        CyDelay(scan);  // Read once every [scan] ms
        
        
        if (measure_timer_flag == 1) {
            Measure_Timer_ReadStatusRegister();
    
            timer1_count = Measure_Timer_ReadCounter();
            dist_measured_1 = (65536 - timer1_count)/ 58;        // Dist measured in cm
    
            sprintf(msg, "\nDist: %d.%02d\n",(int)dist_measured_1, ((int)(dist_measured_1 * 100.0 + 0.5))%100);
            UART_PutString(msg);
    
            if (dist_measured_1 <= dist) {
                LED_Pin_Write(1);
                
                zone = 5;
            }
            else if (dist_measured_1 >= dist) {
                stop_measurement = 0;
                LED_Pin_Write(0);
                
                zone = 7;
            }
            measure_timer_flag = 0;
        }
        
    }
    return zone;
}

/* --- Sub-system: Navigation --- */

void Robot_Stop(int PWM_master, double PWM_slave) 
{
    MOTOR_1_IN_1_Write(0);      // stop
    MOTOR_1_IN_2_Write(0);
    PWM_Motor_1_WriteCompare(PWM_master); //PWM value for left wheel
  
    MOTOR_2_IN_3_Write(0);      //stop
    MOTOR_2_IN_4_Write(0);
    PWM_Motor_2_WriteCompare(PWM_slave); //PWM value for right wheel 
}

void Robot_Move(int direction, int PWM_master, double Kp, int ir, int counter_val , int uss, int limit_switch) 
{
    int n = 0; int PWM_slave = PWM_master;
    robot_stop = 0;
    int error = 0;

    MOTOR_1_IN_1_Write(direction); // forward
    MOTOR_1_IN_2_Write(~direction);
    PWM_Motor_1_WriteCompare(PWM_master); //PWM value for left wheel
  
    MOTOR_2_IN_3_Write(direction); //forward
    MOTOR_2_IN_4_Write(~direction);
    PWM_Motor_2_WriteCompare(PWM_slave); //PWM value for right wheel 

    while(robot_stop == 0){
        counter_master = QuadDec_Motor_1_GetCounter();
        counter_slave = QuadDec_Motor_2_GetCounter();
        
        error = counter_master - counter_slave;
        error = abs(error);
        
        //sprintf(msg, "\nCounter master: %d\n Counter slave: %d\n", counter_master, counter_slave);
        //UART_PutString(msg);
        /*
        if ( error > 0 )    // master is faster
        {
            PWM_slave = PWM_slave + 50;
            PWM_Motor_2_WriteCompare(PWM_slave);
        }
        else if ( error < 0 )    // slave is faster
        {
            PWM_slave = PWM_slave - 50;
            PWM_Motor_2_WriteCompare(PWM_slave);
        }
        */
        sprintf(msg, "\nError: %d\n", error);
        UART_PutString(msg);
        //if ( error > 250) 
        //{ 
            //sprintf(msg, "\nError in direction 1: %d\n", error);
            //UART_PutString(msg);
            //PWM_slave = error *Kp + PWM_master;
            if(direction == 1)
            {
            PWM_slave = (counter_master - counter_slave) *Kp + PWM_master; 
            }
            else if (direction == 0)
            {
            PWM_slave = (counter_slave - counter_master) *Kp + PWM_master;  
            }
            //PWM_Motor_1_WriteCompare(PWM_master);
            PWM_Motor_2_WriteCompare(PWM_slave);
        //}

        sprintf(msg, "\nPWM master: %d\n PWM slave: %d\n", PWM_master, PWM_slave);
        UART_PutString(msg);
        /*
        else if ( (error > 280) && direction == 0) 
        { 
            //sprintf(msg, "\nError in direction 0: %d\n", error);
            //UART_PutString(msg);
            PWM_slave = error *Kp + PWM_master;
            //PWM_slave = -(counter_master + counter_slave) *Kp + PWM_master; 

            //PWM_Motor_1_WriteCompare(PWM_master);
            PWM_Motor_2_WriteCompare(PWM_slave);
        }
        */

        if (ir == 1){
            if (IR_Sensor_Read() == 0) {
                sprintf(msg, "IR Sensor Detected.\n");
                UART_PutString(msg);
                robot_stop = 1;
            }
        }
        /*
        else if (counter_val > 0) {
            if (n >= counter_val) {
                n = 0;
                robot_stop = 1;
            }
        }
        */
        else if (counter_val > 0) {
            if ( (counter_master >= counter_val) && (direction == 1) ) {
                n = 0;
                robot_stop = 1;
            }
            else if ( (counter_master <= -counter_val) && (direction == 0) ) {
                n = 0;
                robot_stop = 1;
            }
        }
        else if (uss > 0) {
            if ( Ultrasonic_Measure(uss, 100) == 1 ) {robot_stop = 1;}
            /*
            if ( dist_measured_1 || dist_measured_2 == uss ) {
                if ((0.9 <= dist_measured_1/dist_measured_2) || ( dist_measured_1/dist_measured_2<= 1.1)) {
                    robot_stop = 1;
                    Ultrasonic_Sensor_Stop();
                }
            }
            */
        }
        else if(limit_switch == 1){
            if (Limit_Switch_L_Read() == 0  && Limit_Switch_R_Read() == 0) {     // stop when aligned
                CyDelay(500);
                robot_stop = 1;
        
                LED_Pin_Write(1);
                //sprintf(msg, "Limit Switch Triggered \n");
                //UART_PutString(msg);
            }
        }
        n++;
    }
    Robot_Stop(0, 0);
    
    QuadDec_Motor_1_SetCounter(0);
    QuadDec_Motor_2_SetCounter(0);

    sprintf(msg, "Move straight COMPLETE.\n");
    UART_PutString(msg);
}

void Robot_Turn(int direction, int PWM_master, int right_count, int left_count) 
{
    QuadDec_Motor_1_SetCounter(0);
    QuadDec_Motor_2_SetCounter(0);

    MOTOR_1_IN_1_Write(direction); // forward
    MOTOR_1_IN_2_Write(~direction);
    PWM_Motor_1_WriteCompare(PWM_master);
    
    MOTOR_2_IN_3_Write(~direction);
    MOTOR_2_IN_4_Write(direction);
    PWM_Motor_2_WriteCompare(PWM_master);
    
    if (direction == 1) {    // right turn
        while ( counter_turn < right_count ) {   // 4100
            counter_turn = QuadDec_Motor_1_GetCounter();
            if ( counter_turn >= right_count )
            {
                QuadDec_Motor_1_SetCounter(0); // Resting to 0 
                QuadDec_Motor_2_SetCounter(0); // Resting to 0
                Robot_Stop(0, 0);
                
            }
        }
    }
    
    else if (direction == 0) {      // left turn
        while ( counter_turn > -left_count ) {  // -3850
            counter_turn = QuadDec_Motor_1_GetCounter();
            if ( counter_turn <= -left_count )
            {
                QuadDec_Motor_1_SetCounter(0); // Resting to 0 
                QuadDec_Motor_2_SetCounter(0); // Resting to 0
                Robot_Stop(0, 0);
                
            }
        }
    }    
    counter_turn = 0;
    sprintf(msg, "Turn COMPLETE. \n");
    UART_PutString(msg);
    //CyDelay(100);
}

int Robot_Orientation() {
    Ultrasonic_Sensor_Start();
    while ( (dist_measured_1 != dist_measured_2) && dist_measured_1 >= 0) {
        if (dist_measured_1 > dist_measured_2) {    // if left > right
            Robot_Turn(1, 200, 10, 0);
            
            Ultrasonic_Sensor_Stop();
            return 1;
        }
        if (dist_measured_1 < dist_measured_2) {    // if right > left
            Robot_Turn(0, 200, 0, 10);
            
            Ultrasonic_Sensor_Stop();
            return 1;
        }
    }
    
    return 0;
}

int Robot_Align_LS() {
    robot_stop = 0;
    
    MOTOR_1_IN_1_Write(0); // forward
    MOTOR_1_IN_2_Write(1);
    PWM_Motor_1_WriteCompare(200); //PWM value for left wheel
  
    MOTOR_2_IN_3_Write(0); //forward
    MOTOR_2_IN_4_Write(1);
    PWM_Motor_2_WriteCompare(200); //PWM value for right wheel 
    
    while (robot_stop == 0) {
        if (Limit_Switch_L_Read() == 0  && Limit_Switch_R_Read() == 0) {     // stop when aligned
            robot_stop = 1;
            Robot_Stop(0, 0);
        
            LED_Pin_Write(1);
            sprintf(msg, "Limit Switch Triggered \n");
            UART_PutString(msg);
        }
        else {LED_Pin_Write(0);}
    }
    
    return 1;
}

/* --- Sub-system: Color Sensing --- */

void Freq_Scaling(int percent)
{
    if (percent == 0){          // Low Power
        S0_Write(0);
        S1_Write(0);
    }
    else if (percent == 2){     // 2%
        S0_Write(0);
        S1_Write(1);
    }
    else if (percent == 20){    // 20%
        S0_Write(1);
        S1_Write(0);
    }
    else if (percent == 100){   // 100%
        S0_Write(1);
        S1_Write(1);
    }

}

void RGB_Filter(char color[20])
{
    if ( strcmp(color,"red") == 0 ){        // Red filter
        S2_Write(0);
        S3_Write(0);
    }
    else if ( strcmp(color,"green") == 0 ){ // Green filter
        S2_Write(1);
        S3_Write(1);
    }
    else if ( strcmp(color,"blue") == 0 ){  // Blue filter
        S2_Write(0);
        S3_Write(1);
    }
    else if ( strcmp(color,"clear") == 0 ){ // Clear filter
        S2_Write(1);
        S3_Write(0);
    }
}

void RGB_Reset()
{
    Control_Reg_RGB_Write(1);
    CyDelay(1);
    Control_Reg_RGB_Write(0);
}

int Color_Sensor()
{
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    CyDelay(1000);
    
    
    /* ----- Clear Filter ----- */
    RGB_Filter("clear");      // clear filter on
    CyDelay(20);
    RGB_Reset();
        
    while (compare_ready == 0) {}
    counter_rgb[3] = Counter_RGB_ReadCapture();
        
    sprintf(msg, "Clear: %d\n", counter_rgb[3]);
    UART_PutString(msg);
        
    compare_ready = 0;
        
    /* ----- Red Filter ----- */
    RGB_Filter("red");      // Red filter on
    CyDelay(20);
    RGB_Reset();
        
    while (compare_ready == 0) {}
    CyDelay(1000);
    counter_rgb[0] = Counter_RGB_ReadCapture();
        
    sprintf(msg, "Red: %d\n", counter_rgb[0]);
    UART_PutString(msg);
        
    compare_ready = 0;
        
    /* ----- Blue Filter ----- */
    RGB_Filter("blue");      // blue filter on
    CyDelay(20);
    RGB_Reset();
        
    while (compare_ready == 0) {}
    counter_rgb[1] = Counter_RGB_ReadCapture();
        
    sprintf(msg, "Blue: %d\n", counter_rgb[1]);
    UART_PutString(msg);
        
    compare_ready = 0;
        
    /* ----- Green Filter ----- */
    RGB_Filter("green");      // green filter on
    CyDelay(20);
    RGB_Reset();
        
    while (compare_ready == 0) {}
    counter_rgb[2] = Counter_RGB_ReadCapture();
        
    sprintf(msg, "Green: %d\n\n", counter_rgb[2]);
    UART_PutString(msg);
        
    compare_ready = 0;
        
    
    // is red > blue? if yes red +1, if no blue +1
    color_result = (counter_rgb[0] > counter_rgb[1]) ? 1 : 2;
    
    // if red is also greater than green
    if (color_result == 1 && counter_rgb[0] > counter_rgb[2]) {
        color_result = 1; // return code for red
        LED_Pin_Write(0);
    }
    
    // if blue is also greater than green
    else if (color_result == 2 && counter_rgb[1] > counter_rgb[2]) {
        color_result = 2; // return code for blue
        LED_Pin_Write(1);
    }
    
    else {
        color_result = 3; // return code for green
        LED_Pin_Write(1);
        CyDelay(100);
        LED_Pin_Write(0);
        CyDelay(100);
        LED_Pin_Write(1);
        CyDelay(100);
        LED_Pin_Write(0);
        CyDelay(100);
    }
    
    IR_Sensor_Write(0);
    
    return color_result;
}

/* --- Sub-system: Puck-handling --- */

void Arm_Raise(int cmp_now, int cmp_final) 
{
    //PWM_Arm_Metal_WriteCompare(cmp_now);      // initializing arm position
    
    while (cmp_now >= cmp_final) {
        if (Status_Reg_Arm_Read() == 1) {
            PWM_Arm_Metal_ReadStatusRegister();
            PWM_Arm_Metal_WriteCompare(cmp_now-1);
            cmp_now = cmp_now - 1;
            CyDelay(10);
            
            //sprintf(msg, "Arm Raise: %d\n", cmp_now);
            //UART_PutString(msg); 
        }
    }
}

void Arm_Lower(int cmp_now, int cmp_final) 
{
    //PWM_Arm_Metal_WriteCompare(cmp_now);      // initializing arm position
    
    while (cmp_now <= cmp_final) {
        if (Status_Reg_Arm_Read() == 1) {
            PWM_Arm_Metal_ReadStatusRegister();
            PWM_Arm_Metal_WriteCompare(cmp_now+1);
            cmp_now = cmp_now + 1;
            CyDelay(10);
            
            //sprintf(msg, "%d\n", cmp_now);
            //UART_PutString(msg);
        }
        
    }
}

void Puck_Handling (int push1_pull2) {
    sprintf(msg, "Now in PUCK HANDLING Function\n\n");
    UART_PutString(msg);
    
    Freq_Scaling(0);
    LED_Write(0);
    
    switch (push1_pull2) {
        case 1: // zones 7 and 8
        // push code here
        Robot_Move(0, 12500, 70, 0, 4100, 0, 0);
        CyDelay(500);
        Arm_Lower(905, 925); 
        CyDelay(500);
        Robot_Move(1, 12500, 70, 0, 2350, 0, 0);
        CyDelay(500);
        Arm_Raise(925, 905);
        
        break;
        case 2: // zones 5 and 6
            Arm_Lower(905, 925); 
            CyDelay(500);
            Robot_Move(0, 12500, 70, 0, 4100, 0, 0);
            CyDelay(500);
            Arm_Raise(925, 905);
            CyDelay(100);
            Robot_Move(1, 12500, 70, 0, 4100, 0, 0); 
        break;
    }

}
/******* --------- Functions END --------- *******/

/** --------- Interupt Handlers --------- **/

CY_ISR(Idle_State) 
{
    Base_State_Reg_Write(0); // Reset previous states
    Zones_Reg_Write(0);
    
    sprintf(msg, "Now in IDLE State\n\n");
    UART_PutString(msg);
    
    //CyDelay(2000); // debouncing delay
    
    //PWM_RGB_Start();   // Color Sensor
    //Counter_RGB_Start();
    //Color_Sensor();
    
    //Ultrasonic_Sensor_Start();
    //CyDelay(8000);
    //Ultrasonic_Sensor_Stop();
    //CyDelay(3000);
    //Robot_Move(1, 12500, 70, 1, 0 , 0, 0); // move straight until puck detection
    //CyDelay(3000);
    //Robot_Move(0, 12500, 70, 1, 0 , 0, 0); // move back until puck detection
    
    while (idle_state == 0) {
        if (Btn_Start_Read() == 1 || zones_visited > 0) {
            Btn_Start_Write(1);
            idle_state = 1;
        }
        
    }
    
    
}

CY_ISR(Zone_5_State) 
{
    sprintf(msg, "Now in ZONE 5 State\n");
    UART_PutString(msg);
    zone[0] = 1;
    zone[1] = 0; zone[2] = 0; zone[3] = 0;
    Btn_Start_Write(0);
    
    // Aligning Robot
    //Robot_Move(int direction, int PWM_master, double Kp, int ir, int counter_val , int uss, int limit_switch)

    // Initial movement to face zones 7 and 5
    Robot_Move(1, 12500, 70, 0, 1700, 0, 0);  // move straight a bit
    CyDelay(100);
    
    Robot_Turn(1, 12500, 4000, 0);; // right turn
    CyDelay(100);
    
    Robot_Move(1, 12500, 70, 0, 1550, 0, 0);  // move right a bit
    CyDelay(100);
    Robot_Turn(0, 12500, 0, 3800);; // left turn
    CyDelay(100);
    
    Robot_Move(0, 12500, 70, 0, 0 , 0, 1); // align with limit switch, reverse
    CyDelay(100);
    LED_Pin_Write(0);
    
    //Robot_Move(int direction, int PWM_master, double Kp, int ir, int counter_val , int uss, int limit_switch) 
    /* Zone 5 starts here*/
    
    Robot_Move(1, 12500, 70, 1, 0 , 0, 0); // move straight until puck detection
    
    
    // Color sensing and puck handling should be here
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    
    Robot_Move(1, 12500, 70 , 0, 1000, 0, 0);  // move straight a bit more
    
    PWM_RGB_Start();   // Color Sensor
    Counter_RGB_Start();
    Color_Sensor();
    CyDelay(50);
    
    if (color_result == 2) {Puck_Handling(2);} // puck handling function
    IR_Sensor_Write(1);

    zones_visited = zones_visited + 1;
    Zones_Reg_Write(0x01);  // Next State to Zone 7
    CyDelay(10);
}

CY_ISR(Zone_7_State) 
{
    sprintf(msg, "Now in ZONE 7 State\n");
    UART_PutString(msg);
    zone[1] = 1;
    zone[0] = 0; zone[2] = 0; zone[3] = 0;
    
    Robot_Move(1, 12500, 70, 1, 0 , 0, 0); // move straight until puck detection
    
    // Color sensing
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    
    Robot_Move(1, 12500, 70, 0, 1000, 0, 0);  // move straight a bit more
    
    PWM_RGB_Start();   // Color Sensor
    Counter_RGB_Start();
    Color_Sensor();
    CyDelay(50);
    
    // Puck Handling
    if (color_result == 3) {Puck_Handling(1);} // puck handling function
    IR_Sensor_Write(1);

    LED_Write(0);   // Turn off LED
    Freq_Scaling(0);  // Color sensor frequency scale

    Robot_Move(0, 12500, 70, 0, 0, 0, 1); // reverse until it hits limit switches
    CyDelay(200);
    
    zones_visited = zones_visited + 1;
    Zones_Reg_Write(0x02);  // Next State to Zone 6
    CyDelay(10);
}

CY_ISR(Zone_6_State) 
{
    sprintf(msg, "Now in ZONE 6 State\n");
    UART_PutString(msg);
    zone[2] = 1;
    zone[0] = 0; zone[1] = 0; zone[3] = 0;

    Robot_Move(1, 12500, 70, 0, 2400, 0, 0);  // move straight a bit
    CyDelay(100);
    
    Robot_Turn(0, 12500, 0, 3400);; // left turn
    CyDelay(100);
    
    Robot_Move(1, 12500, 70, 0, 3575, 0, 0);  // move left a bit
    CyDelay(100);
    Robot_Turn(1, 12500, 3600, 0); // right turn
    CyDelay(100);
    
    Robot_Move(0, 12500, 70, 0, 0 , 0, 1); // align with limit switch, reverse
    CyDelay(100);
    LED_Pin_Write(0);

    //Robot_Move(int direction, int PWM_master, double Kp, int ir, int counter_val , int uss, int limit_switch) 
    /* Zone 6 starts here*/
    Robot_Move(1, 12500, 70, 1, 0 , 0, 0); // move straight until puck detection
    
    // Color sensing and puck handling should be here
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    
    Robot_Move(1, 12500, 70, 0, 1000, 0, 0);  // move straight a bit more
    
    PWM_RGB_Start();   // Color Sensor
    Counter_RGB_Start();
    Color_Sensor();
    CyDelay(50);
    
    if (color_result == 3) {Puck_Handling(2);} // if puck is green
    IR_Sensor_Write(1);
    
    zones_visited = zones_visited + 1;
    Zones_Reg_Write(100) ;  // Next State to Zone 8
    CyDelay(10);
}

CY_ISR(Zone_8_State) 
{
    sprintf(msg, "Now in ZONE 8 State\n");
    UART_PutString(msg);
    zone[3] = 1;
    zone[0] = 0; zone[1] = 0; zone[2] = 0;

    Robot_Move(1, 12500, 70, 1, 0 , 0, 0); // move straight until puck detection
    
    // Color sensing
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    
    Robot_Move(1, 12500, 70, 0, 1000, 0, 0);  // move straight a bit more
    
    PWM_RGB_Start();   // Color Sensor
    Counter_RGB_Start();
    Color_Sensor();
    CyDelay(50);
    
    // Puck Handling
    if (color_result == 1) {Puck_Handling(1);} // puck handling function
    IR_Sensor_Write(1);

    LED_Write(0);   // Turn off LED
    Freq_Scaling(0);  // Color sensor frequency scale

    Robot_Move(0, 12500, 70, 0, 0, 0, 1); // reverse until it hits limit switches
    
    zones_visited = zones_visited + 1;
    Zones_Reg_Write(0); // Reset zone registers
    Base_State_Reg_Write(0x01);  // Next State to Base
    CyDelay(10);
}

CY_ISR(Return_Back_State) 
{
    sprintf(msg, "Now in RETURN BACK State\n\n\n");
    UART_PutString(msg);
    
    Robot_Move(1, 12500, 70, 0, 2200, 0, 0);  // move straight a bit
    CyDelay(100);

    Robot_Turn(1, 12500, 3900, 0); // right turn
    CyDelay(100);
    
    if (zones_visited < 8) {
        Robot_Move(1, 12500, 70, 0, 1700, 0, 0);  // move right a bit
        CyDelay(100);
    }
    else if (zones_visited >= 8) {
        Robot_Move(1, 12500, 70, 0, 1650, 0, 0);  // move right a bit
        CyDelay(100);
    }
    
    Robot_Turn(0, 12500, 0, 3725);; // left turn
    CyDelay(100);

    Robot_Move(0, 12500, 70, 0, 0 , 0, 1); // align with limit switch, reverse
    CyDelay(100);

    Robot_Move(1, 12500, 70, 0, 200, 0, 0);  // move straight a bit
    CyDelay(1000);
    LED_Pin_Write(0);
    idle_state = 0;

    CyDelay(0);
    Base_State_Reg_Write(0x02); // Next State
}

/* ------------------------------------------------------------ */

CY_ISR(Color_Sensor_Handler) 
{
    //sprintf(msg, "Currently in color sensor isr\n");
    //UART_PutString(msg);
    
    PWM_RGB_ReadStatusRegister();
    
    compare_ready = 1;
}

CY_ISR(Trigger_Timer_Handler) 
{
    //sprintf(msg, "Currently in trigger timer isr\n");
    //UART_PutString(msg);
    
    Trigger_Timer_ReadStatusRegister();
    
    if (uss_1_trig) {
        Ultrasonic_Sensor_1();
    }
    else if (uss_2_trig) {
        Ultrasonic_Sensor_2();
    }
    
}

CY_ISR(Measure_Timer_Handler) 
{
    //sprintf(msg, "Currently in measure timer isr\n");
    //UART_PutString(msg);
    
    Measure_Timer_ReadStatusRegister();
    measure_timer_flag = 1;
    
    if (uss_1_trig && print_count >= 4) {
        timer1_count = Measure_Timer_ReadCounter();
        dist_measured_1 = (65536 - timer1_count)/ 58;
        
        //sprintf(msg, "Dist 1: %d.%02d\n", (int)dist_measured_1, ((int)(dist_measured_1 * 100.0 + 0.5))%100);
        //UART_1_PutString(msg);
        
        uss_1_trig = 0;
        uss_2_trig = 1;
        sprintf(msg, "Sensor 1: %.2f\n", dist_measured_1);
        UART_PutString(msg);
    }
    else if (uss_2_trig && print_count >= 4) {
        timer2_count = Measure_Timer_ReadCounter();
        dist_measured_2 = (65536 - timer2_count)/ 58;        // Dist measured in cm
        
        uss_2_trig = 0;
        uss_1_trig = 1;
        sprintf(msg, "Sensor 2: %.2f\n\n", dist_measured_2);
        UART_PutString(msg);
    }
    
    print_count++;
}
/*
CY_ISR(PWM_Handler)
{
    PWM_Arm_Metal_ReadStatusRegister();
    PWM_Arm_Metal_WriteCompare(cmp_now-1);
    cmp_now = cmp_now - 1;
    
    if (cmp_now <= cmp_final) {
        isr_pwm_Disable();
    }
}
*/
/******* --------- Interupt Handlers END --------- *******/


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Motor intializations
    PWM_Motor_1_Start(); PWM_Motor_2_Start(); 
    QuadDec_Motor_1_Start(); QuadDec_Motor_2_Start(); 
    UART_Start();

    //Counter_master =  QuadDec_Motor_1_GetCounter();
	//Counter_slave =  QuadDec_Motor_2_GetCounter();
    
    // Servo 
    PWM_Arm_Metal_Start();  
    
    // Ultrasonic Sensor
    Trig_Control_Write(1);
    Echo_Control_Write(1);
    
    /* Interrupt Initialization */
    trigger_isr_StartEx(Trigger_Timer_Handler);       // Ultrasonic Sensor
    measure_isr_StartEx(Measure_Timer_Handler);
    
    //PWM_RGB_Start();                                    // Color Sensor
    //Counter_RGB_Start(); 
    rgb_isr_StartEx(Color_Sensor_Handler);
    
    idle_isr_StartEx(Idle_State);           // State ISRs
    zone7_isr_StartEx(Zone_7_State);    zone5_isr_StartEx(Zone_5_State);
    zone8_isr_StartEx(Zone_8_State);    zone6_isr_StartEx(Zone_6_State);
    return_back_isr_StartEx(Return_Back_State);
    
    //Arm_Raise(961, 915);
    /*
    LED_Write(1);   // Turn on LED
    Freq_Scaling(100);  // Color sensor frequency scale
    CyDelay(1000);
    Color_Sensor();
    CyDelay(500);
    LED_Pin_Write(0);
    CyDelay(1000);
    IR_Sensor_Write(0);
    */
    //Color_Sensor();
    //LED_Pin_Write(0);
    /* **************** */
    
  
    for (;;)
    {
        

        
    }
     
}

/* [] END OF FILE */
