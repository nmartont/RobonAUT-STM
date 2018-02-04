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

uint8_t lst_obs_roundabout_direction = LST_INFRA_DIR_LEFT;
uint8_t lst_obs_roundabout_exit =      LST_INFRA_EXIT_ONE;

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

  LST_Obs_ResetStateMachine();

  //LST_Obs_Search_Reset();
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
  default:
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
  // Follow the line
  LST_Steering_Follow();

  // Go slowly
  LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM); // TODO TEST 2018. 02. 03.

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
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_ROUNDABOUT){
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
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_DRONE){
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
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_BARREL){
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
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_LONGLINE){
    // Check for one long line, once more:
    if (lst_obs_search_line_no == 255){
      if(LST_Obs_Search_Long_Line_Detected()){
        lst_obs_search_line_no = 1;
      }
      else{
        // ERROR, no repeated long line found, reset
        LST_Obs_Search_Reset();
      }
    }

    // Check for one lines
    else if (lst_obs_search_line_no == 1){
      uint8_t temp = LST_Obs_Search_Long_Line_Detected();
      if(!temp){ // If no long line
        lst_obs_search_cntr++;
      }
      else{ // If long line detected
        // if there were at least 2 non-long lines between long lines
        if (lst_obs_search_cntr > LST_OBS_SEARCH_ONE_LINE_BETWEEN_LONGS_THRESHOLD){ // 1
          // found double lines, train tracks
          lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
          lst_obs_lap_mode = LST_OBS_LAP_MODE_TRAINSTOP;
        }
      }

      // if there is at least 11 non-long lines after the single long line, it's the end
      if(lst_obs_search_cntr > 10){
        // found the End
        lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
        lst_obs_lap_mode = LST_OBS_LAP_MODE_END;
      }
    }
  }

  // Convoy search
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_CONVOY){
    // Search only for 1 Sharp sensor
    if(LST_Obs_Search_Sharp_Detection(1)){
      lst_obs_search_cntr++;
    }
    else{
      // ERROR, we are expecting 1 Sharp here
      LST_Obs_Search_Reset();
    }

    // Found obstacle
    if(lst_obs_search_cntr > LST_OBS_SEARCH_SHARP_THRESHOLD){ // 50
      lst_obs_search_mode = LST_OBS_SEARCH_MODE_FOUND;
      lst_obs_lap_mode = LST_OBS_LAP_MODE_CONVOY;
    }
  }

  // Corner search
  else if(lst_obs_search_mode == LST_OBS_SEARCH_MODE_CORNER){
    // Search only for 2 Sharp sensor
    if(LST_Obs_Search_Sharp_Detection(2)){
      lst_obs_search_cntr++;
    }
    else{
      // ERROR, we are expecting 2 Sharps here
      LST_Obs_Search_Reset();
    }

    // Found obstacle
    if(lst_obs_search_cntr > LST_OBS_SEARCH_SHARP_THRESHOLD){ // 50
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

  	// Convoy direction control set
  	if (LST_Sharp_GetLeftDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD)
  	{
  		lst_obs_convoy_wallDirection = LST_OBS_CON_WALLDIRECTION_LEFT;
  	}
  	if (LST_Sharp_GetRightDistance_mm() < LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD)
  	{
  		lst_obs_convoy_wallDirection = LST_OBS_CON_WALLDIRECTION_RIGHT;
  	}

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
    if(lst_spi_master1_rx[temp + 6] > LST_OBS_SEARCH_LED_THRESHOLD){ // 150
      cntr++;
      if(cntr > LST_OBS_SEARCH_LONG_LINE_SIZE_THRESHOLD){ // 7
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

	switch (lst_obs_drone_stage)
	{

	case LST_OBS_DRO_STAGE_APPROACH:

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		LST_Steering_Follow();

		if (LST_Sharp_GetFrontDistance_mm() < LST_OBS_DRO_SHARP_DIST_DRONE_IN)
		{

			lst_obs_drone_stage = LST_OBS_DRO_STAGE_WATCH;

		}

		break;

	case LST_OBS_DRO_STAGE_WATCH:

		LST_Movement_Stop();

		LST_Steering_Follow();

		if (LST_Sharp_GetFrontDistance_mm() > LST_OBS_DRO_SHARP_DIST_DRONE_OUT)
		{

			// Init timer
			lst_obs_drone_takeoffTimer = LST_OBS_DRO_TAKEOFFTIMER_PERIOD;

			lst_obs_drone_stage = LST_OBS_DRO_STAGE_WAIT;

		}

		break;

	case LST_OBS_DRO_STAGE_WAIT:

		LST_Movement_Stop();

		LST_Steering_Follow();

		if (lst_obs_drone_takeoffTimer <= 0)
		{

			lst_obs_drone_stage = LST_OBS_DRO_STAGE_EXIT;

		}
		else
		{

			lst_obs_drone_takeoffTimer--;

		}

		break;

	case LST_OBS_DRO_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

		break;

	}

}

/**
 * @brief Mode for the corner
 */

// TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MAKE IT WORK TWO DIRECTIONS !!!!!!!!!!!!!!!!!!!!!!!!
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

			// Next stage
			lst_obs_corner_stage =
					LST_OBS_COR_STAGE_GETNEARWALL;

			LST_Steering_Lock(0); // Curved line would pull the steering

			// Init variable
			lst_obs_corner_junctionTimer = LST_OBS_COR_JUNCTIONTIMER_PERIOD;

			}

		break;

	case LST_OBS_COR_STAGE_GETNEARWALL:

		// Move forward more so the center of the junction is not identified
		// as the point to start backing up. Multiple tests showed if the
		// Wall is even 0.5cm closer to the junction center, then the car
		// starts backing up there.

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		if (lst_obs_corner_junctionTimer <= 0)
		{

			lst_obs_corner_stage = LST_OBS_COR_STAGE_CURVEDLINEFOUND;

			// Save direction control
			if (LST_Sharp_GetRightDistance_mm() < LST_Sharp_GetLeftDistance_mm())
			{

				lst_obs_corner_directionControl = LST_OBS_COR_DIR_LEFT;

			}
			else
			{

				lst_obs_corner_directionControl = LST_OBS_COR_DIR_RIGHT;

			}

		}
		else
		{

			lst_obs_corner_junctionTimer--;

		}

		break;

	case LST_OBS_COR_STAGE_CURVEDLINEFOUND:

		// Lock because curved line interferes
		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		// Direction control (AND)
		if ((LST_Sharp_GetRightDistance_mm() > LST_OBS_COR_SHARP_DIST_WALL)
				&& (LST_Sharp_GetLeftDistance_mm() > LST_OBS_COR_SHARP_DIST_WALL))
		{

			LST_Movement_Stop(); // Not really needed

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_FIRST;

		}

		break;


	// STAGE: back up until the wall to the right/left is lost
	case LST_OBS_COR_STAGE_BACKING_FIRST:


		// Direction control
		if (lst_obs_corner_directionControl == LST_OBS_COR_DIR_LEFT)
			LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK_LEFT);
		else
			LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK_RIGHT);

		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		// Back up a bit to lose the wall to the right
		if (LST_Distance_Measure_mm(LST_OBS_COR_BACKING_DISTANCE))
		{

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_SECOND;

			// Init var for next
			lst_obs_cor_backingSharp_previous = 50000;

		}

		break;

	// STAGE: Back up until the right Sharp distance is increasing again
	case LST_OBS_COR_STAGE_BACKING_SECOND:

		// Direction control
		if (lst_obs_corner_directionControl == LST_OBS_COR_DIR_LEFT)
			LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK_LEFT);
		else
			LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK_RIGHT);

		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		// Direction control

		// Rising Sharp values AND close enough to wall
		// Otherwise it detects the far corner
		if (lst_obs_corner_directionControl == LST_OBS_COR_DIR_LEFT)
			if ((LST_Sharp_GetRawRightDistance() > lst_obs_cor_backingSharp_previous)
					&& (LST_Sharp_GetRightDistance_mm() < LST_OBS_COR_SHARP_BACKING_WALL))
			{

				lst_obs_corner_stage = LST_OBS_COR_STAGE_OUTGOING;

				LST_Steering_Lock(0);

				//LST_Distance_Measure_mm(-150);

			}
			else
			{

				lst_obs_cor_backingSharp_previous = LST_Sharp_GetRawRightDistance();

			}
		else
			if ((LST_Sharp_GetRawLeftDistance() > lst_obs_cor_backingSharp_previous)
					&& (LST_Sharp_GetLeftDistance_mm() < LST_OBS_COR_SHARP_BACKING_WALL))
			{

				lst_obs_corner_stage = LST_OBS_COR_STAGE_OUTGOING;

				LST_Steering_Lock(0);

			}
			else
			{

				lst_obs_cor_backingSharp_previous = LST_Sharp_GetRawLeftDistance();

			}

		break;

	case LST_OBS_COR_STAGE_OUTGOING:

		if (LST_Sharp_GetRightDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL)
		{

			// TODO STEERING CONTROL W/ SHARP
			// TODO ADD direction control if implemented
			LST_Steering_Lock(0);

		}
		else
		{

			LST_Steering_Lock(0);

		}

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		if ((LST_Sharp_GetLeftDistance_mm() > LST_OBS_COR_SHARP_DIST_WALL) &&
				(LST_Sharp_GetRightDistance_mm() > LST_OBS_COR_SHARP_DIST_WALL))
		{

			// In the center of the intersection
			lst_obs_corner_stage =
								LST_OBS_COR_STAGE_CENTER;

		}

		break;

	case LST_OBS_COR_STAGE_CENTER:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// If between two walls (only check one! other can be far)
		if ((LST_Sharp_GetLeftDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL) ||
				((LST_Sharp_GetRightDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL)))
		{

			lst_obs_corner_stage = LST_OBS_COR_STAGE_ALIGNMENT;

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

		// Override previous if one sharp is too far from the wall
		// Because of the sensor characteristics, if a side is to close
		if (LST_Sharp_GetLeftDistance_mm() > LST_OBS_COR_SHARP_FAR_WALL)
		{

			LST_Steering_Lock(LST_OBS_COR_LEFT_LOCK);

		}

		if (LST_Sharp_GetRightDistance_mm() > LST_OBS_COR_SHARP_FAR_WALL)
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
// TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// MAKE IT WORK TWO DIRECTIONS !!!!!!!!!!!!!!!!!!!!!!!!
static void LST_Obs_Convoy(){ // TODO CONTINUE HERE
  // ToDo TEST 2018. 02. 01.


	switch (lst_obs_convoy_stage)
	{

	case LST_OBS_CON_STAGE_APPROACH:

		lst_obs_convoy_stage = LST_OBS_CON_STAGE_WATCH;

		break;

	case LST_OBS_CON_STAGE_WATCH:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// Direction control
		// Wait for one car
		if (lst_obs_convoy_wallDirection == LST_OBS_CON_WALLDIRECTION_LEFT)
		{
			if (LST_Sharp_GetRightDistance_mm() < LST_OBS_CON_SHARP_DIST_CAR)
			{

				// Jump to next
				lst_obs_convoy_stage = LST_OBS_CON_STAGE_WAIT;

				// Init variable
				lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

			}
		}
		else
		{
			if (LST_Sharp_GetLeftDistance_mm() < LST_OBS_CON_SHARP_DIST_CAR)
			{

				// Jump to next
				lst_obs_convoy_stage = LST_OBS_CON_STAGE_WAIT;

				// Init variable
				lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

			}
		}

		break;

	case LST_OBS_CON_STAGE_WAIT:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// Direction control
		// Wait until no car is seen
		if (lst_obs_convoy_wallDirection == LST_OBS_CON_WALLDIRECTION_LEFT)
		{
			if (LST_Sharp_GetRightDistance_mm() > LST_OBS_CON_SHARP_DIST_CAR)
			{

				// Jump to next
				lst_obs_convoy_stage = LST_OBS_CON_STAGE_COUNT;

				// Init variable
				lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

			}
		}
		else
		{
			if (LST_Sharp_GetLeftDistance_mm() > LST_OBS_CON_SHARP_DIST_CAR)
			{

				// Jump to next
				lst_obs_convoy_stage = LST_OBS_CON_STAGE_COUNT;

				// Init variable
				lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

			}
		}

		break;

	case LST_OBS_CON_STAGE_COUNT:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// If sufficient time has elapsed without a car seen, then
		// it is safe to proceed, else wait more
		if (lst_obs_convoy_lastCarTimer <= 0)
		{

			// Jump to next
			lst_obs_convoy_stage = LST_OBS_CON_STAGE_TURNIN;

			// Init variable
			lst_obs_convoy_turnTimer = LST_OBS_CON_TURNTIMER_PERIOD;

		}
		else
		{

			// Direction control
			// Reset counter if another car appears
			if (lst_obs_convoy_wallDirection == LST_OBS_CON_WALLDIRECTION_LEFT)
				if (LST_Sharp_GetRightDistance_mm() < LST_OBS_CON_SHARP_DIST_CAR)
				{

					lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

				}
				else
				{

					lst_obs_convoy_lastCarTimer--;

				}
			else
				if (LST_Sharp_GetLeftDistance_mm() < LST_OBS_CON_SHARP_DIST_CAR)
				{

					lst_obs_convoy_lastCarTimer = LST_OBS_CON_LASTCARTIMER_PERIOD;

				}
				else
				{

					lst_obs_convoy_lastCarTimer--;

				}

		}

		break;

	case LST_OBS_CON_STAGE_TURNIN:

		// Direction control
		if (lst_obs_convoy_wallDirection == LST_OBS_CON_WALLDIRECTION_LEFT)
			LST_Steering_Lock(LST_OBS_CON_STEERINGLOCK_RIGHT);
		else
			LST_Steering_Lock(LST_OBS_CON_STEERINGLOCK_LEFT);

		LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM);

		if (lst_obs_convoy_turnTimer <= 0)
		{

			// Jump to next
			lst_obs_convoy_stage = LST_OBS_CON_STAGE_FINDIN;


		}
		else
		{

			lst_obs_convoy_turnTimer--;

		}

		break;

	case LST_OBS_CON_STAGE_FINDIN:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		if (lst_control_line_no > 0)
		{

			lst_obs_convoy_stage = LST_OBS_CON_STAGE_ATTACH;

			// Init variable
			lst_obs_convoy_attachTimer = LST_OBS_CON_ATTACHTIMER_PERIOD;

		}


		break;

	case LST_OBS_CON_STAGE_ATTACH:

		// Move a bit forward to stabilise so car can start steering watch

		LST_Steering_Follow();

		LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM);

		if (lst_obs_convoy_attachTimer <= 0)
		{

			lst_obs_convoy_stage = LST_OBS_CON_STAGE_FOLLOW;

			// Init variables
			lst_obs_con_steeringCount = 0;
			lst_obs_con_steeringState = 0;
			lst_obs_con_steeringHigh = 0;
			lst_obs_con_steeringLow = 0;

		}
		else
		{

			lst_obs_convoy_attachTimer--;

		}

		break;

	case LST_OBS_CON_STAGE_FOLLOW:

		LST_Steering_Follow();

		// Hysteresis speed control

		if (lst_obs_convoy_follow_state)
			LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM);
		else LST_Movement_Stop();

		// Moving forward
		if (LST_Sharp_GetFrontDistance_mm() < LST_OBS_CON_SHARP_FOLLOW_LOW)
		{

			lst_obs_convoy_follow_state = 0;

		}

		if (LST_Sharp_GetFrontDistance_mm() > LST_OBS_CON_SHARP_FOLLOW_HIGH)
		{

			lst_obs_convoy_follow_state = 1;

		}


		// Find exit point, watch turns

		// Use other control variables for proper steering watch operation
		lst_control_steeringP = LST_OBS_CON_STEERING_P;
		lst_control_steeringD = LST_OBS_CON_STEERING_D;

		// Watch straight
		if (abs(lst_control_steering) < LST_OBS_CON_STEERINGWATCH_LOW)
		{

			lst_obs_con_steeringLow++;

			// Detected straight
			if (lst_obs_con_steeringLow > LST_OBS_CON_STEERING_THRESHOLD)
			{

				// If previous state was turn, then increase
				if (lst_obs_con_steeringState)
				{

					lst_obs_con_steeringCount++;
					lst_obs_con_steeringState = 0;

				}

			}

		}
		else
		{

			lst_obs_con_steeringLow = 0;

		}

		// Watch turn
		if (abs(lst_control_steering) > LST_OBS_CON_STEERINGWATCH_HIGH)
		{

			lst_obs_con_steeringHigh++;

			// Detected turn
			if (lst_obs_con_steeringHigh > LST_OBS_CON_STEERING_THRESHOLD)
			{

				// If previous state was straight, then increase
				if (!lst_obs_con_steeringState)
				{

					lst_obs_con_steeringCount++;
					lst_obs_con_steeringState = 1;

				}

			}

		}
		else
		{

			lst_obs_con_steeringHigh = 0;

		}

		// If enough turns have elapsed, exit
		if (lst_obs_con_steeringCount >= LST_OBS_CON_STEERING_COUNT)
		{

			lst_obs_convoy_stage = LST_OBS_CON_STAGE_TURNOUT;

			// Init variable
			lst_obs_convoy_turnTimer = LST_OBS_CON_TURNTIMER_PERIOD;

		}

		break;

	case LST_OBS_CON_STAGE_TURNOUT:

		lst_control_steeringP = LST_OBS_STEERING_P;
		lst_control_steeringD = LST_OBS_STEERING_D;

		// Direction control
		if (lst_obs_convoy_wallDirection == LST_OBS_CON_WALLDIRECTION_LEFT)
			LST_Steering_Lock(LST_OBS_CON_STEERINGLOCK_LEFT);
		else
			LST_Steering_Lock(LST_OBS_CON_STEERINGLOCK_RIGHT);

		LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM);

		if (lst_obs_convoy_turnTimer <= 0)
		{

			// Jump to next
			lst_obs_convoy_stage = LST_OBS_CON_STAGE_FINDOUT;

		}
		else
		{

			lst_obs_convoy_turnTimer--;

		}

		break;

	case LST_OBS_CON_STAGE_FINDOUT:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		if (lst_control_line_no > 0)
		{

			lst_obs_convoy_stage = LST_OBS_CON_STAGE_LEAVELINE;

		}

		break;

	case LST_OBS_CON_STAGE_LEAVELINE:

		LST_Steering_Follow();

		LST_Movement_Move(LST_MOVEMENT_FB_MEDIUM);

		if (LST_Distance_Measure_mm(LST_OBS_CON_DISTANCE_LEAVELINE))
		{

			lst_obs_convoy_stage = LST_OBS_CON_STAGE_EXIT;

		}

		break;

	case LST_OBS_CON_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

		break;

	}

}

/**
 * @brief Mode for the barrel
 */
static void LST_Obs_Barrel(){
  // ToDo

	// TEST ONLY
	LST_Steering_Sharp(0, LST_OBS_RND_SHARP_SIDE_DIST);
	LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

	return; // TODO REMOVE AFTER TESTS

	// Barrel code

	switch (lst_obs_barrel_stage)
	{

	case LST_OBS_BRL_STAGE_APPROACH:

		// Init variable
		lst_obs_barrel_approachTimer = LST_OBS_BRL_APPROACHTIMER_PERIOD;

		// Next stage
		lst_obs_barrel_stage = LST_OBS_BRL_STAGE_APPROACHRAMP;

		break;

	case LST_OBS_BRL_STAGE_APPROACHRAMP:

		// Approach slowly until front is stuck

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		LST_Steering_Follow();

		if (lst_obs_barrel_approachTimer <= 0)
		{

			// Jump to next stage
			lst_obs_barrel_stage = LST_OBS_BRL_STAGE_ONTHERAMP;

			// Init variable
			lst_obs_barrel_rampTimer = LST_OBS_BRL_RAMPTIMER_PERIOD;

		}
		else
		{

			lst_obs_barrel_approachTimer--;

		}

		break;

	case LST_OBS_BRL_STAGE_ONTHERAMP:

		// Power through the ramp

		LST_Movement_Move_EncoderLess(LST_MOVEMENT_FAST);

		LST_Steering_Lock(0);

		if (lst_obs_barrel_rampTimer <= 0)
		{

			lst_obs_barrel_stage = LST_OBS_BRL_STAGE_INTHETUBE;

		}
		else
		{

			lst_obs_barrel_rampTimer--;

		}

		break;

	case LST_OBS_BRL_STAGE_INTHETUBE:

		// Quickly through the tube

		LST_Movement_Move(LST_MOVEMENT_FB_FAST);

		break;

	case LST_OBS_BRL_STAGE_OUTGOING:

		// On the exit ramp

		break;

	case LST_OBS_BRL_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

		break;

	}

}

/**
 * @brief Mode for the roundabout
 */
static void LST_Obs_Roundabout(){
	// ToDo TEST

  LST_Steering_Follow();
  LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

  switch (lst_obs_roundabout_stage){
  case LST_OBS_RND_STAGE_APPROACH:
    // Approach the roundabout with Sharps
    if(LST_Sharp_GetFrontDistance_mm() < LST_OBS_RND_SHARP_DIST_APPROACH){ // 400
      // Stop, wait for Infra
      LST_Movement_Stop();
      // Check if Infra is available
      if(lst_infra_is_available){
        lst_obs_roundabout_stage = LST_OBS_RND_STAGE_FIRST_TURN;
        lst_obs_roundabout_turnInTimer = LST_OBS_RND_TURNINTIMER_PERIOD;
        lst_obs_roundabout_direction = LST_Infra_Get_Direction();
        lst_obs_roundabout_exit = LST_Infra_Get_Exit();
      }
      else{
        lst_obs_roundabout_cntr++;
        if(lst_obs_roundabout_cntr>LST_OBS_RND_INFRA_ERROR_MAX){ // 200
          // Set default values and hope for the best
          lst_obs_roundabout_stage = LST_OBS_RND_STAGE_FIRST_TURN;
          lst_obs_roundabout_turnInTimer = LST_OBS_RND_TURNINTIMER_PERIOD;
          lst_obs_roundabout_direction = LST_INFRA_DIR_LEFT;
          lst_obs_roundabout_exit = LST_INFRA_EXIT_THREE;
        }
      }
    }
    break;

  case LST_OBS_RND_STAGE_FIRST_TURN:
    // Go slowly
    LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

    // Check direction for the turn
    switch (lst_obs_roundabout_direction){
    case LST_INFRA_DIR_LEFT:
      // Lock steering to the left
      LST_Steering_Lock(LST_OBS_RND_FIRST_LEFT_TURN_VALUE);  // -1000
      break;
    case LST_INFRA_DIR_RIGHT:
      // Lock steering to the right
      LST_Steering_Lock(LST_OBS_RND_FIRST_RIGHT_TURN_VALUE); // +1000
      break;
    }

    // Try with time measurement
    if (lst_obs_roundabout_turnInTimer <= 0)
    {
    	lst_obs_roundabout_stage = LST_OBS_RND_STAGE_TRAVEL;
    }
    else
    {
    	lst_obs_roundabout_turnInTimer--;
    }

    /*
    // Go forward like 30cm
    if (!LST_Distance_Measure_mm(LST_OBS_RND_FIRST_TURN_DISTANCE)){
      LST_Movement_Move(LST_MOVEMENT_FB_SLOW);
    }
    else{
    // Switch to travel mode
      lst_obs_roundabout_stage = LST_OBS_RND_STAGE_TRAVEL;
    }
    */
    break;

  case LST_OBS_RND_STAGE_TRAVEL:
    // Go slowly
    LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

    // Based on direction, control steering to left/right sharp sensor
    switch (lst_obs_roundabout_direction){
    case LST_INFRA_DIR_LEFT:
      // Control to left Sharp sensor
      LST_Steering_Sharp(0, LST_OBS_RND_SHARP_SIDE_DIST);
      break;
    case LST_INFRA_DIR_RIGHT:
      // Control to right Sharp sensor
      LST_Steering_Sharp(1, LST_OBS_RND_SHARP_SIDE_DIST);
      break;
    }

    // Based on exit, travel some distance
    switch (lst_obs_roundabout_exit){
    case LST_INFRA_EXIT_ONE:
      if (LST_Distance_Measure_mm(LST_OBS_RND_TRAVEL_DISTANCE_ONE)){
        lst_obs_roundabout_stage = LST_OBS_RND_STAGE_LAST_TURN;
      }
      break;
    case LST_INFRA_EXIT_TWO:
      if (LST_Distance_Measure_mm(LST_OBS_RND_TRAVEL_DISTANCE_TWO)){
        lst_obs_roundabout_stage = LST_OBS_RND_STAGE_LAST_TURN;
      }
      break;
    case LST_INFRA_EXIT_THREE:
      if (LST_Distance_Measure_mm(LST_OBS_RND_TRAVEL_DISTANCE_THREE)){
        lst_obs_roundabout_stage = LST_OBS_RND_STAGE_LAST_TURN;
      }
      break;
    }
    break;

  case LST_OBS_RND_STAGE_LAST_TURN:
    // Go slowly
    LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

    // Based on direction, fix steering
    switch (lst_obs_roundabout_direction){
    case LST_INFRA_DIR_LEFT:
      LST_Steering_Lock(LST_OBS_RND_LAST_LEFT_TURN_VALUE);  // -1000
      break;
    case LST_INFRA_DIR_RIGHT:
      LST_Steering_Lock(LST_OBS_RND_LAST_RIGHT_TURN_VALUE); // +1000
      break;
    }

    // Travel until the car hits a line
    if(lst_control_line_no == 1){
      lst_obs_roundabout_stage = LST_OBS_RND_STAGE_FINISH;
    }
    break;

  case LST_OBS_RND_STAGE_FINISH:
    // Line follow, Go slowly
    LST_Steering_Follow();
    LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

    // Go forward like 400mm, then go into SEARCH mode
    if (LST_Distance_Measure_mm(LST_OBS_RND_FINISH_DISTANCE)){ // 400
      // Search mode
      lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
    }
    break;
  }
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
			lst_obs_train_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

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
				lst_obs_train_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

			}


		break;

	case LST_OBS_TRA_STAGE_COUNT:

		LST_Steering_Lock(0);

		LST_Movement_Stop();

		// If sufficient time has elapsed without a car seen, then
		// it is safe to proceed, else wait more
		if (lst_obs_train_lastCarTimer <= 0)
		{

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSS;

		}
		else
		{

			// Reset counter if another car appears
			if (LST_Sharp_GetFrontDistance_mm() < LST_OBS_TRA_SHARP_DIST_CAR)
			{

				lst_obs_train_lastCarTimer = LST_OBS_TRA_LASTCARTIMER_PERIOD;

			}
			else
			{

				lst_obs_train_lastCarTimer--;

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
			lst_obs_train_crossingTimer = LST_OBS_TRA_CROSSINGTIMER_PERIOD;

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSSING;

		}

		break;

	case LST_OBS_TRA_STAGE_CROSSING:

		LST_Steering_Lock(0);

		LST_Movement_Move(LST_MOVEMENT_FB_SLOW); // TODO MEDIUM !!!!!!!!!!!!!!!

		// Delay a bit to get to the no line part
		if (lst_obs_train_crossingTimer <= 0)
		{

			// Jump to next
			lst_obs_train_stage = LST_OBS_TRA_STAGE_CROSSED;

		}
		else
		{

			lst_obs_train_crossingTimer--;

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
  // ToDo TEST

  // Go forward like 30cm
  if (!LST_Distance_Measure_mm(LST_OBS_END_DISTANCE_MM))
  {

    LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

  }
  else
  {

  	// Stop
  	LST_Movement_Stop();
  	LST_Steering_Lock(0);

  	lst_obs_lap_mode = LST_OBS_MODE_NO_CONTROL;

  }

}

/**
 * @brief Resets the state machine
 */
static void LST_Obs_ResetStateMachine(){
  lst_obs_lap_mode             = LST_OBS_LAP_MODE_START;

  lst_obs_corner_stage = 0;
  lst_obs_train_stage = 0;
  lst_obs_drone_stage = 0;
  lst_obs_convoy_stage = 0;
  lst_obs_roundabout_stage = 0;
  lst_obs_barrel_stage = 0;

  lst_obs_roundabout_cntr = 0;

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

/**
 * @brief DIP handler at the start
 */
void LST_Obs_DIP_Handler_Start(){
  if(lst_dip_settings[1] == 1){
    lst_obs_mode = LST_OBS_MODE_BT;
  }
  else{
    lst_obs_mode = LST_OBS_MODE_LAP;
  }

  if(lst_dip_settings[2] == 1){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
  }
  else{
    lst_obs_lap_mode = LST_OBS_LAP_MODE_START;
  }
}
