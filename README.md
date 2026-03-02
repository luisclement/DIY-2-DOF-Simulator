# 2-DOF Motion Simulator: The "Seat Mover" Build Guide

This repository contains the documentation and design decisions for a high-performance, DIY 2-DOF (Degree of Freedom) motion simulator, specifically optimized for flight and space simulations (VR-ready).

---

## 1. Mechanical Design Decisions
### The "Seat Mover" Configuration
* [cite_start]**Decision:** We chose a **"Seat Mover"** design where the motors are mounted at the base and push/pull against the back of the seat at shoulder height[cite: 5, 145].
* **Reasoning:** Unlike full-platform movers, moving only the seat provides a more intense feeling of G-forces and weight transfer. [cite_start]By mounting the rods at shoulder height, we gain a massive mechanical advantage (leverage), allowing the motors to move the user with less strain and higher speed[cite: 144, 145].
* [cite_start]**Integrated Platform:** I decided to make the moving platform long enough to hold the pedals, HOTAS, and wheel together as one "closed-top" unit[cite: 129, 131]. [cite_start]This ensures that your controls stay in the same position relative to your body as the seat moves, which is vital for immersion, especially in VR[cite: 78, 104].

### The Pivot (U-Joint)
* [cite_start]**Decision:** Use a **Flanged Industrial Joint (212/1310)**[cite: 129].
* [cite_start]**Reasoning:** Industrial flanged joints are superior to automotive steering joints because they feature flat mounting surfaces[cite: 112]. [cite_start]This allows them to be bolted directly to aluminum profiles, ensuring a rigid center of gravity and removing the need for complex custom welding[cite: 114].


---

## 2. Hardware & Frame Specs
### Aluminum Profiles
* [cite_start]**Decision:** A mix of **4040** and **4080** T-slot aluminum profiles[cite: 129].
* [cite_start]**Reasoning:** While 4040 is perfect for the base and platform rails, I chose **4080 (80mm x 40mm)** specifically for the **Gantry Uprights** behind the seat[cite: 147]. These uprights handle the most torque and force from the motor rods; [cite_start]4080 ensures the frame is stiff enough to prevent "flex," which would otherwise absorb the motion cues and make the rig feel "mushy"[cite: 147].

### Fasteners
* [cite_start]**Decision:** **DIN 7380F (Flanged Button Head)** M8 bolts[cite: 155].
* [cite_start]**Reasoning:** These bolts were chosen because the integrated flange acts like a built-in washer[cite: 155]. [cite_start]This provides a wider surface area to apply torque against the aluminum profile, ensuring the rig doesn't rattle loose during high-vibration flight maneuvers[cite: 156].

---

## 3. Electrical & Power System
### Motors
* [cite_start]**Decision:** **L-faster 24V 350W Brushed Gear Motors (120 RPM)**[cite: 169].
* [cite_start]**Reasoning:** We prioritized the 120 RPM version over the 75 RPM version to ensure the rig is fast enough for "washout" effects and rapid turbulence[cite: 51, 64]. [cite_start]These motors feature a 17mm keyed shaft, which provides a reliable mechanical connection for the motor levers[cite: 191].

### Power Supply (PSU)
* [cite_start]**Decision:** **24V 50A (1200W)** switching power supply[cite: 159, 162].
* **Reasoning:** Motion rigs experience massive current "spikes" when motors instantly change direction. [cite_start]A 50A supply provides enough "headroom" so the system doesn't shut down or brown out during aggressive maneuvers[cite: 162, 167].

### Safety: The "Capacitor Fix"
* [cite_start]**Decision:** **35V 10,000uF capacitors** placed directly across the **+ and - input terminals** of each IBT_2 motor driver[cite: 180, 208].
* [cite_start]**Reasoning:** Brushed motors act as generators when they are pushed by the weight of the rig (Regenerative Braking)[cite: 177, 180]. This sends voltage spikes back toward the electronics. [cite_start]Placing large capacitors at the IBT_2 inputs "catches" these spikes immediately, protecting the power supply and the motor drivers from being fried[cite: 210].


---

## 4. Wiring & Connectivity
* [cite_start]**Wire Gauge:** **12 AWG** for all power-carrying lines[cite: 170].
* [cite_start]**Connectors:** **XT60 or XT90** connectors for the motor-to-driver interface[cite: 201].
* [cite_start]**Ferrules:** **4mm² (AWG 12) wire ferrules** for all terminal connections[cite: 217].
* [cite_start]**Reasoning:** I decided to **crimp** these ferrules rather than solder them[cite: 217, 218]. [cite_start]In a high-vibration environment like a motion rig, a mechanical crimp is less likely to fatigue and snap than a solder joint[cite: 218]. [cite_start]The XT-series connectors ensure a high-current, secure connection that won't vibrate loose[cite: 201].

---

## 5. Software Control
* [cite_start]**Controller:** **Arduino Uno**[cite: 203].
* [cite_start]**Motor Drivers:** Two **IBT_2 (BTS7960)** boards[cite: 203].
* [cite_start]**Logic:** The Arduino runs firmware (like SMC3) to receive telemetry from the PC and translate it into PWM signals for the IBT_2 drivers[cite: 203]. [cite_start]This setup allows for fine-tuning the "PID" (Proportional-Integral-Derivative) to ensure the seat moves smoothly without overshooting its target[cite: 203].

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
| **IBT_2 (B+/B-)** | **Capacitor** | Pre-soldered | **10,000uF 35V** (Across Terminals) |
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

Reasoning: For your height, angling the pedals prevents ankle strain and ensures your legs maintain a proper bend to absorb motion shocks.

