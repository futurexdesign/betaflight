/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>

#include <platform.h>

#ifdef TARGET_CONFIG

#include "common/axis.h"

#include "config/feature.h"

#include "drivers/pwm_esc_detect.h"

#include "fc/config.h"

#include "flight/mixer.h"
#include "flight/pid.h"

#include "rx/rx.h"

#include "io/serial.h"

#include "telemetry/telemetry.h"

#include "sensors/battery.h"

#include "hardware_revision.h"

#ifdef BRUSHED_MOTORS_PWM_RATE
#undef BRUSHED_MOTORS_PWM_RATE
#endif

#define BRUSHED_MOTORS_PWM_RATE 32000           // 32kHz

// alternative defaults settings for AlienFlight targets
void targetConfiguration(void)
{
    if (hardwareMotorType == MOTOR_BRUSHED) {
        motorConfigMutable()->dev.motorPwmRate = BRUSHED_MOTORS_PWM_RATE;
        pidConfigMutable()->pid_process_denom = 1;
    }

    if (hardwareRevision == AFF4_REV_1) {
        rxConfigMutable()->serialrx_provider = SERIALRX_SPEKTRUM2048;
        rxConfigMutable()->spektrum_sat_bind = 5;
        rxConfigMutable()->spektrum_sat_bind_autoreset = 1;
    } else {
        rxConfigMutable()->serialrx_provider = SERIALRX_SBUS;
        rxConfigMutable()->sbus_inversion = 0;
        serialConfigMutable()->portConfigs[findSerialPortIndexByIdentifier(SERIALRX_UART)].functionMask = FUNCTION_TELEMETRY_FRSKY | FUNCTION_RX_SERIAL;
        telemetryConfigMutable()->telemetry_inversion = 0;
        batteryConfigMutable()->voltageMeterSource = VOLTAGE_METER_ADC;
        batteryConfigMutable()->currentMeterSource = CURRENT_METER_ADC;
        featureSet(FEATURE_TELEMETRY);
    }

    pidProfilesMutable(0)->pid[PID_ROLL].P = 53;
    pidProfilesMutable(0)->pid[PID_ROLL].I = 45;
    pidProfilesMutable(0)->pid[PID_ROLL].D = 52;
    pidProfilesMutable(0)->pid[PID_PITCH].P = 53;
    pidProfilesMutable(0)->pid[PID_PITCH].I = 45;
    pidProfilesMutable(0)->pid[PID_PITCH].D = 52;
    pidProfilesMutable(0)->pid[PID_YAW].P = 64;
    pidProfilesMutable(0)->pid[PID_YAW].D = 18;

    *customMotorMixerMutable(0) = (motorMixer_t){ 1.0f, -0.414178f,  1.0f, -1.0f };    // REAR_R
    *customMotorMixerMutable(1) = (motorMixer_t){ 1.0f, -0.414178f, -1.0f,  1.0f };    // FRONT_R
    *customMotorMixerMutable(2) = (motorMixer_t){ 1.0f,  0.414178f,  1.0f,  1.0f };    // REAR_L
    *customMotorMixerMutable(3) = (motorMixer_t){ 1.0f,  0.414178f, -1.0f, -1.0f };    // FRONT_L
    *customMotorMixerMutable(4) = (motorMixer_t){ 1.0f, -1.0f, -0.414178f, -1.0f };    // MIDFRONT_R
    *customMotorMixerMutable(5) = (motorMixer_t){ 1.0f,  1.0f, -0.414178f,  1.0f };    // MIDFRONT_L
    *customMotorMixerMutable(6) = (motorMixer_t){ 1.0f, -1.0f,  0.414178f,  1.0f };    // MIDREAR_R
    *customMotorMixerMutable(7) = (motorMixer_t){ 1.0f,  1.0f,  0.414178f, -1.0f };    // MIDREAR_L
}
#endif
