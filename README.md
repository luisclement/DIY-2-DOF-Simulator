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



