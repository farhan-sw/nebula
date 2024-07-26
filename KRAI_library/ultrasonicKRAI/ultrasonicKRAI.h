#ifndef ULTRASONIC_KRAI_H
#define ULTRASONIC_KRAI_H

#include "mbed.h"

class ultrasonicKRAI {
    public:
        /** !
         * @typedef UltrasonicState
         *          State of the ultrasonic
         * 
         * @param TRIGGER Ultrasonic is starting the trigger signal
         * @param WAITING_ECHO Ultrasonic is waiting for echo signal to be high
         * @param READING_ECHO Ultrasonic is reading the echo signal
        */
        typedef enum UltrasonicState {
            TRIGGER,
            WAITING_ECHO,
            READING_ECHO
        } UltrasonicState;

        /********************* CONSTRUCTOR *********************/

        /** 
         * @brief Create ultrasonic HC-SR04 interface
         * 
         * @param trigger Trigger pin
         * @param echo Echo pin
         * @param *dist Store the distance
        */
        ultrasonicKRAI(PinName trigger, PinName echo, float *dist);

        /********************* GETTER *********************/

        /** 
         * @brief Get the state of the ultrasonic
         * 
         * @param None
         * 
         * @return The state of the ultrasonic
        */
        UltrasonicState getState();

        /** 
         * @brief Get the echo signal
         * 
         * @param None
         * 
         * @return The echo signal
        */
        int getEcho();

        /********************* SETTER *********************/

        /** 
         * @brief Set the state of the ultrasonic
         * 
         * @param state The state of the ultrasonic
        */
        void setState(UltrasonicState state);

        /********************* METHODS *********************/
        
        /** Generate a pulse and store the distance
         * 
         * @param None
         * 
         * Warning: undone, still need to use naive threading
        */
        void sensor();

        /**
         * @brief Use wait_us to do triggering
         * 
         * @param None
        */
        void trigger();

        /**
         * @brief Waiting for echo signal to be high
         * 
         * @param None
        */
        void waitingEcho();

        /**
         * @brief Waiting for echo signal to be low, or waiting the transmitted signal to be back
         * 
         * @param None
        */
        void readingEcho();

        /**
         * @brief Check if the distance is readable
         * 
         * @param None
         * 
         * @return True if the distance is readable, false if not
        */
        bool readable();
        
    private:
        /* PIN */
        DigitalOut _trigger;
        DigitalIn _echo;

        /* VARIABLES */
        float *_dist;
        UltrasonicState _state;
        bool _readable;

        /* TIME SAMPLING */
        int read_time, trigger_time;
};

#endif