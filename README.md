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
| **Connectors**| Connectors | XT90 (High Current) |
| **Logic** | Arduino | Arduino Uno R3 |


# DIY 2-DOF Motion Simulator: "The Seat Mover" Build Guide

This repository serves as a comprehensive build guide and documentation for a 2-DOF (Degree of Freedom) motion simulator. This design is optimized for high-intensity flight, space, and racing simulations, specifically designed for VR immersion.

---

## 1. Mechanical Design & Logic
The primary goal of this build was to create a "Seat Mover" that provides realistic G-force cues with high speed and precision.

### The "Seat Mover" Philosophy
* **The Decision:** We chose a design where the motors are mounted behind the seat, pushing and pulling at shoulder height.
* **The Reasoning:** Most DIY builds try to move the whole platform from the base. By acting on the back of the seat, we gain a massive mechanical advantage (leverage). This allows the motors to respond faster to game telemetry, providing sharper cues for turbulence or gear shifts without straining the hardware.
* **Integrated Platform:** We decided on a "closed-top" unit. The seat, pedals, and HOTAS/Wheel are all bolted to the same moving frame.
    * *Teaching Tip:* In VR, if your seat moves but your hands and feet stay stationary, the immersion is broken. Keeping everything on the moving platform ensures your body and controls are always in sync.

### The Pivot: Industrial Flanged U-Joint
* **The Decision:** Use a **Flanged Industrial Joint (212/1310)**.
* **The Reasoning:** Unlike automotive steering joints which require custom welding, an industrial flanged joint can be bolted directly to aluminum profiles. This provides a rock-solid center of rotation and simplifies the assembly.



---

## 2. Hardware & Frame Specs
To handle the high torque of 350W motors, the frame must be extremely rigid.

### Profile Selection (4040 & 4080)
* **The Decision:** Use 4040 profiles for the general frame, but **4080 (80mm x 40mm)** for the vertical gantry uprights.
* **The Reasoning:** The vertical uprights handle the most "push-pull" force from the motors. Standard 4040 can flex under high load; 4080 ensures that every bit of motor energy goes into moving the seat, not bending the aluminum.

### Fasteners: DIN 7380F
* **The Decision:** Use **DIN 7380F (Flanged Button Head)** M8 bolts.
* **The Reasoning:** The integrated flange acts as a built-in washer, distributing the clamping force over a wider area of the profile slot. This prevents the rig from rattling loose over time due to constant vibrations.

---

## 3. Electrical System & Safety
Electrical reliability is critical. We designed this system to handle high current and protect against "Back-EMF."

### Motors & Power
* **Motors:** **24V 350W 120RPM Brushed DC Motors** (17mm keyed shaft).
    * *Reasoning:* 120 RPM provides the necessary speed for "washout" effects and rapid vibration, while still maintaining enough torque to move a full-grown adult.
* **Power Supply:** **24V 50A (1200W)**.
    * *Reasoning:* When motors reverse direction instantly, they draw massive "inrush" current. A 50A PSU ensures the system doesn't shut down during aggressive maneuvers.

### The "Capacitor Rule"
* **The Decision:** Install **35V 10,000uF capacitors** directly across the **+ and - input terminals** of each IBT_2 driver.
* **The Reasoning:** Brushed motors act as generators when pushed. This sends voltage spikes back through the wires. These capacitors "catch" those spikes before they can travel back and damage your power supply.



---

## 4. Wiring & Connectivity
* **Wire Gauge:** **12 AWG** for all power paths to prevent heat and voltage drop.
* **Connectors:** **XT60 or XT90** between drivers and motors for a secure, high-current connection.
* **Ferrules:** Always use **4mm² (AWG 12) wire ferrules** for terminal blocks.
    * *Teaching Tip:* You must **crimp** these ferrules. Never solder them. Solder makes the wire brittle; a crimp stays flexible and can handle the constant vibration of the rig without snapping.

---

## 5. Software & Control
* **Controller:** **Arduino Uno** running **SMC3 Firmware**.
* **Drivers:** Two **IBT_2 (BTS7960)** boards.
* **Function:** The Arduino receives telemetry from the PC (via SimTools or FlyPT Mover) and translates it into PWM signals for the motors.

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


