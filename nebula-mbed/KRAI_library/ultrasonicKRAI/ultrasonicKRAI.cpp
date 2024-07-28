#include "ultrasonicKRAI.h"

#define timer_read_f(x)     chrono::duration<float>((x).elapsed_time()).count()
#define timer_read_s(x)     chrono::duration_cast<chrono::seconds>((x).elapsed_time()).count();
#define timer_read_ms(x)    chrono::duration_cast<chrono::milliseconds>((x).elapsed_time()).count()
#define timer_read_us(x)    (x).elapsed_time().count()

ultrasonicKRAI::ultrasonicKRAI(PinName trigger, PinName echo, float *dist) : _trigger(trigger), _echo(echo) {
    this->_state = TRIGGER;
    this->_dist = dist;
    this->_readable = false;
};

ultrasonicKRAI::UltrasonicState ultrasonicKRAI::getState() {
    return this->_state;
}

int ultrasonicKRAI::getEcho() {
    return this->_echo.read();
}

void ultrasonicKRAI::setState(UltrasonicState state) {
    this->_state = state;
}

void ultrasonicKRAI::trigger() {
    this->_readable = false;

    this->_trigger.write(1);
    wait_us(10);
    this->_trigger.write(0);

    this->trigger_time = us_ticker_read();
    this->_state = WAITING_ECHO;
}

void ultrasonicKRAI::waitingEcho() {
    if (this->_echo.read() == 1) {
        this->read_time = us_ticker_read();

        this->_state = READING_ECHO;
    } else {
        if (us_ticker_read() - this->trigger_time > 100000) {
            // printf("Echo Timeout\n");

            this->_state = TRIGGER;
        }
    }
}

void ultrasonicKRAI::readingEcho() {
    if (this->_echo.read() == 0) {
        this->_readable = true;

        *(this->_dist) = (us_ticker_read() - read_time) * 343.2f / 2.0f / 1000000 * 100;
        
        this->_state = TRIGGER;
    }
}

bool ultrasonicKRAI::readable() {
    return this->_readable;
}

void ultrasonicKRAI::sensor() {
    switch (this->_state) {
        case TRIGGER:
            this->trigger();
            break;
        case WAITING_ECHO:
            this->waitingEcho();
            break;
        case READING_ECHO:
            this->readingEcho();
            break;
    }
}