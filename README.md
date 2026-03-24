# 2-DOF Motion Simulator: The "Platform Mover" Build Guide

This repository contains the documentation and design decisions for a high-performance, DIY 2-DOF (Degree of Freedom) motion simulator, specifically optimized for flight and space simulations (VR-ready). The estimated cost of building one yourself will fall around 1600 EUR for total material and will give a very solid biuld solution, full aluminum profiles, fully adjustable and customizable, rock solid with about Combined Rated Torque: 22 N.m and Combined Peak Torque: ~45 N.m. Mechanical Advantage: The 80mm lever choice prioritizes high-frequency response (road texture/vibrations) over raw lifting capacity, making it ideal for GT3 and Formula style simulation where rapid weight transfer cues are essential.

# If you would like to buy a complete solution from me and you are in Europe (France) just send me an email at: luisclement@gmail.com

---

## 1. Mechanical Design Decisions
### The "Platform Mover" Configuration
***Decision:** We chose a **"Platform Mover"** design where the motors are mounted at the base and push/pull against the back of the seat at shoulder height.
* **Reasoning:** By mounting the rods at shoulder height, we gain a massive mechanical advantage (leverage), allowing the motors to move the user with less strain and higher speed.
* ]**Integrated Platform:** I decided to make the moving platform long enough to hold the pedals, HOTAS, and wheel together as one "closed-top" unit]. This ensures that your controls stay in the same position relative to your body as the seat moves, which is vital for immersion, especially in VR.

### The Pivot (U-Joint)
* **Decision:** Use a **Flanged Industrial Joint (212/1310)**.
***Reasoning:** Industrial flanged joints are superior to automotive steering joints because they feature flat mounting surfaces.This allows them to be bolted directly to aluminum profiles, ensuring a rigid center of gravity and removing the need for complex custom welding[cite: 114].

---

## 2. Hardware & Frame Specs
### Aluminum Profiles
* **Decision:** A mix of **4040** and **4080** T-slot aluminum profiles[cite: 129].
***Reasoning:** While 4040 is perfect for the base and platform rails, I chose **4080 (80mm x 40mm)** specifically for the **Gantry Uprights** behind the seat[cite: 147]. These uprights handle the most torque and force from the motor rods; 4080 ensures the frame is stiff enough to prevent "flex," which would otherwise absorb the motion cues and make the rig feel "mushy".

### Fasteners
***Decision:** **DIN 7380F (Flanged Button Head)** M8 bolts[cite: 155].
**Reasoning:** These bolts were chosen because the integrated flange acts like a built-in washer[cite: 155]. [cite_start]This provides a wider surface area to apply torque against the aluminum profile, ensuring the rig doesn't rattle loose during high-vibration flight maneuvers.

---

## 3. Electrical & Power System
### Motors
* [cite_start]**Decision:** **L-faster 24V 350W Brushed Gear Motors (120 RPM)**[cite: 169].
* [cite_start]**Reasoning:** We prioritized the 120 RPM version over the 75 RPM version to ensure the rig is fast enough for "washout" effects and rapid turbulence[cite: 51, 64]. [cite_start]These motors feature a 17mm keyed shaft, which provides a reliable mechanical connection for the motor levers[cite: 191].

### Power Supply (PSU)
* [cite_start]**Decision:** **24V 50A (1200W)** switching power supply[cite: 159, 162].
* **Reasoning:** Motion rigs experience massive current "spikes" when motors instantly change direction. [cite_start]A 50A supply provides enough "headroom" so the system doesn't shut down or brown out during aggressive maneuvers[cite: 162, 167].

### Safety: The "Capacitor Fix"
* **Decision:** **63V 10,000uF capacitors** 
* **Reasoning:** Brushed motors act as generators when they are pushed by the weight of the rig (Regenerative Braking). This sends voltage spikes back toward the electronics. [cite_start]Placing large capacitors at the IBT_2 inputs "catches" these spikes immediately, protecting the power supply and the motor drivers from being fried[cite: 210].


---

## 4. Wiring & Connectivity
* [cite_start]**Wire Gauge:** **12 AWG** for all power-carrying lines[cite: 170].
* [cite_start]**Connectors:** **XT60 or XT90** connectors for the motor-to-driver interface[cite: 201].
* [cite_start]**Ferrules:** **4mm² (AWG 12) wire ferrules** for all terminal connections[cite: 217].
* [cite_start]**Reasoning:** I decided to **crimp** these ferrules rather than solder them[cite: 217, 218]. [cite_start]In a high-vibration environment like a motion rig, a mechanical crimp is less likely to fatigue and snap than a solder joint[cite: 218]. [cite_start]The XT-series connectors ensure a high-current, secure connection that won't vibrate loose[cite: 201].

---

## 5. Software Control
* **Controller:** **Arduino Uno**[cite: 203].
* **Motor Drivers:** Two **IBT_2 (BTS7960)** boards[cite: 203].
* **Logic:** The Arduino runs firmware (like SMC3) to receive telemetry from the PC and translate it into PWM signals for the IBT_2 drivers[cite: 203]. [cite_start]This setup allows for fine-tuning the "PID" (Proportional-Integral-Derivative) to ensure the seat moves smoothly without overshooting its target[cite: 203].

## 5.a Software Stack & Integration

The rig utilizes a dual-software approach to maximize telemetry resolution and peripheral support.

1. **Motion Engine: SimTools**
   * **Role:** Telemetry extraction and motor positioning.
   * **Interface:** Communicates with the Arduino via the SMC3 (Mode 2) protocol.
   * **Logic:** Configured with specific "Washout" filters for Flight Simming to protect the 350W motors during sustained banking/climbing maneuvers.

2. **Haptics & Dash: SimHub**
   * **Role:** Drives auxiliary systems (Dashboards, Wind Simulation, Bass Shakers).
   * **Logic:** Synchronized with SimTools via UDP relay to ensure zero-latency across all feedback systems.

---

## 6. Aluminum Profile BOM & Cut List

### Moving Platform (Top Frame)
| Part Name | Profile Type | Length | Qty | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| **Side Rails** | 4040 | 1400mm | 2 | Main longitudinal frame |
| **Cross Members** | 4040 | 500mm | 3 | Front, Mid, and Pivot support |
| **Gantry Uprights**| 4080 | 600mm | 2 | Rod attachment at shoulder height |
| **Gantry Cross** | 4040 | 500mm | 1 | Connects the top of the 4080s |

### Fixed Base (Bottom Frame)
| Part Name | Profile Type | Length | Qty | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| **Base Rails** | 4040 | 1000mm | 2 | Main floor contact rails |
| **Base Cross** | 4040 | 580mm | 3 | Front, Mid (Pivot), and Motor mounts |

### Hardware Summary
* **4040 Profile:** ~7.5 Meters total.
* **4080 Profile:** ~1.2 Meters total.
* **L-Brackets:** 20x for 4040 joints, 4x for 4080 joints.
* **Bolts:** M8 DIN 7380F (16mm length recommended for brackets).

## Summary of Main Components
| Component | Choice | Reason |
| :--- | :--- | :--- |
| **Platform** | Closed-top Seat Mover | High immersion & control stability |
| **Pivot** | Flanged Industrial Joint | Easy bolting & high load capacity |
| **Verticals** | 4080 Aluminum | Maximum stiffness for force transmission |
| **Motors** | 24V 350W 120RPM | Speed for rapid sim effects |
| **Protection** | 10,000uF Capacitors | Absorbs back-EMF voltage spikes |
| **Connectors** | XT90 / XT60 | Secure, high-current handling |

---

## VI. Bill of Materials (Shopping List)

| Category | Item | Specification |
| :--- | :--- | :--- |
| **Mechanical** | U-Joint | Industrial Flanged (212/1310) |
| **Mechanical** | Motors | 2x 24V 350W 120RPM (17mm Shaft) |
| **Hardware** | Profiles | 4040 (Base) & 4080 (Uprights) |
| **Hardware** | Bolts | DIN 7380F M8 Flanged |
| **Electrical** | PSU | 24V 50A 1200W |
| **Electrical** | Drivers | 2x IBT_2 (BTS7960) |
| **Electrical** | Protection | 2x 35V 10,000uF Capacitors |
| **Wiring** | Wire | 12 AWG (Silicone coated preferred) |
| **Connectors**| Connectors | XT60 (High Current) |
| **Logic** | Arduino | Arduino Uno R3 |


## VII. Electrical Wiring & Schematic

A motion simulator is a high-current environment. Following this wiring schema ensures that the "Back-EMF" from the motors doesn't destroy your logic controllers or Power Supply.

### 1. The Schematic Logic
* **Control Path:** The **Arduino Uno** acts as the brain. It sends low-voltage PWM signals to the **IBT_2** drivers.
* **Power Path:** The **24V 50A PSU** provides the "muscle." It is wired directly to the IBT_2 drivers using **12 AWG** wire.
* **Protection Path:** Fuses and capacitors sit between the power and the drivers to act as a buffer and safety shut-off.

### 2. Wiring Diagram

| From (Source) | To (Destination) | Wire Type | Component/Note |
| :--- | :--- | :--- | :--- |
| **PSU (+) 24V** | IBT_2 (B+) | 12 AWG | **30A Inline Fuse** |
| **PSU (-) GND** | IBT_2 (B-) | 12 AWG | Common Ground |
| **IBT_2 (B+/B-)** | **Capacitor** | Pre-soldered | **10,000uF 63V** (Across Terminals) |
| **IBT_2 (M+/M-)** | **Motor** | 12 AWG | **XT90/XT60 Connectors** |
| **Arduino Pin 8/9** | IBT_2 (L/R_PWM) | Dupont | PWM Control Signals |
| **Potentiometer** | Arduino (A0/A1) | Shielded | Feedback Loop |

### 3. Critical Component: The Capacitor "Buffer"
One of the most important decisions in this build was placing the **35V 10,000uF capacitors** directly across the IBT_2 input screws.

* **The Problem:** When the 350W motors stop suddenly, they generate a massive reverse voltage spike (Back-EMF).
* **The Solution:** The capacitor acts like a "sponge," absorbing that spike before it can travel back to the Power Supply. Without this, the PSU will likely enter "Protection Mode" and shut down the rig mid-flight.

### 4. Connection Standards
To prevent electrical fires or signal noise, we use:
* **Ferrules:** All 12 AWG wires going into the IBT_2 screw terminals are crimped with **4mm² ferrules**. This provides a solid mechanical connection that won't vibrate loose.
* **Shielded Cable:** The wires for the potentiometers (feedback) are shielded to prevent "EMI" from the high-power motor cables from interfering with the position signal.

## VIII. Ergonomics & Layout (1.73m Pilot)
Seating & Pedals
Seat Height: Elevate the seat roughly 22cm above the platform rails to create a proper "command" seating position.

Seat Recline: Backrest at ~27°; seat base tilted ~7° upward for GT3-style comfort.

Pedal Setup (Fanatec CSL): Angle the pedals at 18°–20° using a custom 4040 wedge.
Vertical Height Offset
Because your seat is at +22cm, I recommend mounting the back of your pedal tray (the end closest to you) on 100mm-120mm risers, and the front of the tray (the end furthest from you) on 160mm-180mm risers.

The Benefit: This creates the incline and raises your heels. In a motion rig, you want your heels to be roughly 10cm below your "hip point" (the bottom of your seat). This "shallow" seating position keeps you locked into the seat bucket during high-pitch motion.

Reasoning: For your height, angling the pedals prevents ankle strain and ensures your legs maintain a proper bend to absorb motion shocks.


## IX. Lever Arm Optimization (80mm vs 60mm)

The choice of an **80mm lever** was made to balance the requirements of both High-Velocity Racing and High-Angle Flight Simulation.

### 1. The Decision: 80mm Effective Length
* **Flight Logic:** Provides the maximum degrees of pitch (tilt) required for sustained climb and dive sensations in flight sims.
* **Racing Logic:** Delivers the high-frequency "snap" needed for gear shifts and curb strikes.

### 2. Compensation for Torque Demand
Because an 80mm lever places higher stress on the motor gearbox, the rig design compensates by:
1. Using **600mm vertical pillars** for high mechanical leverage.
2. Lowering seat risers to **160mm** to bring the Center of Gravity closer to the pivot.
3. Utilizing **30A fuses and 63V capacitors** to manage the resulting electrical spikes (Back-EMF).

## X. Motion Range & Degrees of Freedom

The rig is mechanically tuned to provide a high range of motion while maintaining motor efficiency.

* **Pitch (Forward/Back):** +/- 16° to 18° from center.
* **Roll (Left/Right):** +/- 16° to 18° from center.
* **The Logic:** This range exceeds the industry standard for GT3 racing (typically 8°-10°) and provides sufficient displacement for sustained G-force simulation in flight and space maneuvers. 
* **Safety Limit:** Hardware limits are set to 18° to prevent the 80mm levers from entering a mechanical "lock-out" or "cam-over" state.

## XI. Post-Assembly Motor Control Parameter Setting (SMC3)

(Note the git for SMC3 is: https://github.com/SimulatorMotorController/SMC3Utils)

**! Important if testing via SMC3 and the motors spins and stops check if the rotation of the motor and the potentiometer are in the same direction, if not will not work. To fix just reverse the motor cables as they are not polarity, just will drive the motors in one or another dircetion!**

The motion system has been successfully commissioned and verified via closed-loop PID control.

### 1. Polarity Verification
* **Confirmed:** Motor M+/M- leads oriented to match Potentiometer feedback direction.
* **State:** Stable Negative Feedback achieved. Target (Blue) and Feedback (Green) lines show <1% tracking error.

## 2. Note: Control Logic Hierarchy

It is vital to distinguish between the **Firmware Logic (SMC3)** and the **Telemetry Engine (SimTools)**.

1. **SMC3 Utility (The "Bios"):** All PID and PWM parameters are stored on the Arduino's EEPROM. These settings dictate the motor's internal physics. Changes must be made in the SMC3 Utility *before* launching SimTools.
2. **SimTools (The "Pilot"):** Only provides the target coordinates. It does not control motor torque or current limits.
3. **Safety Protocol:** - PWMmax should be capped at 60% (150/255) during initial center-of-gravity testing.
   - Max Limits must be set to ensure the 80mm lever never rotates into the 4080 frame members.

### 3. How to test and program each motor limits with SMC3 (Data will be stored in the Arduino)
* Disconnect the motor power supply
* Make sure Simtools is not running – we’re not ready for that yet!
* Wire up the Arduino (with SMC3 installed) to your H-Bridges and connect to your computer via USB
* Run the Windows SMC3 Utility software and make sure it communicates with the Arduino (There is no need to set baud rates, they are not configurable)
* Set the Kp, Ki, Kd, PWMmin, PWMmax, PWMrev to 0 for ALL motors (This will make sure the motors don’t move)
* Set Clip to 255 (you need to do this first) and Limit to 255 (This will give you plenty of margin if something goes wrong while setting up)
* Turn on the power to your motors – nothing should move at this stage!
* Set Kp to about 400
* Now slowly, increase PWMmax… at some point the motor should start to move. When it does check the “Green” feedback line is moving toward the “Blue” target position.
* If it is then that motor and feedback is wired correctly - proceed to test other motors.
* If it is moving away turn off motor power immediately (or quickly reduce PWMmax again). In this case you need to either reverse the wires to the motor being tested –OR– reverse the +5V and GND wires to your feedback pot for the motor being tested (do not do both). Restart the test from the beginning.
* Do the above for each motor

* ### 4. SMC3 Configuration Profiles (v1.01)
* Detailed settings are here: https://github.com/luisclement/DIY-2-DOF-Simulator/blob/main/SMC3_Parameters_CheckList.md

These profiles are specifically tuned for a **Full-Frame Rig (~130kg)** utilizing **350W 24V Motors** and **80mm Levers**. 

### 5. Profile: Commissioning & Safe Start
*Use this for initial mechanical testing, clearance checks, and finding the Center of Gravity (CoG).*

| Parameter | Value | Description |
| :--- | :--- | :--- |
| **Kp** | 150 | Reduced gain to prevent violent initial jerks. |
| **Ki** | 0 | Integral gain - always 0 for motion sims. |
| **Kd** | 30 | Damping to handle frame inertia. |
| **Ks** | 0 | Static friction - keep at 0 for now. |
| **LPF** | 0 | Low Pass Filter - keep at 0 for tuning. |
| **Deadzone** | 8 | High value to ignore play in hand-filed keyways. |
| **PWMmin** | 0 | Minimum power - 0 for silent idle during testing. |
| **PWMmax** | 80 | Safety ceiling (approx. 30% total power). |
| **PWMrev** | 50 | Softens the impact of direction changes. |
| **Fpwm** | 35kHz | High-frequency silent switching. |
| **Max Limits**| 200 | Safety "virtual wall" (Adjust to 100 if using 0-100 scale). |
| **Clip Input**| 255 | Full target range visibility. |

---

### 6. Profile: Performance & Final Target
*Use this only after the rig is balanced, the keyway play is fixed, and the mechanical limits are verified.*

| Parameter | Value | Description |
| :--- | :--- | :--- |
| **Kp** | 420 | High torque for rapid VR weight-transfer cues. |
| **Ki** | 0 | Remains at 0 to avoid telemetry lag. |
| **Kd** | 60 | High damping to stop the 130kg mass from oscillating. |
| **Ks** | 0 | Static friction - leave at 0 unless "sticking" occurs. |
| **LPF** | 0 | Filter - typically handled by SimTools instead. |
| **Deadzone** | 3 | Tightens response; requires zero mechanical slop. |
| **PWMmin** | 15 | Overcomes motor friction for micro-vibrations/road rumble. |
| **PWMmax** | 255 | Full 24V power capability for high-G maneuvers. |
| **PWMrev** | 100 | Sharp reversal for gear-shift "thumps." |
| **Fpwm** | 35kHz | Silent operation. |
| **Max Limits**| 255 | Maximum utilization of physical range. |
| **Clip Input**| 255 | Maximum signal resolution. |

> **Note:** If the rig exhibits "hunting" (shaking at rest), increase the **Deadzone** or **Kd**. If the motors get hot while sitting still, the rig is likely out of balance on the U-joint.

### 7. Recommended Tuning Profiles Summary (SMC3 v1.01)

Due to the 130kg mass of the full-frame assembly and 80mm lever geometry, the following PID profiles are recommended.

### 1. Commissioning Profile (Safety-First)
Used for mechanical clearance checks and U-joint balance verification.
* Kp: 150 | Kd: 30 | PWMmax: 80 | Deadzone: 8

### 2. Performance Profile (GT3/Formula)
Optimized for high-fidelity VR telemetry and rapid weight transfer.
* Kp: 420 | Kd: 60 | PWMmax: 255 | Deadzone: 3
* **Note:** High Kd is required to dampen the inertial moment of the 4080 frame. If the rig vibrates at rest, increase Deadzone to compensate for mechanical backlash in the hand-filed motor-shaft interface.

### 3. PWM Frequency
* **Fpwm:** 35kHz (Silent Operation). Setting lower than 20kHz may result in audible motor whine and reduced feedback resolution.

### 8. My docummnted final values

* This could be the final but needs to start at safer base:
PWMmax = 255: This is "Full Beast Mode." It means the Arduino is allowed to send 100% of your 24V power to the motors. For a first run with a hand-filed keyway and an unbalanced 130kg rig, this is dangerous. > Advice: Drop this to 150 until you've verified the mechanical limits.

Kp = 420: This is very high gain. On a light seat-mover, it's fine. On a heavy Full-Frame rig, this might cause "Oscillation" (the whole rig shaking violently because it's trying to be too precise with a heavy weight).

Max Limits = 255 / Clip Input = 255: Earlier you mentioned these stopped at 100, but your screenshot shows 255. If you can set them to 255, do it. It gives the software the most "room" to breathe.

Fpwm = 35kHz: This is excellent. It keeps the motor noise above the range of human hearing, so you won't hear a high-pitched "ringing" from the motors.

* <img width="1510" height="915" alt="Screenshot 2026-03-21 220130" src="https://github.com/user-attachments/assets/7afce373-878a-4a7a-a292-15dd86c36142" />

## XII. Power & Kinematics Analysis

The motion platform is driven by dual 350W geared DC motors, optimized for high-velocity seat-mover telemetry.

* **Combined Rated Torque:** 22 N.m
* **Combined Peak Torque:** ~45 N.m
* **Lever Length:** 80mm (Center-to-Center)
* **Effective Rated Force:** 275 Newtons (~28kg-f)
* **Velocity:** Calculated at ~150mm/s at rated RPM.
* **Mechanical Advantage:** The 80mm lever choice prioritizes high-frequency response (road texture/vibrations) over raw lifting capacity, making it ideal for GT3 and Formula style simulation where rapid weight transfer cues are essential.

## XIII. Lever Centering & Range of Motion

The system utilizes a "Horizontal Neutral" configuration to optimize motor torque and ensure linear telemetry response.

### 1. Mechanical Center-Point
* **Neutral Angle:** 0° (Horizontal / 9 o'clock position).
* **Total Physical Arc:** 106° (From -46° Bottom to +60° Top).
* **Operational Arc:** ~±20° from horizontal (Total 40° used for 100% SimTools output).

### 2. Torque Efficiency
By centering at 0°, the lever operates in the "Torque Peak" zone. This minimizes current draw on the IBT_2 drivers and reduces heat soak in the 350W motor windings when maintaining static pitch (e.g., long uphill climbs in Assetto Corsa).

### 3. Safety Buffers
Mechanical hard-stops are located at -46° and +60°. Software limits in SMC3 (Max Limits) are set to 20° below/above horizontal, providing a 26° "Crush Zone" to protect the 4080 frame and rod ends from collision during accidental runaway scenarios.

* SIMTools working axis:
<img width="927" height="506" alt="image" src="https://github.com/user-attachments/assets/2aec4761-6a5e-4ea2-ac75-dd7cb4aef0f6" />








