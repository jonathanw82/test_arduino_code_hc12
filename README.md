# test_arduino_code_hc12

The Arduino in the top of the hive (Transmitter) will send temperature, humidity, Sound frequency and battery voltage to the Arduino (Receiver) at the bottom of the hive wirelessly via the HC-12 transceivers.  

The Transmitter will sleep for a set period of time then wake, it will then send the capital A via a serial print function, if the transmitter has not received a capital B if will try 30 times then go back to sleep, this is a fail-safe to protect the batteries if the receiver has gone down. If the receiver wakes as intended, on receipt of the capital A it will transmit a capital B, the transmitter will then collect the data and transmit it back to the receiver vis a serial print function, then go back to sleep.

Boot order of Transmitter.
On Wake, set the sleep delay to zero, wake variable to true, detect the sound frequency, battery voltage, temperature, humidity. Then establish a connection and send data.


Why am I metering the sound frequencies in the beehive?.  There is scientific data that correlates changes in sound frequency inside the beehive when the behaviour of the bees change, this data can be used to predict swarming or intent to swarm, the doc can be found here[https://www.researchgate.net/publication/229224190_Monitoring_of_swarming_sounds_in_bee_hives_for_early_detection_of_the_swarming_period]. It also relates to where the hive is queenless.

Quote form the doc ( Swarming is indicated by an increase in the power spectral density at about 110 Hz; approaching to swarm the sound augmented in amplitude and frequency to 300 Hz, occasionally a rapid change occurred from 150 Hz to 500 Hz. Another finding indicating the initiation of a swarming period is the rise in temperature from 33 °C to 35 °C until the actual time of swarming when the temperature starts dropping to 32 °C. With more activity, ventilation from bee wings causes a drop of temperature.) with this I mind is the reason we are also collecting the internal hive temperature.


Credits

Voltage metering, https://create.arduino.cc/projecthub/next-tech-lab/voltmeter-using-arduino-00e7d1


https://www.researchgate.net/publication/229224190_Monitoring_of_swarming_sounds_in_bee_hives_for_early_detection_of_the_swarming_period

http://www.beehacker.com/wp/?page_id=103