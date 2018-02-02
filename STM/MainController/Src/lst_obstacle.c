/*
 * lst_obstacle.c
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_obstacle.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t lst_obs_mode = LST_OBS_MODE_BT;
uint8_t lst_obs_lap_mode = LST_OBS_LAP_MODE_START;

uint8_t lst_obs_search_cntr_error = 0;
uint8_t lst_obs_search_mode =       LST_OBS_SEARCH_MODE_BEGIN;
uint8_t lst_obs_search_cntr =       0;
uint8_t lst_obs_search_line_no =    0;


/* External variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void LST_Obs_ResetStateMachine();
static void LST_Obs_GamePadHandler();
static void LST_Obs_StateMachine();
static void LST_Obs_Lap();
static void LST_Obs_Search();
static void LST_Obs_Drone();
static void LST_Obs_Corner();
static void LST_Obs_Convoy();
static void LST_Obs_Barrel();
static void LST_Obs_Roundabout();
static void LST_Obs_Trainstop();
static void LST_Obs_End();

static void LST_Obs_Search_Reset();
static uint8_t LST_Obs_Search_Sharp_Detection(uint8_t sharp_number);
static uint8_t LST_Obs_Search_Long_Line_Detected();

/******************************************************************************/
/*                Obstacle lap handler for RobonAUT 2018 Team LST             */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Obstacle lap module
 */
void LST_Obs_Init() {
  lst_control_steeringP = LST_OBS_STEERING_P;
  lst_control_steeringD = LST_OBS_STEERING_D;

  // INIT TODO TEST
  lst_temp = 0;

  LST_Obs_Search_Reset();
}

/**
 * @brief Main handler logic of the obstacle lap
 */
void LST_Obs_Logic(){
  /* Get line data, sensor data, etc */
  LST_Control_Commons();

  /* Switch between control modes based on the GamePad */
  LST_Obs_GamePadHandler();

  /* Main state machine for the Obstacle Lap mode (eg. BT control, lap mode, etc) */
  LST_Obs_StateMachine();

  /* Set the control of the servo and motor */
  LST_Control_ServoAndMotor();
}

/**
 * @brief Main state machine of the obstacle lap mode
 */
static void LST_Obs_StateMachine(){
  switch(lst_obs_mode){
  case LST_OBS_MODE_BT:
    LST_Obs_ResetStateMachine();

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_OBS_MODE_LAP:

    /* Q2 logic */

  	// Periodic controls
  	LST_Movement_Set();
  	LST_Steering_Set();

  	// Obstacle logic
    LST_Obs_Lap();

    break;
  case LST_OBS_MODE_NO_CONTROL:
    /* Leave values on default */
  default:
    /* Leave values at default */
    LST_Obs_ResetStateMachine();
    break;
  }
}

/**
 * @brief The state machine of the lap mode
 */
static void LST_Obs_Lap(){
  switch(lst_obs_lap_mode){
  case LST_OBS_LAP_MODE_START:
    /* Return if the radio hasn't received the start message */
    if(lst_radio_msg_0_received != LST_RADIO_MSG_RECEIVED) return;

    /* Switch to SEARCH mode */
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
    break;
  case LST_OBS_LAP_MODE_SEARCH:
    LST_Obs_Search();
    break;
  case LST_OBS_LAP_MODE_DRONE:
    LST_Obs_Drone();
    break;
  case LST_OBS_LAP_MODE_CORNER:
    LST_Obs_Corner();
    break;
  case LST_OBS_LAP_MODE_CONVOY:
    LST_Obs_Convoy();
    break;
  case LST_OBS_LAP_MODE_BARREL:
    LST_Obs_Barrel();
    break;
  case LST_OBS_LAP_MODE_ROUNDABOUT:
    LST_Obs_Roundabout();
    break;
  case LST_OBS_LAP_MODE_TRAINSTOP:
    LST_Obs_Trainstop();
    break;
  case LST_OBS_LAP_MODE_END:
    LST_Obs_End();
    break;
  default:
    break;
  }
}

/**
 * @brief Mode for searching obstacles
 */
static void LST_Obs_Search(){

  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_BEGIN){
    // Line detection
    if(LST_Obs_Search_Long_Line_Detected()){
      lst_obs_search_line_no = 255;
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_LONGLINE;
    }
    else if (lst_control_line_no == 0){
      lst_obs_search_line_no = 0;
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_ROUNDABOUT;
    }
    else if (lst_control_line_no == 2){
      lst_obs_search_line_no = 2;
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_BARREL;
    }
    else if (lst_control_line_no == 3){
      lst_obs_search_line_no = 3;
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_DRONE;
    }
    // One Sharp detection
    else if(LST_Obs_Search_Sharp_Detection(1)){
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_CONVOY;
    }
    // Two Sharps detection
    else if(LST_Obs_Search_Sharp_Detection(2)){
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_CORNER;
    }
  }
  // Roundabout search
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_ROUNDABOUT){
    // Search for some zero-lines first
    if(lst_obs_search_line_no == 0){
      if(lst_control_line_no == 0){
        lst_obs_search_cntr++;
      }
      else{
        // ERROR, we are expecting some 0 lines
        LST_Obs_Search_Reset();
      }

      if(lst_obs_search_cntr > LST_OBS_SEARCH_LINE_THRESHOLD){ // 4
        lst_obs_search_cntr = 0;
        lst_obs_search_line_no = 1;
      }
    }

    // Search for some 1-lines
    else if(lst_obs_search_line_no == 1){
      if(lst_control_line_no == 1){
        lst_obs_search_cntr++;
      }
      else{
        lst_obs_search_cntr = 0;
        lst_obs_search_cntr_error++;
        if(lst_obs_search_cntr_error > LST_OBS_SEARCH_LINE_ERROR_THRESHOLD){ // 100
          // ERROR, we were expecting some 1 lines
          LST_Obs_Search_Reset();
        }
      }

      // Found obstacle
      if(lst_obs_search_cntr > LST_OBS_SEARCH_LINE_THRESHOLD){ // 4
        lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
        lst_obs_lap_mode = LST_OBS_LAP_MODE_ROUNDABOUT;
      }
    }

  }

  // Drone search
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_DRONE){
    // Search for a long 3 line section
    if(lst_obs_search_line_no == 3){
      if(lst_control_line_no == 3){
        lst_obs_search_cntr++;
      }
      else{
        lst_obs_search_cntr = 0;
        lst_obs_search_cntr_error++;
        if(lst_obs_search_cntr_error > LST_OBS_SEARCH_LINE_ERROR_THRESHOLD){ // 100
          // ERROR, we were expecting some 3 lines
          LST_Obs_Search_Reset();
        }
      }

      // Found obstacle
      if(lst_obs_search_cntr > LST_OBS_SEARCH_LONG_LINE_THRESHOLD){ // 20
        lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
        lst_obs_lap_mode = LST_OBS_LAP_MODE_DRONE;
      }
    }
  }

  // Barrel search
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_BARREL){
    // Search for some 2-lines first
    if(lst_obs_search_line_no == 2){
      if(lst_control_line_no == 2){
        lst_obs_search_cntr++;
      }
      else{
        // ERROR, we are expecting some 2 lines
        LST_Obs_Search_Reset();
      }

      if(lst_obs_search_cntr > LST_OBS_SEARCH_LINE_THRESHOLD){ // 4
        lst_obs_search_cntr = 0;
        lst_obs_search_line_no = 1;
      }
    }

    // Search for some 1-lines
    else if(lst_obs_search_line_no == 1){
      if(lst_control_line_no == 1){
        lst_obs_search_cntr++;
      }
      else{
        lst_obs_search_cntr = 0;
        lst_obs_search_cntr_error++;
        if(lst_obs_search_cntr_error > LST_OBS_SEARCH_LINE_ERROR_THRESHOLD){ // 100
          // ERROR, we were expecting some 1 lines
          LST_Obs_Search_Reset();
        }
      }

      // Found obstacle
      if(lst_obs_search_cntr > LST_OBS_SEARCH_LINE_THRESHOLD){ // 4
        lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
        lst_obs_lap_mode = LST_OBS_LAP_MODE_BARREL;
      }
    }
  }

  // LongLine search for End/Traintracks
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_LONGLINE){
    // Check for one long line
    if (lst_obs_search_line_no == 255){
      if(LST_Obs_Search_Long_Line_Detected()){
        lst_obs_search_line_no = 1;
      }
      else{
        // ERROR, reset
        LST_Obs_Search_Reset();
      }
    }

    // Check for one lines
    else if (lst_obs_search_line_no == 1){
      uint8_t temp = LST_Obs_Search_Long_Line_Detected();
      if(lst_control_line_no == 1 && !temp){
        lst_obs_search_cntr++;
      }
      else if(temp){ // If long line detected
        if (lst_obs_search_cntr > LST_OBS_SEARCH_ONE_LINE_BETWEEN_LONGS_THRESHOLD){ // 1
          // found double lines, traintracks
          lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
          lst_obs_lap_mode = LST_OBS_LAP_MODE_TRAINSTOP;
        }
      }
      else{
        // line_no is not 1, but no long line
        // this would be rather bad... so i'm ignoring it
      }

      if(lst_obs_search_cntr > 10){
        // found the End
        lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
        lst_obs_lap_mode = LST_OBS_LAP_MODE_END;
      }
    }
  }

  // Convoy search
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_CONVOY){
    // Search only for 1 Sharp sensor
    if(LST_Obs_Search_Sharp_Detection(1)){
      lst_obs_search_cntr++;
    }
    else{
      // ERROR, we are expecting 1 Sharp here
      LST_Obs_Search_Reset();
    }

    // Found obstacle
    if(lst_obs_search_cntr > LST_OBS_SEARCH_SHARP_THRESHOLD){ // 10
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
      lst_obs_lap_mode = LST_OBS_LAP_MODE_CONVOY;
    }
  }

  // Corner search
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_CORNER){
    // Search only for 2 Sharp sensor
    if(LST_Obs_Search_Sharp_Detection(2)){
      lst_obs_search_cntr++;
    }
    else{
      // ERROR, we are expecting 2 Sharp here
      LST_Obs_Search_Reset();
    }

    // Found obstacle
    if(lst_obs_search_cntr > LST_OBS_SEARCH_SHARP_THRESHOLD){ // 10
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
      lst_obs_lap_mode = LST_OBS_LAP_MODE_CORNER;
    }
  }

  // Found obstacle
  if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_FOUND){
    LST_Obs_Search_Reset();
  }

  // VONALÉRZÉKELÕS rész:
  //    Nincs vonal -> Körforg
  //      Van vonal
  //      Nincs vonal
  //    Három vonal (hosszú vonal) -> Útonálló drón
  //      Három hosszú vonal hosszan
  //    Két vonal -> Forgó hordó
  //      Egy vonal
  //      Két vonal
  //    Hosszú vonal -> Vasúti átjáró/Cél
  //      Egy vonal hosszan -> cél
  //      Egy vonal röviden
  //      Hosszú vonal -> Vasút

  //  SHARP érzékelés
  //    Egyik oldalt van csak:
  //      Hosszan csak egy oldalt van -> konvoj
  //    Mindkét oldalt van hosszan -> utcasarok

  // Ha talált akadályt, RESET_SEARCH_VARS

}

static void LST_Obs_Search_Reset(){
  lst_obs_search_mode = LST_OBS_SEARCH_MODE_BEGIN;
  lst_obs_search_cntr = 0;
  lst_obs_search_cntr_error = 0;
}

static uint8_t LST_Obs_Search_Sharp_Detection(uint8_t sharp_number){
  if(sharp_number == 1){
    return
       (LST_Sharp_GetLeftDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD   &&
        LST_Sharp_GetRightDistance_mm() > LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD) ||
       (LST_Sharp_GetLeftDistance_mm() > LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD   &&
        LST_Sharp_GetRightDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD);
  }
  else{
    return
       (LST_Sharp_GetLeftDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD   &&
        LST_Sharp_GetRightDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD);
  }
}

static uint8_t LST_Obs_Search_Long_Line_Detected(){
  // See if at least 10 values next to each other are at least 110
  uint8_t temp = 0;
  uint8_t cntr = 0;
  for(temp = 0; temp < 32; temp++){
    if(lst_spi_master1_rx[temp + 6] > LST_OBS_SEARCH_LED_THRESHOLD){ // 110
      cntr++;
      if(cntr > LST_OBS_SEARCH_LONG_LINE_SIZE_THRESHOLD){ // 9
        return 1;
      }
    }
    else{
      cntr = 0;
    }
  }

  return 0;
}

static void LST_Obs_Drone(){
  // ToDo TEST 2018. 02. 01.

	LST_Steering_Follow();

	if (LST_Sharp_GetFrontDistance_mm() > 300)
	{

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

	}
	else
	{

		LST_Movement_Stop();

	}

}

/**
 * @brief Mode for the corner
 */

// TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// IMPROVEMENT: When backing completed and moving forward, watch right
// Sharp distance, if increases, turn a bit right, if decreases, a bit left
// but only if the wall is in range
static void LST_Obs_Corner(){

	switch (lst_obs_corner_stage)
	{

	case LST_OBS_COR_STAGE_APPROACH:

		LST_Steering_Follow();

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// Jump to next stage if the Line disappears (in the center of the junction)
		if (lst_control_line_no < 1) lst_obs_corner_stage =
				LST_OBS_COR_STAGE_PASSEDJUNCTION;

		break;

	case LST_OBS_COR_STAGE_PASSEDJUNCTION:

		LST_Steering_Follow();

		// Slow down
		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		// Jump
		if (lst_control_line_no > 1)
			{

			// Advance a certain distance to prepare for backing up
			// (start the measurement)
			//LST_Distance_Measure_mm(300); // NEXT STAGE A VERSION !!!

			// Next stage
			lst_obs_corner_stage =
					LST_OBS_COR_STAGE_CURVEDLINEFOUND;

			}


		break;

	case LST_OBS_COR_STAGE_CURVEDLINEFOUND:

		// Lock because curved line interferes
		LST_Steering_Lock(0);

		/* A VERSION
		// Continue ahead until the distance is reached
		if (!LST_Distance_Measure_mm(0))
		{

			LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		}
		else
		{

			LST_Movement_Stop(); // Not really needed

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING;

		}
		*/

		// B VERSION - move until wall ends

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		if (LST_Sharp_GetRightDistance_mm() > 200)
		{

			LST_Movement_Stop(); // Not really needed

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_FIRST;

			LST_Distance_Measure_mm(-800);

		}

		break;


	// STAGE: back up until the wall to the right is lost
	case LST_OBS_COR_STAGE_BACKING_FIRST:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		//LST_Movement_Move(LST_MOVEMENT_FB_BACKING_SLOWEST);
		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		// Back up a bit to lose the wall to the right
		if (LST_Distance_Measure_mm(0))
		{

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_SECOND;

			// Init var for next
			lst_obs_cor_rightSharp_previous = 10000;

		}

		break;

	// STAGE: Back up until the right Sharp distance is increasing again
	case LST_OBS_COR_STAGE_BACKING_SECOND:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		//LST_Movement_Move(LST_MOVEMENT_FB_BACKING_SLOWEST);
		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		if (LST_Sharp_GetRightDistance_mm() > lst_obs_cor_rightSharp_previous)
		{

			lst_obs_corner_stage = LST_OBS_COR_STAGE_OUTGOING;
			//LST_Distance_Measure_mm(-150);

		}
		else
		{

			lst_obs_cor_rightSharp_previous = LST_Sharp_GetRightDistance_mm();

		}

		break;
/*
	// STAGE: Back up a bit more to align better TODO NOT NEEDED?
	case LST_OBS_COR_STAGE_BACKING_THIRD:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		// Back up a bit to lose the wall to the right
		if (LST_Distance_Measure_mm(0))
		{

			// Next stage
			lst_obs_corner_stage = lst_obs_corner_stage =
					LST_OBS_COR_STAGE_OUTGOING;

		}

		break;
*/
	case LST_OBS_COR_STAGE_OUTGOING:

		LST_Steering_Lock(0);

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		if ((LST_Sharp_GetLeftDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL) &&
				(LST_Sharp_GetRightDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL))
		{

			// Between outgoing walls, heading for exit
			lst_obs_corner_stage =
								LST_OBS_COR_STAGE_ALIGNMENT;

		}

		break;

	case LST_OBS_COR_STAGE_ALIGNMENT:


		// Align with steering
		if (LST_Sharp_GetLeftDistance_mm() > LST_Sharp_GetRightDistance_mm())
		{

			LST_Steering_Lock(LST_OBS_COR_LEFT_LOCK);

		}
		else
		{

			LST_Steering_Lock(LST_OBS_COR_RIGHT_LOCK);

		}

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// No need for alignment
		if (lst_control_line_no > 0)
		{

			lst_obs_corner_stage =
					LST_OBS_COR_STAGE_EXIT;

		}

		break;

	case LST_OBS_COR_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

		break;

	}

}

/**
 * @brief Mode for the convoy
 */
static void LST_Obs_Convoy(){
  // ToDo TEST 2018. 02. 01.

	LST_Movement_Move(60);

}

/**
 * @brief Mode for the barrel
 */
static void LST_Obs_Barrel(){
  // ToDo

  // TODO TEST

	LST_Steering_Lock(0);

	if (!lst_temp)
		if (!LST_Distance_Measure_mm(-300))
		{

			LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		}
		else
		{

			//LST_Movement_Stop();
			lst_temp = 1;

		}
	else
	{

		LST_Movement_Stop();

	}

}

/**
 * @brief Mode for the roundabout
 */
static void LST_Obs_Roundabout(){
	// ToDo TEST 2018. 02. 02.

		LST_Movement_Move_Encoderless(-400);

		// lst_control_motor(-100);
}

/**
 * @brief Mode for the train stop
 */
static void LST_Obs_Trainstop(){

	// TODO TEST 2018. 02. 02.

	switch (lst_obs_train_stage)
	{

	case LST_OBS_TRA_STAGE_PREPARE:

		// Init
		lst_obs_train_repeatedCrossing = 0;

		// Jump to next
		lst_obs_train_stage = LST_OBS_TRA_STAGE_APPROACH;

		break;

	case LST_OBS_TRA_STAGE_APPROACH:

		// Follow the line
		LST_Steering_Follow();

		// Slowest speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		// Next stage if line ends
		if (lst_control_line_no < 1)
		{

			// Lock now! TODO TEST 2018.02.02.
			// also implement for corner
			LST_Steering_Lock(0);

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_WATCH;

		}

		break;

	case LST_OBS_TRA_STAGE_WATCH:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// Wait for one car
		if (LST_Sharp_GetFrontDistance_mm() < LST_OBS_TRA_SHARP_DIST_CAR)
		{

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_WAIT;

			// Init variable
			lst_obs_tra_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

		}


		break;

	case LST_OBS_TRA_STAGE_WAIT:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// Wait until no car is seen
		if (LST_Sharp_GetFrontDistance_mm() > LST_OBS_TRA_SHARP_DIST_CAR)
			{

				// Jump to next
				lst_obs_train_stage = LST_OBS_TRA_STAGE_COUNT;

				// Init variable
				lst_obs_tra_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

			}


		break;

	case LST_OBS_TRA_STAGE_COUNT:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// If sufficient time has elapsed without a car seen, then
		// it is safe to proceed, else wait more
		if (lst_obs_tra_lastCarTimer <= 0)
		{

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSS;

		}
		else
		{

			// Reset counter if another car appears
			if (LST_Sharp_GetFrontDistance_mm() < LST_OBS_TRA_SHARP_DIST_CAR)
			{

				lst_obs_tra_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

			}
			else
			{

				lst_obs_tra_lastCarTimer--;

			}

		}

		break;

	case LST_OBS_TRA_STAGE_CROSS:

		LST_Steering_Lock(0); // TODO Test: perpendicular line pulls steering in
		// the right direction if not aligned, but is it better than
		// locking at 0?

		// Quickly through
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW); // TODO MEDIUM !!!!!!!!!!!!!!!

		// Reached perpendicular line (train track)
		if (LST_Obs_Search_Long_Line_Detected()) // TEEEEEEEEEEEEEEST
		{

			// Init variable
			lst_obs_tra_crossingTimer = LST_OBS_TRA_CROSSINGTIMER_PERIOD;

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSSING;

		}

		break;

	case LST_OBS_TRA_STAGE_CROSSING:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW); // TODO MEDIUM !!!!!!!!!!!!!!!

		// Delay a bit to get to the no line part
		if (lst_obs_tra_crossingTimer <= 0)
		{

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSSED;

		}
		else
		{

			lst_obs_tra_crossingTimer--;

		}

		break;

	case LST_OBS_TRA_STAGE_CROSSED:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// Find the line between the tracks
		if (lst_control_line_no > 0)
		{

			// If this was the first crossing, repeat
			if (!lst_obs_train_repeatedCrossing)
			{

				lst_obs_train_stage = LST_OBS_TRA_STAGE_APPROACH;

				lst_obs_train_repeatedCrossing = 1;

			}
			else
			{

				lst_obs_train_stage = LST_OBS_TRA_STAGE_EXIT;

			}

		}

		break;

	case LST_OBS_TRA_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

	}

}

/**
 * @brief Mode for the End of the course :^)
 */
static void LST_Obs_End(){
  // ToDo

  LST_Movement_Stop();
}

/**
 * @brief Resets the state machine
 */
static void LST_Obs_ResetStateMachine(){
  lst_obs_lap_mode             = LST_OBS_LAP_MODE_START;
  lst_obs_corner_stage = LST_OBS_COR_STAGE_APPROACH;
  // ToDo reset other variables
  LST_Obs_Search_Reset();
}

/**
 * @brief Switches between different modes of control based on the GamePad
 */
static void LST_Obs_GamePadHandler(){
  /* Switch between BT/automatic mode */
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_A] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_BT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_B] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_LAP;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_Y] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_NO_CONTROL;
  }

  /* Change control parameters with DPad and triggers */
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_NORTH){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_START;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_SOUTH){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_WEST){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_DRONE;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_EAST){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_CORNER;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_CONVOY;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_BARREL;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_ROUNDABOUT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_TRAINSTOP;
  }
}
