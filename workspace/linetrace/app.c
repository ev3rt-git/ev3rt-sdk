/**
 * This sample program shows a PID controller for line following.
 *
 * Robot construction: Educator Vehicle
 *
 * References:
 * http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdf
 * http://thetechnicgear.com/2014/03/howto-create-line-following-robot-using-mindstorms/
 */

#include "ev3api.h"
#include "app.h"
#include "stdlib.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

const int color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C;

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
#if !defined(BUILD_MODULE)
        syslog(LOG_NOTICE, "Back button clicked.");
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
#endif
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
        break;
    }
}

void main_task(intptr_t unused) {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    // Configure sensors
    ev3_sensor_config(color_sensor2, COLOR_SENSOR);
    ev3_sensor_config(color_sensor3, COLOR_SENSOR);

    /**
     * PID controller
     */
    float lasterror = 0, integral = 0;
    while (1) {
        float error = ev3_color_sensor_get_reflect(color_sensor3) - ev3_color_sensor_get_reflect(color_sensor2);
        integral = error + integral * 0.5;
        float steer = 0.07 * error + 0.5 * integral + 1 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 15, steer);
        lasterror = error;
        tslp_tsk(1);
    }
}
