# Project 9 – Keypad Combination Lock

**4x4 membrane keypad · SG90 servo · 2 LEDs · active buzzer · Arduino UNO R3**

October 15, 2026

I type a 4-digit code on the keypad and press `#`. If it's right, the servo turns 90° like a bolt sliding open, the green LED comes on, and the buzzer beeps twice. After 5 seconds it locks itself again. Three wrong codes in a row lock the keypad out for 30 seconds.

![Project 9 build](Project_9_CombinationLock_Photo.jpeg)

▶️ [Watch the test video](Project_9_CombinationLock_Video.mp4)

## What I noticed

- **The code worked almost the first time. The hardware didn't.** When I connected the servo, the upload got stuck and the board's LEDs went dark. The cause was a rubber band I'd left on the servo arm from Project 4. It jammed the arm, and a stuck SG90 can draw around 650–800 mA. A laptop USB port gives about 500 mA, so the Arduino lost power.
- **At first the relock looked too fast: 4.75 s instead of 5 s.** Then I worked out why. The timer starts *before* the two unlock beeps (100 + 50 + 100 = 250 ms), but "Unlocked" prints *after* them. So 4.75 + 0.25 ≈ 5.0 s, exactly as designed. Checking the timestamps, not just watching the lock, is what proved the timer was right.
- **My first "clear" test didn't prove anything.** The log showed `cleared` with no `**` in front of it, so there were no digits to clear. I redid it and got `** cleared`, then an unlock.
- **Stage 1 printed only 14 keys.** It missed 4 and 6. Both are in Row 2, but 5 (also Row 2) worked, and their columns worked for other keys. So it wasn't the wiring; I had just skipped them. When I pressed them again, all 16 keys printed.

## Why this matters for my aquarium

My aquarium failed in Grades 7–8 because I couldn't control light, CO₂, temperature and water chemistry at the same time. The capstone controller needs a way to change settings, like the target temperature or the light schedule, without plugging in a laptop. A keypad is the simplest way to type numbers.

A device that switches a heater also shouldn't let anyone walking past change its settings, so the lock matters too. The A, B, C and D keys are already saved for the capstone's settings menu.

The biggest lesson was about power. A motor under load can't share USB power with the controller. My pumps, heater and lights will need their own supply, switched through relays. That's what Projects 7 and 8 are for.

## How it works

The lock is a state machine, like the traffic light in Project 5. The difference is that key presses move it between states, and timers only bring it back.

| State | What it means |
|---|---|
| LOCKED | Servo at 0°, waiting for a code |
| UNLOCKED | Servo at 90°, green LED on, relocks after 5 s or `*` |
| LOCKOUT | Red LED on, all keys ignored for 30 s |

![State diagram](Project_9_CombinationLock_State_Diagram.png)

- **Keypad scanning:** 16 keys use only 8 wires (4 rows + 4 columns). The Keypad library sets one row LOW at a time and checks which column also reads LOW. Sixteen separate buttons would need 16 pins.
- **Attempt limit:** a 4-digit code has 10,000 combinations. With 3 tries per 30-second lockout, trying them all takes about 10,000 ÷ 3 × 30 s ≈ 28 hours. That isn't real security, but it shows the idea: slowing down guessing matters as much as having a code.
- **Hiding the code:** Serial Monitor prints `*` for each digit, because printing the real numbers would defeat the point of a lock.

## Circuit

| Part | Arduino pin | Notes |
|---|---|---|
| Keypad rows (ribbon pins 1–4) | D9, D8, D7, D6 | Library turns on internal pull-ups, so no resistors |
| Keypad columns (ribbon pins 5–8) | D5, D4, D3, D2 | |
| Servo signal (orange) | D10 | Red to 5V, brown to GND |
| Green LED | A0 | Through 220 Ω to GND |
| Red LED | A1 | Through 220 Ω to GND |
| Active buzzer + | A2 | − to GND |

The keypad uses D2–D9, so I moved the LEDs and buzzer to A0–A2. Analog pins work as normal digital outputs. Each LED gets about (5 V − 2 V) ÷ 220 Ω ≈ 13.6 mA, which is under the 20 mA per pin that Arduino recommends.

| Breadboard (Fritzing) | Schematic (Fritzing) |
|---|---|
| ![Breadboard](Project_9_CombinationLock_Breadboard.jpg) | ![Schematic](Project_9_CombinationLock_Schematic.jpg) |

## Test results

| Test | What I did | Serial Monitor evidence | Result |
|---|---|---|---|
| Correct code | `1234` then `#` | Unlocked 21:41:12.855, Locked 21:41:17.608 | Pass |
| Wrong code | `1111` then `#` | Wrong code 1/3, red LED flashed 3 times | Pass |
| Lockout | 3 wrong codes in a row | Wrong code 3/3 at 21:40:35.089, Lockout over at 21:41:05.199 (30.1 s) | Pass |
| Clear | `12`, `*`, then `1234 #` | `** cleared`, then `****`, then Unlocked | Pass |

These times come from Serial Monitor, so the computer adds them, not the Arduino. They're good enough to check a 5-second and a 30-second timer, but they aren't exact measurements.

| Stage 1: all 16 keys | Stage 2: full lock |
|---|---|
| ![Stage 1 Serial Monitor](Project_9_CombinationLock_Stage_1_Serial.png) | ![Stage 2 Serial Monitor](Project_9_CombinationLock_Stage_2_Serial.png) |

## Problems I ran into

1. **The upload got stuck and the board went dark.** It was the servo jammed by the leftover rubber band (see above). I removed the servo and the upload worked, so I knew the rest of the circuit was fine. I took the band off, checked that the arm turned freely by hand, and reconnected it.
2. **Wrong COM port.** The IDE was set to COM4, but my board was on COM6. Now I check the port before every upload.
3. **Serial Monitor wouldn't reconnect.** Picking the right port didn't fix it. Closing and reopening the Arduino IDE did, because that fully let go of the port.

## Files

| File | What it is |
|---|---|
| `CombinationLock.ino` | Final sketch |
| `Project_9_CombinationLock_R1.docx` | Full project report |
| `Project_9_CombinationLock_Video.mp4` | Test video (unlock, wrong code, lockout) |
| `Project_9_CombinationLock_Photo.jpeg` | Photo of the real build |
| `Project_9_CombinationLock_Breadboard.jpg`, `Project_9_CombinationLock_Schematic.jpg` | Fritzing breadboard and schematic |
| `Project_9_CombinationLock_State_Diagram.png` | State diagram |
| `Project_9_CombinationLock_Stage_1_Serial.png`, `Project_9_CombinationLock_Stage_2_Serial.png` | Serial Monitor screenshots |

## What I'd do next time

- Let the user change the password with the `A` key.
- Remove the blocking `delay()` in the wrong-code alarm. That's fine here, but not in the aquarium controller, which has to keep watching the temperature.
- Use a `char` array instead of `String`, which can break up memory on a board with only 2 KB of RAM.
- Give the servo its own power supply.

**Libraries:** Keypad (Mark Stanley and Alexander Brevig), Servo (built into the Arduino IDE).

**Help:** ELEGOO starter kit tutorials for the keypad and servo.
