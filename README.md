# Automated Aquarium Controller Portfolio

**Arlen Johnson** · Grade 12, STEM Pathway · Fort Bend ISD, Texas
Intended major: Electrical Engineering

## Why I started this

In middle school I had a planted aquarium. I loved setting it up, but I could not keep it healthy. The plants and fish depended on light, CO2, and water conditions, and I usually found out something was wrong only after it got bad. After many tries I stopped.

In tenth grade I built a VEX vehicle with a sonar sensor. It kept checking the distance to the wall and fixing its speed so it would not crash. That idea stuck with me. My aquarium never had anything like that.

In eleventh grade I bought an ELEGOO UNO R3 Arduino kit to learn electronics on my own. My goal is an aquarium controller that checks the water and lights by itself, so I don't find problems too late. Each project below is a part I need for it.

## Finished projects

| # | Project | What it does | Link |
|---|---|---|---|
| 1 | LED Binary Counter | Counts in binary with LEDs. My first time controlling outputs with code. | [Project 1](Project1_LED_Binary_Counter) |
| 2 | Environment Monitor | Reads sensors and shows the values on an LCD. | [Project 2](Project2_Environment_Monitor) |
| 3 | PWM Motor Controller | Changes DC motor speed with PWM and a motor driver. | [Project 3](Project3_PWM_Motor_Controller) |
| 4 | Ultrasonic Radar | Measures distance with an ultrasonic sensor, like my VEX sonar. | [Project 4](Project4_Ultrasonic_Radar) |
| 5 | Traffic Light | Runs lights through a timed order of steps. | [Project 5](Project5_Traffic_Light) |
| 6 | Water Temperature Logger | Saves water temperature and time to an SD card, no computer needed. | [Project 6](Project6_Water_Temperature_Logger) |
| 9 | Keypad Combination Lock | Opens a servo "bolt" with a 4-digit code and locks the keypad for 30 s after three wrong tries. | [Project 9](Project9_Combination_Lock) |

## Still to do

| # | Project | Status | Why I need it |
|---|---|---|---|
| 7 | Grow Light | Planned | Turn a light on and off on a schedule with a relay and a DS3231 clock |
| 8 | 4-Relay Control | Planned | Switch more than one device from one board |
| — | **Aquarium Controller (Capstone)** | Planned | Put the sensors, timing, and switching together in one system |

## Things that surprised me

- **Project 3:** At low PWM values the motor just hummed. It needed a certain amount of power before it would start turning.
- **Project 6:** Some times in my log file went backward. The clock was resetting every time the Arduino restarted, so the data looked fine but was wrong.
- **Project 9:** The upload got stuck and the board went dark. A rubber band left on the servo arm jammed it, and the stalled servo pulled more power than USB could give. My pumps and lights will need their own supply.

My next step is Project 7, the grow light.

---

Each project folder has its own README, report, code, circuit diagrams, and photos.
